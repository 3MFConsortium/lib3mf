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

NMR_ExportStream_GCC_Win32.cpp implements the CExportStream_GCC_Win32 Class.
This is an abstract base stream class for exporting with GCC under Windows.

--*/

#include "Common/Platform/NMR_ExportStream_GCC_Win32.h"
#include "Common/NMR_Exception.h"

#ifdef __GCC_WIN32
#include "Common/NMR_Exception_Windows.h"
#endif // __GCC_WIN32

namespace NMR {

#ifdef __GCC_WIN32

	CExportStream_GCC_Win32::CExportStream_GCC_Win32(_In_ const nfWChar * pwszFileName)
	{
	    if (pwszFileName == nullptr)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        // Create handle
		m_hHandle = CreateFileW (pwszFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (m_hHandle == INVALID_HANDLE_VALUE)
            throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATESTREAM, GetLastError());

	}

    CExportStream_GCC_Win32::~CExportStream_GCC_Win32()
    {
		if (m_hHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hHandle);
            m_hHandle = 0;
		}
    }


	nfBool CExportStream_GCC_Win32::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
	    HRESULT hResult = S_OK;
	    nfBool bSuccess = true;

		LARGE_INTEGER nPosition;
		nPosition.QuadPart = position;

		nPosition.LowPart = SetFilePointer(m_hHandle, nPosition.LowPart, &nPosition.HighPart, FILE_BEGIN);
        if (nPosition.LowPart == INVALID_SET_FILE_POINTER) {
            hResult = GetLastError();
            if (hResult != S_OK)
                bSuccess = false;
        }

        if (bSuccess && (!bHasToSucceed))
            throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

        return bSuccess;
	}

	nfBool CExportStream_GCC_Win32::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
	    HRESULT hResult = S_OK;
	    nfBool bSuccess = true;

		LARGE_INTEGER nPosition;
		nPosition.QuadPart = bytes;

		nPosition.LowPart = SetFilePointer(m_hHandle, nPosition.LowPart, &nPosition.HighPart, FILE_CURRENT);
        if (nPosition.LowPart == INVALID_SET_FILE_POINTER) {
            hResult = GetLastError();
            if (hResult != S_OK)
                bSuccess = false;
        }

        if (bSuccess && (!bHasToSucceed))
            throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

        return bSuccess;
	}

	nfBool CExportStream_GCC_Win32::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
	    HRESULT hResult = S_OK;
	    nfBool bSuccess = true;

		LARGE_INTEGER nPosition;
		nPosition.QuadPart = bytes;

		nPosition.LowPart = SetFilePointer(m_hHandle, nPosition.LowPart, &nPosition.HighPart, FILE_END);
        if (nPosition.LowPart == INVALID_SET_FILE_POINTER) {
            hResult = GetLastError();
            if (hResult != S_OK)
                bSuccess = false;
        }

        if (bSuccess && (!bHasToSucceed))
            throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

        return bSuccess;
	}

	nfUint64 CExportStream_GCC_Win32::getPosition()
    {
	    HRESULT hResult = S_OK;

		LARGE_INTEGER nPosition;
		nPosition.QuadPart = 0;

		nPosition.LowPart = SetFilePointer(m_hHandle, nPosition.LowPart, &nPosition.HighPart, FILE_CURRENT);
        if (nPosition.LowPart == INVALID_SET_FILE_POINTER) {
            hResult = GetLastError();
            if (hResult != S_OK)
                throw CNMRException_Windows(NMR_ERROR_COULDNOTGETSTREAMPOSITION, hResult);
        }

        return nPosition.QuadPart;
	}

	nfUint64 CExportStream_GCC_Win32::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
	    if (pBuffer == nullptr)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

	    const nfByte * pBufP = (const nfByte *) pBuffer;
        nfUint64 cbBytesLeft = cbTotalBytesToWrite;
        nfUint64 cbTotalBytesWritten = 0;

        while (cbBytesLeft > 0) {
            nfUint32 cbBytesToWrite;
            long unsigned int cbWrittenBytes = 0;

            if (cbBytesLeft > NMR_EXPORTSTREAM_WRITEBUFFERSIZE) {
                cbBytesToWrite = NMR_EXPORTSTREAM_WRITEBUFFERSIZE;
            } else {
                cbBytesToWrite = (nfUint32) cbBytesLeft;
            }

            if (!WriteFile(m_hHandle, pBufP, cbBytesToWrite, &cbWrittenBytes, nullptr))
            {
                throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, GetLastError());
            }

            if (cbWrittenBytes != cbBytesToWrite)
                throw CNMRException(NMR_ERROR_COULDNOTWRITEFULLDATA);

            pBufP += cbWrittenBytes;
            cbTotalBytesWritten += cbWrittenBytes;
            cbBytesLeft -= cbWrittenBytes;
        }

        return cbTotalBytesWritten;

	}

#endif // __GCC_WIN32


}
