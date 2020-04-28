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

NMR_ModelReaderNode093_Object.cpp implements the Model Reader Object Node Class.
A object reader model node is a parser for the object node of an XML Model
Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Object.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Mesh.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Components.h"

#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Object::CModelReaderNode093_Object(_In_ CModel * pModel, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pColorMapping.get() != nullptr);

		m_nID = 0;
		m_sType = "";
		m_bHasType = false;

		m_pModel = pModel;
		m_pObject = NULL;
		m_sThumbnail = "";
		m_sName = "";

		m_nObjectLevelColorID = 0;
		m_nObjectLevelMaterialID = 0;

		m_pColorMapping = pColorMapping;
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

		// Set Object Type (might fail, if string is invalid)
		if (m_sType.length() > 0) {
			if (!m_pObject->setObjectTypeString(m_sType, false))
				m_pWarnings->addWarning(NMR_ERROR_INVALIDMODELOBJECTTYPE, mrwInvalidOptionalValue);
		}
	}

	void CModelReaderNode093_Object::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);

			// Increase ID to avoid 0 (was allowed for 0.9.3)
			m_nID++;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_TYPE) == 0) {
			if (m_bHasType)
				throw CNMRException(NMR_ERROR_DUPLICATEOBJECTTYPE);

			// Convert to integer and make a input and range check!
			m_sType = pAttributeValue;
			m_bHasType = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_NAME) == 0) {
			m_sName = pAttributeValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_COLOR_ID) == 0) {

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX)) {
				m_nObjectLevelColorID = nValue + 1;
			}
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_OBJECT_MATERIALID) == 0) {
			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nObjectLevelMaterialID = nValue + 1;
		}


	}

	void CModelReaderNode093_Object::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {

			// Read a mesh object
			if (strcmp(pChildName, XML_3MF_ELEMENT_MESH) == 0) {
				// If we already have parsed an object, the node is duplicate
				if (m_pObject.get())
					throw CNMRException(NMR_ERROR_AMBIGUOUSOBJECTDEFINITON);

				// Create Empty Mesh
				PMesh pMesh = std::make_shared<CMesh>();
				// Create Mesh Object
				m_pObject = std::make_shared<CModelMeshObject>(m_nID, m_pModel, pMesh);

				PModelBaseMaterialResource pBaseMaterialResource;
				if (m_nObjectLevelMaterialID > 0) {
					// every v93 material becomes its own v100 base material group resource
					pBaseMaterialResource = std::dynamic_pointer_cast<NMR::CModelBaseMaterialResource>(m_pModel->findResource(m_pModel->currentPath(), m_nObjectLevelMaterialID));
					if (pBaseMaterialResource.get() == nullptr) {
						m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDMODELRESOURCE), mrwInvalidOptionalValue);
					}
				}

				// Read Mesh
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Mesh>(m_pModel, pMesh.get(), m_pColorMapping, pBaseMaterialResource, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Add Object to Parent
				m_pModel->addResource(m_pObject);

			}


			// Read a component object
			if (strcmp(pChildName, XML_3MF_ELEMENT_COMPONENTS) == 0) {
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
