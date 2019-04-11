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

NMR_ImportStream.h defines the CImportStream Class.
This is an abstract base stream class for importing from various data sources.

--*/

#ifndef __NMR_IMPORTSTREAM
#define __NMR_IMPORTSTREAM

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#define NMR_IMPORTSTREAM_COPYBUFFERSIZE (128 * 1024)
#define NMR_IMPORTSTREAM_READBUFFERSIZE (128 * 1024)

namespace NMR {

	class CImportStream;
	typedef std::shared_ptr <CImportStream> PImportStream;

	class CImportStream {
	private:
	public:
		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed) = 0;
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) = 0;
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) = 0;
		virtual nfUint64 readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll) = 0;
		virtual nfUint64 retrieveSize() = 0;
		virtual void writeToFile(_In_ const nfWChar * pwszFileName) = 0;
		virtual PImportStream copyToMemory() = 0;
		virtual nfUint64 getPosition() = 0;
	};

}

#endif // __NMR_IMPORTSTREAM
