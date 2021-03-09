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

NMR_XmlWriter_Native.h implements an XML Writer Class without dependencies.

--*/

#ifndef __NMR_XMLWRITER_NATIVE
#define __NMR_XMLWRITER_NATIVE

#include "Common/Platform/NMR_XmlWriter.h"
#include <array>
#include <map>
#include <list>
#include <string>

#define NATIVEXMLSPACINGBUFFERSIZE 256
#define NATIVEXMLSPACING 9

#define NATIVEXMLWRITEBUFFERSIZE 65536
#define NATIVEXMLMAXSTRINGLENGTH 1048576

#define NATIVEXMLENCODING "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
#define NATIVEXMLFIXEDENCODINGLENGTH 2048
#define NATIVEXMLENCODINGBUFFERSIZE ((NATIVEXMLFIXEDENCODINGLENGTH * 6) + 1)


namespace NMR {

	class CXmlWriter_Native : public CXmlWriter {
	private:
		std::array<nfByte, NATIVEXMLSPACINGBUFFERSIZE> m_SpacingBuffer;
		std::list<std::string> m_NodeStack;

		std::array<nfChar, NATIVEXMLENCODINGBUFFERSIZE> m_FixedEncodingBuffer;

		std::map<std::string, std::string> m_sNameSpaces;

		nfBool m_bElementIsOpen;
		nfBool m_bIsFreshLine;

		nfByte m_nLineEndingBuffer[2];
		nfUint32 m_nLineEndingCharCount;
		nfUint32 m_nSpacesPerLayer;
		nfUint32 m_nLayer;

		void writeSpaces(_In_ nfUint32 cbCount);
		void writeData(_In_ const void * pData, _In_ nfUint32 cbLength);
		void writeUTF8(_In_ const nfChar * pszString, _In_ nfBool bNewLine);
		void writeUTF16(_In_ const nfWChar * pszString, _In_ nfBool bNewLine);

		void closeCurrentElement(_In_ nfBool bNewLine);

		// Buffer needs to be at least 6 times the length of pszwpszString
		void escapeXMLString(_In_z_ const nfChar * pszString, _Out_ nfChar * pszBuffer);

	public:
		CXmlWriter_Native(_In_ PExportStream pExportStream);

		virtual void WriteStartDocument();
		virtual void WriteEndDocument();
		virtual void Flush();

		virtual void WriteAttributeString(_In_opt_ const nfChar *  pszPrefix, _In_opt_ const nfChar *  pszLocalName, _In_opt_ const nfChar *  pszNamespaceUri, _In_opt_ const nfChar *  pszValue);
		virtual void WriteStartElement(_In_opt_  const nfChar *  pszPrefix, _In_  const nfChar *  pszLocalName, _In_opt_  const nfChar *  pszNamespaceUri);
		virtual void WriteEndElement();
		virtual void WriteFullEndElement();

		virtual void WriteText(_In_ const nfChar * pszContent, _In_ const nfUint32 cbLength);
		virtual void WriteRawLine(_In_ const nfChar * pszRawData, _In_ nfUint32 cbCount);

		virtual bool GetNamespacePrefix(const std::string &sNameSpaceURI, std::string &sNameSpacePrefix);
		virtual void RegisterCustomNameSpace(const std::string &sNameSpace, const std::string &sNameSpacePrefix);
		virtual nfUint32 GetNamespaceCount();
		virtual std::string GetNamespacePrefix(nfUint32 nIndex);
		virtual std::string GetNamespace(nfUint32 nIndex);
	};

	typedef std::shared_ptr<CXmlWriter_Native> PXmlWriter_Native;

}

#endif // __NMR_XMLWRITER_NATIVE
