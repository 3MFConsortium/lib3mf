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

NMR_PortableZIPWriterEntry.cpp implements a portable and fast writer of ZIP files

--*/

#include "Common/Platform/NMR_PortableZIPWriterEntry.h"
#include "Common/Platform/NMR_ExportStream_ZIP.h"
#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 
#include "Libraries/zlib/zlib.h"

namespace NMR {

	CPortableZIPWriterEntry::CPortableZIPWriterEntry(_In_ const std::string sUTF8Name, _In_ nfUint16 nLastModTime, _In_ nfUint16 nLastModDate, _In_ nfUint64 nFilePosition, _In_ nfUint64 nExtInfoPosition, _In_ nfUint64 nDataPosition)
	{
		m_sUTF8Name = sUTF8Name;
		m_nCRC32 = 0;
		m_nCompressedSize = 0;
		m_nUncompressedSize = 0;
		m_nLastModTime = nLastModTime;
		m_nLastModDate = nLastModDate;
		m_nFilePosition = nFilePosition;
		m_nExtInfoPosition = nExtInfoPosition;
		m_nDataPosition = nDataPosition;
	}

	std::string CPortableZIPWriterEntry::getUTF8Name()
	{
		return m_sUTF8Name;
	}

	nfUint32 CPortableZIPWriterEntry::getCRC32()
	{
		return m_nCRC32;
	}

	nfUint64 CPortableZIPWriterEntry::getCompressedSize()
	{
		return m_nCompressedSize;
	}

	nfUint64 CPortableZIPWriterEntry::getUncompressedSize()
	{
		return m_nUncompressedSize;
	}

	nfUint16 CPortableZIPWriterEntry::getLastModTime()
	{
		return m_nLastModTime;
	}

	nfUint16 CPortableZIPWriterEntry::getLastModDate()
	{
		return m_nLastModDate;
	}

	nfUint64 CPortableZIPWriterEntry::getFilePosition()
	{
		return m_nFilePosition;
	}
		
	nfUint64 CPortableZIPWriterEntry::getExtInfoPosition()
	{
		return m_nExtInfoPosition;
	}

	nfUint64 CPortableZIPWriterEntry::getDataPosition()
	{
		return m_nDataPosition;
	}

	void CPortableZIPWriterEntry::increaseCompressedSize(_In_ nfUint32 nCompressedSize)
	{
		m_nCompressedSize += nCompressedSize;
	}

	void CPortableZIPWriterEntry::increaseUncompressedSize(_In_ nfUint32 nUncompressedSize)
	{
		m_nUncompressedSize += nUncompressedSize;
	}

	void CPortableZIPWriterEntry::calculateChecksum(_In_ const void * pBuffer, _In_ nfUint32 cbCount)
	{
		m_nCRC32 = crc32(m_nCRC32, (Bytef*) pBuffer, cbCount);
	}

}
