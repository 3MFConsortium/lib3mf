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

NMR_ModelReaderNode093_Triangles.cpp implements the Model Reader Triangles Node
Class. A triangles reader model node is a parser for the triangles node of an
XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangles.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangle.h"

#include "Model/Classes/NMR_ModelTexture2D.h"
#include "Model/Classes/NMR_ModelTexture2DGroup.h" 
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Triangles::CModelReaderNode093_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping, _In_ PModelBaseMaterialResource pMaterialResource, _In_ PModelWarnings pWarnings)
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
		m_pDefaultMaterialResource = pMaterialResource;
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

				nfInt32 nTextureIndex1, nTextureIndex2, nTextureIndex3;

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
							pXMLNode->retrieveTextureIndices(nTextureIndex1, nTextureIndex2, nTextureIndex3);
						}
					}

					// Create Texture Info
					if (nTextureID > 0) {
						CMeshInformation_Properties * pProperties = createPropertiesInformation();
						MESHINFORMATION_PROPERTIES* pFaceData = (MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(pFace->m_index);
						if (pFaceData) {

							PModelTexture2DResource pTexture2dResource;

							// Convert Texture Coordinates to in memory representation
							PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), nTextureID);
							if (pID.get()) {
								pTexture2dResource = std::dynamic_pointer_cast<NMR::CModelTexture2DResource>(m_pModel->findResource(pID->getUniqueID()));
							}
							if (!pTexture2dResource)
								throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

							// find or create ModelTexture2DGroup that points to this texture
							CModelTexture2DGroupResource* pTextureGroup = nullptr;
							for (nfUint32 iGroup = 0; iGroup < m_pModel->getTexture2DGroupCount(); iGroup++) {
								auto p = m_pModel->getTexture2DGroup(iGroup);
								if (p->getTexture2D() == pTexture2dResource) {
									pTextureGroup = p;
									break;
								}
							}
							if (!pTextureGroup) {
								// Create Resource
								// This is pretty bad way to set a new Model ResourceID, but we do not have read all resources yet...
								ModelResourceID nNewModelResourceID = (ModelResourceID)((long)XML_3MF_MAXRESOURCEINDEX - 1 - (long)m_pModel->getTexture2DGroupCount());
								
								PModelTexture2DGroupResource pSharedTextureGroup = std::make_shared<CModelTexture2DGroupResource>(nNewModelResourceID, m_pModel, pTexture2dResource);
								m_pModel->addResource(pSharedTextureGroup);
								pTextureGroup = pSharedTextureGroup.get();
							}

							pFaceData->m_nUniqueResourceID = pTextureGroup->getPackageResourceID()->getUniqueID();
							ModelResourceIndex nResourceIndex;
							MODELTEXTURE2DCOORDINATE sUV;
							nfFloat fU;
							nfFloat fV;
							m_pTexCoordMapping->findTexCoords(1, nTextureIndex1, nResourceIndex, fU, fV);
							sUV.m_dU = fU;
							sUV.m_dV = fV;
							pFaceData->m_nPropertyIDs[0] = pTextureGroup->addUVCoordinate(sUV);

							m_pTexCoordMapping->findTexCoords(1, nTextureIndex2, nResourceIndex, fU, fV);
							sUV.m_dU = fU;
							sUV.m_dV = fV;
							pFaceData->m_nPropertyIDs[1] = pTextureGroup->addUVCoordinate(sUV);

							m_pTexCoordMapping->findTexCoords(1, nTextureIndex3, nResourceIndex, fU, fV);
							sUV.m_dU = fU;
							sUV.m_dV = fV;
							pFaceData->m_nPropertyIDs[2] = pTextureGroup->addUVCoordinate(sUV);
						}
					}
					else {
						if ((nColorID1 > 0) && (nColorID2 > 0) && (nColorID3 > 0)) {
							throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
						}
						else {
							if (nMaterialID > 0) {
								// every v93 material becomes its own v100 base material group resource
								PModelBaseMaterialResource pBaseMaterialResource = std::dynamic_pointer_cast<NMR::CModelBaseMaterialResource>(m_pModel->findResource(m_pModel->currentPath(), nMaterialID));
								if (pBaseMaterialResource.get() != nullptr) {
									if (!pBaseMaterialResource->hasResourceIndexMap())
										pBaseMaterialResource->buildResourceIndexMap();

									CMeshInformation_Properties * pProperties = createPropertiesInformation();
									MESHINFORMATION_PROPERTIES* pFaceData = (MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(pFace->m_index);
									if (pFaceData) {
										pFaceData->m_nUniqueResourceID = pBaseMaterialResource->getPackageResourceID()->getUniqueID();
										pFaceData->m_nPropertyIDs[0] = 1;
										pFaceData->m_nPropertyIDs[1] = 1;
										pFaceData->m_nPropertyIDs[2] = 1;
									}

									MESHINFORMATION_PROPERTIES* pDefaultData = (MESHINFORMATION_PROPERTIES*)pProperties->getDefaultData();
									if (m_pDefaultMaterialResource != nullptr) {
										if (!pDefaultData) {
											pDefaultData = new MESHINFORMATION_PROPERTIES;
											pDefaultData->m_nUniqueResourceID = 0;
											pDefaultData->m_nPropertyIDs[0] = 1;
											pDefaultData->m_nPropertyIDs[1] = 1;
											pDefaultData->m_nPropertyIDs[2] = 1;
											pProperties->setDefaultData((MESHINFORMATIONFACEDATA*)pDefaultData);
										}
										if (pDefaultData->m_nUniqueResourceID == 0) {
											pDefaultData->m_nUniqueResourceID = m_pDefaultMaterialResource->getPackageResourceID()->getUniqueID();
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

	_Ret_notnull_ CMeshInformation_Properties * CModelReaderNode093_Triangles::createPropertiesInformation()
	{
		CMeshInformationHandler * pMeshInformationHandler = m_pMesh->createMeshInformationHandler();

		CMeshInformation * pInformation = pMeshInformationHandler->getInformationByType(0, emiProperties);
		CMeshInformation_Properties * pProperties = nullptr;

		if (pInformation)
			pProperties = dynamic_cast<CMeshInformation_Properties *> (pInformation);

		if (!pProperties) {
			PMeshInformation_Properties pNewMeshInformation = std::make_shared<CMeshInformation_Properties>(m_pMesh->getFaceCount());
			pMeshInformationHandler->addInformation(pNewMeshInformation);

			pProperties = pNewMeshInformation.get();
		}

		return pProperties;
	}
}



