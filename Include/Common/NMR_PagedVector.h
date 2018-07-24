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

NMR_PagedVector.h defines a vector class which allocates its memory block-wise, leading to 
significant performance improvements against a standard template library vector.

--*/

#ifndef __NMR_PAGEDVECTOR
#define __NMR_PAGEDVECTOR

#include "Common/NMR_Local.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Exception.h"
#include <vector>

#include <array>

namespace NMR {

	template <class T, unsigned int DEFAULTBLOCKSIZE = 1024>
	class CPagedVector {
		nfUint32 m_nBlockSize;
		nfUint32 m_nCount;
		T * m_pHeadBlock;
		std::vector<T *> m_pBlocks;
	public:

		CPagedVector() {
			m_nCount = 0;
			m_pHeadBlock = NULL;
			m_nBlockSize = DEFAULTBLOCKSIZE;
			if (m_nBlockSize == 0)
				throw CNMRException(NMR_ERROR_INVALIDBLOCKSIZE);
		}

		CPagedVector(_In_ nfUint32 nBlockSize) {
			m_nCount = 0;
			m_pHeadBlock = NULL;
			m_nBlockSize = nBlockSize;
			if (m_nBlockSize == 0)
				throw CNMRException(NMR_ERROR_INVALIDBLOCKSIZE);
		}

		~CPagedVector() {
			clearAllData();
		}

		nfUint32 getCount() {
			return m_nCount;
		}

		_Ret_notnull_ T * allocData() {
			nfUint32 nIdx = (m_nCount % m_nBlockSize);

			// Allocate new node block if necessary
			if (nIdx == 0) {
				m_pHeadBlock = new T[m_nBlockSize];
				m_pBlocks.push_back(m_pHeadBlock);
			}

			T * pResult = &m_pHeadBlock[nIdx];
			m_nCount++;

			return pResult;
		}

		_Ret_notnull_ T * allocData(_Out_ nfUint32 & nNewIndex) {
			nNewIndex = m_nCount;
			return allocData();
		}

		T& allocDataRef(_Out_ nfUint32& nNewIndex) {
			nfUint32 nIdx = (m_nCount % m_nBlockSize);

			// Allocate new node block if necessary
			if (nIdx == 0) {
				m_pHeadBlock = new T[m_nBlockSize];
				m_pBlocks.push_back(m_pHeadBlock);
			}

			nNewIndex = m_nCount;
			m_nCount++;

			return m_pHeadBlock[nIdx];
		}

		_Ret_notnull_ T * getData(_In_ nfUint32 nIdx) {
			if (nIdx >= m_nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			T * block = m_pBlocks[nIdx / m_nBlockSize];
			return &block[nIdx % m_nBlockSize];
		}

		T& getDataRef(_In_ nfUint32 nIdx) {
			if (nIdx >= m_nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			T* block = m_pBlocks[nIdx / m_nBlockSize];
			return block[nIdx % m_nBlockSize];
		}

		void clearAllData() {
			for (auto iIterator = m_pBlocks.begin(); iIterator != m_pBlocks.end(); iIterator++)
			{
				T * pBlock = *iIterator;
				delete[] pBlock;
			}

			m_pBlocks.clear();
			m_nCount = 0;
			m_pHeadBlock = NULL;
		}

		nfUint32 getBlockSize() {
			return m_nBlockSize;
		}
	};

}

#endif // __NMR_PAGEDVECTOR
