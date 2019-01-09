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

NMR_ImportStream_Memory.cpp implements the CImportStream_Memory Class.
This is an abstract platform independent class for keeping data in a memory stream.

--*/

#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <string>

namespace NMR {

	CImportStream_Memory::~CImportStream_Memory()
	{
	}

	nfBool CImportStream_Memory::seekPosition(_In_ nfUint64 nPosition, _In_ nfBool bHasToSucceed)
	{
		if (nPosition > m_cbSize) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			return false;
		}

		m_nPosition = nPosition;

		return true;
	}

	nfBool CImportStream_Memory::seekForward(_In_ nfUint64 cbBytes, _In_ nfBool bHasToSucceed)
	{
		nfUint64 nPosition = m_nPosition + cbBytes;

		if (nPosition > m_cbSize) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			return false;
		}

		m_nPosition = nPosition;
		return true;
	}

	nfBool CImportStream_Memory::seekFromEnd(_In_ nfUint64 cbBytes, _In_ nfBool bHasToSucceed)
	{
		if (cbBytes > m_cbSize) {
			if (bHasToSucceed)
				throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
			return false;
		}

		m_nPosition = m_cbSize - cbBytes;
		return true;
	}

	nfUint64 CImportStream_Memory::getPosition()
	{
		return m_nPosition;
	}

	nfUint64 CImportStream_Memory::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		__NMRASSERT(m_nPosition <= m_cbSize);
		nfUint64 cbBytesLeft = (m_cbSize - m_nPosition);
		nfUint64 cbBytesToRead = cbTotalBytesToRead;

		if (cbBytesToRead > cbBytesLeft)
			cbBytesToRead = cbBytesLeft;

		if (cbBytesToRead > 0) {
			nfUint64 nIndex = cbBytesToRead;
			const nfByte * pSource = getAt(m_nPosition);
			nfByte * pTarget = pBuffer;

			while (nIndex > 0) {
				*pTarget = *pSource;
				nIndex--;
				pSource++;
				pTarget++;
			}

			m_nPosition += cbBytesToRead;
		}

		if ((cbBytesToRead != cbTotalBytesToRead) && bNeedsToReadAll)
			throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);

		return cbBytesToRead;

	}

	nfUint64 CImportStream_Memory::retrieveSize()
	{
		return m_cbSize;
	}

	void CImportStream_Memory::writeToFile(_In_ const nfWChar * pwszFileName)
	{
		if (pwszFileName == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		std::string sUTF8FileName = fnUTF16toUTF8(pwszFileName);
		PExportStream pExportStream = fnCreateExportStreamInstance(sUTF8FileName.c_str());
		if (m_cbSize > 0) {
			pExportStream->writeBuffer(getAt(0), m_cbSize);
		}
	}
}
