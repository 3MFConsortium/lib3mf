
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

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreCipherValue.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	// BEGIN PLACEHOLDER
	// this is placeholder code until we have proper decoding
	std::vector<nfByte> decode(std::string cipherValue) {
		std::vector<nfByte> iv = { 0xa4, 0xfc, 0xa, 0xe8, 0x4, 0x9, 0x6f, 0x29, 0xb3, 0xe6, 0x4b, 0x31 };
		std::vector<nfByte> key = { 0x55, 0x71, 0x20, 0x42, 0xd5, 0xee, 0x8b, 0x72, 0xf8, 0xb0, 0x70, 0x8e, 0x3a, 0xe1, 0x66, 0xda, 0x8c, 0xd0, 0x8d, 0x16, 0xc5, 0xea, 0xb4, 0x96, 0x6d, 0x44, 0x23, 0xc2, 0x65, 0xcb, 0xc0, 0x2f };
		std::vector<nfByte> tag = { 0xaa, 0x2e, 0x9a, 0x20, 0x30, 0x5, 0x30, 0xe3, 0xf5, 0xc9, 0x72, 0xb8, 0xd0, 0x98, 0x3, 0x74 };
		std::vector<nfByte> decoded = {};
		decoded.insert(decoded.end(), iv.begin(), iv.end());
		decoded.insert(decoded.end(), key.begin(), key.end());
		decoded.insert(decoded.end(), tag.begin(), tag.end());
		return decoded;
	}
	// END PLACEHOLDER

	CModelReaderNode_KeyStoreCipherValue::CModelReaderNode_KeyStoreCipherValue(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{
	}

	CIPHERVALUE CModelReaderNode_KeyStoreCipherValue::getCipherValue()
	{
		return m_sCipherValue;
	}

	void CModelReaderNode_KeyStoreCipherValue::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_KeyStoreCipherValue::OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		std::string pTextString = std::string(pText);
		std::remove(pTextString.begin(), pTextString.end(), '\r');
		std::remove(pTextString.begin(), pTextString.end(), '\n');

		std::vector<nfByte> decoded = decode(pTextString);
		auto decodedIvBegin = decoded.begin();
		auto decodedKeyBegin = decodedIvBegin + KEYSTORE_TYPES_IVSIZE;
		auto decodedTagBegin = decoded.end() - KEYSTORE_TYPES_TAGSIZE;

		m_sCipherValue.m_iv = std::vector<nfByte>(decodedIvBegin, decodedKeyBegin);
		m_sCipherValue.m_key = std::vector<nfByte>(decodedKeyBegin, decodedTagBegin);
		m_sCipherValue.m_tag = std::vector<nfByte>(decodedTagBegin, decoded.end());
	}

}
