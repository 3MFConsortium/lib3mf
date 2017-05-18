/*++

Copyright (C) 2015 Microsoft Corporation
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

NMR_ExportStream_COM.cpp implements the ExportStream_COM Class.
This is an abstract base stream class for exporting to COM IStreams.

--*/

#include "Common/Platform/NMR_ExportStream_COM.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include <cmath>

namespace NMR {

	CExportStream_COM::CExportStream_COM()
	{
		// empty on purpose
	}

	CExportStream_COM::CExportStream_COM(_In_ CComPtr <IStream> pStream) : CExportStream()
	{
		if (pStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pStream = pStream;
	}

	CExportStream_COM::CExportStream_COM(_In_ LPCWSTR pwszFileName)
	{
		if (!pwszFileName)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		HRESULT hResult = SHCreateStreamOnFileEx(pwszFileName, STGM_CREATE | STGM_WRITE, 0, false, NULL, &m_pStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEFILE, hResult);
	}

	nfBool CExportStream_COM::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER filepos;
		HRESULT hResult;

		filepos.QuadPart = position;
		hResult = m_pStream->Seek(filepos, STREAM_SEEK_SET, NULL);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}

	nfBool CExportStream_COM::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER offset;
		HRESULT hResult;

		offset.QuadPart = bytes;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_CUR, NULL);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}

	nfBool CExportStream_COM::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER offset;
		HRESULT hResult;

		offset.QuadPart = bytes;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_END, NULL);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}

	nfUint64 CExportStream_COM::getPosition()
	{
		LARGE_INTEGER offset;
		ULARGE_INTEGER filepos;
		HRESULT hResult;

		offset.QuadPart = 0;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_CUR, &filepos);

		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return filepos.QuadPart;
	}

	nfUint64 CExportStream_COM::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
		HRESULT hResult;
		ULONG cbBytesToWrite;
		ULONG cbWrittenBytes;
		nfUint64 cbTotalWrittenBytes = 0;
		nfUint64 cbBytesLeft = cbTotalBytesToWrite;

		if (cbBytesLeft == 0)
			return cbBytesLeft;

		nfByte * pData = (nfByte *)pBuffer;
		
		while (cbBytesLeft > 0) {
			if (cbBytesLeft > NMR_EXPORTSTREAM_WRITEBUFFERSIZE)
				cbBytesToWrite = NMR_EXPORTSTREAM_WRITEBUFFERSIZE;
			else
				cbBytesToWrite = (ULONG)cbBytesLeft;

			cbBytesLeft -= cbBytesToWrite;

			hResult = m_pStream->Write(pData, cbBytesToWrite, &cbWrittenBytes);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, hResult);

			cbTotalWrittenBytes += cbWrittenBytes;
			if (cbWrittenBytes != cbBytesToWrite)
				break;

			pData += cbWrittenBytes;
		}

		if (cbTotalWrittenBytes != cbTotalBytesToWrite)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITEFULLDATA, hResult);

		return cbTotalWrittenBytes;
	}

	CComPtr <IStream> CExportStream_COM::getCOMStream()
	{
		return m_pStream;
	}

}
