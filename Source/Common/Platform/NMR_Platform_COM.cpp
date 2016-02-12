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

NMR_Platform_COM.cpp implements several factory functions which create platform
specific classes for COM.

--*/

#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_Exception.h"

#ifndef __GNUC__
// Only implement this, if we compile with Visual Studio

#include "Common/Platform/NMR_ImportStream_COM.h"
#include "Common/Platform/NMR_ExportStream_COM.h"
#include "Common/Platform/NMR_XmlReader_COM.h"

namespace NMR {

	PImportStream fnCreateImportStreamInstance (_In_ const nfWChar * pwszFileName)
	{
		return std::make_shared<CImportStream_COM> (pwszFileName);
	}

	PExportStream fnCreateExportStreamInstance (_In_ const nfWChar * pwszFileName)
	{
		return std::make_shared<CExportStream_COM> (pwszFileName);
	}
		
	PXmlReader fnCreateXMLReaderInstance (_In_ PImportStream pImportStream)
	{
		return std::make_shared<CXmlReader_COM> (pImportStream);
	}


}

#endif // __GNUC__
