/*++

Copyright (C) 2018 3MF Consortium

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

#ifndef __GNUC__
		m_nLineEndingBuffer[0] = 0x0d;
		m_nLineEndingBuffer[1] = 0x0a;
		m_nLineEndingCharCount = 2;
#else
		m_nLineEndingBuffer[0] = 0x0a;
		m_nLineEndingBuffer[1] = 0x0a;
		m_nLineEndingCharCount = 1;
#endif // __GNUC__

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

	void CXmlWriter_Native::WriteAttributeString(_In_opt_ LPCSTR pszPrefix, _In_opt_ LPCSTR pszLocalName, _In_opt_ LPCSTR pszNamespaceUri, _In_opt_ LPCSTR pszValue)
	{
		if (m_bElementIsOpen) {
			writeUTF8(" ", false);

			if (pszPrefix != nullptr) {
				if (*pszPrefix != 0) {
					writeUTF8(pszPrefix, false);
					writeUTF8(":", false);
				}
			}

			writeUTF8(pszLocalName, false);
			writeUTF8("=\"", false);

			size_t nLength = strlen(pszValue);
			if (nLength < NATIVEXMLFIXEDENCODINGLENGTH) {
				// for small attribute lengths, use fixed size buffer
				nfChar * pszAttribBuffer = &m_FixedEncodingBuffer[0];
				escapeXMLString(pszValue, pszAttribBuffer);
				writeUTF8(pszAttribBuffer, false);
			}
			else {
				if (nLength > NATIVEXMLMAXSTRINGLENGTH)
					throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

				// for large attribute lengths, use dynamic buffer
				std::vector<nfChar> Buffer;
				Buffer.resize((nLength + 1) * 6);
				nfChar * pszAttribBuffer = &Buffer[0];
				escapeXMLString(pszValue, pszAttribBuffer);
			}

			writeUTF8("\"", false);
		}
	}

	void CXmlWriter_Native::WriteStartElement(_In_opt_  LPCSTR pszPrefix, _In_  LPCSTR pszLocalName, _In_opt_  LPCSTR pszNamespaceUri)
	{
		closeCurrentElement(true);

		writeSpaces(m_nLayer * m_nSpacesPerLayer);
		writeUTF8("<", false);

		m_bElementIsOpen = true;

		std::string sNodePrefix;
		if (pszPrefix != nullptr) {
			if (*pszPrefix != 0) {
				sNodePrefix = pszPrefix;
				sNodePrefix += ":";

				writeUTF8(sNodePrefix.c_str(), false);
			}
		}

		writeUTF8(pszLocalName, false);

		std::string sNodeName(pszLocalName);
		m_NodeStack.push_back(sNodePrefix + sNodeName);
		m_nLayer++;

		if (pszNamespaceUri != nullptr) {
			if (*pszNamespaceUri != 0) {
				writeUTF8(" xmlns=\"", false);
				writeUTF8(pszNamespaceUri, false);
				writeUTF8("\"", false);
			}
		}
	}

	void CXmlWriter_Native::WriteEndElement()
	{
		if (m_bElementIsOpen) {
			writeUTF8("/>", true);
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

		std::string sNodeName = m_NodeStack.back();
		m_NodeStack.pop_back();
		m_nLayer--;

		if (m_bIsFreshLine)
			writeSpaces(m_nLayer * m_nSpacesPerLayer);

		writeUTF8("</", false);
		writeUTF8(sNodeName.c_str(), false);
		writeUTF8(">", true);
	}

	void CXmlWriter_Native::WriteText(_In_ const nfChar * pszContent, _In_ const nfUint32 cbLength)
	{
		if (pszContent == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		closeCurrentElement(false);
		writeUTF8(pszContent, false);
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

#ifdef __GNUC__
		size_t cbSize = strlen(pszString);
#else
		size_t cbSize = strnlen_s(pszString, NATIVEXMLMAXSTRINGLENGTH);
#endif // __GNUC__
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

	void CXmlWriter_Native::escapeXMLString(_In_z_ const nfChar * pszString, _Out_ nfChar * pszBuffer)
	{
		__NMRASSERT(pszString);
		__NMRASSERT(pszBuffer);

		const nfChar * pSrcChar = pszString;
		nfChar * pDstChar = pszBuffer;
		while (*pSrcChar) {
			nfUint32 nReplaceLength = 0;
			const nfChar * pszReplacement = nullptr;

			switch (*pSrcChar) {
			case 34: // "
				pszReplacement = "&quot;";
				nReplaceLength = 6;
				break;
			case 38: // &
				pszReplacement = "&amp;";
				nReplaceLength = 5;
				break;
			case 39: // '
				pszReplacement = "&apos;";
				nReplaceLength = 6;
				break;
			case 60: // <
				pszReplacement = "&lt;";
				nReplaceLength = 4;
				break;
			case 62: // >
				pszReplacement = "&gt;";
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

	bool CXmlWriter_Native::GetNamespacePrefix(const std::string &sNameSpaceURI, std::string &sNameSpacePrefix)
	{
		auto iIterator = m_sNameSpaces.find(sNameSpaceURI);
		if (iIterator != m_sNameSpaces.end()) {
			sNameSpacePrefix = iIterator->second.c_str();
			return true;
		}
		else {
			return false;
		}
	}

	void CXmlWriter_Native::RegisterCustomNameSpace(const std::string &sNameSpace, const std::string &sNameSpacePrefix)
	{
		std::string sDummy;
		if (GetNamespacePrefix(sNameSpace, sDummy))
		{
			throw CNMRException(NMR_ERROR_XMLNAMESPACEALREADYREGISTERED);
		}
		for (auto it = m_sNameSpaces.begin(); it != m_sNameSpaces.end(); ++it)
			if (it->second == sNameSpacePrefix)
				throw CNMRException(NMR_ERROR_XMLPREFIXALREADYREGISTERED);

		m_sNameSpaces.insert(std::make_pair(sNameSpace, sNameSpacePrefix));
	}

	nfUint32 CXmlWriter_Native::GetNamespaceCount()
	{
		return nfUint32(m_sNameSpaces.size());
	}

	std::string CXmlWriter_Native::GetNamespacePrefix(nfUint32 nIndex)
	{
		auto iter = m_sNameSpaces.begin();
		for (nfUint32 i = 0; i < nIndex; i++)
			iter++;
		return iter->second;
	}

	std::string CXmlWriter_Native::GetNamespace(nfUint32 nIndex)
	{
		auto iter = m_sNameSpaces.begin();
		for (nfUint32 i = 0; i < nIndex; i++)
			iter++;
		return iter->first;
	}
}
