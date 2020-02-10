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

NMR_ImportStream_Compressed.h defines the CImportStream_Compressed Class.
This is a stream class for importing from a compressed object.

--*/

#ifndef __NMR_IMPORTSTREAM_COMPRESSED
#define __NMR_IMPORTSTREAM_COMPRESSED

#include "Common/Platform/NMR_ImportStream.h"
#include "Libraries/zlib/zlib.h"

#define IMPORTSTREAM_READ_BUFFER_CHUNKSIZE 1024
#define IMPORTSTREAM_COMPRESSED_CHUNKSIZE 1024

namespace NMR {

	class CImportStream_Compressed : public CImportStream {
	private:
		PImportStream m_pCompressedStream;
		z_stream m_strm;
		std::vector<nfByte> m_decompressedBuffer;
		nfByte in[IMPORTSTREAM_COMPRESSED_CHUNKSIZE];
	public:
		CImportStream_Compressed(PImportStream pCompressedStream);
		~CImportStream_Compressed();

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 readBuffer(nfByte * pBuffer, nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll);
		virtual nfUint64 retrieveSize();
		virtual void writeToFile(_In_ const nfWChar * pwszFileName);
		virtual PImportStream copyToMemory();
		virtual nfUint64 getPosition();
	};

}

#endif // __NMR_IMPORTSTREAM_COMPRESSED
