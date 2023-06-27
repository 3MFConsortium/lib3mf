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

--*/

#ifndef __NMR_CHUNKEDBINARYSTREAMREADER
#define __NMR_CHUNKEDBINARYSTREAMREADER

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Common/Platform/NMR_ImportStream.h"

#include <map>
#include <vector>

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamTypes.h" 

namespace NMR {

	class CChunkedBinaryStreamReader;
	typedef std::shared_ptr <CChunkedBinaryStreamReader> PChunkedBinaryStreamReader;

	class CChunkedBinaryStreamReaderChunk;
	typedef std::shared_ptr <CChunkedBinaryStreamReaderChunk> PChunkedBinaryStreamReaderChunk;

	class CChunkedBinaryStreamReaderChunk {
	private:
		CChunkedBinaryStreamReader * m_pReader;
		std::vector <BINARYCHUNKFILEENTRY> m_ChunkEntries;		

		nfBool m_bHasCachedData;
		std::vector <nfByte> m_Data;

		BINARYCHUNKFILECHUNK m_Chunk;
		nfUint32 m_nChunkIndex;
		nfUint32 m_nCurrentReadPosition;
		nfUint32 m_nCurrentEndPosition;

	public:
		CChunkedBinaryStreamReaderChunk(CChunkedBinaryStreamReader * pReader, const BINARYCHUNKFILECHUNK & Chunk, const nfUint32 nChunkIndex);

		void loadData();
		void unloadData();

		void getInformation(nfUint32 nEntryIndex, eChunkedBinaryDataType & dataType, nfUint32 & nCount);

		void seekToEntry(nfUint32 nEntryIndex, nfUint32 & nEntryType, nfUint32 & nEntrySize);
		nfInt32 readInt32();
		nfFloat readFloat();

	};

	class CChunkedBinaryStreamReader {
	private:
		PImportStream m_pImportStream;

		std::vector<PChunkedBinaryStreamReaderChunk> m_Chunks;
		std::map <nfUint32, std::pair<nfUint32, nfUint32>> m_ChunkMap;

		void readHeader();		
		CChunkedBinaryStreamReaderChunk * findChunkByEntry (_In_ nfUint32 nEntryID,  _Out_ nfUint32 & nEntryIndex);

	public:
	
		CChunkedBinaryStreamReader(PImportStream pImportStream);

		void findChunkInformation (nfUint32 nEntryID, eChunkedBinaryDataType & dataType, nfUint32 & nCount);
		nfUint32 getTypedChunkEntryCount(nfUint32 nEntryID, eChunkedBinaryDataType dataType);

		void readIntArray(nfUint32 nEntryID, nfInt32 * pData, nfUint32 nDataCount);
		void readFloatArray(nfUint32 nEntryID, nfFloat * pData, nfUint32 nDataCount);

		void clearCache ();

		friend class CChunkedBinaryStreamReaderChunk;

	};


}

#endif // __NMR_CHUNKEDBINARYSTREAMREADER

