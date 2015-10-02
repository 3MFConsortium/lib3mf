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

NMR_XMLWriter.h defines the XML Writer Class in a portable way.

--*/

#ifndef __NMR_XMLWRITER
#define __NMR_XMLWRITER

#include "Common/Platform/NMR_ExportStream.h"

namespace NMR {

	class CXmlWriter {
	protected:
		PExportStream m_pExportStream;
	public:
		CXmlWriter(_In_ PExportStream pExportStream);

		virtual void WriteStartDocument() = 0;
		virtual void WriteEndDocument() = 0;
		virtual void Flush() = 0;

		virtual void WriteAttributeString(_In_opt_ const nfWChar * pwszPrefix, _In_opt_ const nfWChar *  pwszLocalName, _In_opt_ const nfWChar *  pwszNamespaceUri, _In_opt_ const nfWChar *  pwszValue) = 0;
		virtual void WriteStartElement(_In_opt_  const nfWChar *  pwszPrefix, _In_  const nfWChar *  pwszLocalName, _In_opt_  const nfWChar *  pwszNamespaceUri) = 0;
		virtual void WriteEndElement() = 0;
		virtual void WriteFullEndElement() = 0;
		virtual void WriteRawLine(_In_ const nfChar * pszRawData, _In_ nfUint32 cbCount) = 0;

		virtual void WriteText(_In_ const nfWChar * pwszContent, _In_ const nfUint32 cbLength) = 0;
	};

	typedef std::shared_ptr<CXmlWriter> PXmlWriter;

}

#endif // __NMR_XMLWRITER
