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

NMR_PortableZIPWriter.h defines a portable and fast writer of ZIP files

--*/

#ifndef __NMR_PORTABLEZIPWRITER
#define __NMR_PORTABLEZIPWRITER

#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_PortableZIPWriterTypes.h"
#include "Common/Platform/NMR_PortableZIPWriterEntry.h"
#include "Common/NMR_Types.h"

#include <string>
#include <list>

namespace NMR {

	class CPortableZIPWriter {
	private:
		PExportStream m_pExportStream;
		PPortableZIPWriterEntry m_pCurrentEntry;
		nfUint32 m_nCurrentEntryKey;
		nfUint32 m_nNextEntryKey;
		nfBool m_bIsFinished;

		nfBool m_bWriteZIP64;
		nfUint16 m_nVersionMade;
		nfUint16 m_nVersionNeeded;

		std::list<PPortableZIPWriterEntry> m_Entries;
		PExportStream m_pCurrentStream;
	public:
		CPortableZIPWriter() = delete;
		CPortableZIPWriter(_In_ PExportStream pExportStream, _In_ nfBool bWriteZIP64);
		~CPortableZIPWriter();

		PExportStream createEntry(_In_ const std::string sName, _In_ nfTimeStamp nUnixTimeStamp);
		void closeEntry();

		void writeDeflatedBuffer(_In_ nfUint32 nEntryKey, _In_ const void * pBuffer, _In_ nfUint32 cbCompressedBytes);
		void calculateChecksum(_In_ nfUint32 nEntryKey, _In_ const void * pBuffer, _In_ nfUint32 cbUncompressedBytes);
		nfUint64 getCurrentSize(_In_ nfUint32 nEntryKey);

		void writeDirectory();
	};

	typedef std::shared_ptr <CPortableZIPWriter> PPortableZIPWriter;

}

#endif //__NMR_PORTABLEZIPWRITER
