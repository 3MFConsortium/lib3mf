/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ImportStream_Callback.cpp implements the CExportStream_Callback Class.
This is an abstract base stream class for importing with a callback to 
custom data reader.

--*/

#include "Common/Platform/NMR_ImportStream_Callback.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string>

namespace NMR {

	CImportStream_Callback::CImportStream_Callback()
		:CImportStream ()
	{
		// empty on purpose
		m_pReadCallback = nullptr;
		m_pUserData = nullptr;
		m_pSeekCallback = nullptr;
		m_nPosition = 0;
		m_nSize = 0;
	}

	CImportStream_Callback::CImportStream_Callback(_In_ ImportStream_ReadCallbackType pReadCallback, _In_ ImportStream_SeekCallbackType pSeekCallback, void * pUserData, _In_ nfUint64 nStreamSize)
	: CImportStream()
	{
		m_pReadCallback = pReadCallback;
		m_pSeekCallback = pSeekCallback;
		m_pUserData = pUserData;
		m_nPosition = 0;
		m_nSize = nStreamSize;
	}

	nfBool CImportStream_Callback::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		if (m_pSeekCallback == nullptr) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_CALLBACKSTREAMCANNOTSEEK);
			return false;
		}
		else {
			if (position > m_nSize)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			nfUint64 nNewPosition = position;
			nfUint32 nErrorCode = m_pSeekCallback(nNewPosition, m_pUserData);
			if (nErrorCode != 0)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			m_nPosition = nNewPosition;
			return true;
		}
	}

	nfBool CImportStream_Callback::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		if (m_pSeekCallback == nullptr) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_CALLBACKSTREAMCANNOTSEEK);
			return false;
		}
		else {
			nfUint64 nNewPosition = m_nPosition + bytes;
			if (nNewPosition > m_nSize)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			nfUint32 nErrorCode = m_pSeekCallback(nNewPosition, m_pUserData);
			if (nErrorCode != 0)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			m_nPosition = nNewPosition;
			return true;
		}
	}

	nfBool CImportStream_Callback::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		if (m_pSeekCallback == nullptr) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_CALLBACKSTREAMCANNOTSEEK);
			return false;
		}
		else {
			if (bytes > m_nSize)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			nfUint64 nNewPosition = m_nSize - bytes;
			nfUint32 nErrorCode = m_pSeekCallback(nNewPosition, m_pUserData);
			if (nErrorCode != 0)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			m_nPosition = nNewPosition;
			return true;
		}
	}

	nfUint64 CImportStream_Callback::getPosition()
	{
		return m_nPosition;
	}

	nfUint64 CImportStream_Callback::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		if (m_pReadCallback == nullptr)
			throw CNMRException(NMR_ERROR_COULDNOTREADFROMCALLBACKSTREAM);

		nfByte * pByte = (nfByte *)pBuffer;
		nfUint64 cbBytesLeft = cbTotalBytesToRead;
		while (cbBytesLeft > 0) {
			nfUint64 cbBytes = cbBytesLeft;
			if (cbBytes > IMPORTSTREAM_CALLBACK_BUFFERSIZE)
				cbBytes = IMPORTSTREAM_CALLBACK_BUFFERSIZE;

			nfUint32 nError = m_pReadCallback(pByte, (nfUint32) cbBytes, m_pUserData);
			if (nError != 0) 
				throw CNMRException(NMR_ERROR_COULDNOTREADFROMCALLBACKSTREAM);

			pByte += cbBytes;
			cbBytesLeft -= cbBytes;
		}

		m_nPosition += cbTotalBytesToRead; 
		if (m_nPosition > m_nSize)
			m_nSize = m_nPosition;

		return cbTotalBytesToRead;
	}



	nfUint64 CImportStream_Callback::retrieveSize() {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	void CImportStream_Callback::writeToFile(_In_ const nfWChar * pwszFileName) {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	PImportStream CImportStream_Callback::copyToMemory() {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}
}
