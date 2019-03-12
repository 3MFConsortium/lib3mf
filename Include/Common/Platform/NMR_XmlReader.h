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

NMR_XMLReader.h defines a XML reader class in a portable way.

--*/

#ifndef __NMR_XMLREADER
#define __NMR_XMLREADER

#include "Common/Platform/NMR_ImportStream.h"
#include <string>

namespace NMR {

	enum eXmlReaderNodeType {
		XMLREADERNODETYPE_UNKNOWN,
		XMLREADERNODETYPE_STARTELEMENT,
		XMLREADERNODETYPE_ENDELEMENT,
		XMLREADERNODETYPE_TEXT
	};

	class CXmlReader {
	protected:
		PImportStream m_pImportStream;
	public:
		CXmlReader(_In_ PImportStream pImportStream);

		virtual void GetLocalName(_Outptr_result_buffer_maybenull_(*pcchLocalName + 1) const nfChar ** ppszLocalName, _Out_opt_ nfUint32 *pcchLocalName) = 0;
		virtual void GetValue(_Outptr_result_buffer_maybenull_(*pcchValue + 1)  const nfChar ** ppszValue, _Out_opt_  nfUint32 *pcwchValue) = 0;
		virtual void GetNamespaceURI(_Outptr_result_buffer_maybenull_(*pcchValue + 1)  const nfChar ** ppszValue, _Out_opt_  nfUint32 *pcchValue) = 0;
		virtual bool GetNamespaceURI(const std::string &sNameSpacePrefix, std::string &sNameSpaceURI) = 0;
		virtual bool NamespaceRegistered(const std::string &sNameSpaceURI) = 0;

		virtual nfBool Read(_Out_ eXmlReaderNodeType & NodeType) = 0;
		virtual nfBool IsEOF() = 0;
		virtual nfBool IsEmptyElement() = 0;
		virtual nfBool MoveToFirstAttribute() = 0;
		virtual nfBool MoveToNextAttribute() = 0;
		virtual nfBool IsDefault() = 0;
		virtual void CloseElement();
	};

	typedef std::shared_ptr<CXmlReader> PXmlReader;

}

#endif // __NMR_XMLREADER
