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

NMR_ExportStream_GCC.h defines the CExportStream_GCC Class.
This is an abstract base stream class for exporting in GCC.

--*/

#ifndef __NMR_EXPORTSTREAM_GCC_WIN32
#define __NMR_EXPORTSTREAM_GCC_WIN32

#include "Common/Platform/NMR_ExportStream.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#ifdef __GCC_WIN32
#include <Windows.h>
#endif // __GCC_WIN32

namespace NMR {

#ifdef __GCC_WIN32

	class CExportStream_GCC_Win32 : public CExportStream {
	private:
	    HANDLE m_hHandle;
	public:
		CExportStream_GCC_Win32(_In_ const nfWChar * pwszFileName);
		~CExportStream_GCC_Win32();

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition ();
		virtual nfUint64 writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite);
		void copyFrom(_In_ CImportStream * pImportStream, _In_ nfUint64 cbCount, _In_ nfUint32 cbBufferSize);
	};

#endif // __GCC_WIN32

}

#endif // __NMR_EXPORTSTREAM_GCC_WIN32
