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

#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include <vector>
#include <cmath>


namespace NMR {


	CChunkedBinaryStreamWriter::CChunkedBinaryStreamWriter(const std::string& sIndexPath, const std::string& sBinaryPath, PExportStreamMemory pBinaryExportStream)
		: m_pBinaryExportStream (pBinaryExportStream),
		m_elementIDCounter(1),
		m_CurrentChunk(nullptr),
		m_ChunkTableStart(0),
		m_bIsFinished(false),
		m_bIsEmpty(true),
		m_bQuantizeFloats(false),
		m_dQuantizationUnits(0.001),
		m_PredictionType (eptNoPredicition),
		m_nLZMALevel (5),
		m_bEnableLZMA (false),
		m_sBinaryPath (sBinaryPath),
		m_sIndexPath (sIndexPath)
	{
		if (pBinaryExportStream.get() == nullptr)
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
		m_CurrentChunk->m_ChunkID = (uint32_t) m_Chunks.size();
		m_CurrentChunkEntries = std::make_shared<std::vector<BINARYCHUNKFILEENTRY>>();
		m_CurrentChunkData.clear();

	}

	void CChunkedBinaryStreamWriter::finishChunk()
	{
		if (m_CurrentChunk == nullptr)
			throw CNMRException(NMR_ERROR_NOSTREAMCHUNKOPEN);

		__NMRASSERT(m_CurrentChunk->m_UncompressedDataSize == (m_CurrentChunkData.size() * 4));

		if (m_CurrentChunkEntries.get() != nullptr) {

			if (m_CurrentChunkEntries->size() > 0) {
				m_CurrentChunk->m_EntryCount = (nfUint32)m_CurrentChunkEntries->size();
				m_CurrentChunk->m_EntryTableStart = m_pBinaryExportStream->getPosition();
				m_pBinaryExportStream->writeBuffer(m_CurrentChunkEntries->data(), m_CurrentChunkEntries->size() * sizeof(BINARYCHUNKFILEENTRY));

				m_ChunkEntries.insert(std::make_pair(m_CurrentChunk->m_ChunkID, m_CurrentChunkEntries));
			}
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

			if (m_bEnableLZMA) {

				// TODO
				throw CNMRException(NMR_ERROR_COULDNOTCOMPRESSDATA);

			}
			else {

				// Uncompressed for LibZ Compression of 3MF Package
				outBufferSize = inputSize;
				for (size_t nIndex = 0; nIndex < inputSize; nIndex++)
					outBuffer[nIndex] = pInputBuffer[nIndex];
				outPropsSize = 0;

			}

			m_CurrentChunk->m_CompressedDataSize = (nfUint32) outBufferSize;
			m_CurrentChunk->m_CompressedPropsSize = (nfUint32) outPropsSize;

			m_CurrentChunk->m_CompressedDataStart = m_pBinaryExportStream->getPosition();
			m_pBinaryExportStream->writeBuffer (outBuffer.data(), outBufferSize);
			m_pBinaryExportStream->writeBuffer (outProps.data(), outPropsSize);
		}

		m_CurrentChunk = nullptr;
	}

	void CChunkedBinaryStreamWriter::setEnableLZMA(bool bEnableLZMA)
	{
		m_bEnableLZMA = bEnableLZMA;
	}

	void CChunkedBinaryStreamWriter::setLZMALevel(uint32_t nLZMALevel)
	{
		m_nLZMALevel = nLZMALevel;
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


		if (m_CurrentChunkEntries.get () != nullptr)
			m_CurrentChunkEntries->push_back(Entry);

		m_CurrentChunk->m_EntryCount++;
		m_CurrentChunk->m_UncompressedDataSize += Entry.m_SizeInBytes;

		return nElementID;

	}


	nfUint32 CChunkedBinaryStreamWriter::addRawFloatArray(const nfFloat* pData, nfUint32 nLength)
	{
		nfUint32 nIndex;

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

		Entry.m_EntryType = BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_RAWDATA;

		for (nIndex = 0; nIndex < nLength; nIndex++) {
			float fValue = pData[nIndex];
			nfInt32* pValue = (nfInt32*)&fValue;

			m_CurrentChunkData.push_back(*pValue);
		}

		if (m_CurrentChunkEntries.get() != nullptr)
			m_CurrentChunkEntries->push_back(Entry);

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


		if (m_CurrentChunkEntries.get() != nullptr)
			m_CurrentChunkEntries->push_back(Entry);

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

		m_pBinaryExportStream->seekPosition(0, true);
		m_pBinaryExportStream->writeBuffer(&Header, sizeof(Header));
		m_pBinaryExportStream->seekFromEnd(0, true);
	}

	nfUint32 CChunkedBinaryStreamWriter::getChunkCount()
	{
		return (nfUint32) m_Chunks.size ();
	}

	void CChunkedBinaryStreamWriter::copyBinaryToStream(PExportStream pStream)
	{
		if (!m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERNOTFINISHED);

		if (pStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const nfByte * pData = m_pBinaryExportStream->getData();
		nfUint64 nDataSize = m_pBinaryExportStream->getDataSize();

		pStream->writeBuffer(pData, nDataSize);

	}

	void CChunkedBinaryStreamWriter::writeIndexXML(PExportStream pStream)
	{
		if (pStream.get () == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		PXmlWriter_Native pXMLWriter = std::make_shared<CXmlWriter_Native>(pStream);

		pXMLWriter->WriteStartElement (nullptr, "binaryindex", XML_3MF_NAMESPACE_BINARYSPEC);

		pXMLWriter->WriteStartElement(nullptr, "chunks", nullptr);
		pXMLWriter->WriteAttributeString(nullptr, "data", "", m_sBinaryPath.c_str());

		for (auto& chunk : m_Chunks) {
			std::string sChunkID = std::to_string(chunk.m_ChunkID);
			std::string sChunkCompressedSize = std::to_string(chunk.m_CompressedDataSize);
			std::string sChunkUncompressedSize = std::to_string(chunk.m_UncompressedDataSize);
			std::string sChunkDataOffset = std::to_string(chunk.m_CompressedDataStart);
			pXMLWriter->WriteStartElement(nullptr, "chunk", nullptr);
			pXMLWriter->WriteAttributeString(nullptr, "id", "", sChunkID.c_str());
			pXMLWriter->WriteAttributeString(nullptr, "fileoffset", "", sChunkDataOffset.c_str());
			pXMLWriter->WriteAttributeString(nullptr, "compressedsize", "", sChunkCompressedSize.c_str());
			pXMLWriter->WriteAttributeString(nullptr, "uncompressedsize", "", sChunkUncompressedSize.c_str());

			auto iIter = m_ChunkEntries.find(chunk.m_ChunkID);
			if (iIter != m_ChunkEntries.end()) {
				for (auto & entry : *iIter->second) {
					pXMLWriter->WriteStartElement(nullptr, "entry", nullptr);
					std::string sEntryID = std::to_string (entry.m_EntryID);
					std::string sEntrySize = std::to_string(entry.m_SizeInBytes);
					std::string sEntryPosition = std::to_string(entry.m_PositionInChunk);
					std::string sEntryType;
					switch (entry.m_EntryType) {
						case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_NOPREDICTION: sEntryType = "int32_noprediction"; break;
						case BINARYCHUNKFILEENTRYTYPE_INT32ARRAY_DELTAPREDICTION: sEntryType = "int32_deltaprediction"; break;
						case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_NOPREDICTION: sEntryType = "float32_noprediction"; break;
						case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_DELTAPREDICTION: sEntryType = "float32_deltaprediction"; break;
						case BINARYCHUNKFILEENTRYTYPE_FLOAT32ARRAY_RAWDATA: sEntryType = "float32_rawdata"; break;
						default:
							sEntryType = "unknown";

					}

					pXMLWriter->WriteAttributeString(nullptr, "id", nullptr, sEntryID.c_str());
					pXMLWriter->WriteAttributeString(nullptr, "size", nullptr, sEntrySize.c_str());
					pXMLWriter->WriteAttributeString(nullptr, "positioninchunk", nullptr, sEntryPosition.c_str());
					pXMLWriter->WriteAttributeString(nullptr, "type", nullptr, sEntryType.c_str());
					pXMLWriter->WriteEndElement();
				}
			}

			pXMLWriter->WriteFullEndElement();

		} 

		pXMLWriter->WriteFullEndElement(); 

		pXMLWriter->WriteFullEndElement(); 

		pXMLWriter->WriteEndDocument();

	}


	void CChunkedBinaryStreamWriter::writeChunkTable()
	{
		if (m_bIsFinished)
			throw CNMRException(NMR_ERROR_STREAMWRITERALREADYFINISHED);

		if (m_CurrentChunk != nullptr)
			throw CNMRException(NMR_ERROR_STREAMCHUNKALREADYOPEN);

		m_ChunkTableStart = m_pBinaryExportStream->getPosition();
		if (m_Chunks.size() > 0) {
			m_pBinaryExportStream->writeBuffer (m_Chunks.data(), sizeof (BINARYCHUNKFILECHUNK) * m_Chunks.size());
		}
	}

	nfBool CChunkedBinaryStreamWriter::isEmpty()
	{
		return m_bIsEmpty;
	}

	void CChunkedBinaryStreamWriter::setDefaultCompressionMode(bool bQuantizeFloat, double dQuantizationUnits, eChunkedBinaryPredictionType predictionType)
	{
		m_bQuantizeFloats = bQuantizeFloat;
		m_dQuantizationUnits = dQuantizationUnits;
		m_PredictionType = predictionType;
	}

	bool CChunkedBinaryStreamWriter::getFloatQuantization()
	{
		return m_bQuantizeFloats;
	}

	double CChunkedBinaryStreamWriter::getFloatQuantizationUnits()
	{
		return m_dQuantizationUnits;
	}

	eChunkedBinaryPredictionType CChunkedBinaryStreamWriter::getPredictionType()
	{
		return m_PredictionType;
	}

	std::string CChunkedBinaryStreamWriter::getBinaryPath()
	{
		return m_sBinaryPath;
	}

	std::string CChunkedBinaryStreamWriter::getIndexPath()
	{
		return m_sIndexPath;
	}

}
