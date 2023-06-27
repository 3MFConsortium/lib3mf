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

NMR_ExportStream_Memory.cpp implements the ExportStream to Memory Class.

--*/

#include "Common/Platform/NMR_ExportStream_Memory.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CExportStreamMemory::CExportStreamMemory() {
		m_Position = 0;
	}

	nfBool CExportStreamMemory::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed) {
		if (position > m_Buffer.size() && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = position;
		return true;
	}

	nfBool CExportStreamMemory::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) {
		if (bytes + m_Position > m_Buffer.size() && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = bytes + m_Position;
		return true;
	}

	nfBool CExportStreamMemory::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) {
		if (bytes > m_Buffer.size() && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = m_Buffer.size() - bytes;
		return true;
	}

	nfUint64 CExportStreamMemory::getPosition() {
		return m_Position;
	}

	nfUint64 CExportStreamMemory::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite) {
		nfByte *pByteBuffer = (nfByte *)pBuffer;
		if ((m_Position + cbTotalBytesToWrite) > m_Buffer.size()) {
			m_Buffer.resize(static_cast<size_t>(m_Position + cbTotalBytesToWrite));
		}
		for (nfUint64 i = 0; i < cbTotalBytesToWrite; i++) {
			m_Buffer[static_cast<size_t>(m_Position + i)] = pByteBuffer[i];
		}
		m_Position += cbTotalBytesToWrite;
		return cbTotalBytesToWrite;
	}

	nfUint64 CExportStreamMemory::getDataSize() {
		return m_Buffer.size();
	}

	const nfByte *CExportStreamMemory::getData() {
		return m_Buffer.data();
	}

}