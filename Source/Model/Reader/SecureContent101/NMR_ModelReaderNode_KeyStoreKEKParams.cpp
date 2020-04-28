
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

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreKEKParams.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "Libraries/cpp-base64/base64.h"

namespace NMR {

	namespace ParserUtils {
		eKeyStoreWrapAlgorithm parseWrapAlgorithm(std::string const & value, bool & hasMgf1) {
			if (XML_3MF_SECURE_CONTENT_KEYWRAPPING_RSA == value) {
				hasMgf1 = true;
				return eKeyStoreWrapAlgorithm::RSA_OAEP;
			} else if (XML_3MF_SECURE_CONTENT_KEYWRAPPING_RSASHORT == value) {
				return eKeyStoreWrapAlgorithm::RSA_OAEP;
			}
			throw CNMRException(NMR_ERROR_KEYSTOREINVALIDALGORITHM);
		}

		eKeyStoreMaskGenerationFunction parseMgf(std::string const & value) {
			if (XML_3MF_SECURE_CONTENT_MGF1_SHA1 == value)
				return eKeyStoreMaskGenerationFunction::MGF1_SHA1;
			else if (XML_3MF_SECURE_CONTENT_MGF1_SHA224 == value)
				return eKeyStoreMaskGenerationFunction::MGF1_SHA224;
			else if (XML_3MF_SECURE_CONTENT_MGF1_SHA256 == value)
				return eKeyStoreMaskGenerationFunction::MGF1_SHA256;
			else if (XML_3MF_SECURE_CONTENT_MGF1_SHA384 == value)
				return eKeyStoreMaskGenerationFunction::MGF1_SHA384;
			else if (XML_3MF_SECURE_CONTENT_MGF1_SHA512 == value)
				return eKeyStoreMaskGenerationFunction::MGF1_SHA512;
			throw CNMRException(NMR_ERROR_KEYSTOREINVALIDMGF);
		}

		eKeyStoreMessageDigest parseMessageDigest(std::string const & value) {
			if (XML_3MF_SECURE_CONTENT_MD_SHA1 == value)
				return eKeyStoreMessageDigest::SHA1;
			else if (XML_3MF_SECURE_CONTENT_MD_SHA256 == value)
				return eKeyStoreMessageDigest::SHA256;
			else if (XML_3MF_SECURE_CONTENT_MD_SHA384 == value)
				return eKeyStoreMessageDigest::SHA384;
			else if (XML_3MF_SECURE_CONTENT_MD_SHA512 == value)
				return eKeyStoreMessageDigest::SHA512;
			throw CNMRException(NMR_ERROR_KEYSTOREINVALIDDIGEST);
		}
	}

	KEKPARAMS CModelReaderNode_KeyStoreKEKParams::getKekParams()
	{
		return m_sKekParams;
	}

	void CModelReaderNode_KeyStoreKEKParams::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		if (!m_bHasAlgorithm)
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREMISSINGALGORTHM, eModelWarningLevel::mrwMissingMandatoryValue);

		if (m_bAlgHasMgf && m_sKekParams.m_eMgf != eKeyStoreMaskGenerationFunction::MGF1_SHA1)
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREINCONSISTENTKEKPARAMS, eModelWarningLevel::mrwInvalidOptionalValue);
	}

	void CModelReaderNode_KeyStoreKEKParams::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		try {
			if (strcmp(XML_3MF_SECURE_CONTENT_WRAPPINGALGORITHM, pAttributeName) == 0) {
				m_bHasAlgorithm = true;
				m_sKekParams.m_eAlgorithm = ParserUtils::parseWrapAlgorithm(pAttributeValue, m_bAlgHasMgf);
			} else if (strcmp(XML_3MF_SECURE_CONTENT_MGFALGORITHM, pAttributeName) == 0) {
				m_sKekParams.m_eMgf = ParserUtils::parseMgf(pAttributeValue);
			} else if (strcmp(XML_3MF_SECURE_CONTENT_DIGESTMETHOD, pAttributeName) == 0) {
				m_sKekParams.m_eDigest = ParserUtils::parseMessageDigest(pAttributeValue);
			} else {
				m_pWarnings->addWarning(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE, mrwInvalidOptionalValue);
			}
		}
		catch (CNMRException const & e) {
			m_pWarnings->addException(e, eModelWarningLevel::mrwInvalidMandatoryValue);
		}
	}

}
