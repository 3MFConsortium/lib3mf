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

NMR_XmlReader_COM.h defines the XML Reader Class using XMLLite on Windows.

--*/

#ifndef __NMR_XMLREADER_COM
#define __NMR_XMLREADER_COM

#include "Common/Platform/NMR_XmlReader.h"
#include "Common/Platform/NMR_CComPtr.h"
#include <xmllite.h>

namespace NMR {

	class CXmlReader_COM : public CXmlReader {
	private:
		CComPtr<IXmlReader> m_pXMLReader;
		CComPtr<IStream> m_pCOMStream;
	public:
		CXmlReader_COM(_In_ PImportStream pImportStream);

		virtual void GetQualifiedName(_Outptr_result_buffer_maybenull_(*pcwchQualifiedName + 1) const nfWChar ** ppwszQualifiedName, _Out_opt_ nfUint32 *pcwchQualifiedName);
		virtual void GetValue(_Outptr_result_buffer_maybenull_(*pcwchValue + 1)  const nfWChar ** ppwszValue, _Out_opt_  nfUint32 *pcwchValue);

		virtual nfBool Read(_Out_ eXmlReaderNodeType & NodeType);
		virtual nfBool IsEOF();
		virtual nfBool IsEmptyElement();
		virtual nfBool MoveToFirstAttribute();
		virtual nfBool MoveToNextAttribute();
		virtual nfBool IsDefault();
	};

	typedef std::shared_ptr<CXmlReader_COM> PXmlReader_COM;

}

#endif // __NMR_XMLREADER_COM
