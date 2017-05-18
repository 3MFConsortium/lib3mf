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

NMR_ImportStream_COM.cpp defines the CImportStream_COM Class.
This is an abstract base stream class for importing from COM IStreams.

--*/

#include "Common/Platform/NMR_ImportStream_COM.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include <cmath>
#include <vector>

namespace NMR {

	CImportStream_COM::CImportStream_COM()
	{
		// empty on purpose
	}

	CImportStream_COM::CImportStream_COM(_In_ CComPtr <IStream> pStream) : CImportStream()
	{
		if (pStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pStream = pStream;
	}

	CImportStream_COM::CImportStream_COM(_In_ LPCWSTR pwszFileName)
	{
		if (!pwszFileName)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		HRESULT hResult = SHCreateStreamOnFileEx(pwszFileName, STGM_READ | STGM_SHARE_DENY_WRITE, 0, false, NULL, &m_pStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTOPENFILE, hResult);
	}

	nfBool CImportStream_COM::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER filepos;
		HRESULT hResult;

		filepos.QuadPart = position;
		hResult = m_pStream->Seek(filepos, STREAM_SEEK_SET, NULL);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}

	nfBool CImportStream_COM::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER offset;
		HRESULT hResult;

		offset.QuadPart = bytes;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_CUR, NULL);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}

	nfBool CImportStream_COM::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		LARGE_INTEGER offset;
		HRESULT hResult;
		ULARGE_INTEGER newfilepos;

		offset.QuadPart = bytes;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_END, &newfilepos);

		if ((hResult != S_OK) && bHasToSucceed)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return (hResult == S_OK);
	}


	nfUint64 CImportStream_COM::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		HRESULT hResult;
		ULONG cbBytesToRead;
		ULONG cbReadBytes;
		nfUint64 cbTotalReadBytes = 0;
		nfUint64 cbBytesLeft = cbTotalBytesToRead;

		if (cbBytesLeft == 0)
			return cbBytesLeft;

		if (!pBuffer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfByte * pData = (nfByte *)pBuffer;

		while (cbBytesLeft > 0) {
			if (cbBytesLeft > NMR_IMPORTSTREAM_READBUFFERSIZE)
				cbBytesToRead = NMR_IMPORTSTREAM_READBUFFERSIZE;
			else
				cbBytesToRead = (ULONG)cbBytesLeft;

			cbBytesLeft -= cbBytesToRead;

			hResult = m_pStream->Read(pData, cbBytesToRead, &cbReadBytes);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTREADSTREAM, hResult);

			cbTotalReadBytes += cbReadBytes;
			if (cbReadBytes != cbBytesToRead)
				break;

			pData += cbReadBytes;
		}

		if ((cbTotalReadBytes != cbTotalBytesToRead) && bNeedsToReadAll)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTREADFULLDATA, hResult);

		return cbTotalReadBytes;
	}

	CComPtr <IStream> CImportStream_COM::getCOMStream()
	{
		return m_pStream;
	}
	
	nfUint64 CImportStream_COM::retrieveSize()
	{
		nfUint64 nOrigPosition = getPosition();
		LARGE_INTEGER offset;
		HRESULT hResult;
		ULARGE_INTEGER newfilepos;

		offset.QuadPart = 0;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_END, &newfilepos);

		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		seekPosition(nOrigPosition, true);

		return newfilepos.QuadPart;
	}

	nfUint64 CImportStream_COM::getPosition()
	{
		LARGE_INTEGER offset;
		HRESULT hResult;
		ULARGE_INTEGER newfilepos;

		offset.QuadPart = 0;
		hResult = m_pStream->Seek(offset, STREAM_SEEK_CUR, &newfilepos);

		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return newfilepos.QuadPart;
	}

	void CImportStream_COM::writeToFile(_In_ const nfWChar * pwszFileName)
	{
		if (!pwszFileName)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CComPtr<IStream> pFileStream;

		HRESULT hResult = SHCreateStreamOnFileEx(pwszFileName, STGM_CREATE | STGM_WRITE, 0, false, NULL, &pFileStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATEFILE, hResult);

		nfUint64 cbStreamSize = retrieveSize();

		std::vector<nfByte> pBuffer;
		pBuffer.resize(NMR_IMPORTSTREAM_COPYBUFFERSIZE);

		nfUint64 cbBytesLeft = cbStreamSize;
		while (cbBytesLeft > 0) {
			nfUint64 cbLength = cbBytesLeft;
			if (cbLength > NMR_IMPORTSTREAM_COPYBUFFERSIZE)
				cbLength = NMR_IMPORTSTREAM_COPYBUFFERSIZE;

			ULONG cbWrittenBytes = 0;
			readBuffer(pBuffer.data(), cbLength, true);
			HRESULT hResult = pFileStream->Write(pBuffer.data(), (nfUint32)cbLength, &cbWrittenBytes);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, hResult);

			if (cbWrittenBytes != cbLength)
				throw CNMRException(NMR_ERROR_COULDNOTWRITEFULLDATA);
			cbBytesLeft -= cbLength;
		}

	}

	PImportStream CImportStream_COM::copyToMemory()
	{
		CComPtr<IStream> pMemoryStream = nullptr;
		HRESULT hResult = CreateStreamOnHGlobal(nullptr, true, &pMemoryStream);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATESTREAM, hResult);

		nfUint64 cbStreamSize = retrieveSize();

		std::vector<nfByte> pBuffer;
		pBuffer.resize(NMR_IMPORTSTREAM_COPYBUFFERSIZE);

		nfUint64 cbBytesLeft = cbStreamSize;
		while (cbBytesLeft > 0) {
			nfUint64 cbLength = cbBytesLeft;
			if (cbLength > NMR_IMPORTSTREAM_COPYBUFFERSIZE)
				cbLength = NMR_IMPORTSTREAM_COPYBUFFERSIZE;

			ULONG cbWrittenBytes = 0;
			readBuffer(pBuffer.data(), cbLength, true);
			HRESULT hResult = pMemoryStream->Write(pBuffer.data(), (nfUint32)cbLength, &cbWrittenBytes);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, hResult);

			if (cbWrittenBytes != cbLength)
				throw CNMRException(NMR_ERROR_COULDNOTWRITEFULLDATA);
			cbBytesLeft -= cbLength;
		}

		LARGE_INTEGER filepos;
		filepos.QuadPart = 0;
		hResult = pMemoryStream->Seek(filepos, STREAM_SEEK_SET, NULL);
		if (hResult != S_OK)
			throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

		return std::make_shared<CImportStream_COM>(pMemoryStream);
	}


}
