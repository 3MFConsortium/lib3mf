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

NMR_ModelReaderNode100_Triangles.cpp implements the Model Reader Triangles Node
Class. A triangles reader model node is a parser for the triangles node of an
XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangles.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Model/Classes/NMR_ModelBaseMaterial.h"
#include "Model/Reader/NMR_ModelReader_ColorMapping.h"

namespace NMR {

	CModelReaderNode100_Triangles::CModelReaderNode100_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReaderWarnings pWarnings, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping, _In_ ModelResourceID nDefaultPropertyID, _In_ ModelResourceIndex nDefaultPropertyIndex)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);
		if (!pColorMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!pTexCoordMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pColorMapping = pColorMapping;
		m_pTexCoordMapping = pTexCoordMapping;

		m_nDefaultPropertyID = nDefaultPropertyID;
		m_nDefaultPropertyIndex = nDefaultPropertyIndex;

		m_nUsedPropertyID = 0;

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

	void CModelReaderNode100_Triangles::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	_Ret_notnull_ CMeshInformation_TexCoords * CModelReaderNode100_Triangles::createTexCoordInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType (0, emiTexCoords);
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

	_Ret_notnull_ CMeshInformation_BaseMaterials * CModelReaderNode100_Triangles::createBaseMaterialInformation()
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

	void CModelReaderNode100_Triangles::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_TRIANGLE) == 0) {
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
					MESHFACE * pFace = m_pMesh->addFace(pNode1, pNode2, pNode3);

					ModelResourceID nPropertyID = m_nDefaultPropertyID;
					ModelResourceIndex nPropertyIndex1 = m_nDefaultPropertyIndex;
					ModelResourceIndex nPropertyIndex2 = m_nDefaultPropertyIndex;
					ModelResourceIndex nPropertyIndex3 = m_nDefaultPropertyIndex;

					if (pXMLNode->retrieveProperties(nPropertyID, nPropertyIndex1, nPropertyIndex2, nPropertyIndex3) || (nPropertyID != 0)) {
						// set potential default properties (i.e. used pid)
						m_nUsedPropertyID = nPropertyID;

						PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nPropertyID);
						if (pID.get()) {
							// Find and Assign Base Material Resource
							PModelBaseMaterialResource pBaseMaterial = m_pModel->findBaseMaterial(pID->getUniqueID());
							if (pBaseMaterial) {
								CMeshInformation_BaseMaterials * pBaseMaterials = createBaseMaterialInformation();
								MESHINFORMATION_BASEMATERIAL* pFaceData = (MESHINFORMATION_BASEMATERIAL*)pBaseMaterials->getFaceData(pFace->m_index);
								if (pFaceData) {
									pFaceData->m_nMaterialGroupID = pID->getUniqueID();
									pFaceData->m_nMaterialIndex = nPropertyIndex1;
								}
							}
						}

						// Find Color Material
						if (m_pColorMapping->hasResource(nPropertyID)) {
							CMeshInformation_NodeColors * pNodeColors = createNodeColorInformation();
							MESHINFORMATION_NODECOLOR* pFaceData = (MESHINFORMATION_NODECOLOR*)pNodeColors->getFaceData(pFace->m_index);
							if (pFaceData) {
								m_pColorMapping->findColor(nPropertyID, nPropertyIndex1, pFaceData->m_cColors[0]);
								m_pColorMapping->findColor(nPropertyID, nPropertyIndex2, pFaceData->m_cColors[1]);
								m_pColorMapping->findColor(nPropertyID, nPropertyIndex3, pFaceData->m_cColors[2]);
							}

						}

						// Find Texture Material
						if (m_pTexCoordMapping->hasResource(nPropertyID)) {
							CMeshInformation_TexCoords * pTexCoords = createTexCoordInformation();
							MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS*)pTexCoords->getFaceData(pFace->m_index);
							if (pFaceData) {
								ModelResourceID nTextureID1;
								ModelResourceID nTextureID2;
								ModelResourceID nTextureID3;
								m_pTexCoordMapping->findTexCoords(nPropertyID, nPropertyIndex1, nTextureID1, pFaceData->m_vCoords[0].m_fields[0], pFaceData->m_vCoords[0].m_fields[1]);
								m_pTexCoordMapping->findTexCoords(nPropertyID, nPropertyIndex2, nTextureID2, pFaceData->m_vCoords[1].m_fields[0], pFaceData->m_vCoords[1].m_fields[1]);
								m_pTexCoordMapping->findTexCoords(nPropertyID, nPropertyIndex3, nTextureID3, pFaceData->m_vCoords[2].m_fields[0], pFaceData->m_vCoords[2].m_fields[1]);

								if ((nTextureID1 == nTextureID2) && (nTextureID1 == nTextureID3) && (nTextureID2 == nTextureID3)) {
									PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->curPath(), nTextureID1);
									if (pID.get())
										pFaceData->m_TextureID = pID->getUniqueID();
								}
							}

						}


					}
				}
				else
					throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATEINDICES);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);

		}
	}

	ModelResourceID CModelReaderNode100_Triangles::getUsedPropertyID() const
	{
		return m_nUsedPropertyID;
	}

}
