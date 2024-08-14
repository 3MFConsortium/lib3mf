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

#ifndef __NMR_CHUNKEDBINARYSTREAMWRITER
#define __NMR_CHUNKEDBINARYSTREAMWRITER

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"

#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamTypes.h" 
#include <string>
#include <map>

namespace NMR {

	class CChunkedBinaryStreamWriter {
	private:
		PExportStreamMemory m_pBinaryExportStream;
		std::string m_sBinaryPath;
		std::string m_sIndexPath;

		nfUint32 m_elementIDCounter;
		nfBool m_bIsFinished;
		nfUint64 m_ChunkTableStart;

		nfBool m_bIsEmpty;

		bool m_bEnableLZMA;
		uint32_t m_nLZMALevel;

		eChunkedBinaryPredictionType m_PredictionType;
		bool m_bQuantizeFloats;
		double m_dQuantizationUnits;

		std::vector<BINARYCHUNKFILECHUNK> m_Chunks;
		std::map<uint32_t, std::shared_ptr<std::vector<BINARYCHUNKFILEENTRY>>> m_ChunkEntries;

		BINARYCHUNKFILECHUNK * m_CurrentChunk;
		std::shared_ptr<std::vector<BINARYCHUNKFILEENTRY>> m_CurrentChunkEntries;
		std::vector<nfInt32> m_CurrentChunkData;

		void writeHeader();
		void writeChunkTable();

	public:
	
		CChunkedBinaryStreamWriter (const std::string & sIndexPath, const std::string & sBinaryPath, PExportStreamMemory pBinaryExportStream);

		void beginChunk();
		void finishChunk();
		nfUint32 getChunkCount ();

		void setEnableLZMA(bool bEnableLZMA);
		void setLZMALevel(uint32_t nLZMALevel);

		void finishWriting ();

		nfUint32 addIntArray (const nfInt32 * pData, nfUint32 nLength, eChunkedBinaryPredictionType predictionType);
		nfUint32 addFloatArray(const nfFloat * pData, nfUint32 nLength, eChunkedBinaryPredictionType predictionType, nfFloat fDiscretizationUnits);
		nfUint32 addRawFloatArray(const nfFloat* pData, nfUint32 nLength);

		void copyBinaryToStream (PExportStream pStream);
		void writeIndexXML(PExportStream pStream);

		nfBool isEmpty();

		void setDefaultCompressionMode (bool bQuantizeFloat, double dQuantizationUnits, eChunkedBinaryPredictionType predictionType);
		bool getFloatQuantization();
		double getFloatQuantizationUnits();
		eChunkedBinaryPredictionType getPredictionType();

		std::string getBinaryPath();

		std::string getIndexPath();
		

	};

	typedef std::shared_ptr <CChunkedBinaryStreamWriter> PChunkedBinaryStreamWriter;

}

#endif // __NMR_CHUNKEDBINARYSTREAMWRITER
