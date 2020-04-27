/*++

Copyright (C) 2020 3MF Consortium (Original Author)

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

Abstract: This is a stub class implementation of CImportStream_Encrypted

*/

#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/NMR_Exception.h"
#include <vector>
#include <array>
namespace NMR {
	CImportStream_Encrypted::CImportStream_Encrypted(PImportStream pEncryptedStream, ContentEncryptionDescriptor pDecryptContext)
		:m_pEncryptedStream(pEncryptedStream), m_pDecryptContext(pDecryptContext)
	{
		if (nullptr == pEncryptedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		m_header.readFrom(pEncryptedStream);
	}

	nfBool CImportStream_Encrypted::seekPosition(nfUint64 position, nfBool bHasToSucceed) {
		return m_pEncryptedStream->seekPosition(position + m_header.headerSize(), bHasToSucceed);
	}

	nfBool CImportStream_Encrypted::seekForward(nfUint64 bytes, nfBool bHasToSucceed) {
		return m_pEncryptedStream->seekForward(bytes, bHasToSucceed);
	}

	nfBool CImportStream_Encrypted::seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed) {
		if (m_pEncryptedStream->getPosition() + m_header.headerSize() >= bytes) {
			return m_pEncryptedStream->seekFromEnd(bytes, bHasToSucceed);
		} else if (bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		return false;
	}

	nfUint64 CImportStream_Encrypted::readBuffer(nfByte * pBuffer, nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll) {
		std::vector<nfByte> decBuffer(cbTotalBytesToRead, 0);
		nfUint64 bytesRead = m_pEncryptedStream->readBuffer(decBuffer.data(), cbTotalBytesToRead, bNeedsToReadAll);
		if (bytesRead > 0) {
			nfUint64 decryted = m_pDecryptContext.m_fnCrypt(bytesRead, decBuffer.data(), pBuffer, m_pDecryptContext.m_sDekDecryptData);
			if (decryted != bytesRead)
				throw CNMRException(NMR_ERROR_CALCULATIONTERMINATED);
		}
		return bytesRead;
	}

	nfUint64 CImportStream_Encrypted::retrieveSize() {
		return m_pEncryptedStream->retrieveSize() - m_header.headerSize();
	}

	void CImportStream_Encrypted::writeToFile(const nfWChar * pwszFileName) {
		m_pEncryptedStream->writeToFile(pwszFileName);
	}

	PImportStream CImportStream_Encrypted::copyToMemory() {
		nfUint64 cbStreamSize = retrieveSize();
		return std::make_shared<CImportStream_Unique_Memory>(this, cbStreamSize, false);
	}

	nfUint64 CImportStream_Encrypted::getPosition() {
		return m_pEncryptedStream->getPosition() - m_header.headerSize();
	}


}