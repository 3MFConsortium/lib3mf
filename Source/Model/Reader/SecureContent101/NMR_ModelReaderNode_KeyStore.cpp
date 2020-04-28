
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

NMR_ModelReaderNode_KeyStore.h defines the Model Reader Node class that is related to <keystore>.

--*/

// TODO: check if needs #include <stdlib.h>
// https://stackoverflow.com/questions/1646031/strtoull-and-long-long-arithmetic

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStore.h"
#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreConsumer.h"
#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreResourceDataGroup.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	void CModelReaderNode_KeyStore::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// Set references
		if (!m_UUID.get()) {
			// We do not have to check for secure content spec, because it is the base spec of a keystore
			m_pWarnings->addWarning(NMR_ERROR_MISSINGUUID, mrwMissingMandatoryValue);
			m_UUID = std::make_shared<CUUID>();
		}
		keystore()->setUUID(m_UUID);
	}

	void CModelReaderNode_KeyStore::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_UUID, pAttributeName) == 0) {
			if (m_UUID.get())
				m_pWarnings->addWarning(NMR_ERROR_DUPLICATEUUID, eModelWarningLevel::mrwInvalidMandatoryValue);
			// this can throw for invalid UUIDs and it's ok so according to other reader nodes
			m_UUID = std::make_shared<CUUID>(pAttributeValue);
		}
		else if (strcmp(XML_3MF_ATTRIBUTE_XMLNS, pAttributeName) != 0)
			m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE, mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStore::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
			// Read a consumer
			if (strcmp(pChildName, XML_3MF_ELEMENT_CONSUMER) == 0) {
				PModelReaderNode_KeyStoreConsumer pXMLNode = extractCopy<CModelReaderNode_KeyStoreConsumer>();
				pXMLNode->parseXML(pXMLReader);
				// consumer adds itself to m_pKeyStore, nothing else to do here
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_RESOURCEDATAGROUP) == 0) {
				PModelReaderNode_KeyStoreResourceDataGroup pXMLNode = extractCopy<CModelReaderNode_KeyStoreResourceDataGroup>();
				pXMLNode->parseXML(pXMLReader);
				// resource data adds itself to m_pKeyStore, nothing else to do here
			}
			else
				m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ELEMENT, mrwInvalidOptionalValue);
		}
	}

}
