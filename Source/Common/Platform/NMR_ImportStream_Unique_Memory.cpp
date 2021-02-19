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

NMR_ImportStream_Unique_Memory.cpp implements the CImportStream_Memory Class.
This is a platform independent class for keeping data in a memory stream that owns the wrapped buffer.

--*/

#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CImportStream_Unique_Memory::CImportStream_Unique_Memory()
	{
		m_cbSize = 0;
		m_nPosition = 0;
	}


	CImportStream_Unique_Memory::CImportStream_Unique_Memory(_In_ CImportStream * pStream, _In_ nfUint64 cbBytesToCopy, _In_ nfBool bNeedsToCopyAllBytes)
	{
		if (pStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (cbBytesToCopy > NMR_IMPORTSTREAM_MAXMEMSTREAMSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);


		// Retrieve Capacity and allocate buffer.
		nfUint64 cbCapacity = cbBytesToCopy;
		try {
			m_Buffer.resize((size_t)cbCapacity);
		}
		catch (std::bad_alloc&) {
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);
		}

		m_cbSize = 0;
		m_nPosition = 0;

		while (m_cbSize < cbCapacity) {

			// Only read in chunks
			nfUint64 cbBytesToRead = cbCapacity - m_cbSize;
			if (cbBytesToRead > NMR_IMPORTSTREAM_READCHUNKSIZE)
				cbBytesToRead = NMR_IMPORTSTREAM_READCHUNKSIZE;

			// Read bytes into memory
			nfUint64 cbBytesRead;
			cbBytesRead = pStream->readBuffer(&m_Buffer[(size_t) m_cbSize], cbBytesToRead, false);

			// increase size
			m_cbSize += cbBytesRead;

			if (cbBytesRead != cbBytesToRead)
				break;
		}

		if ((m_cbSize != cbCapacity) && (bNeedsToCopyAllBytes))
			throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);


	}

	CImportStream_Unique_Memory::CImportStream_Unique_Memory(_In_ const nfByte * pBuffer, _In_ nfUint64 cbBytes)
	{
		if (pBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (cbBytes > NMR_IMPORTSTREAM_MAXMEMSTREAMSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// Retrieve Capacity and allocate buffer.
		m_Buffer.resize((size_t) cbBytes);

		m_cbSize = cbBytes;
		m_nPosition = 0;

		const nfByte * pSource = pBuffer;
		nfByte * pTarget = &m_Buffer[0];

		nfUint64 nIndex = cbBytes;
		while (nIndex > 0) {
			*pTarget = *pSource;

			pSource++;
			pTarget++;
			nIndex--;
		}
	}	

	PImportStream CImportStream_Unique_Memory::copyToMemory()
	{
		__NMRASSERT(m_nPosition <= m_cbSize);

		return std::make_shared<CImportStream_Unique_Memory>(this, m_cbSize - m_nPosition, true);
	}

	__NMR_INLINE const nfByte * CImportStream_Unique_Memory::getAt(nfUint64 nPosition) { 

		if (nPosition >= (uint64_t) m_Buffer.size())
			throw CNMRException(NMR_ERROR_COULDNOTREADSTREAM);

		// On 32 bit system, nPosition is definitely a 32bit value, because m_Buffer.size () is a size_t
		return &m_Buffer[(size_t) nPosition]; 
	}

}