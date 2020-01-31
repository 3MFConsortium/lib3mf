
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

NMR_ModelReaderNode_KeyStoreDecryptRight.h defines the Model Reader Node class that is related to <decryptright>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreDecryptRight.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreCipherValue.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {


	CModelReaderNode_KeyStoreDecryptRight::CModelReaderNode_KeyStoreDecryptRight(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{
		m_bHasCipherData = false;
	}

	PARSEDDECRYPTRIGHT CModelReaderNode_KeyStoreDecryptRight::GetParsedDecryptRight()
	{
		return m_parsedDecryptRight;
	}

	void CModelReaderNode_KeyStoreDecryptRight::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// TODO: check that all m_parsedDecryptRight properties are set
	}

	void CModelReaderNode_KeyStoreDecryptRight::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		

		if (strcmp(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, pAttributeName) == 0) {
			if (!m_parsedDecryptRight.m_consumerIndex.empty())
				m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATEKEYSTORECONSUMERINDEX), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
			m_parsedDecryptRight.m_consumerIndex = pAttributeValue;
		}
		else if (strcmp(XML_3MF_SECURE_CONTENT_ENCRYPTION_ALGORITHM, pAttributeName) == 0) {
			if (strcmp(XML_3MF_SECURE_CONTENT_ENCRYPTION_AES256, pAttributeValue) == 0) {
				m_parsedDecryptRight.m_encryptionAlgorithm = eKeyStoreEncryptAlgorithm::Aes256Gcm;
			}
			else if (strcmp(XML_3MF_SECURE_CONTENT_ENCRYPTION_RSA, pAttributeValue) == 0) {
				m_parsedDecryptRight.m_encryptionAlgorithm = eKeyStoreEncryptAlgorithm::RsaOaepMgf1p;
			}
			else {
				// TODO: what to do now?
			}
		}
	}

	void CModelReaderNode_KeyStoreDecryptRight::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pNameSpace);

	}

	void CModelReaderNode_KeyStoreDecryptRight::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);


		if (strcmp(pChildName, XML_3MF_ELEMENT_CIPHERDATA) == 0) {
			m_bHasCipherData = true;
		}
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CIPHERVALUESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_CIPHERVALUE) == 0) {
				if (m_bHasCipherData) {
					PModelReaderNode_KeyStoreCipherValue pXMLNode = std::make_shared<CModelReaderNode_KeyStoreCipherValue>(m_pKeyStore, m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					m_parsedDecryptRight.m_cipherValue = pXMLNode->GetCipherValue();
				}
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
	}

}
