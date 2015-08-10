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

NMR_XmlWriter_COM.h defines the XML Writer Class using XMLLite on Windows.

--*/

#ifndef __NMR_XMLWRITER_COM
#define __NMR_XMLWRITER_COM

#include "Common/Platform/NMR_XmlWriter.h"
#include "Common/Platform/NMR_CComPtr.h"
#include <xmllite.h>

namespace NMR {

	class CXmlWriter_COM : public CXmlWriter {
	private:
		CComPtr<IXmlWriter> m_pXMLWriter;
		CComPtr<IStream> m_pCOMStream;
	public:
		CXmlWriter_COM(_In_ PExportStream pExportStream);

		virtual void WriteStartDocument();
		virtual void WriteEndDocument();
		virtual void Flush();

		virtual void WriteAttributeString(_In_opt_ LPCWSTR pwszPrefix, _In_opt_ LPCWSTR pwszLocalName, _In_opt_ LPCWSTR pwszNamespaceUri, _In_opt_ LPCWSTR pwszValue);
		virtual void WriteStartElement(_In_opt_  LPCWSTR pwszPrefix, _In_  LPCWSTR pwszLocalName, _In_opt_  LPCWSTR pwszNamespaceUri);
		virtual void WriteEndElement();
		virtual void WriteFullEndElement();

		virtual void WriteText(_In_ const nfWChar * pwszContent, _In_ const nfUint32 cbLength);
	};

	typedef std::shared_ptr<CXmlWriter_COM> PXmlWriter_COM;

}

#endif // __NMR_XMLWRITER_COM
