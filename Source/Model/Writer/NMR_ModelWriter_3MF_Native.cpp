/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelWriter_3MF_Native.cpp implements the platform independent 3MF Model Writer Class. 
This model writer exports the in memory represenation into a 3MF file,
using LibZ and a native XML writer implementation.


--*/

#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Model/Classes/NMR_ModelTextureAttachment.h" 
#include "Common/Platform/NMR_ImportStream.h" 
#include "Common/NMR_Exception.h" 
#include "Common/Platform/NMR_XmlWriter.h" 
#include "Common/Platform/NMR_XmlWriter_Native.h" 
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"
#include "Common/NMR_StringUtils.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_ModelWarnings.h"
#include <functional>
#include <sstream>

namespace NMR {
	
	CModelWriter_3MF_Native::CModelWriter_3MF_Native(_In_ PModel pModel) : CModelWriter_3MF(pModel)
	{
		m_pOtherModel = nullptr;
	}

	// These are OPC dependent functions
	void CModelWriter_3MF_Native::createPackage(_In_ CModel * pModel)
	{
		__NMRASSERT(pModel != nullptr);
		m_pOtherModel = pModel;
	}

	void CModelWriter_3MF_Native::releasePackage()
	{
		m_pPackageWriter->close();
		m_pPackageWriter = nullptr;
		m_pOtherModel = nullptr;
	}

	void CModelWriter_3MF_Native::writePackageToStream(_In_ PExportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_pOtherModel == nullptr)
			throw CNMRException(NMR_ERROR_NOMODELTOWRITE);

		// Maximal progress = NrResources + NrAttachments + Build + Cleanup
		monitor()->SetMaxProgress(m_pOtherModel->getResourceCount() + m_pOtherModel->getAttachmentCount() + 1 + 1);

		// Write Model Stream
		m_pPackageWriter = std::make_shared<CKeyStoreOpcPackageWriter>(pStream, *this);
		POpcPackagePart pModelPart = m_pPackageWriter->addPart(m_pOtherModel->rootPath());
		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pModelPart->getExportStream());

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITEROOTMODEL);
		monitor()->ReportProgressAndQueryCancelled(true);

		writeModelStream(pXMLWriter.get(), m_pOtherModel);

		// add Root relationships
		m_pPackageWriter->addRootRelationship(PACKAGE_START_PART_RELATIONSHIP_TYPE, pModelPart.get());

		PModelAttachment pPackageThumbnail = m_pOtherModel->getPackageThumbnail();
		if (pPackageThumbnail.get() != nullptr)
		{
			// create Package Thumbnail Part
			POpcPackagePart pThumbnailPart = m_pPackageWriter->addPart(pPackageThumbnail->getPathURI());
			PExportStream pExportStream = pThumbnailPart->getExportStream();
			// Copy data
			PImportStream pPackageThumbnailStream = pPackageThumbnail->getStream();
			pPackageThumbnailStream->seekPosition(0, true);
			pExportStream->copyFrom(pPackageThumbnailStream.get(), pPackageThumbnailStream->retrieveSize(), MODELWRITER_NATIVE_BUFFERSIZE);
			// add root relationship
			m_pPackageWriter->addRootRelationship(pPackageThumbnail->getRelationShipType(), pThumbnailPart.get());
		}

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITENONROOTMODELS);
		monitor()->ReportProgressAndQueryCancelled(true);

		addNonRootModels();
		
		// add Attachments
		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITEATTACHMENTS);
		monitor()->ReportProgressAndQueryCancelled(true);

		addAttachments(m_pOtherModel, pModelPart);

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITECONTENTTYPES);
		monitor()->ReportProgressAndQueryCancelled(true);

		// add Content Types
		m_pPackageWriter->addContentType(PACKAGE_3D_RELS_EXTENSION, PACKAGE_3D_RELS_CONTENT_TYPE);
		m_pPackageWriter->addContentType(PACKAGE_3D_MODEL_EXTENSION, PACKAGE_3D_MODEL_CONTENT_TYPE);
		m_pPackageWriter->addContentType(PACKAGE_3D_TEXTURE_EXTENSION, PACKAGE_TEXTURE_CONTENT_TYPE);
		m_pPackageWriter->addContentType(PACKAGE_3D_PNG_EXTENSION, PACKAGE_PNG_CONTENT_TYPE);
		m_pPackageWriter->addContentType(PACKAGE_3D_JPEG_EXTENSION, PACKAGE_JPG_CONTENT_TYPE);
		m_pPackageWriter->addContentType(PACKAGE_3D_JPG_EXTENSION, PACKAGE_JPG_CONTENT_TYPE);

		std::map<std::string, std::string> CustomContentTypes = m_pOtherModel->getCustomContentTypes();
		std::map<std::string, std::string>::iterator iContentTypeIterator;

		for (iContentTypeIterator = CustomContentTypes.begin(); iContentTypeIterator != CustomContentTypes.end(); iContentTypeIterator++) {
			if (!m_pOtherModel->contentTypeIsDefault(iContentTypeIterator->first)) {
				m_pPackageWriter->addContentType(iContentTypeIterator->first, iContentTypeIterator->second);
			}
		}
	}

	void CModelWriter_3MF_Native::addNonRootModels() {

		// do this based on resource-paths
		std::vector<PPackageModelPath> vctPPaths = m_pOtherModel->retrieveAllModelPaths();
		nfUint64 nCount = vctPPaths.size();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITENONROOTMODELS);
			monitor()->ReportProgressAndQueryCancelled(true);

			std::string sNonRootModelPath = vctPPaths[nIndex]->getPath();
			if (sNonRootModelPath == m_pOtherModel->rootPath())
				continue;

			m_pOtherModel->setCurrentPath(sNonRootModelPath);
			PImportStream pStream;
			{
				PExportStreamMemory pExportStream = std::make_shared<CExportStreamMemory>();
				PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pExportStream);
				writeNonRootModelStream(pXMLWriter.get());

				pStream = std::make_shared<CImportStream_Unique_Memory>(pExportStream->getData(), pExportStream->getDataSize());
			}
			
			// check, whether this non-root model is already in here
			PModelAttachment pNonRootModelAttachment = m_pOtherModel->findModelAttachment(sNonRootModelPath);
			if (pNonRootModelAttachment.get() != nullptr) {
				if (pNonRootModelAttachment->getRelationShipType() != PACKAGE_START_PART_RELATIONSHIP_TYPE)
					warnings()->addWarning(NMR_ERROR_DUPLICATEATTACHMENTPATH, eModelWarningLevel::mrwFatal);
				pNonRootModelAttachment->setStream(pStream);
			}
			else
				pNonRootModelAttachment = m_pOtherModel->addAttachment(sNonRootModelPath, PACKAGE_START_PART_RELATIONSHIP_TYPE, pStream);
		}
	}

	void CModelWriter_3MF_Native::addAttachments(_In_ CModel * pModel, _In_ POpcPackagePart pModelPart)
	{
		__NMRASSERT(pModel != nullptr);
		__NMRASSERT(pModelPart.get() != nullptr);
		__NMRASSERT(pPackageWriter.get() != nullptr);

		nfUint32 nCount = pModel->getAttachmentCount();
		nfUint32 nIndex;

		if (nCount > 0) {
			for (nIndex = 0; nIndex < nCount; nIndex++) {

				monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITEATTACHMENTS);
				monitor()->ReportProgressAndQueryCancelled(true);

				PModelAttachment pAttachment = pModel->getModelAttachment(nIndex);
				PImportStream pStream = pAttachment->getStream();
				
				std::string sPath = fnIncludeLeadingPathDelimiter(pAttachment->getPathURI());
				std::string sRelationShipType = pAttachment->getRelationShipType();

				if (pStream.get() == nullptr)
					throw CNMRException(NMR_ERROR_INVALIDPARAM);

				// create Attachment Part
				POpcPackagePart pAttachmentPart = m_pPackageWriter->addPart(sPath);
				PExportStream pExportStream = pAttachmentPart->getExportStream();

				// Copy data
				pStream->seekPosition(0, true);
				pExportStream->copyFrom(pStream.get(), pStream->retrieveSize(), MODELWRITER_NATIVE_BUFFERSIZE);

				// add relationships
				m_pPackageWriter->addPartRelationship(pModelPart, sRelationShipType.c_str(), pAttachmentPart.get());

				m_pPackageWriter->addWriterSpecificRelationships(pModelPart, pAttachmentPart.get());

				monitor()->IncrementProgress(1);
			}
		}
	}
}
