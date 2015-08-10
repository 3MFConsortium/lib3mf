/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_XmlReader_GCC.cpp implements the XML Reader Class using GCC.

--*/

#include "Common/Platform/NMR_XmlReader_GCC.h"
#include "Common/Platform/NMR_ImportStream_GCC.h"
#include "Common/NMR_Exception_Windows.h"
#include <wchar.h>
#include <string>

namespace NMR {

    nfUint32 wstrlen (const nfWChar * pwszValue)
    {
        return 0;
    }


	CXmlReader_GCC::CXmlReader_GCC(_In_ PImportStream pImportStream)
		: CXmlReader (pImportStream)
	{
		m_sCurrentName = L"";
		m_bEOF = false;
		m_bEmptyElement = false;
		m_bParsingAttributes = false;
	}


	void CXmlReader_GCC::GetValue(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
	{
	    if (!ppwszValue)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
	    if (!pcwchValue)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (m_bParsingAttributes && (m_nAttributeIndex < m_nAttributeCount)) {
            *ppwszValue = (const nfWChar *) m_pXMLReader->getAttributeValue ((int) m_nAttributeIndex);
            if (*ppwszValue != nullptr) {
                *pcwchValue = wstrlen(*ppwszValue);
            } else {
                *pcwchValue = 0;
            }
        } else {
            *ppwszValue = (const nfWChar *) m_pXMLReader->getNodeData ();
            if (*ppwszValue != nullptr) {
                *pcwchValue = wstrlen(*ppwszValue);
            } else {
                *pcwchValue = 0;
            }
        }
	}

	nfBool CXmlReader_GCC::Read(_Out_ eXmlReaderNodeType & NodeType)
	{
	    // Read to next node
	    const nfWChar * pszwCurrentName;
        bool bSuccess = m_pXMLReader->read();
        if (!bSuccess) {
            m_bEOF = true;
            return XMLREADERNODETYPE_UNKNOWN;
        }

        m_nAttributeCount = m_pXMLReader->getAttributeCount();
        m_nAttributeIndex = 0;
        m_bParsingAttributes = false;

		irr::io::EXML_NODE eXMLType = m_pXMLReader->getNodeType();
        switch (eXMLType) {
            case irr::io::EXN_ELEMENT:
                m_bEmptyElement = m_pXMLReader->isEmptyElement();

                pszwCurrentName = (const nfWChar*) m_pXMLReader->getNodeName();
                if (pszwCurrentName != nullptr) {
                    m_sCurrentName = std::wstring();
                    m_sNameStack.push_back(m_sCurrentName);
                    return XMLREADERNODETYPE_STARTELEMENT;
                }

                return XMLREADERNODETYPE_UNKNOWN;

            case irr::io::EXN_ELEMENT_END:
                if (m_sNameStack.size() == 0)
                   throw CNMRException(NMR_ERROR_XMLELEMENTNOTOPEN);

                m_sCurrentName = m_sNameStack.back();
                m_sNameStack.pop_back ();

                return XMLREADERNODETYPE_ENDELEMENT;


            case irr::io::EXN_TEXT:
                return XMLREADERNODETYPE_TEXT;

            default:
                return XMLREADERNODETYPE_UNKNOWN;
        }

	}

	nfBool CXmlReader_GCC::IsEOF()
	{
		return m_bEOF;
	}

	nfBool CXmlReader_GCC::IsEmptyElement()
	{
		return m_bEmptyElement;
	}

	nfBool CXmlReader_GCC::MoveToFirstAttribute()
	{
		m_nAttributeIndex = 0;
		m_bParsingAttributes = true;
		return (m_nAttributeIndex < m_nAttributeCount);
	}

	nfBool CXmlReader_GCC::MoveToNextAttribute()
	{
		m_nAttributeIndex++;
		if (m_nAttributeIndex >= m_nAttributeCount) {
            m_bParsingAttributes = false;
            return false;
		} else {
            return true;
		}
	}

	nfBool CXmlReader_GCC::IsDefault()
	{
		return false;
	}

    void CXmlReader_GCC::GetLocalName(_Outptr_result_buffer_maybenull_(*pcwchLocalName + 1) const nfWChar ** ppwszLocalName, _Out_opt_ nfUint32 *pcwchLocalName)
    {
	    if (!ppwszLocalName)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
	    if (!pcwchLocalName)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (m_bParsingAttributes && (m_nAttributeIndex < m_nAttributeCount)) {
            *ppwszLocalName = (const nfWChar *) m_pXMLReader->getAttributeName (m_nAttributeIndex);
            if (*ppwszLocalName != nullptr) {
                *pcwchLocalName = wstrlen(*ppwszLocalName);
            } else {
                *pcwchLocalName = 0;
            }
        } else {

            *ppwszLocalName = m_sCurrentName.c_str();
            size_t cbLength = m_sCurrentName.size();

            if (cbLength > NMR_MAXXMLNAMELENGTH)
                throw CNMRException(NMR_ERROR_INVALIDXMLNAME);

            *pcwchLocalName = (nfUint32) cbLength;
        }
    }

    void CXmlReader_GCC::GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
    {
	    if (!ppwszValue)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
	    if (!pcwchValue)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        *ppwszValue = nullptr;
        *pcwchValue = 0;
   }


}
