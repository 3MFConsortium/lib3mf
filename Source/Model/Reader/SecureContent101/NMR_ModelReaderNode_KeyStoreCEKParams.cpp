
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

NMR_ModelReaderNode_KeyStoreCipherValue.h defines the Model Reader Node class that is related to <xenc:CipherValue>.

--*/

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCEKParams.h"
#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCipherValue.h"
#include "Model/Reader/NMR_ModelReaderNode_StringValue.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Model/Classes/NMR_KeyStoreFactory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "base64.h"

namespace NMR {
	namespace ParserUtils {
		eKeyStoreEncryptAlgorithm parseEncryptionAlgorithm(std::string const & value) {
			if (XML_3MF_SECURE_CONTENT_ENCRYPTION_AES256 == value) {
				return eKeyStoreEncryptAlgorithm::AES256_GCM;
			}
			throw CNMRException(NMR_ERROR_KEYSTOREINVALIDALGORITHM);
		}
	}

	PKeyStoreCEKParams CModelReaderNode_KeyStoreCEKParams::getCEKParams()
	{
		return CKeyStoreFactory::makeCEKParams(m_bCompressed, m_eAlgorithm, m_aad, m_iv, m_tag);
	}

	void CModelReaderNode_KeyStoreCEKParams::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
		if (!m_bHasAlgorithm)
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGALGORTHM, eModelWarningLevel::mrwMissingMandatoryValue);
	}

	void CModelReaderNode_KeyStoreCEKParams::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_ENCRYPTION_ALGORITHM, pAttributeName) == 0) {
			try {
				m_bHasAlgorithm = true;
				m_eAlgorithm = ParserUtils::parseEncryptionAlgorithm(pAttributeValue);
			} catch (CNMRException const &) {
				m_eAlgorithm = eKeyStoreEncryptAlgorithm::AES256_GCM;
				m_pWarnings->addWarning(NMR_ERROR_KEYSTOREINVALIDALGORITHM, eModelWarningLevel::mrwInvalidMandatoryValue);
			}
		}
		else if (strcmp(XML_3MF_SECURE_CONTENT_COMPRESSION, pAttributeName) == 0) {
			if (strcmp(XML_3MF_SECURE_CONTENT_COMPRESSION_DEFLATE, pAttributeValue) == 0) {
				m_bCompressed = true;
			} else if (strcmp(XML_3MF_SECURE_CONTENT_COMPRESSION_NONE, pAttributeValue) == 0) {
				m_bCompressed = false;
			} else {
				m_pWarnings->addWarning(NMR_ERROR_KEYSTOREINVALIDCOMPRESSION, mrwInvalidOptionalValue);
			}
		}
		else
			m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE, mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreCEKParams::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);
		try{
			if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
				if (strcmp(pChildName, XML_3MF_SECURE_CONTENT_IV) == 0) {
					PModelReaderNode_StringValue pXMLNode = std::make_shared<CModelReaderNode_StringValue>(m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					std::string result = base64_decode(pXMLNode->getValue());
					m_iv = std::vector<nfByte>(result.begin(), result.end());
				} else if (strcmp(pChildName, XML_3MF_SECURE_CONTENT_TAG) == 0) {
					PModelReaderNode_StringValue pXMLNode = std::make_shared<CModelReaderNode_StringValue>(m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					std::string result = base64_decode(pXMLNode->getValue());
					m_tag = std::vector<nfByte>(result.begin(), result.end());
				} else if (strcmp(pChildName, XML_3MF_SECURE_CONTENT_AAD) == 0) {
					PModelReaderNode_StringValue pXMLNode = std::make_shared<CModelReaderNode_StringValue>(m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					std::string result = base64_decode(pXMLNode->getValue());
					m_aad = std::vector<nfByte>(result.begin(), result.end());
				} else {
					m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ELEMENT, mrwInvalidOptionalValue);
				}
			}
		}
		catch (...) {
			// base64_decode throws an exception if the input is not valid base64
			m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREINVALIDENCODING), mrwInvalidOptionalValue);
        }
	}
}
