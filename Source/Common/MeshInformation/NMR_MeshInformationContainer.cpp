/*++

Copyright (C) 2015 Microsoft Corporation
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

NMR_MeshInformationContainer.cpp implements the Mesh Information Container
Class. This class provides a memory container for holding the texture
information state of a complete mesh structure.

--*/

#include "Common/MeshInformation/NMR_MeshInformationContainer.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>

namespace NMR {

	CMeshInformationContainer::CMeshInformationContainer()
	{
		m_nFaceCount = 0;
		m_nRecordSize = 0;
		m_CurrentDataBlock = NULL;
	}

	CMeshInformationContainer::CMeshInformationContainer(nfUint32 nCurrentFaceCount, nfUint32 nRecordSize)
	{
		m_nFaceCount = 0;
		m_nRecordSize = nRecordSize;
		m_CurrentDataBlock = NULL;

		nfUint32 nIdx;
		for (nIdx = 1; nIdx <= nCurrentFaceCount; nIdx++)
			addFaceData(nIdx);
	}

	CMeshInformationContainer::~CMeshInformationContainer()
	{
		clear();
	}

	_Ret_notnull_ MESHINFORMATIONFACEDATA * CMeshInformationContainer::addFaceData(nfUint32 nNewFaceCount)
	{
		nfUint32 j, nPageSize;
		MESHINFORMATIONFACEDATA * pByte;

		if (m_nRecordSize == 0)
			throw CNMRException(NMR_ERROR_INVALIDRECORDSIZE);

		nfUint32 nIdx = m_nFaceCount % MESHINFORMATIONCOUNTER_BUFFERSIZE;
		if (nIdx == 0) {
			nPageSize = m_nRecordSize * MESHINFORMATIONCOUNTER_BUFFERSIZE;
			m_CurrentDataBlock = new MESHINFORMATIONFACEDATA[nPageSize];

			m_DataBlocks.push_back(m_CurrentDataBlock);

			pByte = m_CurrentDataBlock;
			for (j = 0; j < nPageSize; j++) {
				*pByte = 0;
				pByte++;
			}
		}

		if (!m_CurrentDataBlock)
			throw CNMRException(NMR_ERROR_INVALIDMESHINFORMATIONDATA);

		MESHINFORMATIONFACEDATA * result = &m_CurrentDataBlock[m_nRecordSize * nIdx];

		m_nFaceCount++;
		if (m_nFaceCount != nNewFaceCount)
			throw CNMRException(NMR_ERROR_MESHINFORMATIONCOUNTMISMATCH);

		return result;
	}

	_Ret_notnull_ MESHINFORMATIONFACEDATA * CMeshInformationContainer::getFaceData(nfUint32 nIdx)
	{
		if (nIdx >= m_nFaceCount)
			throw CNMRException(NMR_ERROR_INVALIDMESHINFORMATIONINDEX);

		nfUint32 nBlockIdx = nIdx / MESHINFORMATIONCOUNTER_BUFFERSIZE;
		nfUint32 nModIdx = nIdx % MESHINFORMATIONCOUNTER_BUFFERSIZE;

		MESHINFORMATIONFACEDATA * pBlock = m_DataBlocks[nBlockIdx];
		return &pBlock[m_nRecordSize * nModIdx];
	}

	nfUint32 CMeshInformationContainer::getCurrentFaceCount()
	{
		return m_nFaceCount;
	}

	void CMeshInformationContainer::clear()
	{
		std::vector<MESHINFORMATIONFACEDATA *>::iterator iter = m_DataBlocks.begin ();
		while (iter != m_DataBlocks.end()){
			MESHINFORMATIONFACEDATA * pBlock = *iter;
			delete[] pBlock;
			iter++;
		}

		m_nFaceCount = 0;
		m_nRecordSize = 0;
		m_CurrentDataBlock = NULL;
	}

}
