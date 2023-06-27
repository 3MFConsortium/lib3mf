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

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamWriter.h" 
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h" 
#include "Common/NMR_Exception.h" 

#include "Libraries/lzma/LzmaLib.h"

#include <vector>


namespace NMR {


	CChunkedBinaryStreamWriter::CChunkedBinaryStreamWriter(PExportStreamMemory pExportStream)
		: m_pExportStream (pExportStream), 
		  m_elementIDCounter (1), 
		  m_CurrentChunk (nullptr), 
		  m_ChunkTableStart (0), 
		  m_bIsFinished (false), 
		  m_bIsEmpty (true)
	{
		if (pExportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		writeHeader();
	}

	void CChunkedBinaryStreamWriter::beginChunk()
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (m_CurrentChunk != nullptr)
			finishChunk ();

		BINARYCHUNKFILECHUNK Chunk;
		Chunk.m_EntryCount = 0;
		Chunk.m_EntryTableStart = 0;
		Chunk.m_CompressedDataSize = 0;
		Chunk.m_UncompressedDataSize = 0;
		Chunk.m_CompressedDataStart = 0;
		for (int j = 0; j < BINARYCHUNKFILECHUNKRESERVED; j++)
			Chunk.m_Reserved[j] = 0;
		for (int j = 0; j < 16; j++)
			Chunk.m_MD5Checksum[j] = 0;
		m_Chunks.push_back(Chunk);

		m_CurrentChunk = &(*m_Chunks.rbegin());
		m_CurrentChunkEntries.clear();
		m_CurrentChunkData.clear();

	}

	void CChunkedBinaryStreamWriter::finishChunk()
	{
		if (m_CurrentChunk == nullptr)
			throw CNMRException(NMR_ERROR_NOSTREAMCHUNKOPEN);

		__NMRASSERT(m_CurrentChunk->m_UncompressedDataSize == (m_CurrentChunkData.size() * 4));

		if (m_CurrentChunkEntries.size() > 0) {
			m_CurrentChunk->m_EntryCount = (nfUint32)m_CurrentChunkEntries.size();
			m_CurrentChunk->m_EntryTableStart = m_pExportStream->getPosition();
			m_pExportStream->writeBuffer(m_CurrentChunkEntries.data(), m_CurrentChunkEntries.size() * sizeof (BINARYCHUNKFILEENTRY));
		}

		if (m_CurrentChunkData.size() > 0) {
			std::vector <nfByte> outBuffer;
			outBuffer.resize(m_CurrentChunk->m_UncompressedDataSize * 2);

			std::vector <nfByte> outProps;
			outProps.resize(m_CurrentChunk->m_UncompressedDataSize * 2);

			nfByte * pInputBuffer = (nfByte*) m_CurrentChunkData.data();

			size_t inputSize = m_CurrentChunkData.size() * 4;
			size_t outBufferSize = outBuffer.size ();
			size_t outPropsSize = outProps.size ();

			int error;
			error = LzmaCompress (
				outBuffer.data(), &outBufferSize, 
				pInputBuffer, inputSize,  
				outProps.data(), &outPropsSize,

//				7, /* 0 <= level <= 9, default = 5 */
//				64 * 1024 * 1024, /* use (1 << N) or (3 << N). 4 KB < dictSize <= 128 MB */
//				5, /* 0 <= lc <= 8, default = 3  */
//				2, /* 0 <= lp <= 4, default = 0  */
//				3, /* 0 <= pb <= 4, default = 2  */
//				64,  /* 5 <= fb <= 273, default = 32 */
//				1 /* 1 or 2, default = 2 */

				3, /* 0 <= level <= 9, default = 5 */
				8 * 1024 * 1024, /* use (1 << N) or (3 << N). 4 KB < dictSize <= 128 MB */
				5, /* 0 <= lc <= 8, default = 3  */
				2, /* 0 <= lp <= 4, default = 0  */
				3, /* 0 <= pb <= 4, default = 2  */
				64,  /* 5 <= fb <= 273, default = 32 */
				1 /* 1 or 2, default = 2 */

			);

			if (error != SZ_OK)
				throw CNMRException(NMR_ERROR_COULDNOTCOMPRESSDATA);


			// Uncompressed for LibZ Compression
//			outBufferSize = inputSize; 
			//for (size_t nIndex = 0; nIndex < inputSize; nIndex++)
				//outBuffer[nIndex] = pInputBuffer[nIndex]; 
			//outPropsSize = 0;

			m_CurrentChunk->m_CompressedDataSize = (nfUint32) outBufferSize;
			m_CurrentChunk->m_CompressedPropsSize = (nfUint32) outPropsSize;

			m_CurrentChunk->m_CompressedDataStart = m_pExportStream->getPosition();
			m_pExportStream->writeBuffer (outBuffer.data(), outBufferSize);
			m_pExportStream->writeBuffer (outProps.data(), outPropsSize);
		}

		m_CurrentChunk = nullptr;
	}

	void CChunkedBinaryStreamWriter::finishWriting()
	{
		if (m_bIsFinished)
			return;

		if (m_CurrentChunk != nullptr)
			finishChunk();

		writeChunkTable();
		writeHeader();

		m_bIsFinished = true;
	}


	nfUint32 CChunkedBinaryStreamWriter::addIntArray(const nfInt32 * pData, nfUint32 nLength, eChunkedBinaryPredictionType predictionType)
	{
		nfUint32 nIndex;

		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (pData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (nLength == 0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_CurrentChunk == nullptr)
			beginChunk ();

		unsigned int nElementID = m_elementIDCounter;
		m_elementIDCounter++;

		m_bIsEmpty = false;

		BINARYCHUNKFILEENTRY Entry;
		Entry.m_EntryID = nElementID;
		Entry.m_SizeInBytes = (nLength * 4);
		Entry.m_PositionInChunk = m_CurrentChunk->m_UncompressedDataSize;

		switch (predictionType) {
			case eptNoPredicition:
				Entry.m_EntryType = BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_NOPREDICTION;
				for (nIndex = 0; nIndex < nLength; nIndex++) 
					m_CurrentChunkData.push_back (pData[nIndex]);

				break;
			case eptDeltaPredicition:
				Entry.m_EntryType = BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_DELTAPREDICTION;
				m_CurrentChunkData.push_back(pData[0]);
				for (nIndex = 1; nIndex < nLength; nIndex++) 
					m_CurrentChunkData.push_back(pData[nIndex] - pData[nIndex - 1]);

				break;
			default:
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

		};


		m_CurrentChunkEntries.push_back(Entry);

		m_CurrentChunk->m_EntryCount++;
		m_CurrentChunk->m_UncompressedDataSize += Entry.m_SizeInBytes;

		return nElementID;

	}


	nfUint32 CChunkedBinaryStreamWriter::addFloatArray(const nfFloat * pData, nfUint32 nLength, eChunkedBinaryPredictionType predictionType, nfFloat fDiscretizationUnits)
	{
		nfUint32 nIndex;
		nfInt64 nValue;
		nfInt64 nOldValue;

		if (fDiscretizationUnits <= 0.0f)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (pData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (nLength == 0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_CurrentChunk == nullptr)
			beginChunk();

		unsigned int nElementID = m_elementIDCounter;
		m_elementIDCounter++;

		m_bIsEmpty = false;

		BINARYCHUNKFILEENTRY Entry;
		Entry.m_EntryID = nElementID;
		Entry.m_SizeInBytes = (nLength * 4) + 4;
		Entry.m_PositionInChunk = m_CurrentChunk->m_UncompressedDataSize;

		// Add Discretization units
		m_CurrentChunkData.push_back(*((nfInt32 *)&fDiscretizationUnits));

		switch (predictionType) {
		case eptNoPredicition:
			Entry.m_EntryType = BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_NOPREDICTION;

			for (nIndex = 0; nIndex < nLength; nIndex++) {
				nfInt64 nValue = (nfInt64) (pData[nIndex] / fDiscretizationUnits);
				if (abs (nValue) > BINARYCHUNKFILE_MAXFLOATUNITS)
					throw CNMRException(NMR_ERROR_BINARYCHUNK_DISCRETIZATIONVALUEOUTOFRANGE);

				m_CurrentChunkData.push_back((nfInt32)nValue);
			}

			break;
		case eptDeltaPredicition:  
			Entry.m_EntryType = BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_DELTAPREDICTION;
			
			nValue = (nfInt64)(pData[0]);
			if (abs(nValue) > BINARYCHUNKFILE_MAXFLOATUNITS)
				throw CNMRException(NMR_ERROR_BINARYCHUNK_DISCRETIZATIONVALUEOUTOFRANGE);
			nOldValue = nValue;

			m_CurrentChunkData.push_back((nfInt32) nValue);
			for (nIndex = 1; nIndex < nLength; nIndex++) {
				nValue = (nfInt64)(pData[nIndex] / fDiscretizationUnits);
				if (abs(nValue) > BINARYCHUNKFILE_MAXFLOATUNITS)
					throw CNMRException(NMR_ERROR_BINARYCHUNK_DISCRETIZATIONVALUEOUTOFRANGE);
				m_CurrentChunkData.push_back(((nfInt32) nValue) - ((nfInt32) nOldValue));

				nOldValue = nValue;
			}

			break;
		default:
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		};


		m_CurrentChunkEntries.push_back(Entry);

		m_CurrentChunk->m_EntryCount++;
		m_CurrentChunk->m_UncompressedDataSize += Entry.m_SizeInBytes;

		return nElementID;

	}

	void CChunkedBinaryStreamWriter::writeHeader()
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (m_CurrentChunk != nullptr)
			throw CNMRException(NMR_ERROR_STREAMCHUNKALREADYOPEN);

		BINARYCHUNKFILEHEADER Header;

		Header.m_Sign = BINARYCHUNKFILEHEADERSIGN;
		Header.m_Version = BINARYCHUNKFILEHEADERVERSION;
		Header.m_ChunkCount = getChunkCount ();
		Header.m_ChunkTableStart = m_ChunkTableStart;
		for (int j = 0; j < BINARYCHUNKFILEHEADERRESERVED; j++)
			Header.m_Reserved[j] = 0;

		m_pExportStream->seekPosition(0, true);
		m_pExportStream->writeBuffer(&Header, sizeof(Header));
		m_pExportStream->seekFromEnd(0, true);
	}

	nfUint32 CChunkedBinaryStreamWriter::getChunkCount()
	{
		return (nfUint32) m_Chunks.size ();
	}

	void CChunkedBinaryStreamWriter::copyToStream(PExportStream pStream)
	{
		if (!m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERNOTFINISHED);

		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const nfByte * pData = m_pExportStream->getData();
		nfUint64 nDataSize = m_pExportStream->getDataSize();

		pStream->writeBuffer(pData, nDataSize);

	}


	void CChunkedBinaryStreamWriter::writeChunkTable()
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (m_CurrentChunk != nullptr)
			throw CNMRException(NMR_ERROR_STREAMCHUNKALREADYOPEN);

		m_ChunkTableStart = m_pExportStream->getPosition();
		if (m_Chunks.size() > 0) {
			m_pExportStream->writeBuffer (m_Chunks.data(), sizeof (BINARYCHUNKFILECHUNK) * m_Chunks.size());
		}
	}

	nfBool CChunkedBinaryStreamWriter::isEmpty()
	{
		return m_bIsEmpty;
	}

}
