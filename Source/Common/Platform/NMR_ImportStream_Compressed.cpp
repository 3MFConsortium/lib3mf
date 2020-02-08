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


// BEGIN remove this code
//#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
//#  include <fcntl.h>
//#  include <io.h>
//#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
//#else
//#  define SET_BINARY_MODE(file)
//#endif

#define CHUNK 16384
int def(FILE *source, FILE *dest, int level)
{
	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return ret;

	/* compress until end of file */
	do {
		strm.avail_in = (uInt) fread(in, 1, CHUNK, source);
		if (ferror(source)) {
			(void)deflateEnd(&strm);
			return Z_ERRNO;
		}
		flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;

		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = deflate(&strm, flush);    /* no bad return value */
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)deflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);
		assert(strm.avail_in == 0);     /* all input will be used */

										/* done when last data in file processed */
	} while (flush != Z_FINISH);
	assert(ret == Z_STREAM_END);        /* stream will be complete */

										/* clean up and return */
	(void)deflateEnd(&strm);
	return Z_OK;
}
int inf(FILE *source, FILE *dest)
{
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = (uInt) fread(in, 1, CHUNK, source);
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
// END remove this code

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
		// BEGIN remove this code
		/*FILE* in = fopen("E:\\project\\lib3mf\\Tests\\TestFiles\\SecureContent\\keystore\\3D\\new-compressed", "r");
		FILE* out = fopen("E:\\project\\lib3mf\\Tests\\TestFiles\\SecureContent\\keystore\\3D\\new-out", "a+");
		SET_BINARY_MODE(in);
		SET_BINARY_MODE(out);
		//int ret = def(in, out, Z_DEFAULT_COMPRESSION);
		int ret = inf(in, out);
		fclose(in);
		fclose(out);*/
		// END remove this code
		
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
