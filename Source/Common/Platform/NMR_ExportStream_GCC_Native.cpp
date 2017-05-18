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

NMR_ExportStream_GCC_Native.cpp implements the CExportStream_GCC_Native Class.
This is an abstract base stream class for exporting with GCC with std::streams

Attention: Only use in UTF8-native environments!

--*/

#include "Common/Platform/NMR_ExportStream_GCC_Native.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string>

namespace NMR {


	CExportStream_GCC_Native::CExportStream_GCC_Native(_In_ const nfWChar * pwszFileName)
	{
		if (pwszFileName == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::wstring sFileName(pwszFileName);
#ifdef _WIN32
		m_Stream.open(sFileName.c_str(), std::ios::out | std::ios::binary);
#else
		std::string sUTF8Name = fnUTF16toUTF8(sFileName);
		m_Stream.open(sUTF8Name.c_str(), std::ios::out | std::ios::binary);
#endif
		if (m_Stream.fail())
			throw CNMRException(NMR_ERROR_COULDNOTCREATEFILE);

	}

	CExportStream_GCC_Native::~CExportStream_GCC_Native()
	{
		if (m_Stream.is_open()) {
			m_Stream.close();
		}
	}


	nfBool CExportStream_GCC_Native::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamPos = position;
		m_Stream.seekp(nStreamPos, std::ios_base::beg);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfBool CExportStream_GCC_Native::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamOffset = bytes;
		m_Stream.seekp(nStreamOffset, std::ios_base::cur);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfBool CExportStream_GCC_Native::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamOffset = bytes;
		m_Stream.seekp(nStreamOffset, std::ios_base::end);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfUint64 CExportStream_GCC_Native::getPosition()
	{
		std::streampos nStreamPosition = m_Stream.tellp();
		if (nStreamPosition < 0)
			throw CNMRException(NMR_ERROR_COULDNOTGETSTREAMPOSITION);

		return nStreamPosition;
	}

	nfUint64 CExportStream_GCC_Native::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite)
	{
		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const char * pChar = (const char*)pBuffer;
		std::streamsize nSize = cbTotalBytesToWrite;
        m_Stream.write(pChar, nSize);
		if (m_Stream.fail())
			throw CNMRException(NMR_ERROR_COULDNOTWRITESTREAM);

        return nSize;
	}

}
