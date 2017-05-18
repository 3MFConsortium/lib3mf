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

NMR_ModelReaderNode093_Object.cpp implements the Model Reader Object Node Class.
A object reader model node is a parser for the object node of an XML Model
Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Object.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Mesh.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Components.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Model/Classes/NMR_ModelDefaultProperty.h"
#include "Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.h"
#include "Model/Classes/NMR_ModelDefaultProperty_Color.h"
#include "Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Object::CModelReaderNode093_Object(_In_ CModel * pModel, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelBaseMaterialResource pMaterialResource, _In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pColorMapping.get() != nullptr);

		m_nID = 0;
		m_sType = L"";
		m_bHasType = false;

		m_pModel = pModel;
		m_pObject = NULL;
		m_sThumbnail = L"";
		m_sName = L"";

		m_nColorID = 0;
		m_nMaterialID = 0;

		m_pColorMapping = pColorMapping;
		m_pMaterialResource = pMaterialResource;

	}

	void CModelReaderNode093_Object::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELOBJECTID);

		// Parse Content
		parseContent(pXMLReader);

		// Check, if we have created an object
		if (!m_pObject.get())
			throw CNMRException(NMR_ERROR_MISSINGOBJECTCONTENT);

		// Set Object Parameters
		m_pObject->setName(m_sName);


		// Set Default Parameters
		if (m_nColorID > 0) {

			nfColor cColor = 0;
			if (m_pColorMapping->findColor(m_nColorID, 0, cColor)) {
				m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));
			}
			else {
				if (m_pColorMapping->hasTextureReference(m_nColorID)) {
					m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_TexCoord2D>(m_nColorID, 0.0f, 0.0f));
				}
			}

		}
		else {

			if (m_nMaterialID > 0) {
				ModelResourceID nMaterialGroupID = 0;
				ModelResourceIndex nMaterialIndex = 0;

				if (m_pMaterialResource.get() != nullptr) {
					ModelResourceIndex nIndex;
					if (m_pColorMapping->getMaterialReference(m_nMaterialID, nIndex)) {
						nMaterialGroupID = m_pMaterialResource->getResourceID()->getUniqueID();
						nMaterialIndex = nIndex;
					}
				}

				if (nMaterialGroupID > 0) {
					m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_BaseMaterial>(nMaterialGroupID, nMaterialIndex));
				}
			}

		}


		// Set Object Type (might fail, if string is invalid)
		if (m_sType.length() > 0) {
			if (!m_pObject->setObjectTypeString(m_sType, false))
				m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMODELOBJECTTYPE, NMR_ERROR_INVALIDMODELOBJECTTYPE, mrwInvalidOptionalValue);
		}
	}

	void CModelReaderNode093_Object::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTID);

			// Convert to integer and make a input and range check!
			m_nID = fnWStringToUint32(pAttributeValue);

			// Increase ID to avoid 0 (was allowed for 0.9.3)
			m_nID++;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_TYPE) == 0) {
			if (m_bHasType)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTTYPE);

			// Convert to integer and make a input and range check!
			std::wstring sString(pAttributeValue);
			m_sType = sString;
			m_bHasType = true;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_NAME) == 0) {
			std::wstring sValue(pAttributeValue);
			m_sName = sValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_COLOR_ID) == 0) {

			nfInt32 nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX)) {
				m_nColorID = nValue + 1;
			}
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_MATERIALID) == 0) {
			nfInt32 nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nMaterialID = nValue + 1;
		}


	}

	void CModelReaderNode093_Object::OnNSChildElement(_In_z_ const nfWChar * pChildName, _In_z_ const nfWChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((wcscmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (wcscmp(pNameSpace, L"") == 0)) {

			// Read a mesh object
			if (wcscmp(pChildName, XML_3MF_ELEMENT_MESH) == 0) {
				// If we already have parsed an object, the node is duplicate
				if (m_pObject.get())
					throw CNMRException(NMR_ERROR_AMBIGUOUSOBJECTDEFINITON);

				// Create Empty Mesh
				PMesh pMesh = std::make_shared<CMesh>();
				// Create Mesh Object
				m_pObject = std::make_shared<CModelMeshObject>(m_nID, m_pModel, pMesh);

				// Read Mesh
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Mesh>(m_pModel, pMesh.get(), m_pColorMapping, m_pMaterialResource, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

			}


			// Read a component object
			if (wcscmp(pChildName, XML_3MF_ELEMENT_COMPONENTS) == 0) {
				// If we already have parsed an object, the node is duplicate
				if (m_pObject.get())
					throw CNMRException(NMR_ERROR_AMBIGUOUSOBJECTDEFINITON);

				// Create Component List Object
				PModelComponentsObject pComponentsObject = std::make_shared<CModelComponentsObject>(m_nID, m_pModel);
				m_pObject = pComponentsObject;

				// Read Components
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Components>(pComponentsObject.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

			}


		}
	}


}
