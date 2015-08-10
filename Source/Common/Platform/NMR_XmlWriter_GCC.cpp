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

NMR_XmlWriter_GCC.cpp implements the XML Writer Class using GCC.

--*/

#include "Common/Platform/NMR_XmlWriter_GCC.h"
#include "Common/Platform/NMR_ExportStream_GCC.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CXmlWriter_GCC::CXmlWriter_GCC(_In_ PExportStream pExportStream)
		: CXmlWriter (pExportStream)
	{
		// empty on purpose
	}

	void CXmlWriter_GCC::WriteStartDocument()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::WriteEndDocument()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::Flush()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::WriteAttributeString(_In_opt_ LPCWSTR pwszPrefix, _In_opt_ LPCWSTR pwszLocalName, _In_opt_ LPCWSTR pwszNamespaceUri, _In_opt_ LPCWSTR pwszValue)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::WriteStartElement(_In_opt_  LPCWSTR pwszPrefix, _In_  LPCWSTR pwszLocalName, _In_opt_  LPCWSTR pwszNamespaceUri)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::WriteEndElement()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CXmlWriter_GCC::WriteFullEndElement()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

    void CXmlWriter_GCC::WriteText(_In_ const nfWChar * pwszContent, _In_ const nfUint32 cbLength)
    {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

}
