/*++

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

NMR_XmlWriter_Native.cpp implements an XML Writer Class without dependencies.

--*/

#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string.h>
#include <vector>

namespace NMR {

	CXmlWriter_Native::CXmlWriter_Native(_In_ PExportStream pExportStream)
		: CXmlWriter(pExportStream)
	{
		m_bIsFreshLine = true;

#ifndef __GCC
		m_nLineEndingBuffer[0] = 0x0d;
		m_nLineEndingBuffer[1] = 0x0a;
		m_nLineEndingCharCount = 2;
#else
		m_nLineEndingBuffer[0] = 0x0a;
		m_nLineEndingBuffer[1] = 0x0a;
		m_nLineEndingCharCount = 1;
#endif // __GCC

		m_nSpacesPerLayer = 1;
		m_nLayer = 0;

		m_SpacingBuffer.fill(NATIVEXMLSPACING);
		m_bElementIsOpen = false;
	}

	void CXmlWriter_Native::WriteStartDocument()
	{
		writeUTF8(NATIVEXMLENCODING, true);
	}

	void CXmlWriter_Native::WriteEndDocument()
	{
	}

	void CXmlWriter_Native::Flush()
	{
	}

	void CXmlWriter_Native::WriteAttributeString(_In_opt_ LPCWSTR pwszPrefix, _In_opt_ LPCWSTR pwszLocalName, _In_opt_ LPCWSTR pwszNamespaceUri, _In_opt_ LPCWSTR pwszValue)
	{
		if (m_bElementIsOpen) {
			writeUTF8(" ", false);

			if (pwszPrefix != nullptr) {
				if (*pwszPrefix != 0) {
					writeUTF16(pwszPrefix, false);
					writeUTF8(":", false);
				}
			}

			writeUTF16(pwszLocalName, false);
			writeUTF8("=\"", false);

			size_t nLength = wcslen(pwszValue);
			if (nLength < NATIVEXMLFIXEDENCODINGLENGTH) {
				// for small attribute lengths, use fixed size buffer
				nfWChar * pwszAttribBuffer = &m_FixedEncodingBuffer[0];
				escapeXMLString(pwszValue, pwszAttribBuffer);
				writeUTF16(pwszAttribBuffer, false);
			}
			else {
				if (nLength > NATIVEXMLMAXSTRINGLENGTH)
					throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

				// for large attribute lengths, use dynamic buffer
				std::vector<nfWChar> Buffer;
				Buffer.resize((nLength + 1) * 6);
				nfWChar * pwszAttribBuffer = &Buffer[0];
				escapeXMLString(pwszValue, pwszAttribBuffer);	
			}

			writeUTF8("\"", false);
		}
	}

	void CXmlWriter_Native::WriteStartElement(_In_opt_  LPCWSTR pwszPrefix, _In_  LPCWSTR pwszLocalName, _In_opt_  LPCWSTR pwszNamespaceUri)
	{
		closeCurrentElement(true);

		writeSpaces(m_nLayer * m_nSpacesPerLayer);
		writeUTF8("<", false);

		m_bElementIsOpen = true;

		std::wstring sNodePrefix;
		if (pwszPrefix != nullptr) {
			if (*pwszPrefix != 0) {
				sNodePrefix = pwszPrefix;
				sNodePrefix += L":";

				writeUTF16(sNodePrefix.c_str(), false);
			}
		}

		writeUTF16(pwszLocalName, false);

		std::wstring sNodeName (pwszLocalName);
		m_NodeStack.push_back(sNodePrefix + sNodeName);
		m_nLayer++;

		if (pwszNamespaceUri != nullptr) {
			if (*pwszNamespaceUri != 0) {
				writeUTF8(" xmlns=\"", false);
				writeUTF16(pwszNamespaceUri, false);
				writeUTF8("\"", false);
			}
		}
	}

	void CXmlWriter_Native::WriteEndElement()
	{
		if (m_bElementIsOpen) {
			writeUTF8(" />", true);
			m_bElementIsOpen = false;

			if ((m_NodeStack.size() == 0) || (m_nLayer == 0))
				throw CNMRException(NMR_ERROR_XMLWRITER_CLOSENODEERROR);
			m_NodeStack.pop_back();
			m_nLayer--;
		}
		else
			WriteFullEndElement();
	}

	void CXmlWriter_Native::WriteFullEndElement()
	{
		closeCurrentElement(false);
		if ((m_NodeStack.size() == 0) || (m_nLayer == 0))
			throw CNMRException(NMR_ERROR_XMLWRITER_CLOSENODEERROR);

		std::wstring sNodeName = m_NodeStack.back();
		m_NodeStack.pop_back();
		m_nLayer--;

		if (m_bIsFreshLine)
			writeSpaces(m_nLayer * m_nSpacesPerLayer);

		writeUTF8("</", false);
		writeUTF16(sNodeName.c_str(), false);
		writeUTF8(">", true);
	}

	void CXmlWriter_Native::WriteText(_In_ const nfWChar * pwszContent, _In_ const nfUint32 cbLength)
	{
		if (pwszContent == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		closeCurrentElement(false);
		writeUTF16(pwszContent, false);
	}

	void CXmlWriter_Native::writeData(_In_ const void * pData, _In_ nfUint32 cbLength)
	{
		if (pData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pExportStream->writeBuffer(pData, cbLength);
	}

	void CXmlWriter_Native::writeUTF8(_In_ const nfChar * pszString, _In_ nfBool bNewLine)
	{
		if (pszString == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

#ifdef __GCC
		size_t cbSize = strlen(pszString);
#else
		size_t cbSize = strnlen_s(pszString, NATIVEXMLMAXSTRINGLENGTH);
#endif // __GCC
		if (cbSize > NATIVEXMLMAXSTRINGLENGTH)
			throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

		if (cbSize > 0) {
			writeData((nfByte *)pszString, (nfUint32)cbSize);
			m_bIsFreshLine = false;
		}

		if (bNewLine) {
			writeData(m_nLineEndingBuffer, m_nLineEndingCharCount);
			m_bIsFreshLine = true;
		}
	}

	void CXmlWriter_Native::writeSpaces(_In_ nfUint32 cbCount)
	{
		while (cbCount > 0) {
			if (cbCount > NATIVEXMLSPACINGBUFFERSIZE) {
				writeData(&m_SpacingBuffer[0], NATIVEXMLSPACINGBUFFERSIZE);
				cbCount -= NATIVEXMLSPACINGBUFFERSIZE;
			}
			else {
				writeData(&m_SpacingBuffer[0], cbCount);
				break;
			}
		}
	}

	void CXmlWriter_Native::writeUTF16(_In_ const nfWChar * pszString, _In_ nfBool bNewLine)
	{
		std::wstring sString(pszString);
		std::string sUTF8String = fnUTF16toUTF8(sString);

		writeUTF8(sUTF8String.c_str(), bNewLine);
	}

	void CXmlWriter_Native::closeCurrentElement(_In_ nfBool bNewLine)
	{
		if (m_bElementIsOpen) {
			writeUTF8(">", bNewLine);
			m_bElementIsOpen = false;
		}
	}

	void CXmlWriter_Native::WriteRawLine(_In_ const nfChar * pszRawData, _In_ nfUint32 cbCount)
	{
		if (m_bElementIsOpen) {
			closeCurrentElement(true);
		}

		writeSpaces(m_nLayer * m_nSpacesPerLayer);
		writeData(pszRawData, cbCount);
		writeData(m_nLineEndingBuffer, m_nLineEndingCharCount);
	}

	void CXmlWriter_Native::escapeXMLString(_In_z_ const nfWChar * pszString, _Out_ nfWChar * pszBuffer)
	{
		__NMRASSERT(pszString);
		__NMRASSERT(pszBuffer);

		const nfWChar * pSrcChar = pszString;
		nfWChar * pDstChar = pszBuffer;
		while (*pSrcChar) {
			nfUint32 nReplaceLength = 0;
			const nfWChar * pszReplacement = nullptr;

			switch (*pSrcChar) {
			case 34: // "
				pszReplacement = L"&quot;";
				nReplaceLength = 6;
				break; 
			case 38: // &
				pszReplacement = L"&amp;";
				nReplaceLength = 5;
				break;
			case 39: // '
				pszReplacement = L"&apos;";
				nReplaceLength = 6;
				break;
			case 60: // <
				pszReplacement = L"&lt;";
				nReplaceLength = 4;
				break;
			case 62: // >
				pszReplacement = L"&gt;";
				nReplaceLength = 4;
				break;
			}

			if (nReplaceLength > 0) {
				nfUint32 j;
				for (j = 0; j < nReplaceLength; j++) {
					*pDstChar = *pszReplacement;
					pDstChar++;
					pszReplacement++;
				}
			}
			else {
				*pDstChar = *pSrcChar;
				pDstChar++;
			}

			pSrcChar++;

		}

		*pDstChar = 0;

	}


}
