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

NMR_ModelReader_InstructionElement.cpp implements a reader for an instruction element

--*/

#include "Model/Reader/NMR_ModelReader_InstructionElement.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"

#include  <algorithm>

namespace NMR {

	CModelReader_InstructionElement::CModelReader_InstructionElement(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_sName("")
	{
	}

	void CModelReader_InstructionElement::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		//// Parse Content
		//parseContent(pXMLReader);

	}

	void CModelReader_InstructionElement::parseName(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		LPCSTR pszName = nullptr;
		pXMLReader->GetLocalName(&pszName, nullptr);
		if (!pszName)
			throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

		m_sName = pszName;
	}

	void CModelReader_InstructionElement::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (m_sName == XML_3MF_ATTRIBUTE_PREFIX_XML) {
			// only do this if this is the xml declaration
			if (strcmp(pAttributeName, "encoding") == 0) {
				m_sEncoding = pAttributeValue;
				std::transform(m_sEncoding.begin(), m_sEncoding.end(), m_sEncoding.begin(), tolower);
				if ((m_sEncoding != "utf-8") && (m_sEncoding != "utf8")) {
					throw CNMRException(NMR_ERROR_INVALIDXMLENCODING);
				}
			}
		}
	}

}
