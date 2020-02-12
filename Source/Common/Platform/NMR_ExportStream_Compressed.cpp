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

#include "Common/Platform/NMR_ExportStream_Compressed.h"
#include "Common/NMR_Exception.h"
 
namespace NMR {

	CExportStream_Compressed::CExportStream_Compressed(PExportStream pUncompressedStream)
	{
		if (nullptr == pUncompressedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		m_pUncompressedStream = pUncompressedStream;

		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;
		if (deflateInit(&m_strm, Z_DEFAULT_COMPRESSION) != Z_OK)
			throw CNMRException(NMR_ERROR_COULDNOTINITDEFLATE);
	}

	CExportStream_Compressed::~CExportStream_Compressed()
	{
		(void)deflateEnd(&m_strm);
	}

	nfBool CExportStream_Compressed::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfBool CExportStream_Compressed::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfBool CExportStream_Compressed::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfUint64 CExportStream_Compressed::getPosition()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfInt32 CExportStream_Compressed::compress(nfInt32 flush) {
		nfInt32 ret;
		do {
			m_strm.avail_out = (nfUint32) EXPORTSTREAM_WRITE_BUFFER_CHUNKSIZE;
			m_strm.next_out = out;
			ret = deflate(&m_strm, flush);
			switch (ret) {
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)deflateEnd(&m_strm);
				throw CNMRException(NMR_ERROR_COULDNOTDEFLATE);
			}
			int toWrite = EXPORTSTREAM_WRITE_BUFFER_CHUNKSIZE - m_strm.avail_out;
			m_pUncompressedStream->writeBuffer(out, toWrite);
		} while (m_strm.avail_out == 0);
		return ret;
	}

	nfUint64 CExportStream_Compressed::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
		if (nullptr == pBuffer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_strm.avail_in = (nfUint32) cbTotalBytesToWrite;
		m_strm.next_in = (Bytef *) pBuffer;
		compress(Z_NO_FLUSH);
		
		return cbTotalBytesToWrite;
	}

	void CExportStream_Compressed::close()
	{
		if (compress(Z_FINISH) != Z_STREAM_END) {
		 	throw CNMRException(NMR_ERROR_COULDNOTDEFLATE);
		}
	}

}
