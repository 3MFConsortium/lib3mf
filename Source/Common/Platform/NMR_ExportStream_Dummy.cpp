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

NMR_ExportStream_Dummy.cpp implementa the ExportStream Dummy class. It will not
store any data written to it, but it will correctly track the seek position and
size of the data. It is designed to be used for measuring the size of the stream.

--*/

#include "Common/Platform/NMR_ExportStream_Dummy.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CExportStreamDummy::CExportStreamDummy() {
		m_Size = 0;
		m_Position = 0;
	}

	nfBool CExportStreamDummy::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed) {
		if (position >= m_Size && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = position;
		return true;
	}

	nfBool CExportStreamDummy::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) {
		if (bytes + m_Position >= m_Size && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = bytes + m_Position;
		return true;
	}

	nfBool CExportStreamDummy::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed) {
		if (bytes >= m_Size && bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		m_Position = m_Size - bytes;
		return true;
	}

	nfUint64 CExportStreamDummy::getPosition() {
		return m_Position;
	}

	nfUint64 CExportStreamDummy::writeBuffer(_In_ const void * pBuffer, _In_ nfUint64 cbTotalBytesToWrite) {
		if ((m_Position + cbTotalBytesToWrite) > m_Size) {
			m_Size = m_Position + cbTotalBytesToWrite;
		}
		m_Position += cbTotalBytesToWrite;
		return cbTotalBytesToWrite;
	}

	nfUint64 CExportStreamDummy::getDataSize() {
		return m_Size;
	}

}
