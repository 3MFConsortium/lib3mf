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

NMR_ModelReaderNode100_Components.cpp implements the Model Components Reader
Node Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Components.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Component.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Components::CModelReaderNode100_Components(_In_ CModelComponentsObject * pComponentsObject, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pComponentsObject);
		m_pComponentsObject = pComponentsObject;
	}

	void CModelReaderNode100_Components::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Components::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode100_Components::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_COMPONENT) == 0) {
				// Read Component
				PModelReaderNode100_Component pXMLNode = std::make_shared<CModelReaderNode100_Component>(m_pComponentsObject->getModel(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Retrieve object and transform
				CModelObject * pObject = pXMLNode->getObject();
				NMATRIX3 mTransform = pXMLNode->getTransform();

				// Check, if we have an associated object
				if (!pObject)
					throw CNMRException(NMR_ERROR_COULDNOTFINDCOMPONENTOBJECT);

				// Create component
				PModelComponent pComponent = std::make_shared<CModelComponent>(pObject, mTransform);
				
				// Set Production references
				PUUID uuid = pXMLNode->uuid();
				if (!uuid.get()) {
					if (pXMLReader->NamespaceRegistered(XML_3MF_NAMESPACE_PRODUCTIONSPEC)) {
						m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGUUID), mrwMissingMandatoryValue);
					}
					uuid = std::make_shared<CUUID>();
				}
				pComponent->setUUID(uuid);

				m_pComponentsObject->addComponent(pComponent);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
