
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

NMR_ModelReaderNode_KeyStoreExponent.h defines the Model Reader Node class that is related to <ds:Exponent>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStore.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreExponent.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode_KeyStoreExponent::CModelReaderNode_KeyStoreExponent(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{

	}

	nfByte * CModelReaderNode_KeyStoreExponent::GetExponent()
	{
		return m_exponent;
	}

	void CModelReaderNode_KeyStoreExponent::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_KeyStoreExponent::OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		std::string pTextString = std::string(pText);
		std::remove(pTextString.begin(), pTextString.end(), '\r');
		std::remove(pTextString.begin(), pTextString.end(), '\n');

		size_t copySize = KEYSTORE_TYPES_EXPONENTBUFFERSIZE > pTextString.size() ? pTextString.size() : KEYSTORE_TYPES_EXPONENTBUFFERSIZE;
		memcpy(m_exponent, pTextString.c_str(), copySize);
		m_exponent[copySize - 1] = '\0';
		// TODO: decide whether or not to throw
		//if (KEYSTORE_TYPES_EXPONENTBUFFERSIZE < strlen(pText)) {
		//	m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		//}
	}

}
