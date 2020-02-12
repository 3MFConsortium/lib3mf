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

NMR_ExportStream_Compressed.h defines a stream to write compressed files

--*/

#ifndef __NMR_EXPORTSTREAM_COMPRESSED
#define __NMR_EXPORTSTREAM_COMPRESSED

#include "Common/NMR_Types.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_PortableZIPWriter.h"
#include "Libraries/zlib/zlib.h"

#define EXPORTSTREAM_WRITE_BUFFER_CHUNKSIZE 1024

namespace NMR {

	class CExportStream_Compressed: public CExportStream {
	private:
		z_stream m_strm;
		PExportStream m_pUncompressedStream;
		nfByte out[EXPORTSTREAM_WRITE_BUFFER_CHUNKSIZE];

		nfInt32 compress(nfInt32 flush);
	public:
		CExportStream_Compressed() = delete;
		CExportStream_Compressed(PExportStream pUncompressedStream);
		~CExportStream_Compressed();

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition();
		virtual nfUint64 writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite);
		virtual void close();
	};

	typedef std::shared_ptr <CExportStream_Compressed> PExportStream_Compressed;

}

#endif // __NMR_EXPORTSTREAM_COMPRESSED
