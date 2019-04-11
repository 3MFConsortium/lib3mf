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

NMR_PairMatchingTree.cpp implements a n-log-n tree class which is able to identify
duplicate pairs of numbers in a given data set.

--*/


#include "Common/Math/NMR_PairMatchingTree.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>
#include <map>

namespace NMR {

	bool operator< (_In_ const PAIRMATCHINGTREEENTRY & entry1, _In_ const PAIRMATCHINGTREEENTRY & entry2)
	{
		for (int i = 0; i < 2; i++) {
			if (entry1.m_pair[i] < entry2.m_pair[i])
				return true;
			if (entry1.m_pair[i] > entry2.m_pair[i])
				return false;
		}
		return false;
	}


	CPairMatchingTree::CPairMatchingTree()
	{
	}

	CPairMatchingTree::~CPairMatchingTree()
	{
	}

	void CPairMatchingTree::addMatch(_In_ nfInt32 data1, _In_ nfInt32 data2, _In_ nfInt32 param)
	{
		PAIRMATCHINGTREEENTRY entry;
		if (data1 < data2) {
			entry.m_pair[0] = data1;
			entry.m_pair[1] = data2;
		}
		else {
			entry.m_pair[0] = data2;
			entry.m_pair[1] = data1;
		}

		m_entries.insert(std::make_pair(entry, param));
	}

	_Success_(return) nfBool CPairMatchingTree::checkMatch(_In_ nfInt32 data1, _In_ nfInt32 data2, _Out_opt_ nfInt32 &param)
	{
		std::map<PAIRMATCHINGTREEENTRY, nfInt32>::iterator iter;
		PAIRMATCHINGTREEENTRY entry;
		if (data1 < data2) {
			entry.m_pair[0] = data1;
			entry.m_pair[1] = data2;
		}
		else {
			entry.m_pair[0] = data2;
			entry.m_pair[1] = data1;
		}

		iter = m_entries.find(entry);
		if (iter != m_entries.end()) {
			param = iter->second;
			return true;
		}
		
		return false;
	}

	void CPairMatchingTree::deleteMatch(_In_ nfInt32 data1, _In_ nfInt32 data2)
	{
		PAIRMATCHINGTREEENTRY entry;
		if (data1 < data2) {
			entry.m_pair[0] = data1;
			entry.m_pair[1] = data2;
		}
		else {
			entry.m_pair[0] = data2;
			entry.m_pair[1] = data1;
		}
		m_entries.erase(entry);

	}

}

