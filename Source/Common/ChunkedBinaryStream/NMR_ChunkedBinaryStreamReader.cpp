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

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamReader.h" 
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h" 
#include "Common/NMR_Exception.h" 
//#include "Libraries/lzma/LzmaLib.h"

#include <vector>


namespace NMR {

	CChunkedBinaryStreamReaderChunk::CChunkedBinaryStreamReaderChunk(CChunkedBinaryStreamReader * pReader, const BINARYCHUNKFILECHUNK & Chunk, const nfUint32 nChunkIndex)
		: m_pReader (pReader), 
		  m_nChunkIndex (nChunkIndex), 
		  m_Chunk (Chunk), 
		  m_bHasCachedData(false),
		  m_nCurrentReadPosition(0),
	      m_nCurrentEndPosition(0)
	{
		if (pReader == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (Chunk.m_UncompressedDataSize > BINARYCHUNKFILEMAXCHUNKDATASIZE)
			throw CNMRException(NMR_ERROR_INVALIDFILECHUNKDATASIZE);
		if (Chunk.m_CompressedDataSize > BINARYCHUNKFILEMAXCHUNKDATASIZE)
			throw CNMRException(NMR_ERROR_INVALIDFILECHUNKDATASIZE);

		CImportStream * pImportStream = pReader->m_pImportStream.get();

		if (Chunk.m_EntryCount > BINARYCHUNKFILEMAXENTRYCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYBINARYCHUNKENTRIES);

		auto ChunkMap = &pReader->m_ChunkMap;

		pImportStream->seekPosition(Chunk.m_EntryTableStart, true);
		if (Chunk.m_EntryCount > 0) {
			m_ChunkEntries.resize(Chunk.m_EntryCount);
			pImportStream->readIntoBuffer((nfByte *) m_ChunkEntries.data(), sizeof (BINARYCHUNKFILEENTRY) * m_ChunkEntries.size(), true);

			for (size_t nEntryIndex = 0; nEntryIndex < m_ChunkEntries.size(); nEntryIndex++) {
				nfUint32 nEntryID = m_ChunkEntries[nEntryIndex].m_EntryID;
				auto iChunkIter = ChunkMap->find(nEntryID);
				if (iChunkIter != ChunkMap->end())
					throw CNMRException(NMR_ERROR_DUPLICATECHUNKENTRY);
				
				pReader->m_ChunkMap.insert(std::make_pair (nEntryID, std::make_pair (m_nChunkIndex, (nfUint32) nEntryIndex)));
			}
		}

	}

	void CChunkedBinaryStreamReaderChunk::loadData()
	{
		if (m_bHasCachedData)
			return;

		m_Data.resize(m_Chunk.m_UncompressedDataSize);

		if (m_Chunk.m_UncompressedDataSize > 0) {

			/*std::vector<nfByte> PropsData;
			std::vector<nfByte> CompressedData;

			if (m_Chunk.m_CompressedPropsSize == 0)
				throw CNMRException(NMR_ERROR_INVALIDCHUNKCOMPRESSEDDATA);
			if (m_Chunk.m_CompressedDataSize == 0)
				throw CNMRException(NMR_ERROR_INVALIDCHUNKCOMPRESSEDDATA);

			PropsData.resize(m_Chunk.m_CompressedPropsSize);
			CompressedData.resize(m_Chunk.m_CompressedDataSize);

			CImportStream * pImportStream = m_pReader->m_pImportStream.get();
			pImportStream->seekPosition(m_Chunk.m_CompressedDataStart, true);

			if (m_Chunk.m_CompressedDataSize > 0)
				pImportStream->readIntoBuffer(CompressedData.data(), m_Chunk.m_CompressedDataSize, true);
			if (m_Chunk.m_CompressedPropsSize > 0)
				pImportStream->readIntoBuffer(PropsData.data(), m_Chunk.m_CompressedPropsSize, true);

			size_t destLen = m_Data.size();
			size_t srcLen = CompressedData.size();
			size_t propsLen = PropsData.size();

			//int error;
			//error = LzmaUncompress(m_Data.data(), &destLen, CompressedData.data(), &srcLen, PropsData.data(), propsLen);
			//if (error != SZ_OK)
				throw CNMRException(NMR_ERROR_COULDNOTUNCOMPRESSDATA); */

		}

		m_bHasCachedData = true;
		m_nCurrentReadPosition = 0;
		m_nCurrentEndPosition = 0;
	}

	void CChunkedBinaryStreamReaderChunk::unloadData()
	{
		if (!m_bHasCachedData)
			return;

		m_Data.resize(0);
		m_bHasCachedData = false;
		m_nCurrentReadPosition = 0;
		m_nCurrentEndPosition = 0;
	}


	void CChunkedBinaryStreamReaderChunk::getInformation(nfUint32 nEntryIndex, eChunkedBinaryDataType & dataType, nfUint32 & nCount)
	{
		if (nEntryIndex >= m_ChunkEntries.size())
			throw CNMRException(NMR_ERROR_INVALIDENTRYINDEX);

		auto pChunkEntry = &m_ChunkEntries[nEntryIndex];

		switch (pChunkEntry->m_EntryType) {
			case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_NOPREDICTION:
			case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_DELTAPREDICTION: 
				dataType = eChunkedBinaryDataType::edtInt32Array;
				if ((pChunkEntry->m_SizeInBytes % 4) != 0)
					throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);

				nCount = (pChunkEntry->m_SizeInBytes / 4);
				break;

			case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_NOPREDICTION: 
			case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_DELTAPREDICTION:
				dataType = eChunkedBinaryDataType::edtFloatArray;
				if (pChunkEntry->m_SizeInBytes < 4)
					throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);
				if ((pChunkEntry->m_SizeInBytes % 4) != 0)
					throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);

				nCount = ((pChunkEntry->m_SizeInBytes - 4) / 4);
				break;

			default:
				dataType = eChunkedBinaryDataType::edtUnknown;
				nCount = 0;
		} 
	}

	void CChunkedBinaryStreamReaderChunk::seekToEntry(nfUint32 nEntryIndex, nfUint32 & nEntryType, nfUint32 & nEntrySize)
	{
		if (nEntryIndex >= m_ChunkEntries.size())
			throw CNMRException(NMR_ERROR_INVALIDENTRYINDEX);

		auto pEntry = &m_ChunkEntries[nEntryIndex];
		if (pEntry->m_PositionInChunk > m_Data.size())
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYPOSITION);
		
		nfUint32 nReadEndPosition;
		nfUint32 nReadStartPosition = pEntry->m_PositionInChunk;
		if ((nEntryIndex + 1) < m_ChunkEntries.size()) {
			nReadEndPosition = m_ChunkEntries[nEntryIndex + 1].m_PositionInChunk;
		}
		else {
			nReadEndPosition = (nfUint32) m_Data.size();
		}
		
		if (nReadEndPosition < nReadStartPosition)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYENDPOSITION);
		nEntrySize = nReadEndPosition - nReadStartPosition;

		if (nEntrySize != pEntry->m_SizeInBytes)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYSIZE);

		m_nCurrentReadPosition = nReadStartPosition;
		m_nCurrentEndPosition = nReadEndPosition;

		nEntryType = pEntry->m_EntryType;
	}

	nfInt32 CChunkedBinaryStreamReaderChunk::readInt32()
	{
		if (m_nCurrentReadPosition > m_Data.size())
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYPOSITION);
		if (m_nCurrentEndPosition > m_Data.size ())
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYENDPOSITION);
		if ((m_nCurrentReadPosition + 4) > m_nCurrentEndPosition)
			throw CNMRException(NMR_ERROR_NOTENOUGHDATATOREADFROMCHUNK);

		nfInt32 * pIntValue = (nfInt32*) &m_Data[m_nCurrentReadPosition];
		m_nCurrentReadPosition += 4;

		return *pIntValue;
	}

	nfFloat CChunkedBinaryStreamReaderChunk::readFloat()
	{
		if (m_nCurrentReadPosition > m_Data.size())
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYPOSITION);
		if (m_nCurrentEndPosition > m_Data.size())
			throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYENDPOSITION);
		if ((m_nCurrentReadPosition + 4) > m_nCurrentEndPosition)
			throw CNMRException(NMR_ERROR_NOTENOUGHDATATOREADFROMCHUNK);

		nfFloat * pFloatValue = (nfFloat*)&m_Data[m_nCurrentReadPosition];
		m_nCurrentReadPosition += 4;

		return *pFloatValue;
	}



	CChunkedBinaryStreamReader::CChunkedBinaryStreamReader(PImportStream pImportStream)
		: m_pImportStream(pImportStream)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		readHeader();
	}

	void CChunkedBinaryStreamReader::readHeader()
	{
		nfUint64 streamSize = m_pImportStream->retrieveSize();
		if (streamSize < sizeof(BINARYCHUNKFILEHEADER))
			throw CNMRException(NMR_ERROR_INVALIDCHUNKSTREAM);

		BINARYCHUNKFILEHEADER Header;
		m_pImportStream->seekPosition(0, true);
		m_pImportStream->readIntoBuffer((nfByte *)&Header, sizeof(BINARYCHUNKFILEHEADER), true);

		if (Header.m_Sign != BINARYCHUNKFILEHEADERSIGN)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKSTREAMSIGN);
		if (Header.m_Version != BINARYCHUNKFILEHEADERVERSION)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKSTREAMVERSION);

		if (Header.m_ChunkCount > BINARYCHUNKFILEMAXCHUNKCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYBINARYCHUNKS);
		
		if (Header.m_ChunkCount > 0) {
			m_Chunks.resize(Header.m_ChunkCount);
			
			std::vector <BINARYCHUNKFILECHUNK> ChunkArray;
			ChunkArray.resize(Header.m_ChunkCount);

			m_pImportStream->seekPosition(Header.m_ChunkTableStart, true);
			m_pImportStream->readIntoBuffer((nfByte *)ChunkArray.data(), sizeof(BINARYCHUNKFILECHUNK) * ChunkArray.size(), true);

			for (size_t nIndex = 0; nIndex < ChunkArray.size(); nIndex++) {
				m_Chunks[nIndex] = std::make_shared<CChunkedBinaryStreamReaderChunk> (this, ChunkArray[nIndex], (nfUint32) nIndex);
			}
		}
			   
	}

	CChunkedBinaryStreamReaderChunk * CChunkedBinaryStreamReader::findChunkByEntry(_In_ nfUint32 nEntryID, _Out_ nfUint32 & nEntryIndex)
	{

		auto iEntryIter = m_ChunkMap.find(nEntryID);
		if (iEntryIter == m_ChunkMap.end())
			throw CNMRException(NMR_ERROR_BINARYCHUNKENTRYNOTFOUND);

		nfUint32 nChunkIndex = iEntryIter->second.first;
		nEntryIndex = iEntryIter->second.second;

		return m_Chunks[nChunkIndex].get();
	}

	void CChunkedBinaryStreamReader::findChunkInformation(nfUint32 nEntryID, eChunkedBinaryDataType & dataType, nfUint32 & nCount)
	{
		nfUint32 nEntryIndex;
		auto pChunk = findChunkByEntry(nEntryID, nEntryIndex);
		pChunk->getInformation(nEntryIndex, dataType, nCount);
	}

	nfUint32 CChunkedBinaryStreamReader::getTypedChunkEntryCount(nfUint32 nEntryID, eChunkedBinaryDataType dataType)
	{
		nfUint32 nCount;
		eChunkedBinaryDataType existingDataType;

		findChunkInformation(nEntryID, existingDataType, nCount);
		if (existingDataType != dataType)
			throw CNMRException(NMR_ERROR_UNEXPECTEDCHUNKDATATYPE);

		return nCount;
	}

	void CChunkedBinaryStreamReader::readIntArray(nfUint32 nEntryID, nfInt32 * pData, nfUint32 nDataCount)
	{
		nfUint32 nEntryIndex, nEntryType, nEntrySize;
		nfUint32 nIndex;
		nfInt32 nLastValue;

		auto pChunk = findChunkByEntry(nEntryID, nEntryIndex);
		pChunk->loadData();
		pChunk->seekToEntry(nEntryIndex, nEntryType, nEntrySize);

		if ((nEntrySize % 4) != 0)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);
		nfUint32 nExistingCount = (nEntrySize / 4);
		if (nDataCount != nExistingCount)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		if (nDataCount > 0) {

			switch (nEntryType) {
				case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_NOPREDICTION:
					for (nIndex = 0; nIndex < nDataCount; nIndex++)
						pData[nIndex] = pChunk->readInt32();

					break;
				case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_DELTAPREDICTION:
					nLastValue = 0;
					for (nIndex = 0; nIndex < nDataCount; nIndex++) {
						pData[nIndex] = pChunk->readInt32() + nLastValue;
						nLastValue = pData[nIndex];
					}
					break;

				default:					
					throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYTYPE);
			}
		}

	}

	void CChunkedBinaryStreamReader::readFloatArray(nfUint32 nEntryID, nfFloat * pData, nfUint32 nDataCount)
	{
		nfUint32 nEntryIndex, nEntryType, nEntrySize;
		nfUint32 nIndex;
		nfInt32 nValue, nLastValue;

		auto pChunk = findChunkByEntry(nEntryID, nEntryIndex);
		pChunk->loadData();
		pChunk->seekToEntry(nEntryIndex, nEntryType, nEntrySize);

		if ((nEntrySize % 4) != 0)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);
		if (nEntrySize < 4)
			throw CNMRException(NMR_ERROR_INVALIDCHUNKDATA);
		nfUint32 nExistingCount = ((nEntrySize - 4) / 4);
		if (nDataCount != nExistingCount)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		nfFloat fUnits = pChunk->readFloat();

		if (nDataCount > 0) {

			switch (nEntryType) {
			case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_NOPREDICTION:
				for (nIndex = 0; nIndex < nDataCount; nIndex++)
					pData[nIndex] = pChunk->readInt32() * fUnits;

				break;
			case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_DELTAPREDICTION:
				nLastValue = 0;
				for (nIndex = 0; nIndex < nDataCount; nIndex++) {
					nValue = pChunk->readInt32() + nLastValue;
					pData[nIndex] = nValue * fUnits;
					nLastValue = nValue;
				}
				break;
			default:
				throw CNMRException(NMR_ERROR_INVALIDCHUNKENTRYTYPE);

			}
		}

	}


	void CChunkedBinaryStreamReader::clearCache()
	{
		for (auto iChunk : m_Chunks) 
			iChunk->unloadData();
	}

}

