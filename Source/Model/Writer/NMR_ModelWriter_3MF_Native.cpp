/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

#include "Model/Writer/NMR_ModelWriter_3MF_Native.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 
#include "Common/Platform/NMR_ImportStream.h" 
#include "Common/NMR_Exception.h" 
#include "Common/Platform/NMR_XmlWriter.h" 
#include "Common/Platform/NMR_XmlWriter_Native.h" 
#include "Common/NMR_StringUtils.h" 
#include <sstream>

namespace NMR {
	
	CModelWriter_3MF_Native::CModelWriter_3MF_Native(_In_ PModel pModel) : CModelWriter_3MF(pModel)
	{
		m_nRelationIDCounter = 0;
		m_pModel = nullptr;
	}

	// These are OPC dependent functions
	void CModelWriter_3MF_Native::createPackage(_In_ CModel * pModel)
	{
		__NMRASSERT(pModel != nullptr);
		m_pModel = pModel;
	}

	void CModelWriter_3MF_Native::releasePackage()
	{
		m_pModel = nullptr;
	}

	void CModelWriter_3MF_Native::writePackageToStream(_In_ PExportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_pModel == nullptr)
			throw CNMRException(NMR_ERROR_NOMODELTOWRITE);

		// Write Model Stream
		POpcPackageWriter pPackageWriter = std::make_shared<COpcPackageWriter>(pStream);
		POpcPackagePart pModelPart = pPackageWriter->addPart(PACKAGE_3D_MODEL_URI);
		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pModelPart->getExportStream());
		writeModelStream(pXMLWriter.get(), m_pModel);

		// add Root relationships
		pPackageWriter->addRootRelationship(generateRelationShipID(), PACKAGE_START_PART_RELATIONSHIP_TYPE, pModelPart.get());

		// add Textures
		addTextureParts(m_pModel, pPackageWriter, pModelPart);

		// add Attachments
		addAttachments(m_pModel, pPackageWriter, pModelPart);

		// add Content Types
		pPackageWriter->addContentType(PACKAGE_3D_RELS_EXTENSION, PACKAGE_3D_RELS_CONTENT_TYPE);
		pPackageWriter->addContentType(PACKAGE_3D_MODEL_EXTENSION, PACKAGE_3D_MODEL_CONTENT_TYPE);
		pPackageWriter->addContentType(PACKAGE_3D_TEXTURE_EXTENSION, PACKAGE_TEXTURE_CONTENT_TYPE);
		pPackageWriter->addContentType(PACKAGE_3D_PNG_EXTENSION, PACKAGE_PNG_CONTENT_TYPE);
		pPackageWriter->addContentType(PACKAGE_3D_JPEG_EXTENSION, PACKAGE_JPG_CONTENT_TYPE);
		pPackageWriter->addContentType(PACKAGE_3D_JPG_EXTENSION, PACKAGE_JPG_CONTENT_TYPE);

		std::map<std::wstring, std::wstring> CustomContentTypes = m_pModel->getCustomContentTypes();
		std::map<std::wstring, std::wstring>::iterator iContentTypeIterator;

		for (iContentTypeIterator = CustomContentTypes.begin(); iContentTypeIterator != CustomContentTypes.end(); iContentTypeIterator++) {
			if (!m_pModel->contentTypeIsDefault(iContentTypeIterator->first)) {
				pPackageWriter->addContentType(iContentTypeIterator->first, iContentTypeIterator->second);
			}
		}

			
	}


	std::wstring CModelWriter_3MF_Native::generateRelationShipID()
	{
		// Create Unique ID String
		std::wstringstream sStream;
		sStream << L"rel" << m_nRelationIDCounter;
		m_nRelationIDCounter++;
		return sStream.str();
	}

	void CModelWriter_3MF_Native::addTextureParts(_In_ CModel * pModel, _In_ POpcPackageWriter pPackageWriter, _In_ POpcPackagePart pModelPart)
	{
		__NMRASSERT(pModel != nullptr);
		__NMRASSERT(pModelPart.get() != nullptr);
		__NMRASSERT(pPackageWriter.get() != nullptr);

		nfUint32 nCount = pModel->getTextureStreamCount();
		nfUint32 nIndex;

		if (nCount > 0) {


			for (nIndex = 0; nIndex < nCount; nIndex++) {
				PImportStream pStream = pModel->getTextureStream(nIndex);
				std::wstring sPath = fnIncludeLeadingPathDelimiter (pModel->getTextureStreamPath(nIndex));

				if (pStream.get() == nullptr)
					throw CNMRException(NMR_ERROR_INVALIDPARAM);

				// create Texture Part
				POpcPackagePart pTexturePart = pPackageWriter->addPart(sPath);
				PExportStream pExportStream = pTexturePart->getExportStream();

				// Copy data
				pStream->seekPosition(0, true);
				pExportStream->copyFrom(pStream.get(), pStream->retrieveSize(), MODELWRITER_NATIVE_BUFFERSIZE);

				// add relationships
				pModelPart->addRelationship(generateRelationShipID(), PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pTexturePart->getURI());

			}
		}
	}

	void CModelWriter_3MF_Native::addAttachments(_In_ CModel * pModel, _In_ POpcPackageWriter pPackageWriter, _In_ POpcPackagePart pModelPart)
	{
		__NMRASSERT(pModel != nullptr);
		__NMRASSERT(pModelPart.get() != nullptr);
		__NMRASSERT(pPackageWriter.get() != nullptr);

		nfUint32 nCount = pModel->getAttachmentCount();
		nfUint32 nIndex;

		if (nCount > 0) {


			for (nIndex = 0; nIndex < nCount; nIndex++) {
				PModelAttachment pAttachment = pModel->getModelAttachment(nIndex);
				PImportStream pStream = pAttachment->getStream();
				
				std::wstring sPath = fnIncludeLeadingPathDelimiter(pAttachment->getPathURI());
				std::wstring sRelationShipType = pAttachment->getRelationShipType();

				if (pStream.get() == nullptr)
					throw CNMRException(NMR_ERROR_INVALIDPARAM);

				// create Texture Part
				POpcPackagePart pAttachmentPart = pPackageWriter->addPart(sPath);
				PExportStream pExportStream = pAttachmentPart->getExportStream();

				// Copy data
				pStream->seekPosition(0, true);
				pExportStream->copyFrom(pStream.get(), pStream->retrieveSize(), MODELWRITER_NATIVE_BUFFERSIZE);

				// add relationships
				pModelPart->addRelationship(generateRelationShipID(), sRelationShipType.c_str(), pAttachmentPart->getURI());

			}
		}
	}


}
