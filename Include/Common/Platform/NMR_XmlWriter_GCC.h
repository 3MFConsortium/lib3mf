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

NMR_XmlWriter_GCC.h defines the XML Writer Class using GCC.

--*/

#ifndef __NMR_XMLWRITER_GCC
#define __NMR_XMLWRITER_GCC

#include "Common/Platform/NMR_XmlWriter.h"

namespace NMR {

	class CXmlWriter_GCC : public CXmlWriter {
	private:
	public:
		CXmlWriter_GCC(_In_ PExportStream pExportStream);

		virtual void WriteStartDocument();
		virtual void WriteEndDocument();
		virtual void Flush();

		virtual void WriteAttributeString(_In_opt_ const nfWChar *  pwszPrefix, _In_opt_ const nfWChar *  pwszLocalName, _In_opt_ const nfWChar *  pwszNamespaceUri, _In_opt_ const nfWChar *  pwszValue);
		virtual void WriteStartElement(_In_opt_  const nfWChar *  pwszPrefix, _In_  const nfWChar *  pwszLocalName, _In_opt_  const nfWChar *  pwszNamespaceUri);
		virtual void WriteEndElement();
		virtual void WriteFullEndElement();
	};

	typedef std::shared_ptr<CXmlWriter_GCC> PXmlWriter_GCC;

}

#endif // __NMR_XMLWRITER_GCC
