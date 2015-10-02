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

NMR_ImportStream_GCC_Native.cpp implements the CImportStream_GCC_Native Class.
This is an abstract base stream class for importing from streams with with std::streams.

--*/

#include "Common/Platform/NMR_ImportStream_GCC_Native.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string>

namespace NMR {

	CImportStream_GCC_Native::CImportStream_GCC_Native(_In_ const nfWChar * pwszFileName)
	{
		if (pwszFileName == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::wstring sFileName(pwszFileName);
		std::string sUTF8Name = fnUTF16toUTF8(sFileName);

		m_Stream.open(sUTF8Name.c_str(), std::ios::in | std::ios::binary);
		if (m_Stream.fail())
			throw CNMRException(NMR_ERROR_COULDNOTOPENFILE);
	}

	CImportStream_GCC_Native::~CImportStream_GCC_Native()
	{
		if (m_Stream.is_open()) {
			m_Stream.close();
		}
	}

	nfBool CImportStream_GCC_Native::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamPos = position;
		m_Stream.seekg(nStreamPos, std::ios_base::beg);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfBool CImportStream_GCC_Native::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamOffset = bytes;
		m_Stream.seekg(nStreamOffset, std::ios_base::cur);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfBool CImportStream_GCC_Native::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		std::streampos nStreamOffset = bytes;
		m_Stream.seekg(nStreamOffset, std::ios_base::end);

		if (m_Stream.fail()) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);

			return false;
		}

		return true;
	}

	nfUint64 CImportStream_GCC_Native::getPosition()
	{
		std::streampos nStreamPosition = m_Stream.tellg();
		if (nStreamPosition < 0)
			throw CNMRException(NMR_ERROR_COULDNOTGETSTREAMPOSITION);

		return nStreamPosition;
	}

	nfUint64 CImportStream_GCC_Native::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		char * pChar = (char*)pBuffer;
		std::streamsize nSize = cbTotalBytesToRead;
		m_Stream.read(pChar, nSize);
		if (m_Stream.fail())
			throw CNMRException(NMR_ERROR_COULDNOTREADSTREAM);

		std::streamsize nReadBytes = m_Stream.gcount();
		if (nReadBytes != nSize) {
			if (bNeedsToReadAll)
				throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);
		}

		return nReadBytes;
	}

	nfUint64 CImportStream_GCC_Native::retrieveSize()
	{
		seekFromEnd(0, true);
		return getPosition();
	}

	void CImportStream_GCC_Native::writeToFile(_In_ const nfWChar * pwszFileName)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	PImportStream CImportStream_GCC_Native::copyToMemory()
	{
		nfUint64 cbStreamSize = retrieveSize();
		return std::make_shared<CImportStream_Memory>(this, cbStreamSize, false);
	}

}
