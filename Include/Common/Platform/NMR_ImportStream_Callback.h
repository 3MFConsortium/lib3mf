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

NMR_ImportStream_Callback.h defines the CImportStream_Callback Class.
This is an abstract base stream class for importing with a callback to 
custom data reader.

--*/

#ifndef __NMR_IMPORTSTREAM_CALLBACK
#define __NMR_IMPORTSTREAM_CALLBACK

#include "Common/Platform/NMR_ImportStream.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <iostream>
#include <fstream>
#include <functional>

#define IMPORTSTREAM_CALLBACK_BUFFERSIZE (1024*1024)

namespace NMR {

	typedef std::function<nfUint32(nfByte*, nfUint64, _In_ void *)> ImportStream_ReadCallbackType;
	typedef std::function<nfUint32(nfUint64, _In_ void *)> ImportStream_SeekCallbackType;

	class CImportStream_Callback : public CImportStream {
	private:
		ImportStream_ReadCallbackType m_pReadCallback;
		ImportStream_SeekCallbackType m_pSeekCallback;
		void * m_pUserData;
		nfUint64 m_nPosition;
		nfUint64 m_nSize;
	public:
		CImportStream_Callback();
		CImportStream_Callback(_In_ ImportStream_ReadCallbackType pReadCallback, _In_ ImportStream_SeekCallbackType pSeekCallback, void * pUserData, _In_ nfUint64 nStreamSize);

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition();
		virtual nfUint64 readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll);
		virtual nfUint64 retrieveSize();
		virtual void writeToFile(_In_ const nfWChar * pwszFileName);
		virtual PImportStream copyToMemory();
		
	};

}

#endif // __NMR_IMPORTSTREAM_CALLBACK
