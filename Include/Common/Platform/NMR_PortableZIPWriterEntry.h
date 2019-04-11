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

NMR_PortableZIPWriterEntry.h defines a portable and fast writer of ZIP files

--*/

#ifndef __NMR_PORTABLEZIPWRITERENTRY
#define __NMR_PORTABLEZIPWRITERENTRY

#include <string>
#include <list>
#include "Common/NMR_Types.h"
#include "Common/Platform/NMR_ExportStream.h"

namespace NMR {

	class CPortableZIPWriterEntry {
	private:
		std::string m_sUTF8Name;
		nfUint32 m_nCRC32;
		nfUint64 m_nCompressedSize;
		nfUint64 m_nUncompressedSize;
		nfUint16 m_nLastModTime;
		nfUint16 m_nLastModDate;
		nfUint64 m_nFilePosition;
		nfUint64 m_nExtInfoPosition;
		nfUint64 m_nDataPosition;
	public:
		CPortableZIPWriterEntry(_In_ const std::string sUTF8Name, _In_ nfUint16 nLastModTime, _In_ nfUint16 nLastModDate, _In_ nfUint64 nFilePosition, _In_ nfUint64 nExtInfoPosition, _In_ nfUint64 nDataPosition);
		std::string getUTF8Name();
		nfUint32 getCRC32();
		nfUint64 getCompressedSize();
		nfUint64 getUncompressedSize();
		nfUint16 getLastModTime();
		nfUint16 getLastModDate();
		nfUint64 getFilePosition();
		nfUint64 getExtInfoPosition();
		nfUint64 getDataPosition();
		void increaseCompressedSize(_In_ nfUint32 nCompressedSize);
		void increaseUncompressedSize(_In_ nfUint32 nUncompressedSize);
		void calculateChecksum(_In_ const void * pBuffer, _In_ nfUint32 cbCount);

	};

	typedef std::shared_ptr <CPortableZIPWriterEntry> PPortableZIPWriterEntry;


}

#endif //__NMR_PORTABLEZIPWRITERENTRY
