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

NMR_PortableZIPWriter.cpp implements a portable stream to write into ZIP files

--*/

#include "Common/Platform/NMR_ExportStream_ZIP.h"
#include "Common/NMR_Exception.h"
 
namespace NMR {

	CExportStream_ZIP::CExportStream_ZIP(_In_ CPortableZIPWriter * pZIPWriter, nfUint32 nEntryKey)
	{
		m_bIsInitialized = false;

		if (pZIPWriter == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (nEntryKey == 0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pZIPWriter = pZIPWriter;
		m_nEntryKey = nEntryKey;

		m_pStream.next_in = nullptr;
		m_pStream.avail_in = 0;
		m_pStream.total_in = 0;

		m_pStream.msg = nullptr;
		m_pStream.state = nullptr;

		m_pStream.zalloc = nullptr;
		m_pStream.zfree = nullptr;
		m_pStream.opaque = nullptr;

		m_pStream.data_type = 0;
		m_pStream.adler = 0;
		m_pStream.reserved = 0;

		m_pStream.next_out = &m_nOutBuffer[0];
		m_pStream.avail_out = ZIPEXPORTBUFFERSIZE;
		m_pStream.total_out = 0;

		nfInt32 nResult = deflateInit2(&m_pStream, Z_BEST_SPEED, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
		if (nResult < 0)
			throw CNMRException(NMR_ERROR_DEFLATEINITFAILED);

		m_bIsInitialized = true;
	}

	CExportStream_ZIP::~CExportStream_ZIP()
	{
		if (m_bIsInitialized) {
			finishDeflate();
		}
	}

	nfBool CExportStream_ZIP::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		if (bHasToSucceed)
			throw CNMRException(NMR_ERROR_ZIPSTREAMCANNOTSEEK);
		return false;
	}

	nfBool CExportStream_ZIP::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		if (bHasToSucceed)
			throw CNMRException(NMR_ERROR_ZIPSTREAMCANNOTSEEK);
		return false;
	}

	nfBool CExportStream_ZIP::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		if (bHasToSucceed)
			throw CNMRException(NMR_ERROR_ZIPSTREAMCANNOTSEEK);
		return false;
	}

	nfUint64 CExportStream_ZIP::getPosition()
	{
		return m_pZIPWriter->getCurrentSize(m_nEntryKey);
	}

	nfUint64 CExportStream_ZIP::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
		if (!m_bIsInitialized)
			throw CNMRException(NMR_ERROR_ZIPALREADYFINISHED);

		nfUint64 cbCount = cbTotalBytesToWrite;
		const nfByte * pByte = (const nfByte *)pBuffer;

		while (cbCount > 0) {
			nfUint32 cbBytesWritten;
			if (cbCount < ZIPEXPORTWRITECHUNKSIZE)
				cbBytesWritten = writeChunk(pByte, (nfUint32)cbCount);
			else
				cbBytesWritten = writeChunk(pByte, ZIPEXPORTWRITECHUNKSIZE);

			if (cbBytesWritten == 0)
				throw CNMRException(NMR_ERROR_COULDNOTDEFLATE);

			cbCount -= cbBytesWritten;
		}

		return cbTotalBytesToWrite;
	}

	nfUint32 CExportStream_ZIP::writeChunk(_In_ const nfByte * pData, nfUint32 cbCount)
	{
		if ((pData == nullptr) || (cbCount == 0) || (cbCount > ZIPEXPORTWRITECHUNKSIZE))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pStream.next_in = (Bytef *) pData;
		m_pStream.avail_in = cbCount;

		m_pZIPWriter->calculateChecksum(m_nEntryKey, pData, cbCount);

		while (m_pStream.avail_in > 0) {
			nfInt32 nResult = deflate(&m_pStream, 0);
			if (nResult < 0)
				throw CNMRException(NMR_ERROR_COULDNOTDEFLATE);

			if (m_pStream.avail_out == 0) {
				m_pZIPWriter->writeDeflatedBuffer(m_nEntryKey, &m_nOutBuffer[0], ZIPEXPORTBUFFERSIZE);

				m_pStream.next_out = &m_nOutBuffer[0];
				m_pStream.avail_out = ZIPEXPORTBUFFERSIZE;
			}
		}

		return cbCount;

	}


	void CExportStream_ZIP::finishDeflate()
	{
		if (!m_bIsInitialized)
			throw CNMRException(NMR_ERROR_ZIPALREADYFINISHED);

		m_pStream.next_in = nullptr;
		m_pStream.avail_in = 0;

		nfBool bContinue = true;
		while (bContinue) {
			nfInt32 nResult = deflate(&m_pStream, Z_FINISH);
			if (nResult < 0)
				throw CNMRException(NMR_ERROR_COULDNOTDEFLATE);

			if ((nResult != Z_STREAM_END) && (m_pStream.avail_out == 0)) {
				m_pZIPWriter->writeDeflatedBuffer(m_nEntryKey, &m_nOutBuffer[0], ZIPEXPORTBUFFERSIZE);

				m_pStream.next_out = &m_nOutBuffer[0];
				m_pStream.avail_out = ZIPEXPORTBUFFERSIZE;
			}
			else
				bContinue = false;
		}

		if (m_pStream.avail_out < ZIPEXPORTBUFFERSIZE) {
			m_pZIPWriter->writeDeflatedBuffer(m_nEntryKey, &m_nOutBuffer[0], ZIPEXPORTBUFFERSIZE - m_pStream.avail_out);
		}

		deflateEnd(&m_pStream);
		m_bIsInitialized = false;
	}

	void CExportStream_ZIP::flushZIPStream()
	{
		finishDeflate();
	}

}
