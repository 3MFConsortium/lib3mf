
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

NMR_ModelReaderNode_KeyStoreConsumer.h defines the Model Reader Node class that is related to <consumer>.

--*/

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreConsumer.h"
#include "Model/Reader/NMR_ModelReaderNode_StringValue.h"
#include "Model/Classes/NMR_KeyStoreFactory.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	void CModelReaderNode_KeyStoreConsumer::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// check consumer id
		if (m_sConsumerID.empty()) {
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGCONSUMERID, mrwMissingMandatoryValue);
			//add some default value
			m_sConsumerID = "ConsumerID" + fnInt32ToString(m_pWarnings->getWarningCount());
		}

		PKeyStoreConsumer c = CKeyStoreFactory::makeConsumer(m_sConsumerID, m_sKeyID, m_sKeyValue);
		m_pKeyStore->addConsumer(c);
	}

	void CModelReaderNode_KeyStoreConsumer::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_CONSUMER_ID, pAttributeName) == 0) {
			if (!m_sConsumerID.empty())
				m_pWarnings->addWarning(NMR_ERROR_KEYSTOREDUPLICATECONSUMERID, eModelWarningLevel::mrwInvalidMandatoryValue);
			m_sConsumerID = pAttributeValue;
		}
		else if (strcmp(XML_3MF_SECURE_CONTENT_KEY_ID, pAttributeName) == 0) {
			if (!m_sKeyID.empty())
				m_pWarnings->addWarning(NMR_ERROR_KEYSTOREDUPLICATECONSUMERKEYID, eModelWarningLevel::mrwInvalidOptionalValue);
			m_sKeyID = pAttributeValue;
		}
		else
			m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE, mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreConsumer::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);


		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_KEYVALUE) == 0) {
				PModelReaderNode_StringValue pXMLNode = std::make_shared<CModelReaderNode_StringValue>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				m_sKeyValue = pXMLNode->getValue();
			}
			else
				m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ELEMENT, mrwInvalidOptionalValue);
		}
	}

}
