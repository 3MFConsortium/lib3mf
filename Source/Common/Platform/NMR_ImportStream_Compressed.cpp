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

#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Compressed.h"
#include "Common/NMR_Exception.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fstream>

namespace NMR {

	CImportStream_Compressed::CImportStream_Compressed(PImportStream pCompressedStream)
	{
		if (nullptr == pCompressedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		m_pCompressedStream = pCompressedStream;
	}

	nfBool CImportStream_Compressed::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		return m_pCompressedStream->seekPosition(position, bHasToSucceed);
	}

	nfBool CImportStream_Compressed::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		return m_pCompressedStream->seekForward(bytes, bHasToSucceed);
	}

	nfBool CImportStream_Compressed::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		return m_pCompressedStream->seekFromEnd(bytes, bHasToSucceed);
	}

	nfUint64 CImportStream_Compressed::getPosition()
	{
		return m_pCompressedStream->getPosition();
	}

	nfUint64 CImportStream_Compressed::readBuffer(nfByte * pBuffer, nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		return m_pCompressedStream->readBuffer(pBuffer, cbTotalBytesToRead, bNeedsToReadAll);
	}

	void CImportStream_Compressed::decompress()
	{
		// Source: http://zlib.net/zpipe.c
		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;
		m_strm.avail_in = 0;
		m_strm.next_in = Z_NULL;

		int ret = inflateInit(&m_strm);
		if (ret != Z_OK)
			throw CNMRException(NMR_ERROR_COULDNOTINITINFLATE);

		nfUint64 totalBytesDecompressed = 0;
		nfByte in[IMPORTSTREAM_COMPRESSED_CHUNKSIZE];
		nfByte out[IMPORTSTREAM_COMPRESSED_CHUNKSIZE];
		do {
			m_strm.avail_in = (nfUint32) readBuffer(in, IMPORTSTREAM_COMPRESSED_CHUNKSIZE, false);
			if (m_strm.avail_in == 0)
				break;
			m_strm.next_in = in;
			do {
				m_strm.avail_out = IMPORTSTREAM_COMPRESSED_CHUNKSIZE;
				m_strm.next_out = out;
				ret = inflate(&m_strm, Z_NO_FLUSH);
				assert(ret != Z_STREAM_ERROR);
				switch (ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&m_strm);
					throw CNMRException(NMR_ERROR_COULDNOTINFLATE);
				}
				nfUint64 bytesDecompressed = IMPORTSTREAM_COMPRESSED_CHUNKSIZE - m_strm.avail_out;
				if (0 < bytesDecompressed) {
					totalBytesDecompressed += bytesDecompressed;
					m_decompressedBuffer.resize(totalBytesDecompressed);
					m_decompressedBuffer.insert(m_decompressedBuffer.end() - bytesDecompressed, &out[0], &out[bytesDecompressed]);
				}
			} while (m_strm.avail_out == 0);
		} while (ret != Z_STREAM_END);

		(void) inflateEnd(&m_strm);
		if (ret != Z_STREAM_END) {
			throw CNMRException(NMR_ERROR_COULDNOTINFLATE);
		}
	}

	nfUint64 CImportStream_Compressed::retrieveSize()
	{
		return m_decompressedBuffer.size();
	}

	void CImportStream_Compressed::writeToFile(_In_ const nfWChar * pwszFileName)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	PImportStream CImportStream_Compressed::copyToMemory()
	{
		decompress();
		return std::make_shared<CImportStream_Shared_Memory>(m_decompressedBuffer.data(), m_decompressedBuffer.size());
	}


}
