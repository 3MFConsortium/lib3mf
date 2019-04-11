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

NMR_ImportStream_ZIP.cpp implements the CImportStream_ZIP Class.
This is a stream class for importing from a libZIP object.

--*/

#include "Common/Platform/NMR_ImportStream_ZIP.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <math.h>
#include <vector>

namespace NMR {

	CImportStream_ZIP::CImportStream_ZIP(_In_ zip_file_t * pFile, _In_ nfUint64 nSize)
	{
		if (pFile == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pFile = pFile;
		m_nSize = nSize;
	}

	CImportStream_ZIP::~CImportStream_ZIP()
	{
		if (m_pFile != nullptr) {
			zip_fclose(m_pFile);
		}
		m_pFile = nullptr;
	}

	nfBool CImportStream_ZIP::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_COULDNOTSEEKINZIP);
	}

	nfBool CImportStream_ZIP::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_COULDNOTSEEKINZIP);
	}

	nfBool CImportStream_ZIP::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_COULDNOTSEEKINZIP);
	}

	nfUint64 CImportStream_ZIP::getPosition() {
		throw CNMRException(NMR_ERROR_COULDNOTSEEKINZIP);
	}

	nfUint64 CImportStream_ZIP::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		nfUint64 cbBytesLeft = cbTotalBytesToRead;
		nfUint64 cbBytesRead = 0;

		_In_ nfByte * pData = pBuffer;
		while (cbBytesLeft > 0) {
			nfUint32 cbBytesToRead;
			if (cbBytesLeft > IMPORTSTREAM_ZIP_CHUNKSIZE)
				cbBytesToRead = IMPORTSTREAM_ZIP_CHUNKSIZE;
			else
				cbBytesToRead = (nfUint32)cbBytesLeft;
			cbBytesLeft -= cbBytesToRead;

			nfInt64 nfSize = zip_fread(m_pFile, pData, cbBytesToRead);
			if (nfSize < 0)
				throw CNMRException(NMR_ERROR_COULDNOTREADSTREAM);
			cbBytesRead += nfSize;

			if (nfSize != (nfInt64)cbBytesToRead)
				break;

            pData += nfSize;


		}


		if ((nfUint64)cbBytesRead != cbTotalBytesToRead) {
			if (bNeedsToReadAll)
				throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);
		}

		return (nfUint64)cbBytesRead;
	}

	nfUint64 CImportStream_ZIP::retrieveSize()
	{
		return m_nSize;
	}

	void CImportStream_ZIP::writeToFile(_In_ const nfWChar * pwszFileName)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	PImportStream CImportStream_ZIP::copyToMemory()
	{
		nfUint64 cbStreamSize = retrieveSize();

		return std::make_shared<CImportStream_Unique_Memory>(this, cbStreamSize, false);
	}


}
