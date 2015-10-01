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

NMR_XMLReader_Native.cpp implements a XML reader class with a native XML parsing implementation.

--*/

#include "Common/Platform/NMR_XmlReader_Native.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

namespace NMR {

	nfUint32 nfWStrLen(_In_ const nfWChar * pszwString)
	{
		nfUint32 nResult = 0;
		const nfWChar * pChar = pszwString;
		while (*pChar) {
			pChar++;
			nResult++;
			if (nResult > NMR_MAXXMLSTRINGLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);
		}

		return nResult;

	}

	CXmlReader_Native::CXmlReader_Native(_In_ PImportStream pImportStream, _In_ nfUint32 cbBufferCapacity)
		: CXmlReader(pImportStream)
	{
		if ((cbBufferCapacity < NMR_NATIVEXMLREADER_MINBUFFERCAPACITY) ||
			(cbBufferCapacity > NMR_NATIVEXMLREADER_MAXBUFFERCAPACITY))
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		m_cbBufferCapacity = cbBufferCapacity;
		m_TempBuffer.resize(cbBufferCapacity);
		m_UTF16Buffer1.resize(cbBufferCapacity);
		m_UTF16Buffer2.resize(cbBufferCapacity);
		m_CurrentEntityList.resize(cbBufferCapacity);
		m_CurrentEntityTypes.resize(cbBufferCapacity);
		m_CurrentEntityPrefixes.resize(cbBufferCapacity);
		m_ZeroInsertArray.resize(cbBufferCapacity);

		m_pNextBuffer = &m_UTF16Buffer1;
		m_pCurrentBuffer = &m_UTF16Buffer2;

		m_nCurrentBufferSize = 0;
		m_cbCurrentOverflowSize = 0;
		m_nCurrentEntityIndex = 0;
		m_nCurrentFullEntityCount = 0;
		m_nCurrentEntityCount = 0;
		m_cNullString = 0;

		// Initialise Status Values
		m_pCurrentName = &m_cNullString;
		m_pCurrentPrefix = &m_cNullString;
		m_pCurrentValue = &m_cNullString;
		m_pCurrentElementName = &m_cNullString;
		m_pCurrentElementPrefix = &m_cNullString;

		m_cbDefaultNameSpaceLength = 0;
		m_bNameSpaceIsAttribute = false;

		m_nZeroInsertIndex = 0;

		m_bIsEOF = false;

		registerNameSpace(NMR_NATIVEXMLNS_XML_PREFIX, NMR_NATIVEXMLNS_XML_URI);
		registerNameSpace(NMR_NATIVEXMLNS_ATTRIBUTE, NMR_NATIVEXMLNS_XML_URI);
	}

	CXmlReader_Native::~CXmlReader_Native()
	{

	}

	void CXmlReader_Native::GetValue(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
	{
		if (ppwszValue == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		*ppwszValue = m_pCurrentValue;
		if (pcwchValue != nullptr)
			*pcwchValue = nfWStrLen(m_pCurrentValue);

	}

	void CXmlReader_Native::GetLocalName(_Outptr_result_buffer_maybenull_(*pcwchLocalName + 1) const nfWChar ** ppwszLocalName, _Out_opt_ nfUint32 *pcwchLocalName)
	{
		if (ppwszLocalName == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		*ppwszLocalName = m_pCurrentName;
		if (pcwchLocalName != nullptr)
			*pcwchLocalName = nfWStrLen(m_pCurrentName);

	}

	void CXmlReader_Native::GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue)
	{
		if (ppwszValue == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 cbLength = 0;
		if (*m_pCurrentPrefix == 0) {
			if (m_bNameSpaceIsAttribute) {
				cbLength = 0;
				*ppwszValue = &m_cNullString;

			}
			else {
				cbLength = m_cbDefaultNameSpaceLength;
				*ppwszValue = m_sDefaultNameSpace.c_str();
			}
		}
		else {
			auto iIterator = m_sNameSpaces.find(m_pCurrentPrefix);
			if (iIterator != m_sNameSpaces.end()) {
				cbLength = (nfUint32)iIterator->second.length();
				*ppwszValue = iIterator->second.c_str();
			}
			else {
				cbLength = 0;
				*ppwszValue = nullptr;
			}
		}

		if (pcwchValue != nullptr)
			*pcwchValue = cbLength;

	}

	nfBool CXmlReader_Native::ensureFilledBuffer()
	{
		if (m_nCurrentEntityIndex >= m_nCurrentFullEntityCount) {
			readNextBufferFromStream();

			__NMRASSERT(m_nCurrentEntityIndex == 0);
			if (m_nCurrentFullEntityCount == 0) {
				m_bIsEOF = true;
				return false;
			}
		}
		return true;
	}

	nfBool CXmlReader_Native::Read(_Out_ eXmlReaderNodeType & NodeType)
	{
		if (!ensureFilledBuffer()) {
			// Here we reached eof!
			NodeType = XMLREADERNODETYPE_UNKNOWN;
			return false;
		}

		__NMRASSERT(m_nCurrentEntityIndex < m_nCurrentFullEntityCount);
		nfByte nType = m_CurrentEntityTypes[m_nCurrentEntityIndex];
		switch (nType) {
		case NMR_NATIVEXMLTYPE_TEXT:
			NodeType = XMLREADERNODETYPE_TEXT;
			m_pCurrentValue = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_pCurrentPrefix = &m_cNullString;
			m_pCurrentName = &m_cNullString;
			m_bNameSpaceIsAttribute = false;
			break;
		case NMR_NATIVEXMLTYPE_ELEMENT:
			NodeType = XMLREADERNODETYPE_STARTELEMENT;
			m_pCurrentValue = &m_cNullString;
			m_pCurrentPrefix = m_CurrentEntityPrefixes[m_nCurrentEntityIndex];
			m_pCurrentName = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_pCurrentElementName = m_pCurrentName;
			m_pCurrentElementPrefix = m_pCurrentPrefix;
			m_bNameSpaceIsAttribute = false;
			break;
		case NMR_NATIVEXMLTYPE_ELEMENTEND:
			NodeType = XMLREADERNODETYPE_ENDELEMENT;
			m_pCurrentValue = &m_cNullString;
			m_pCurrentPrefix = m_CurrentEntityPrefixes[m_nCurrentEntityIndex];
			m_pCurrentName = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_pCurrentElementName = m_pCurrentName;
			m_pCurrentElementPrefix = m_pCurrentPrefix;
			m_bNameSpaceIsAttribute = false;
			break;
		case NMR_NATIVEXMLTYPE_CLOSEELEMENT:
			NodeType = XMLREADERNODETYPE_ENDELEMENT;
			m_pCurrentValue = &m_cNullString;
			m_pCurrentPrefix = m_pCurrentElementPrefix;
			m_pCurrentName = m_pCurrentElementName;
			m_pCurrentElementName = &m_cNullString;
			m_pCurrentElementPrefix = &m_cNullString;
			break;
		default:
			NodeType = XMLREADERNODETYPE_UNKNOWN;
		}

		m_nCurrentEntityIndex++;

		return true;
	}

	nfBool CXmlReader_Native::IsEOF()
	{
		return m_bIsEOF;
	}

	nfBool CXmlReader_Native::IsEmptyElement()
	{
		return false;
	}

	nfBool CXmlReader_Native::MoveToFirstAttribute()
	{
		return MoveToNextAttribute();
	}

	nfBool CXmlReader_Native::MoveToNextAttribute()
	{
		if (!ensureFilledBuffer())
			return false;

		m_bNameSpaceIsAttribute = true;

		__NMRASSERT(m_nCurrentEntityIndex < m_nCurrentFullEntityCount);
		nfByte nNameType = m_CurrentEntityTypes[m_nCurrentEntityIndex];

		if (nNameType == NMR_NATIVEXMLTYPE_ATTRIBNAME) {
			// Read Attribute Name
			m_pCurrentPrefix = m_CurrentEntityPrefixes[m_nCurrentEntityIndex];
			m_pCurrentName = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_nCurrentEntityIndex++;

			if (!ensureFilledBuffer())
				return false;

			// Read Attribute Value
			__NMRASSERT(m_nCurrentEntityIndex < m_nCurrentFullEntityCount);
			nfByte nValueType = m_CurrentEntityTypes[m_nCurrentEntityIndex];
			if (nValueType != NMR_NATIVEXMLTYPE_ATTRIBVALUE)
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDATTRIBVALUE);

			m_pCurrentValue = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_nCurrentEntityIndex++;

			// register Namespaces
			if ((*m_pCurrentPrefix == 0) && (wcscmp(m_pCurrentName, NMR_NATIVEXMLNS_ATTRIBUTE) == 0)) {
				m_sDefaultNameSpace = m_pCurrentValue;
				m_cbDefaultNameSpaceLength = (nfUint32)m_sDefaultNameSpace.length();
			}
			if (wcscmp(m_pCurrentPrefix, NMR_NATIVEXMLNS_ATTRIBUTE) == 0)
				registerNameSpace(m_pCurrentName, m_pCurrentValue);

			return true;
		}

		return false;
	}

	nfBool CXmlReader_Native::IsDefault()
	{
		return false;
	}

	void CXmlReader_Native::CloseElement()
	{
		// Empty by purpose
	}

	void CXmlReader_Native::readNextBufferFromStream()
	{
		nfUint64 cbBytesRead;
		nfUint32 j;

		clearZeroInserts();

		// Read buffer (minus safety margin for overread/overwrite)
		nfUint32 cbReadSize = m_cbBufferCapacity - NMR_NATIVEXMLREADER_BUFFERMARGIN;
		if (cbReadSize < m_cbCurrentOverflowSize)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);
		if (m_nCurrentBufferSize < m_cbCurrentOverflowSize)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// Copy over unfinished elements of current buffer into new buffer
		if (m_cbCurrentOverflowSize > 0) {
			nfUint32 nDeltaIndex = m_nCurrentBufferSize - m_cbCurrentOverflowSize;
			nfWChar * pSrc = &(*m_pCurrentBuffer)[nDeltaIndex];
			nfWChar * pDst = &(*m_pNextBuffer)[0];
			for (j = 0; j < m_cbCurrentOverflowSize; j++)
				pDst[j] = pSrc[j];

			m_nCurrentBufferSize = m_cbCurrentOverflowSize;
			cbReadSize -= m_cbCurrentOverflowSize;
			m_cbCurrentOverflowSize = 0;
		}
		else {
			m_nCurrentBufferSize = 0;
			m_cbCurrentOverflowSize = 0;
		}

		// Read temp buffer into memory
		cbBytesRead = m_pImportStream->readBuffer(&m_TempBuffer[0], cbReadSize, false);
		if (cbBytesRead > 0) {
			nfUint32 pnLastChar = 0;
			nfUint32 cbNeededCharacters = 0;

			// Convert buffer to UTF16
			nfUint64 cbCharsConverted = fnBufferedUTF8toUTF16((nfChar*)&m_TempBuffer[0], &(*m_pNextBuffer)[m_nCurrentBufferSize], (nfUint32)cbBytesRead, &pnLastChar, &cbNeededCharacters);
			m_nCurrentBufferSize += (nfUint32)cbCharsConverted;

			if (cbNeededCharacters > 0) {
				// We just need to write the last bytes of a multibyte character!
				__NMRASSERT(cbNeededCharacters < NMR_NATIVEXMLREADER_BUFFERMARGIN);
				m_pImportStream->readBuffer(&m_TempBuffer[(nfUint32)cbBytesRead], cbNeededCharacters, true);
				cbBytesRead += cbNeededCharacters;

				// Convert final character
				cbCharsConverted = fnBufferedUTF8toUTF16((nfChar*)&m_TempBuffer[pnLastChar], &(*m_pNextBuffer)[m_nCurrentBufferSize], (nfUint32)(cbBytesRead - pnLastChar), &pnLastChar, &cbNeededCharacters);
				m_nCurrentBufferSize += (nfUint32)cbCharsConverted;

				// Check if everything is finished
				if (cbNeededCharacters != 0)
					throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);
			}

			// Everything is converted to UTF16!
			__NMRASSERT(pszLastChar);
		}

		// Reset Entity parser
		m_nCurrentEntityCount = 0;
		m_nCurrentFullEntityCount = 0;
		m_nCurrentEntityIndex = 0;

		// Swap Buffers
		std::vector<nfWChar> * pDummy = m_pCurrentBuffer;
		m_pCurrentBuffer = m_pNextBuffer;
		m_pNextBuffer = pDummy;

		// parse Content
		m_pCurrentEntityPointer = nullptr;
		parseUnknown(&(*m_pCurrentBuffer)[0], &(*m_pCurrentBuffer)[m_nCurrentBufferSize]);

		if (m_pCurrentEntityPointer != nullptr) {

			// Calculate Overflow buffer
			nfUint64 nEndPtr = (nfUint64)m_pCurrentEntityPointer;
			nfUint64 nStartPtr = (nfUint64)&(*m_pCurrentBuffer)[0];

			if (nStartPtr > nEndPtr)
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDPARSERESULT);

			nfUint64 nUsedChars = ((nEndPtr - nStartPtr) / sizeof (nfWChar));
			if (nUsedChars > (nfUint64)m_nCurrentBufferSize)
				throw CNMRException(NMR_ERROR_XMLPARSER_TOOMANYUSEDCHARS);

			m_cbCurrentOverflowSize = m_nCurrentBufferSize - ((nfUint32)nUsedChars);
		}
		else {
			m_cbCurrentOverflowSize = 0;
		}
	}

	void CXmlReader_Native::pushEntity(_In_ nfWChar * pszwEntityStartChar, _In_ nfWChar * pszwEntityEndDelimiter, _In_ nfWChar * pszwNextEntityChar, _In_ nfByte nType, _In_ nfBool bParseForNamespaces, _In_ nfBool bEntityIsFinished)
	{
		if (bParseForNamespaces) {
			nfWChar * pChar = pszwEntityStartChar;
			nfWChar * pColon = nullptr;
			while (pChar != pszwEntityEndDelimiter) {
				if (*pChar == 0)
					throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDENDDELIMITER);

				if (*pChar == L':') {
					if (pColon != nullptr)
						throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDNAMESPACEPREFIX);

					pColon = pChar;
				}

				pChar++;
			}

			if (pColon != nullptr) {
				pushZeroInsert (pColon);
				pColon++;

				m_CurrentEntityList[m_nCurrentEntityCount] = pColon;
				m_CurrentEntityPrefixes[m_nCurrentEntityCount] = pszwEntityStartChar;
			}
			else {
				m_CurrentEntityList[m_nCurrentEntityCount] = pszwEntityStartChar;
				m_CurrentEntityPrefixes[m_nCurrentEntityCount] = &m_cNullString;
			}

		}
		else {
			m_CurrentEntityList[m_nCurrentEntityCount] = pszwEntityStartChar;
			m_CurrentEntityPrefixes[m_nCurrentEntityCount] = &m_cNullString;
		}

		m_CurrentEntityTypes[m_nCurrentEntityCount] = nType;
		m_nCurrentEntityCount++;

		if (bEntityIsFinished) {
			// We have closed a full entity
			m_pCurrentEntityPointer = pszwNextEntityChar;
			m_nCurrentFullEntityCount = m_nCurrentEntityCount;

			// Insert all zeros of the entity, as we do not need to rollback.
			performZeroInserts();
		}
	}


	nfWChar * CXmlReader_Native::parseUnknown(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			nfWChar * pLastChar = pChar;
			pChar = parseText(pChar, pszwEnd);

			if (pChar == pLastChar)
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTPARSEENTITY);
		}

		return pChar;
	}

	nfWChar * CXmlReader_Native::parseText(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case L'<':
				if (pChar != pszwStart)
					pushEntity(pszwStart, pChar, pChar, NMR_NATIVEXMLTYPE_TEXT, false, true);
				pushZeroInsert(pChar);
				pChar++;

				return parseElement(pChar, pszwEnd);

			default:
				pChar++;
			}

		}

		return pChar;
	}


	nfWChar * CXmlReader_Native::parseElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				if (pszwStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYELEMENTNAME);
				pushEntity(pszwStart, pChar, pChar, NMR_NATIVEXMLTYPE_ELEMENT, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseAttributes(pChar, pszwEnd);

			case L'?':
				if (pszwStart != pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDCHARACTERINELEMENTNAME);
				pChar++;
				return parseProcessingInstruction(pChar, pszwEnd);

			case L'>':
				pushZeroInsert(pChar);
				pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENT, true, true);
				pChar++;
				return pChar;

			case L'/':
				if (pszwStart == pChar) {
					pChar++;
					return parseEndElement(pChar, pszwEnd);
				}
				else {
					pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENT, true, false);
					pushZeroInsert(pChar);
					pChar++;

					return parseCloseElement(pChar, pszwEnd);
				}

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfWChar * CXmlReader_Native::parseProcessingInstruction(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				if (pszwStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYINSTRUCTIONNAME);
				pushEntity(pszwStart, pChar, pChar, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseAttributes(pChar, pszwEnd);

				break;

			case L'/':
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDINSTRUCTIONNAME);

			case L'?':
				if (pszwStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYINSTRUCTIONNAME);
				pushEntity(pszwStart, pChar, pChar, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseCloseProcessingInstruction(pChar, pszwEnd);

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfWChar * CXmlReader_Native::parseCloseProcessingInstruction(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		if (pszwStart != pszwEnd) {
			if (*pszwStart != L'>')
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTCLOSEINSTRUCTION);

			pushZeroInsert(pszwStart);
			pushEntity(pszwStart, pszwStart, pszwStart + 1, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTIONEND, false, true);

			pszwStart++;
			return pszwStart;
		}

		return pszwStart;
	}


	nfWChar * CXmlReader_Native::parseEndElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				pChar++;
				break;

			case L'/':
			case L'?':
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTENDELEMENT);

			case L'>':
				if (pszwStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYENDELEMENT);
				pushZeroInsert(pChar);
				pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENTEND, false, true);
				pChar++;

				return pChar;

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfWChar * CXmlReader_Native::parseCloseElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		if (pszwStart != pszwEnd) {
			if (*pszwStart != L'>')
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTCLOSEELEMENT);

			pushZeroInsert (pszwStart);
			pushEntity(pszwStart, pszwStart, pszwStart + 1, NMR_NATIVEXMLTYPE_CLOSEELEMENT, false, true);

			pszwStart++;
			return pszwStart;
		}

		return pszwStart;
	}

	nfWChar * CXmlReader_Native::parseAttributes(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				pChar = skipSpaces(++pChar, pszwEnd);
				break;

			case L'>':
				pChar++;
				return pChar;

			case L'/':
				pChar++;
				return parseCloseElement(pChar, pszwEnd);

			case L'?':
				pChar++;
				return parseCloseProcessingInstruction(pChar, pszwEnd);

			default:
				pChar = parseAttributeName(pChar, pszwEnd);
				pChar = parseAttributeValue(pChar, pszwEnd);
			}

		}

		return pChar;
	}


	nfWChar * CXmlReader_Native::skipSpaces(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				pChar++;

			default:
				return pChar;

			}
		}


		return pChar;
	}

	nfWChar * CXmlReader_Native::parseAttributeName(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfBool bHadSpacing = false;
		nfWChar * pChar = skipSpaces(pszwStart, pszwEnd);
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				bHadSpacing = true;
				pChar++;

				break;

			case 34:
			case 39:
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDATTRIBUTENAME);

			case L'=':
				pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBNAME, true, false);
				pushZeroInsert(pChar);
				pChar++;
				return pChar;

			default:
				if (bHadSpacing)
					throw CNMRException(NMR_ERROR_XMLPARSER_SPACEINATTRIBUTENAME);
				pChar++;
			}
		}


		return pChar;
	}

	nfWChar * CXmlReader_Native::parseAttributeValue(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfBool bHadSpacing = false;
		nfWChar * pChar = skipSpaces(pszwStart, pszwEnd);
		while (pChar != pszwEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				bHadSpacing = true;
				pChar++;

				break;

			case 34:
				pChar++;
				return parseAttributeValueDoubleQuote(pChar, pszwEnd);

			case 39:
				pChar++;
				return parseAttributeValueSingleQuote(pChar, pszwEnd);

			default:
				if (bHadSpacing)
					throw CNMRException(NMR_ERROR_XMLPARSER_NOQUOTESAROUNDATTRIBUTE);
				pChar++;
			}
		}


		return pChar;
	}

	nfWChar * CXmlReader_Native::parseAttributeValueDoubleQuote(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {

			case 34:
				pushZeroInsert(pChar);
				pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBVALUE, false, false);
				pChar++;
				return pChar;

			default:
				pChar++;
			}
		}


		return pChar;
	}

	nfWChar * CXmlReader_Native::parseAttributeValueSingleQuote(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd)
	{
		nfWChar * pChar = pszwStart;
		while (pChar != pszwEnd) {
			switch (*pChar) {

			case 39:
				pushZeroInsert(pChar);
				pushEntity(pszwStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBVALUE, false, false);
				pChar++;
				return pChar;

			default:
				pChar++;
			}
		}


		return pChar;
	}


	void CXmlReader_Native::registerNameSpace(_In_ std::wstring sPrefix, _In_ std::wstring sURI)
	{
		m_sNameSpaces.insert(std::make_pair(sPrefix, sURI));

	}


	void CXmlReader_Native::pushZeroInsert(_In_ nfWChar * pChar)
	{
		__NMRASSERT(pChar != nullptr);
		m_ZeroInsertArray[m_nZeroInsertIndex] = pChar;
		m_nZeroInsertIndex++;
	}

	void CXmlReader_Native::performZeroInserts()
	{
		nfUint32 nIndex;
		for (nIndex = 0; nIndex < m_nZeroInsertIndex; nIndex++) {
			nfWChar * pChar = m_ZeroInsertArray[nIndex];
			*pChar = 0;
		}

		m_nZeroInsertIndex = 0;
	}

	void CXmlReader_Native::clearZeroInserts()
	{
		m_nZeroInsertIndex = 0;
	}


}
