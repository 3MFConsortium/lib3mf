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

NMR_ModelReader_3MF.cpp implements the Model Reader Class for
3MF Files. A 3MF model reader reads in a 3MF file and generates an in-memory representation of it.

--*/

#include "Model/Reader/NMR_ModelReader_3MF.h"
#include "Model/Reader/NMR_ModelReaderNode_ModelBase.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshImport/NMR_MeshImporter_STL.h"
#include "Common/Platform/NMR_Platform.h"
#include "Model/Classes/NMR_ModelAttachment.h" 

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRefModel.h"
#include "Model/Reader/NMR_ModelReader_InstructionElement.h"

#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	CModelReader_3MF::CModelReader_3MF(_In_ PModel pModel)
		: CModelReader(pModel)
	{
		// empty on purpose
	}

	void readProductionAttachmentModels(_In_ PModel pModel, _In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor)
	{
		nfUint32 prodAttCount = pModel->getProductionAttachmentCount();
		for (nfInt32 i = prodAttCount-1; i >=0; i--)
		{
			if (pProgressMonitor) {
				pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READNONROOTMODELS);
				pProgressMonitor->ReportProgressAndQueryCancelled(true);
			}

			PModelAttachment pProdAttachment = pModel->getProductionModelAttachment(i);
			std::string sPath = pProdAttachment->getPathURI();
			PImportStream pSubModelStream = pProdAttachment->getStream();

			// Create XML Reader
			PXmlReader pXMLReader = fnCreateXMLReaderInstance(pSubModelStream, pProgressMonitor);

			nfBool bHasModel = false;
			eXmlReaderNodeType NodeType;
			// Read all XML Root Nodes
			while (!pXMLReader->IsEOF()) {
				if (!pXMLReader->Read(NodeType))
					break;

				// Get Node Name
				LPCSTR pszLocalName = nullptr;
				pXMLReader->GetLocalName(&pszLocalName, nullptr);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				if (strcmp(pszLocalName, XML_3MF_ATTRIBUTE_PREFIX_XML) == 0) {
					PModelReader_InstructionElement pXMLNode = std::make_shared<CModelReader_InstructionElement>(pWarnings);
					pXMLNode->parseXML(pXMLReader.get());
				}

				// Compare with Model Node Name
				if (strcmp(pszLocalName, XML_3MF_ELEMENT_MODEL) == 0) {
					if (bHasModel)
						throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
					bHasModel = true;

					PModelReaderNode_ModelBase pXMLNode;
					pModel->setCurrentPath(sPath);

					pXMLNode = std::make_shared<CModelReaderNode_ModelBase>(pModel.get(), pWarnings, sPath, pProgressMonitor);
					pXMLNode->setIgnoreBuild(true);
					pXMLNode->setIgnoreMetaData(true);
					pXMLNode->parseXML(pXMLReader.get());

					if (!pXMLNode->getHasResources())
						throw CNMRException(NMR_ERROR_NORESOURCES);
					if (!pXMLNode->getHasBuild())
						throw CNMRException(NMR_ERROR_BUILDITEMNOTFOUND);
				}
			}
		}
	}


	void CModelReader_3MF::readStream(_In_ PImportStream pStream)
	{
		__NMRASSERT(pStream != nullptr);

		nfBool bHasModel = false;

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READSTREAM);

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_EXTRACTOPCPACKAGE);
		
		// Extract Stream from Package
		PImportStream pModelStream = extract3MFOPCPackage(pStream);
		
		// before reading the root model, read the other models in the file
		readProductionAttachmentModels(model(), warnings(), monitor());

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READROOTMODEL);
		monitor()->ReportProgressAndQueryCancelled(true);

		// Create XML Reader
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pModelStream, monitor());

		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCSTR pszLocalName = nullptr;
			pXMLReader->GetLocalName(&pszLocalName, nullptr);
			if (!pszLocalName)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

			if (strcmp(pszLocalName, XML_3MF_ATTRIBUTE_PREFIX_XML) == 0) {
				PModelReader_InstructionElement pXMLNode = std::make_shared<CModelReader_InstructionElement>(warnings());
				pXMLNode->parseXML(pXMLReader.get());
			}

			// Compare with Model Node Name
			if (strcmp(pszLocalName, XML_3MF_ELEMENT_MODEL) == 0) {
				if (bHasModel)
					throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
				bHasModel = true;

				model()->setCurrentPath(model()->rootPath());
				PModelReaderNode_ModelBase pXMLNode = std::make_shared<CModelReaderNode_ModelBase>(model().get(), warnings(), model()->rootPath(), monitor());
				pXMLNode->parseXML(pXMLReader.get());

				if (!pXMLNode->getHasResources())
					throw CNMRException(NMR_ERROR_NORESOURCES);
				if (!pXMLNode->getHasBuild())
					throw CNMRException(NMR_ERROR_NOBUILD);
			}

		}

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_CLEANUP);
		monitor()->ReportProgressAndQueryCancelled(false);

		// Release Memory of 3MF Package
		release3MFOPCPackage();

		if (!bHasModel)
			throw CNMRException(NMR_ERROR_NOMODELNODE);

		monitor()->SetProgressIdentifier(ProgressIdentifier::PROGRESS_DONE);
		monitor()->ReportProgressAndQueryCancelled(false);
	}

	void CModelReader_3MF::addTextureAttachment(_In_ std::string sPath, _In_ PImportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		model()->addAttachment(sPath, PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pStream);
	}

}
