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

NMR_XMLReader_Native.h defines a XML reader class with a native XML parsing implementation.

--*/

#ifndef __NMR_XMLREADER_NATIVE
#define __NMR_XMLREADER_NATIVE

#include "Common/Platform/NMR_XmlReader.h"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <array>


#define NMR_MAXXMLNAMELENGTH 1000000
#define NMR_MAXXMLSTRINGLENGTH 1000000
#define NMR_MAXXMLDEPTH 1024

#define NMR_NATIVEXMLREADER_MINBUFFERCAPACITY 1024
#define NMR_NATIVEXMLREADER_MAXBUFFERCAPACITY (1024 * 1024 * 1024)

#define NMR_NATIVEXMLREADER_BUFFERMARGIN 8 

#define NMR_NATIVEXMLTYPE_NONE 0
#define NMR_NATIVEXMLTYPE_TEXT 1
#define NMR_NATIVEXMLTYPE_ELEMENT 2
#define NMR_NATIVEXMLTYPE_ELEMENTEND 3
#define NMR_NATIVEXMLTYPE_CLOSEELEMENT 4
#define NMR_NATIVEXMLTYPE_ATTRIBNAME 5
#define NMR_NATIVEXMLTYPE_ATTRIBVALUE 6
#define NMR_NATIVEXMLTYPE_COMMENT 7
#define NMR_NATIVEXMLTYPE_CDATA 8
#define NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTION 9
#define NMR_NATIVEXMLTYPE_PROCESSINGINSTRUCTIONEND 10

#define NMR_NATIVEXMLNS_XML_PREFIX L"xml"
#define NMR_NATIVEXMLNS_XML_URI L"http://www.w3.org/XML/1998/namespace"

#define NMR_NATIVEXMLNS_XMLNS_PREFIX L"xmlns"
#define NMR_NATIVEXMLNS_XMLNS_URI L"http://www.w3.org/2000/xmlns/"

namespace NMR {

	class CXmlReader_Native : public CXmlReader {
	private:
		nfUint32 m_cbBufferCapacity;
		// Allocated memory of current and next chunk
		std::vector<nfWChar> m_UTF16Buffer1;
		std::vector<nfWChar> m_UTF16Buffer2;

		// Temporary buffer to read into
		std::vector<nfByte> m_TempBuffer;

		// Double buffer pointers to UTF16BufferX
		std::vector<nfWChar> * m_pCurrentBuffer;
		std::vector<nfWChar> * m_pNextBuffer;

		// parsed entity list
		std::vector<nfWChar *> m_CurrentEntityList;
		std::vector<nfWChar *> m_CurrentEntityPrefixes;
		std::vector<nfByte> m_CurrentEntityTypes;

		void performEscapeStringDecoding();

		// Insert List for 0 characters
		nfUint32 m_nZeroInsertIndex;
		std::vector<nfWChar *> m_ZeroInsertArray;
		void pushZeroInsert(_In_ nfWChar * pChar);
		void performZeroInserts();
		void clearZeroInserts();

		// how large is the current buffer
		nfUint32 m_nCurrentBufferSize;
		nfUint32 m_nCurrentEntityCount;
		nfUint32 m_nCurrentVerifiedEntityCount;
		nfUint32 m_nCurrentFullEntityCount;
		nfUint32 m_nCurrentEntityIndex;
		nfWChar * m_pCurrentEntityPointer;

		// Parsing Flag
		nfBool m_bIsEOF;

		// How many characters have to be transferred into the next buffer?
		nfUint32 m_cbCurrentOverflowSize;
		nfWChar * m_pCurrentName;
		nfWChar * m_pCurrentPrefix;
		nfWChar * m_pCurrentElementName;
		nfWChar * m_pCurrentElementPrefix;
		nfWChar * m_pCurrentValue;
		nfWChar m_cNullString;

		// NameSpace handling
		std::wstring m_sDefaultNameSpace;
		nfUint32 m_cbDefaultNameSpaceLength;
		nfBool m_bNameSpaceIsAttribute;
		std::map<std::wstring, std::wstring> m_sNameSpaces;
		void registerNameSpace(_In_ std::wstring sPrefix, _In_ std::wstring sURI);


		// Fill next buffer chunk
		nfBool ensureFilledBuffer();
		void readNextBufferFromStream();

		// Parse Text Buffer
		nfWChar * parseUnknown(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseText(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseComment(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseEndElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseCloseElement(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseAttributes(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * skipSpaces(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseAttributeName(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseAttributeValue(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseAttributeValueDoubleQuote(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseAttributeValueSingleQuote(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);

		nfWChar * parseProcessingInstruction(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);
		nfWChar * parseCloseProcessingInstruction(_In_ nfWChar * pszwStart, _In_ nfWChar * pszwEnd);

		void pushEntity(_In_ nfWChar * pszwEntityStartChar, _In_ nfWChar * pszwEntityEndDelimiter, _In_ nfWChar * pszwNextEntityChar, _In_ nfByte nType, _In_ nfBool bParseForNamespaces, _In_ nfBool bEntityIsFinished);

	public:
		CXmlReader_Native(_In_ PImportStream pImportStream, _In_ nfUint32 cbBufferCapacity);
		~CXmlReader_Native();

		virtual void GetValue(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue);
		virtual void GetLocalName(_Outptr_result_buffer_maybenull_(*pcwchLocalName + 1) const nfWChar ** ppwszLocalName, _Out_opt_ nfUint32 *pcwchLocalName);
		virtual void GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue);
		virtual bool GetNamespaceURI(const std::wstring &sNameSpacePrefix, std::wstring &sNameSpaceURI);
		virtual bool NamespaceRegistered(const std::wstring &sNameSpaceURI);

		virtual nfBool Read(_Out_ eXmlReaderNodeType & NodeType);
		virtual nfBool IsEOF();
		virtual nfBool IsEmptyElement();
		virtual nfBool MoveToFirstAttribute();
		virtual nfBool MoveToNextAttribute();
		virtual nfBool IsDefault();
		virtual void CloseElement();

	};

	typedef std::shared_ptr<CXmlReader_Native> PXmlReader_Native;

}

#endif // __NMR_XMLREADER_NATIVE
