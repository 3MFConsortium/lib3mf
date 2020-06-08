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
#include "Model/Reader/NMR_ModelReader_ColorMapping.h"

namespace NMR {

	CModelReaderNode100_Triangles::CModelReaderNode100_Triangles(_In_ CModel * pModel, _In_ CMesh * pMesh,
		_In_ PModelWarnings pWarnings, _In_ PPackageResourceID pObjectLevelPropertyID, _In_ ModelResourceIndex nDefaultPropertyIndex)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);

		m_pObjectLevelPropertyID = pObjectLevelPropertyID;
		m_nDefaultResourceIndex = nDefaultPropertyIndex;

		m_nUsedResourceID = 0;

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


	_Ret_notnull_ CMeshInformation_Properties * CModelReaderNode100_Triangles::createPropertiesInformation()
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

					ModelResourceID nModelResourceID = 0;
					if (m_pObjectLevelPropertyID)
						nModelResourceID = m_pObjectLevelPropertyID->getModelResourceID();
					ModelResourceIndex nResourceIndex1 = m_nDefaultResourceIndex;
					ModelResourceIndex nResourceIndex2 = m_nDefaultResourceIndex;
					ModelResourceIndex nResourceIndex3 = m_nDefaultResourceIndex;

					if (pXMLNode->retrieveProperties(nModelResourceID, nResourceIndex1, nResourceIndex2, nResourceIndex3) || (nModelResourceID != 0)) {
						// set potential default properties (i.e. used pid)
						m_nUsedResourceID = nModelResourceID;

						PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), nModelResourceID);
						if (pID.get()) {
							// Find and Assign Resource of this Property
							PModelResource pResource = m_pModel->findResource(pID->getUniqueID());
							if (pResource.get () != nullptr) {
								if (!pResource->hasResourceIndexMap())
									pResource->buildResourceIndexMap();

								ModelPropertyID pPropertyID1;
								ModelPropertyID pPropertyID2;
								ModelPropertyID pPropertyID3;
								if (pResource->mapResourceIndexToPropertyID(nResourceIndex1, pPropertyID1)
									&& pResource->mapResourceIndexToPropertyID(nResourceIndex2, pPropertyID2) 
									&& pResource->mapResourceIndexToPropertyID(nResourceIndex3, pPropertyID3)) {

									CMeshInformation_Properties * pProperties = createPropertiesInformation();
									MESHINFORMATION_PROPERTIES* pFaceData = (MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(pFace->m_index);
									if (pFaceData) {
										pFaceData->m_nUniqueResourceID = pID->getUniqueID();
										pFaceData->m_nPropertyIDs[0] = pPropertyID1;
										pFaceData->m_nPropertyIDs[1] = pPropertyID2;
										pFaceData->m_nPropertyIDs[2] = pPropertyID3;
									}
								} else {
									m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDMESHINFORMATIONINDEX), mrwInvalidOptionalValue);
								}
							}
						}
						else {
							m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDMODELRESOURCE), mrwInvalidOptionalValue);
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
		return m_nUsedResourceID;
	}

}
