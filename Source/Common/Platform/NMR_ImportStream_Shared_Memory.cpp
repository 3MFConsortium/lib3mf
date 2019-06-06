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

NMR_ImportStream_Shared_Memory.cpp implements the CImportStream_Memory Class.
This is a platform independent class for keeping data in a memory stream that doesn't own the wrapped buffer.

--*/

#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CImportStream_Shared_Memory::CImportStream_Shared_Memory(_In_ const nfByte * pBuffer, _In_ nfUint64 cbBytes)
	{
		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (cbBytes > NMR_IMPORTSTREAM_MAXMEMSTREAMSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		m_cbSize = cbBytes;
		m_nPosition = 0;

		m_Buffer = pBuffer;
	}	

	PImportStream CImportStream_Shared_Memory::copyToMemory()
	{
		__NMRASSERT(m_nPosition <= m_cbSize);

		return std::make_shared<CImportStream_Unique_Memory>(this, m_cbSize - m_nPosition, true);
	}

	__NMR_INLINE const nfByte * CImportStream_Shared_Memory::getAt(nfUint64 nPosition) { 
		return &m_Buffer[nPosition]; 
	}

}