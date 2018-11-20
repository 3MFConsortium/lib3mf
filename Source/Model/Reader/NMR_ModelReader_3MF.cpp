/*++

Copyright (C) 2018 3MF Consortium

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
#include "Model/Reader/NMR_ModelReaderNode_Model.h"
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

	void readProductionAttachmentModels(_In_ PModel pModel, _In_ PModelReaderWarnings pWarnings, _In_ CProgressMonitor* pProgressMonitor)
	{
		nfUint32 prodAttCount = pModel->getProductionAttachmentCount();
		for (nfInt32 i = prodAttCount-1; i >=0; i--)
		{
			if (pProgressMonitor && !pProgressMonitor->Progress(double(prodAttCount - i -1) / double(prodAttCount), ProgressIdentifier::PROGRESS_READNONROOTMODELS))
				throw CNMRException(NMR_USERABORTED);

			PModelAttachment pProdAttachment = pModel->getProductionModelAttachment(i);
			std::string path = pProdAttachment->getPathURI();
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

					PModelReaderNode_Model pXMLNode;
					pModel->setCurPath(path.c_str());

					if (pProgressMonitor)
						pProgressMonitor->PushLevel(double(prodAttCount - i - 1) / double(prodAttCount), double(prodAttCount - i) / double(prodAttCount));
					pXMLNode = std::make_shared<CModelReaderNode_Model>(pModel.get(), pWarnings, path.c_str(), pProgressMonitor);
					pXMLNode->setIgnoreBuild(true);
					pXMLNode->setIgnoreMetaData(true);
					pXMLNode->parseXML(pXMLReader.get());
					if (pProgressMonitor)
						pProgressMonitor->PopLevel();

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

		m_pProgressMonitor->ResetLevels();
		if ( !m_pProgressMonitor->Progress(0, ProgressIdentifier::PROGRESS_READSTREAM) )
			throw CNMRException(NMR_USERABORTED);

		if (!m_pProgressMonitor->Progress(0.05, ProgressIdentifier::PROGRESS_EXTRACTOPCPACKAGE))
			throw CNMRException(NMR_USERABORTED);

		// Extract Stream from Package
		PImportStream pModelStream = extract3MFOPCPackage(pStream);
		
		if (!m_pProgressMonitor->Progress(0.1, ProgressIdentifier::PROGRESS_READNONROOTMODELS))
			throw CNMRException(NMR_USERABORTED);
		
		double dProgressNonRoot = 0.6;
		if (m_pModel->getProductionAttachmentCount() == 0)
			dProgressNonRoot = 0.1;

		// before reading the root model, read the other models in the file
		m_pProgressMonitor->PushLevel(0.1, dProgressNonRoot);
		readProductionAttachmentModels(m_pModel, m_pWarnings, m_pProgressMonitor.get());
		m_pProgressMonitor->PopLevel();

		if (!m_pProgressMonitor->Progress(dProgressNonRoot, ProgressIdentifier::PROGRESS_READROOTMODEL))
			throw CNMRException(NMR_USERABORTED);
		// Create XML Reader
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pModelStream, m_pProgressMonitor.get());

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
				PModelReader_InstructionElement pXMLNode = std::make_shared<CModelReader_InstructionElement>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
			}

			// Compare with Model Node Name
			if (strcmp(pszLocalName, XML_3MF_ELEMENT_MODEL) == 0) {
				if (bHasModel)
					throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
				bHasModel = true;

				m_pProgressMonitor->PushLevel(dProgressNonRoot, 0.95);
				m_pModel->setCurPath(m_pModel->rootPath().c_str());
				PModelReaderNode_Model pXMLNode = std::make_shared<CModelReaderNode_Model>(m_pModel.get(), m_pWarnings, m_pModel->rootPath().c_str(), m_pProgressMonitor.get());
				pXMLNode->parseXML(pXMLReader.get());
				m_pProgressMonitor->PopLevel();

				if (!pXMLNode->getHasResources())
					throw CNMRException(NMR_ERROR_NORESOURCES);
				if (!pXMLNode->getHasBuild())
					throw CNMRException(NMR_ERROR_NOBUILD);
			}

		}

		if (!m_pProgressMonitor->Progress(0.95, ProgressIdentifier::PROGRESS_CLEANUP))
			throw CNMRException(NMR_USERABORTED);

		m_pModel->removeReferencedSliceStackResources();

		// Release Memory of 3MF Package
		release3MFOPCPackage();

		if (!bHasModel)
			throw CNMRException(NMR_ERROR_NOMODELNODE);

		if (!m_pProgressMonitor->Progress(1.0, ProgressIdentifier::PROGRESS_DONE))
			throw CNMRException(NMR_USERABORTED);
	}

	void CModelReader_3MF::addTextureAttachment(_In_ std::string sPath, _In_ PImportStream pStream)
	{
		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModel->addAttachment(sPath, PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pStream);
	}

}
