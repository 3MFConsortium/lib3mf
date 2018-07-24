/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ExportStream_Callback.h defines the CExportStream_Callback Class.
This is an abstract base stream class for exporting with a callback to 
custom data writer.

--*/

#ifndef __NMR_EXPORTSTREAM_CALLBACK
#define __NMR_EXPORTSTREAM_CALLBACK

#include "Common/Platform/NMR_ExportStream.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <iostream>
#include <fstream>

#define EXPORTSTREAM_CALLBACK_BUFFERSIZE (1024*1024)

namespace NMR {

	typedef nfUint32(LIB3MF_CDECL *ExportStream_WriteCallbackType)(_In_ nfByte * pData, _In_ nfUint32 cbBytes, _In_ void * pUserData);
	typedef nfUint32(LIB3MF_CDECL *ExportStream_SeekCallbackType)(_In_ nfUint64 nPosition, _In_ void * pUserData);

	class CExportStream_Callback : public CExportStream {
	private:
		ExportStream_WriteCallbackType m_pWriteCallback;
		ExportStream_SeekCallbackType m_pSeekCallback;
		void * m_pUserData;
		nfUint64 m_nPosition;
		nfUint64 m_nSize;
	public:
		CExportStream_Callback();
		CExportStream_Callback(_In_ ExportStream_WriteCallbackType pWriteCallback, _In_ ExportStream_SeekCallbackType pSeekCallback, void * pUserData);

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition();
		virtual nfUint64 writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite);
	};

}

#endif // __NMR_EXPORTSTREAM_CALLBACK
