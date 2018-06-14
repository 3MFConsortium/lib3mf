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

NMR_ExportStream_GCC_Native.cpp implements the CExportStream_GCC_Native Class.
This is an abstract base stream class for exporting with GCC with std::streams

Attention: Only use in UTF8-native environments!

--*/

#include "Common/Platform/NMR_ExportStream_Callback.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string>

namespace NMR {

	CExportStream_Callback::CExportStream_Callback()
		:CExportStream ()
	{
		// empty on purpose
		m_pWriteCallback = nullptr;
		m_pUserData = nullptr;
		m_pSeekCallback = nullptr;
		m_nPosition = 0;
		m_nSize = 0;
	}

	CExportStream_Callback::CExportStream_Callback(_In_ ExportStream_WriteCallbackType pWriteCallback, _In_ ExportStream_SeekCallbackType pSeekCallback, void * pUserData)
	: CExportStream()
	{
		m_pWriteCallback = pWriteCallback;
		m_pSeekCallback = pSeekCallback;
		m_pUserData = pUserData;
		m_nPosition = 0;
		m_nSize = 0;
	}

	nfBool CExportStream_Callback::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
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

	nfBool CExportStream_Callback::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
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

	nfBool CExportStream_Callback::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
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

	nfUint64 CExportStream_Callback::getPosition()
	{
		return m_nPosition;
	}

	nfUint64 CExportStream_Callback::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
		if (m_pWriteCallback == nullptr)
			throw CNMRException(NMR_ERROR_COULDNOTWRITETOCALLBACKSTREAM);

		nfByte * pByte = (nfByte *)pBuffer;
		nfUint64 cbBytesLeft = cbTotalBytesToWrite;
		while (cbBytesLeft > 0) {
			nfUint64 cbBytes = cbBytesLeft;
			if (cbBytes > EXPORTSTREAM_CALLBACK_BUFFERSIZE)
				cbBytes = EXPORTSTREAM_CALLBACK_BUFFERSIZE;

			nfUint32 nError = m_pWriteCallback(pByte, (nfUint32) cbBytes, m_pUserData);
			if (nError != 0) 
				throw CNMRException(NMR_ERROR_COULDNOTWRITETOCALLBACKSTREAM);

			pByte += cbBytes;
			cbBytesLeft -= cbBytes;
		}

		m_nPosition += cbTotalBytesToWrite; 
		if (m_nPosition > m_nSize)
			m_nSize = m_nPosition;

		return cbTotalBytesToWrite;
	}


}
