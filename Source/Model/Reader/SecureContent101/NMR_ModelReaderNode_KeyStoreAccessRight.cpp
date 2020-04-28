
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

NMR_ModelReaderNode_KeyStoreAccessRight.h defines the Model Reader Node class that is related to <Accessright>.

--*/

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreAccessRight.h"
#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCipherValue.h"
#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreKEKParams.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreFactory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	PKeyStoreAccessRight CModelReaderNode_KeyStoreAccessRight::getAccessRight()
	{
		return CKeyStoreFactory::makeAccessRight(
			m_pKeyStore->getConsumer(m_nConsumerIndex), 
			m_sParams.m_eAlgorithm,
			m_sParams.m_eMgf,
			m_sParams.m_eDigest, 
			m_rgCipherValue);
	}

	void CModelReaderNode_KeyStoreAccessRight::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// this can throw for values that are not numbers and it's ok so according to other reader nodes
		if (!m_bHasConsumerIndex) {
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGCONSUMERINDEX, eModelWarningLevel::mrwMissingMandatoryValue);
			m_nConsumerIndex = 0;
		}
		if (m_nConsumerIndex >= m_pKeyStore->getConsumerCount())
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX, eModelWarningLevel::mrwFatal);
		if (!m_bHasParams)
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGKEKPARAMS, eModelWarningLevel::mrwFatal);
		if (!m_bHasCipherData)
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGCIPHERDATA, eModelWarningLevel::mrwFatal);
	}

	void CModelReaderNode_KeyStoreAccessRight::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, pAttributeName) == 0) {
			if (m_bHasConsumerIndex)
				m_pWarnings->addWarning(NMR_ERROR_KEYSTOREDUPLICATECONSUMERINDEX, eModelWarningLevel::mrwInvalidMandatoryValue);
			m_bHasConsumerIndex = true;
			m_nConsumerIndex = fnStringToUint32(pAttributeValue);
		}
		else
			m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE, mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreAccessRight::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_KEKPARAMS) == 0) {
				if (!m_bHasParams) {
					m_bHasParams = true;
					PModelReaderNode_KeyStoreKEKParams pXMLNode = extractCopy<CModelReaderNode_KeyStoreKEKParams>();
					pXMLNode->parseXML(pXMLReader);
					m_sParams = pXMLNode->getKekParams();
				}
			} else if (strcmp(pChildName, XML_3MF_ELEMENT_CIPHERDATA) == 0) {
				if (!m_bHasCipherData) {
					m_bHasCipherData = true;
					PModelReaderNode_KeyStoreCipherValue pXMLNode = extractCopy<CModelReaderNode_KeyStoreCipherValue>();
					pXMLNode->parseXML(pXMLReader);
					m_rgCipherValue = pXMLNode->getCipherValue();
				}
			} else {
				m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ELEMENT, mrwInvalidOptionalValue);
			}
		}
	}

}
