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

NMR_ModelReaderNode093_Triangles.cpp implements the Model Reader Triangles Node
Class. A triangles reader model node is a parser for the triangles node of an
XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangles.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Model/Classes/NMR_ModelBaseMaterial.h"
#include "Model/Reader/NMR_ModelReader_ColorMapping.h"

namespace NMR {

	CModelReaderNode093_Triangles::CModelReaderNode093_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping, _In_ PModelBaseMaterialResource pMaterialResource, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);
		__NMRASSERT(pColorMapping.get() != nullptr);
		__NMRASSERT(pTexCoordMapping.get() != nullptr);

		m_pModel = pModel;
		m_pMesh = pMesh;

		m_pColorMapping = pColorMapping;
		m_pTexCoordMapping = pTexCoordMapping;
		m_pMaterialResource = pMaterialResource;
	}

	void CModelReaderNode093_Triangles::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_Triangles::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	_Ret_notnull_ CMeshInformation_TexCoords * CModelReaderNode093_Triangles::createTexCoordInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType(0, emiTexCoords);
		CMeshInformation_TexCoords * pTexCoords = nullptr;

		if (pInformation)
			pTexCoords = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);

		if (!pTexCoords) {
			PMeshInformation_TexCoords pNewMeshInformation = std::make_shared<CMeshInformation_TexCoords>(m_pMesh->getFaceCount());
			pMeshInformationHandler->addInformation(pNewMeshInformation);

			pTexCoords = pNewMeshInformation.get();
		}

		return pTexCoords;
	}

	_Ret_notnull_ CMeshInformation_NodeColors * CModelReaderNode093_Triangles::createNodeColorInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType(0, emiNodeColors);
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

	_Ret_notnull_ CMeshInformation_BaseMaterials * CModelReaderNode093_Triangles::createBaseMaterialInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType(0, emiBaseMaterials);
		CMeshInformation_BaseMaterials * pBaseMaterials = nullptr;

		if (pInformation)
			pBaseMaterials = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);

		if (!pBaseMaterials) {
			PMeshInformation_BaseMaterials pNewMeshInformation = std::make_shared<CMeshInformation_BaseMaterials>(m_pMesh->getFaceCount());
			pMeshInformationHandler->addInformation(pNewMeshInformation);

			pBaseMaterials = pNewMeshInformation.get();
		}

		return pBaseMaterials;
	}

	void CModelReaderNode093_Triangles::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {

			if (strcmp(pChildName, XML_3MF_ELEMENT_TRIANGLE) == 0) {
				// Parse XML
				PModelReaderNode093_Triangle pXMLNode = std::make_shared<CModelReaderNode093_Triangle>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Retrieve node indices
				nfInt32 nIndex1, nIndex2, nIndex3;
				pXMLNode->retrieveIndices(nIndex1, nIndex2, nIndex3, m_pMesh->getNodeCount());

				// Create face if valid
				if ((nIndex1 != nIndex2) && (nIndex1 != nIndex3) && (nIndex2 != nIndex3)) {
					MESHNODE * pNode1 = m_pMesh->getNode(nIndex1);
					MESHNODE * pNode2 = m_pMesh->getNode(nIndex2);
					MESHNODE * pNode3 = m_pMesh->getNode(nIndex3);
					MESHFACE * pFace = m_pMesh->addFace(pNode1, pNode2, pNode3);

					nfInt32 nColorID1, nColorID2, nColorID3;
					pXMLNode->retrieveColorIDs(nColorID1, nColorID2, nColorID3);
					nfInt32 nMaterialID = pXMLNode->retrieveMaterialID();
					nfInt32 nTextureID = pXMLNode->retrieveTextureID();

					// map colors to textures
					if ((nColorID1 == nColorID2) && (nColorID1 == nColorID3)) {
						if (m_pColorMapping->hasTextureReference(nColorID1)) {
							nTextureID = m_pColorMapping->getTextureReference(nColorID1);
						}
					}

					// Create Texture Info
					if (nTextureID > 0) {
						CMeshInformation_TexCoords * pTexCoords = createTexCoordInformation();
						MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS*)pTexCoords->getFaceData(pFace->m_index);
						if (pFaceData) {

							// Convert Texture Coordinates to in memory representation
							PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nTextureID);
							if (pID.get()) {
								pFaceData->m_TextureID = pID->getUniqueID();
							}
							nfInt32 nTextureIndex1 = 0;
							nfInt32 nTextureIndex2 = 0;
							nfInt32 nTextureIndex3 = 0;
							ModelResourceIndex nTextureID = 0;

							pFaceData->m_vCoords[0].m_fields[0] = 0.0f;
							pFaceData->m_vCoords[0].m_fields[1] = 0.0f;
							pFaceData->m_vCoords[1].m_fields[0] = 0.0f;
							pFaceData->m_vCoords[1].m_fields[1] = 0.0f;
							pFaceData->m_vCoords[2].m_fields[0] = 0.0f;
							pFaceData->m_vCoords[2].m_fields[1] = 0.0f;

							if (pXMLNode->retrieveTextureIndices(nTextureIndex1, nTextureIndex2, nTextureIndex3)) {
								m_pTexCoordMapping->findTexCoords(1, nTextureIndex1, nTextureID, pFaceData->m_vCoords[0].m_fields[0], pFaceData->m_vCoords[0].m_fields[1]);
								m_pTexCoordMapping->findTexCoords(1, nTextureIndex2, nTextureID, pFaceData->m_vCoords[1].m_fields[0], pFaceData->m_vCoords[1].m_fields[1]);
								m_pTexCoordMapping->findTexCoords(1, nTextureIndex3, nTextureID, pFaceData->m_vCoords[2].m_fields[0], pFaceData->m_vCoords[2].m_fields[1]);
							}
						}

					}
					else {
						if ((nColorID1 > 0) && (nColorID2 > 0) && (nColorID3 > 0)) {
							CMeshInformation_NodeColors * pNodeColors = createNodeColorInformation();
							MESHINFORMATION_NODECOLOR* pFaceData = (MESHINFORMATION_NODECOLOR*)pNodeColors->getFaceData(pFace->m_index);
							nfInt32 j;

							if (pFaceData) {
								// Convert Colors to in memory representation
								for (j = 0; j < 3; j++) 
									pFaceData->m_cColors[j] = 0;
								m_pColorMapping->findColor(nColorID1, 0, pFaceData->m_cColors[0]);
								m_pColorMapping->findColor(nColorID2, 0, pFaceData->m_cColors[1]);
								m_pColorMapping->findColor(nColorID3, 0, pFaceData->m_cColors[2]);
							}

						}
						else {

							if (nMaterialID > 0) {
								CMeshInformation_BaseMaterials * pBaseMaterials = createBaseMaterialInformation();
								MESHINFORMATION_BASEMATERIAL* pFaceData = (MESHINFORMATION_BASEMATERIAL*)pBaseMaterials->getFaceData(pFace->m_index);
								if (pFaceData != nullptr) {
									pFaceData->m_nMaterialGroupID = 0;
									pFaceData->m_nMaterialIndex = 0;

									if (m_pMaterialResource.get() != nullptr) {
										ModelResourceIndex nIndex;
										if (m_pColorMapping->getMaterialReference(nMaterialID, nIndex)) {
											pFaceData->m_nMaterialGroupID = m_pMaterialResource->getResourceID()->getUniqueID();
											pFaceData->m_nMaterialIndex = nIndex;
										}
									}
								}

							}
						}
					}
				}
			}
		}
	}
}

