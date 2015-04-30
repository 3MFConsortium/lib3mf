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

NMR_ModelReaderNode100_Triangles.cpp implements the Model Reader Triangles Node
Class. A triangles reader model node is a parser for the triangles node of an
XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangles.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"

namespace NMR {

	CModelReaderNode100_Triangles::CModelReaderNode100_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_pMesh = pMesh;
	}

	void CModelReaderNode100_Triangles::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Triangles::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	_Ret_notnull_ CMeshInformation_TexCoords * CModelReaderNode100_Triangles::createTexCoordInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformation(emiTexCoords);
		CMeshInformation_TexCoords * pTexCoords = nullptr;

		if (pInformation)
			pTexCoords = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);

		if (!pTexCoords) {
			PMeshInformation_TexCoords pNewMeshInformation = std::make_shared<CMeshInformation_TexCoords>(m_pMesh->getFaceCount ());
			pMeshInformationHandler->addInformation(pNewMeshInformation);

			pTexCoords = pNewMeshInformation.get();
		}

		return pTexCoords;
	}

	_Ret_notnull_ CMeshInformation_NodeColors * CModelReaderNode100_Triangles::createNodeColorInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformation(emiNodeColors);
		CMeshInformation_NodeColors * pNodeColors = nullptr;

		if (pInformation)
			pNodeColors = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);

		if (!pNodeColors) {
			PMeshInformation_NodeColors pNewMeshInformation = std::make_shared<CMeshInformation_NodeColors>(m_pMesh->getFaceCount());
			pMeshInformationHandler->addInformation(pNewMeshInformation);

			pNodeColors = pNewMeshInformation.get();
		}

		return pNodeColors;
	}

	void CModelReaderNode100_Triangles::OnChildElement(_In_z_ const nfWChar * pChildName, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);

		if (wcscmp(pChildName, XML_3MF_ELEMENT_TRIANGLE) == 0) {
			// Parse XML
			PModelReaderNode100_Triangle pXMLNode = std::make_shared<CModelReaderNode100_Triangle>(m_pWarnings);
			pXMLNode->parseXML(pXMLReader);

			// Retrieve node indices
			nfInt32 nIndex1, nIndex2, nIndex3;
			pXMLNode->retrieveIndices(nIndex1, nIndex2, nIndex3, m_pMesh->getNodeCount());

			// Create face if valid
			if ((nIndex1 != nIndex2) && (nIndex1 != nIndex3) && (nIndex2 != nIndex3)) {
				MESHNODE * pNode1 = m_pMesh->getNode(nIndex1);
				MESHNODE * pNode2 = m_pMesh->getNode(nIndex2);
				MESHNODE * pNode3 = m_pMesh->getNode(nIndex3);
				m_pMesh->addFace(pNode1, pNode2, pNode3);
			}
		}
	}

}
