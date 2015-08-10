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

NMR_ModelReaderNode100_Object.cpp implements the Model Reader Object Node Class.
A object reader model node is a parser for the object node of an XML Model
Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Object.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Mesh.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Components.h"

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

	CModelReaderNode100_Object::CModelReaderNode100_Object(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelReader_TexCoordMapping pTexCoordMapping)
		: CModelReaderNode(pWarnings)
	{
		// Initialize variables
		if (!pColorMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!pTexCoordMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_nID = 0;
		m_sType = L"";
		m_bHasType = false;

		m_pModel = pModel;
		m_pObject = NULL; 
		m_sThumbnail = L"";
		m_sPartNumber = L"";
		m_sName = L"";

		m_nDefaultPropertyID = 0;
		m_nDefaultPropertyIndex = 0;

		m_pColorMapping = pColorMapping;
		m_pTexCoordMapping = pTexCoordMapping;
	}

	void CModelReaderNode100_Object::parseXML(_In_ CXmlReader * pXMLReader)
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
		m_pObject->setPartNumber(m_sPartNumber);

		// Set Object Type (might fail, if string is invalid)
		if (m_sType.length() > 0) {
			if (!m_pObject->setObjectTypeString(m_sType, false))
				m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMODELOBJECTTYPE, NMR_ERROR_INVALIDMODELOBJECTTYPE, mrwInvalidOptionalValue);
		}
	}

	void CModelReaderNode100_Object::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTID);

			// Convert to integer and make a input and range check!
			m_nID = fnWStringToUint32(pAttributeValue);
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

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER) == 0) {
			std::wstring sValue(pAttributeValue);
			m_sPartNumber = sValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PID) == 0) {
			m_nDefaultPropertyID = fnWStringToUint32(pAttributeValue);
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_PINDEX) == 0) {
			m_nDefaultPropertyIndex = fnWStringToUint32(pAttributeValue);
		}

	}

	void CModelReaderNode100_Object::OnNSChildElement(_In_z_ const nfWChar * pChildName, _In_z_ const nfWChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (wcscmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {

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
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Mesh>(m_pModel, pMesh.get(), m_pWarnings, m_pColorMapping, m_pTexCoordMapping, m_nDefaultPropertyID, m_nDefaultPropertyIndex);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

				// Create Default Properties
				createDefaultProperties();

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
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Components>(pComponentsObject.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

				// Create default Properties
				createDefaultProperties();

			}
		}

	}

	void CModelReaderNode100_Object::createDefaultProperties()
	{
		if (m_pObject.get() == nullptr)
			return;

		if (m_nDefaultPropertyID != 0) {
			// Assign Default Resource Property
			CModelBaseMaterialResource * pResource = m_pModel->findBaseMaterial(m_nDefaultPropertyID);
			if (pResource != nullptr) {
				m_pObject->setDefaultProperty (std::make_shared<CModelDefaultProperty_BaseMaterial>(m_nDefaultPropertyID, m_nDefaultPropertyIndex));
			}

			// Assign Default ColorResource Property
			nfColor cColor;
			if (m_pColorMapping->findColor(m_nDefaultPropertyID, m_nDefaultPropertyIndex, cColor)) {
				m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));
			}

			// Assign Default TextureResource Property
			ModelResourceID nTextureID;
			nfFloat fU;
			nfFloat fV;
			if (m_pTexCoordMapping->findTexCoords(m_nDefaultPropertyID, m_nDefaultPropertyIndex, nTextureID, fU, fV)) {
				m_pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_TexCoord2D>(nTextureID, fU, fV));
			}

		}
	}

}
