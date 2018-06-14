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

NMR_ImportStream_Memory.h defines the CImportStream_Memory Class.
This is a platform independent class for keeping data in a memory stream.

--*/

#ifndef __NMR_IMPORTSTREAM_MEMORY
#define __NMR_IMPORTSTREAM_MEMORY

#include "Common/Platform/NMR_ImportStream.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <vector>

#define NMR_IMPORTSTREAM_READCHUNKSIZE (1024*1024)

#define NMR_IMPORTSTREAM_MAXMEMSTREAMSIZE (1024ULL*1024ULL*1024ULL*1024ULL)

namespace NMR {

	class CImportStream_Memory : public CImportStream {
	private:
		std::vector<nfByte> m_Buffer;
		nfUint64 m_cbSize;
		nfUint64 m_nPosition;
	public:
		CImportStream_Memory();
		CImportStream_Memory(_In_ CImportStream * pStream, _In_ nfUint64 cbBytesToCopy, _In_ nfBool bNeedsToCopyAllBytes);
		CImportStream_Memory(_In_ const nfByte * pBuffer, _In_ nfUint64 cbBytes);
		~CImportStream_Memory();

		virtual nfBool seekPosition(_In_ nfUint64 nPosition, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 cbBytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 cbBytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition();
		virtual nfUint64 readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll);
		virtual nfUint64 retrieveSize();
		virtual void writeToFile(_In_ const nfWChar * pwszFileName);
		virtual PImportStream copyToMemory();
	};

}

#endif // __NMR_IMPORTSTREAM_MEMORY
