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

NMR_XMLReader_Native.cpp implements a XML reader class with a native XML parsing implementation.

--*/

#include "Common/Platform/NMR_XmlReader_Native.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include "Common/3MF_ProgressMonitor.h"

#include <algorithm>

namespace NMR {

	nfUint32 nfStrLen(_In_ const nfChar * pszString)
	{
		nfUint32 nResult = 0;
		const nfChar * pChar = pszString;
		while (*pChar) {
			pChar++;
			nResult++;
			if (nResult > NMR_MAXXMLSTRINGLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);
		}
		return nResult;
	}

	CXmlReader_Native::CXmlReader_Native(_In_ PImportStream pImportStream, _In_ nfUint32 cbBufferCapacity, _In_ CProgressMonitor* pProgressMonitor)
		: CXmlReader(pImportStream), m_progressCounter(0), m_pProgressMonitor(pProgressMonitor)
	{
		if ((cbBufferCapacity < NMR_NATIVEXMLREADER_MINBUFFERCAPACITY) ||
			(cbBufferCapacity > NMR_NATIVEXMLREADER_MAXBUFFERCAPACITY))
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		m_cbBufferCapacity = cbBufferCapacity;
		m_UTF8Buffer1.resize(cbBufferCapacity);
		m_UTF8Buffer2.resize(cbBufferCapacity);
		m_CurrentEntityList.resize(cbBufferCapacity);
		m_CurrentEntityTypes.resize(cbBufferCapacity);
		m_CurrentEntityPrefixes.resize(cbBufferCapacity);
		m_ZeroInsertArray.resize(cbBufferCapacity);

		m_pNextBuffer = &m_UTF8Buffer1;
		m_pCurrentBuffer = &m_UTF8Buffer2;

		m_nCurrentBufferSize = 0;
		m_cbCurrentOverflowSize = 0;
		m_nCurrentEntityIndex = 0;
		m_nCurrentFullEntityCount = 0;
		m_nCurrentEntityCount = 0;
		m_nCurrentVerifiedEntityCount = 0;
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
		registerNameSpace(NMR_NATIVEXMLNS_XMLNS_PREFIX, NMR_NATIVEXMLNS_XMLNS_URI);

	}

	CXmlReader_Native::~CXmlReader_Native()
	{

	}

	void CXmlReader_Native::GetValue(_Outptr_result_buffer_maybenull_(*pcchValue + 1)  const nfChar ** ppszValue, _Out_opt_  nfUint32 *pcchValue)
	{
		if (ppszValue == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		*ppszValue = m_pCurrentValue;
		
		if (pcchValue != nullptr)
			*pcchValue = nfStrLen(m_pCurrentValue);

	}

	void CXmlReader_Native::GetLocalName(_Outptr_result_buffer_maybenull_(*pcchLocalName + 1) const nfChar ** ppszLocalName, _Out_opt_ nfUint32 *pcchLocalName)
	{
		if (ppszLocalName == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		*ppszLocalName = m_pCurrentName;
		if (pcchLocalName != nullptr)
			*pcchLocalName = nfStrLen(m_pCurrentName);

	}

	void CXmlReader_Native::GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcchValue + 1)  const nfChar ** ppszValue, _Out_opt_  nfUint32 *pcchValue)
	{
		if (ppszValue == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 cbLength = 0;
		if (*m_pCurrentPrefix == 0) {
			if (m_bNameSpaceIsAttribute) {
				cbLength = 0;
				*ppszValue = &m_cNullString;
			}
			else {
				cbLength = m_cbDefaultNameSpaceLength;
				*ppszValue = m_sDefaultNameSpace.c_str();
			}
		}
		else {
			auto iIterator = m_sNameSpaces.find(m_pCurrentPrefix);
			if (iIterator != m_sNameSpaces.end()) {
				cbLength = (nfUint32)iIterator->second.length();
				*ppszValue = iIterator->second.c_str();
			}
			else {
				cbLength = 0;
				*ppszValue = nullptr;
			}
		}

		if (pcchValue != nullptr)
			*pcchValue = cbLength;

	}

	bool CXmlReader_Native::GetNamespaceURI(const std::string &sNameSpacePrefix, std::string &sNameSpaceURI)
	{
		auto iIterator = m_sNameSpaces.find(sNameSpacePrefix);
		if (iIterator != m_sNameSpaces.end()) {
			sNameSpaceURI = iIterator->second.c_str();
			return true;
		}
		else {
			return false;
		}
	}

	bool CXmlReader_Native::NamespaceRegistered(const std::string &sNameSpaceURI)
	{
		for (auto it : m_sNameSpaces) {
			if (it.second == sNameSpaceURI) {
				return true;
			}
		}
		return false;
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


		case NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION:
			NodeType = XMLREADERNODETYPE_STARTELEMENT;
			m_pCurrentValue = &m_cNullString;
			m_pCurrentPrefix = m_CurrentEntityPrefixes[m_nCurrentEntityIndex];
			m_pCurrentName = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_pCurrentElementName = m_pCurrentName;
			m_pCurrentElementPrefix = m_pCurrentPrefix;
			m_bNameSpaceIsAttribute = false;
			break;

		case NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTIONEND:
			NodeType = XMLREADERNODETYPE_ENDELEMENT;
			m_pCurrentValue = &m_cNullString;
			m_pCurrentPrefix = m_CurrentEntityPrefixes[m_nCurrentEntityIndex];
			m_pCurrentName = m_CurrentEntityList[m_nCurrentEntityIndex];
			m_pCurrentElementName = m_pCurrentName;
			m_pCurrentElementPrefix = m_pCurrentPrefix;
			m_bNameSpaceIsAttribute = false;
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
			if ((*m_pCurrentPrefix == 0) && (strcmp(m_pCurrentName, NMR_NATIVEXMLNS_XMLNS_PREFIX) == 0)) {
				m_sDefaultNameSpace = m_pCurrentValue;
				m_cbDefaultNameSpaceLength = (nfUint32)m_sDefaultNameSpace.length();
			}
			if (strcmp(m_pCurrentPrefix, NMR_NATIVEXMLNS_XMLNS_PREFIX) == 0)
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
		if (m_progressCounter++ > PROGRESS_READBUFFERUPDATE) {
			if (m_pProgressMonitor)
				if (!m_pProgressMonitor->QueryCancelled())
					throw CNMRException(NMR_USERABORTED);
			m_progressCounter = 0;
		}
		
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
			nfChar * pSrc = &(*m_pCurrentBuffer)[nDeltaIndex];
			nfChar * pDst = &(*m_pNextBuffer)[0];
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

		// Read buffer into memory
		cbBytesRead = m_pImportStream->readBuffer((nfByte*)(&((*m_pNextBuffer)[m_nCurrentBufferSize])), cbReadSize, false);
		m_nCurrentBufferSize += (nfUint32)cbBytesRead;

		// Reset Entity parser
		m_nCurrentEntityCount = 0;
		m_nCurrentVerifiedEntityCount = 0;
		m_nCurrentFullEntityCount = 0;
		m_nCurrentEntityIndex = 0;

		// Swap Buffers
		std::vector<nfChar> * pDummy = m_pCurrentBuffer;
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

			nfUint64 nUsedChars = ((nEndPtr - nStartPtr) / sizeof (nfChar));
			if (nUsedChars > (nfUint64)m_nCurrentBufferSize)
				throw CNMRException(NMR_ERROR_XMLPARSER_TOOMANYUSEDCHARS);

			m_cbCurrentOverflowSize = m_nCurrentBufferSize - ((nfUint32)nUsedChars);
		}
		else {
			m_cbCurrentOverflowSize = 0;
		}
	}

	void CXmlReader_Native::pushEntity(_In_ nfChar * pszEntityStartChar, _In_ nfChar * pszEntityEndDelimiter, _In_ nfChar * pszNextEntityChar, _In_ nfByte nType, _In_ nfBool bParseForNamespaces, _In_ nfBool bEntityIsFinished)
	{
		if (bParseForNamespaces) {
			nfChar * pChar = pszEntityStartChar;
			nfChar * pColon = nullptr;
			while (pChar != pszEntityEndDelimiter) {
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
				m_CurrentEntityPrefixes[m_nCurrentEntityCount] = pszEntityStartChar;
			}
			else {
				m_CurrentEntityList[m_nCurrentEntityCount] = pszEntityStartChar;
				m_CurrentEntityPrefixes[m_nCurrentEntityCount] = &m_cNullString;
			}

		}
		else {
			m_CurrentEntityList[m_nCurrentEntityCount] = pszEntityStartChar;
			m_CurrentEntityPrefixes[m_nCurrentEntityCount] = &m_cNullString;
		}

		m_CurrentEntityTypes[m_nCurrentEntityCount] = nType;
		m_nCurrentEntityCount++;

		if (bEntityIsFinished) {
			// We have closed a full entity
			m_pCurrentEntityPointer = pszNextEntityChar;
			m_nCurrentFullEntityCount = m_nCurrentEntityCount;

			// Insert all zeros of the entity, as we do not need to rollback.
			performZeroInserts();

			performEscapeStringDecoding();
		}
	}


	nfChar * CXmlReader_Native::parseUnknown(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			nfChar * pLastChar = pChar;
			pChar = parseText(pChar, pszEnd);

			if (pChar == pLastChar)
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTPARSEENTITY);
		}

		return pChar;
	}

	nfChar * CXmlReader_Native::parseText(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {
			case '<':
				if (pChar+1 != pszEnd && *(pChar+1) == '!' &&
					pChar+2 != pszEnd && *(pChar+2) == '-' &&
					pChar+3 != pszEnd && *(pChar+3) == '-'){
					pChar += 4;
					return parseComment(pChar, pszEnd);
				} else {
					if (pChar != pszStart)
						pushEntity(pszStart, pChar, pChar, NMR_NATIVEXMLTYPE_TEXT, false, true);
					pushZeroInsert(pChar);
					pChar++;

					return parseElement(pChar, pszEnd);
				}
			default:
				pChar++;
			}

		}

		return pChar;
	}


	nfChar * CXmlReader_Native::parseElement(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {
			case 9:  // Tab
			case 10: // LF
			case 13: // CR
			case 32: // Space
				if (pszStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYELEMENTNAME);
				pushEntity(pszStart, pChar, pChar, NMR_NATIVEXMLTYPE_ELEMENT, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseAttributes(pChar, pszEnd);

			case '?':
				if (pszStart != pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDCHARACTERINELEMENTNAME);
				pChar++;
				return parseProcessingInstruction(pChar, pszEnd);

			case '>':
				pushZeroInsert(pChar);
				pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENT, true, true);
				pChar++;
				return pChar;

			case '/':
				if (pszStart == pChar) {
					pChar++;
					return parseEndElement(pChar, pszEnd);
				}
				else {
					pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENT, true, false);
					pushZeroInsert(pChar);
					pChar++;

					return parseCloseElement(pChar, pszEnd);
				}

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfChar * CXmlReader_Native::parseComment(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		static nfChar commentEnd [3] = {'-', '-', '>'};
		nfInt32 endPosition = 0;
		while (pChar != pszEnd) {
			if (*pChar == commentEnd[endPosition]){
				if (endPosition == 2)
					return pChar;
				endPosition++;
			} else {
				if (*pChar != '-')
				  endPosition = 0;
			}
			pChar++;
		}
		return pChar;
	}

	nfChar * CXmlReader_Native::parseProcessingInstruction(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				if (pszStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYINSTRUCTIONNAME);
				pushEntity(pszStart, pChar, pChar, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseAttributes(pChar, pszEnd);

				break;

			case '/':
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDINSTRUCTIONNAME);

			case '?':
				if (pszStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYINSTRUCTIONNAME);
				pushEntity(pszStart, pChar, pChar, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION, true, false);
				pushZeroInsert(pChar);
				pChar++;

				return parseCloseProcessingInstruction(pChar, pszEnd);

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfChar * CXmlReader_Native::parseCloseProcessingInstruction(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		if (pszStart != pszEnd) {
			if (*pszStart != '>')
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTCLOSEINSTRUCTION);

			pushZeroInsert(pszStart);
			pushEntity(pszStart, pszStart, pszStart + 1, NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTIONEND, false, true);

			pszStart++;
			return pszStart;
		}

		return pszStart;
	}


	nfChar * CXmlReader_Native::parseEndElement(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				pChar++;
				break;

			case '/':
			case '?':
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTENDELEMENT);

			case '>':
				if (pszStart == pChar)
					throw CNMRException(NMR_ERROR_XMLPARSER_EMPTYENDELEMENT);
				pushZeroInsert(pChar);
				pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ELEMENTEND, true, true);
				pChar++;

				return pChar;

			default:
				pChar++;
			}

		}

		return pChar;

	}

	nfChar * CXmlReader_Native::parseCloseElement(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		if (pszStart != pszEnd) {
			if (*pszStart != '>')
				throw CNMRException(NMR_ERROR_XMLPARSER_COULDNOTCLOSEELEMENT);

			pushZeroInsert (pszStart);
			pushEntity(pszStart, pszStart, pszStart + 1, NMR_NATIVEXMLTYPE_CLOSEELEMENT, false, true);

			pszStart++;
			return pszStart;
		}

		return pszStart;
	}

	nfChar * CXmlReader_Native::parseAttributes(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {
			case 9:
			case 10:
			case 13:
			case 32:
				pChar = skipSpaces(++pChar, pszEnd);
				break;

			case '>':
				pChar++;
				return pChar;

			case '/':
				pChar++;
				return parseCloseElement(pChar, pszEnd);

			case '?':
				pChar++;
				return parseCloseProcessingInstruction(pChar, pszEnd);

			default:
				pChar = parseAttributeName(pChar, pszEnd);
				pChar = parseAttributeValue(pChar, pszEnd);
			}

		}

		return pChar;
	}


	nfChar * CXmlReader_Native::skipSpaces(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
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

	nfChar * CXmlReader_Native::parseAttributeName(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfBool bHadSpacing = false;
		nfChar * pChar = skipSpaces(pszStart, pszEnd);
		while (pChar != pszEnd) {
			switch (*pChar) {
			// name-ending characters
			case 9:
			case 10:
			case 13:
			case 32:
				// push the entity at one of the n name-ending characters
				// but continue to run through the string unti L'=' is reached
				// if another name-constituting character appears, throw exception
				bHadSpacing = true;
				pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBNAME, true, false);
				pushZeroInsert(pChar);
				pChar++;
				break;

			case 34:
			case 39:
				throw CNMRException(NMR_ERROR_XMLPARSER_INVALIDATTRIBUTENAME);

			case '=':
				if (!bHadSpacing) {
					pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBNAME, true, false);
					pushZeroInsert(pChar);
					pChar++;	
				}
				return pChar;

			// name-constituting characters
			default:
				if (bHadSpacing)
					throw CNMRException(NMR_ERROR_XMLPARSER_SPACEINATTRIBUTENAME);
				pChar++;
			}
		}


		return pChar;
	}

	nfChar * CXmlReader_Native::parseAttributeValue(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfBool bHadSpacing = false;
		nfChar * pChar = skipSpaces(pszStart, pszEnd);
		while (pChar != pszEnd) {
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
				return parseAttributeValueDoubleQuote(pChar, pszEnd);

			case 39:
				pChar++;
				return parseAttributeValueSingleQuote(pChar, pszEnd);

			default:
				if (bHadSpacing)
					throw CNMRException(NMR_ERROR_XMLPARSER_NOQUOTESAROUNDATTRIBUTE);
				pChar++;
			}
		}


		return pChar;
	}

	nfChar * CXmlReader_Native::parseAttributeValueDoubleQuote(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {

			case 34:
				pushZeroInsert(pChar);
				pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBVALUE, false, false);
				pChar++;
				return pChar;

			default:
				pChar++;
			}
		}


		return pChar;
	}

	nfChar * CXmlReader_Native::parseAttributeValueSingleQuote(_In_ nfChar * pszStart, _In_ nfChar * pszEnd)
	{
		nfChar * pChar = pszStart;
		while (pChar != pszEnd) {
			switch (*pChar) {

			case 39:
				pushZeroInsert(pChar);
				pushEntity(pszStart, pChar, pChar + 1, NMR_NATIVEXMLTYPE_ATTRIBVALUE, false, false);
				pChar++;
				return pChar;

			default:
				pChar++;
			}
		}


		return pChar;
	}


	void CXmlReader_Native::registerNameSpace(_In_ std::string sPrefix, _In_ std::string sURI)
	{
		m_sNameSpaces.insert(std::make_pair(sPrefix, sURI));
	}


	void CXmlReader_Native::pushZeroInsert(_In_ nfChar * pChar)
	{
		__NMRASSERT(pChar != nullptr);
		m_ZeroInsertArray[m_nZeroInsertIndex] = pChar;
		m_nZeroInsertIndex++;
	}
	
	inline void decodeXMLEscapeXMLStrings(nfChar* pChar) {
		if (strpbrk(pChar, "&") == nullptr) {
			return;
		}
		nfChar *pIterChar = pChar;
		nfChar *pWriteChar = pChar;

		nfChar *pAmp = nullptr;
		nfChar *pColon = nullptr;
		while (*pIterChar != 0) {
			if (*pIterChar == '&') {
				pAmp = pIterChar;
				pColon = nullptr;
			}
			else if (pAmp == nullptr) {
				*pWriteChar = *pIterChar;
				pWriteChar++;
			} else if (*pIterChar == ';') {
				if (pAmp != nullptr) {
					pColon = pIterChar;
					long long compareLen = pColon - pAmp + 1;
					if (strncmp(pAmp, "&quot;", static_cast<size_t>(std::min((long long)(6), compareLen))) == 0) {
						*pWriteChar = '\"';
						pWriteChar++;
					}
					else if (strncmp(pAmp, "&apos;", static_cast<size_t>(std::min((long long)(6), compareLen))) == 0) {
						*pWriteChar = '\'';
						pWriteChar++;
					}
					else if (strncmp(pAmp, "&lt;", static_cast<size_t>(std::min((long long)(4), compareLen))) == 0) {
						*pWriteChar = '<';
						pWriteChar++;
					}
					else if (strncmp(pAmp, "&gt;", static_cast<size_t>(std::min((long long)(4), compareLen))) == 0) {
						*pWriteChar = '>';
						pWriteChar++;
					}
					else if (strncmp(pAmp, "&amp;", static_cast<size_t>(std::min((long long)(5), compareLen))) == 0) {
						*pWriteChar = '&';
						pWriteChar++;
					}
					else {
						throw CNMRException(NMR_ERROR_XMLPARSER_INVALID_ESCAPESTRING);
					}
					pAmp = nullptr;
				}
			}
			pIterChar++;
		};
		if (pAmp != nullptr)
			throw CNMRException(NMR_ERROR_XMLPARSER_INVALID_ESCAPESTRING);
		*pWriteChar = 0;
	}

	void CXmlReader_Native::performEscapeStringDecoding()
	{
		for (nfUint32 nIndex  = m_nCurrentVerifiedEntityCount; nIndex < m_nCurrentEntityCount-1; nIndex++) {
			if (m_CurrentEntityTypes[nIndex] != NMR_NATIVEXMLTYPE_COMMENT) {
				decodeXMLEscapeXMLStrings(m_CurrentEntityList[nIndex]);
			}
		}
		m_nCurrentVerifiedEntityCount = m_nCurrentEntityCount;
	}

	void CXmlReader_Native::performZeroInserts()
	{
		nfUint32 nIndex;
		for (nIndex = 0; nIndex < m_nZeroInsertIndex; nIndex++) {
			nfChar * pChar = m_ZeroInsertArray[nIndex];
			*pChar = 0;
		}

		m_nZeroInsertIndex = 0;
	}

	void CXmlReader_Native::clearZeroInserts()
	{
		m_nZeroInsertIndex = 0;
	}


}
