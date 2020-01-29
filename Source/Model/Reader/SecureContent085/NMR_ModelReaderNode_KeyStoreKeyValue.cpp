
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

NMR_ModelReaderNode_KeyStoreKeyValue.h defines the Model Reader Node class that is related to <keyvalue>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreKeyValue.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_Modulus.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_Exponent.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode_KeyStoreKeyValue::CModelReaderNode_KeyStoreKeyValue(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{
		m_bHasKeyValue = false;
	}

	RSAKEYVALUE CModelReaderNode_KeyStoreKeyValue::GetKeyValue()
	{
		return m_keyValue;
	}

	nfBool CModelReaderNode_KeyStoreKeyValue::HasKeyValue()
	{
		return m_bHasKeyValue;
	}

	void CModelReaderNode_KeyStoreKeyValue::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_KeyStoreKeyValue::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_DIGITALSIGNATURESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_RSAKEYVALUE) == 0) {
				m_bHasKeyValue = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_MODULUS) == 0) {
				if (m_bHasKeyValue) {
					PModelReaderNode_KeyStoreModulus pXMLNode = std::make_shared<CModelReaderNode_KeyStoreModulus>(m_pKeyStore, m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					memcpy(m_keyValue.m_modulus, pXMLNode->GetModulus(), KEYSTORE_TYPES_MODULUSBUFFERSIZE * sizeof(nfByte));
				}
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_EXPONENT) == 0) {
				if (m_bHasKeyValue) {
					PModelReaderNode_KeyStoreExponent pXMLNode = std::make_shared<CModelReaderNode_KeyStoreExponent>(m_pKeyStore, m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
					memcpy(m_keyValue.m_exponent, pXMLNode->GetExponent(), KEYSTORE_TYPES_EXPONENTBUFFERSIZE * sizeof(nfByte));
				}
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
	}

}
