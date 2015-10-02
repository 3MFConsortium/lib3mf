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

NMR_PortableZIPWriter.h defines a portable stream to write into ZIP files

--*/

#ifndef __NMR_EXPORTSTREAM_ZIP
#define __NMR_EXPORTSTREAM_ZIP

#include "Common/NMR_Types.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_PortableZIPWriter.h"
#include "Libraries/zlib/zlib.h"

#include <array>

#define ZIPEXPORTBUFFERSIZE 65536
#define ZIPEXPORTWRITECHUNKSIZE 1048576

namespace NMR {

	class CExportStream_ZIP : public CExportStream {
	private:
		CPortableZIPWriter * m_pZIPWriter;
		nfUint32 m_nEntryKey;
		z_stream m_pStream;
		std::array<nfByte, ZIPEXPORTBUFFERSIZE> m_nOutBuffer;

		nfBool m_bIsInitialized;

		nfUint32 writeChunk(_In_ const nfByte * pData, nfUint32 cbCount);
		void finishDeflate();
	public:
		CExportStream_ZIP() = delete;
		CExportStream_ZIP(_In_ CPortableZIPWriter * pZIPWriter, nfUint32 nEntryKey);
		~CExportStream_ZIP();

		virtual nfBool seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed);
		virtual nfBool seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfBool seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed);
		virtual nfUint64 getPosition();
		virtual nfUint64 writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite);

		void flushZIPStream();
	};

	typedef std::shared_ptr <CExportStream_ZIP> PExportStream_ZIP;

}

#endif // __NMR_EXPORTSTREAM_ZIP
