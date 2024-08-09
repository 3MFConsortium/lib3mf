
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

#include "Model/Reader/SecureContent101/NMR_ModelReaderNode_KeyStoreCipherValue.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "Model/Reader/NMR_ModelReaderNode_StringValue.h"
#include "base64.h"

namespace NMR {

	std::vector<nfByte> const & CModelReaderNode_KeyStoreCipherValue::getCipherValue() const
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

		if (m_sCipherValue.empty()) {
			m_pWarnings->addWarning(NMR_ERROR_KEYSTOREINVALIDCIPHERVALUE, eModelWarningLevel::mrwMissingMandatoryValue);
		}
	}

	void CModelReaderNode_KeyStoreCipherValue::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) {
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CIPHERVALUESPEC) == 0
			&& strcmp(pChildName, XML_3MF_ELEMENT_CIPHERVALUE) == 0) {
			PModelReaderNode_StringValue pNode = std::make_shared<CModelReaderNode_StringValue>(m_pWarnings);
			pNode->parseXML(pXMLReader);
			try {
				std::string result = base64_decode(pNode->getValue());
				m_sCipherValue = std::vector<nfByte>(result.begin(), result.end());
			}
			catch (...) {
				// base64_decode throws an exception if the input is not valid base64
				m_pWarnings->addException(CNMRException(NMR_ERROR_KEYSTOREINVALIDENCODING), NMR::eModelWarningLevel::mrwInvalidMandatoryValue);
            }
		}
	}

}
