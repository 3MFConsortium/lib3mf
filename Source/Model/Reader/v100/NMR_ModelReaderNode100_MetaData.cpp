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

NMR_ModelReaderNode100_MetaData.cpp implements the Model Reader MetaData
Node Class. A metadata reader model node is a parser for the triangle node
of an XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_MetaData::CModelReaderNode100_MetaData(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_sKey = "";
		m_sValue = "";
		m_bPreserve = true;
		m_sType = "xs:string";
	}

	void CModelReaderNode100_MetaData::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_MetaData::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_METADATA_NAME) == 0) {
			m_sKey = std::string(pAttributeValue);
		} else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_METADATA_PRESERVE) == 0) {
			m_bPreserve = !(std::string(pAttributeValue) == "0");
		} else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_METADATA_TYPE) == 0) {
			m_sType = std::string(pAttributeValue);
		}

	}

	void CModelReaderNode100_MetaData::OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pText);
		m_sValue += std::string(pText);
	}

	std::string CModelReaderNode100_MetaData::getKey()
	{
		return m_sKey;
	}

	std::string CModelReaderNode100_MetaData::getValue()
	{
		return m_sValue;
	}

	std::string CModelReaderNode100_MetaData::getType()
	{
		return m_sType;
	}

	nfBool CModelReaderNode100_MetaData::getPreserve()
	{
		return m_bPreserve;
	}

}
