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

NMR_PairMatchingTree.h defines a n-log-n tree class which is able to identify
duplicate pairs of numbers in a given data set.

--*/

#ifndef __NMR_PAIRMATCHINGTREE
#define __NMR_PAIRMATCHINGTREE

#include "Common/Math/NMR_Geometry.h" 
#include "Common/NMR_Types.h" 

#include <map>

namespace NMR {

	typedef struct {
		nfInt32 m_pair[2];
	} PAIRMATCHINGTREEENTRY;

	bool operator< (_In_ const PAIRMATCHINGTREEENTRY & entry1, _In_ const PAIRMATCHINGTREEENTRY & entry2);

	class CPairMatchingTree {
	private:
		std::map <PAIRMATCHINGTREEENTRY, nfInt32> m_entries;
	public:
		CPairMatchingTree();
		~CPairMatchingTree();

		void addMatch(_In_ nfInt32 data1, _In_ nfInt32 data2, _In_ nfInt32 param);
		_Success_(return) nfBool checkMatch(_In_ nfInt32 data1, _In_ nfInt32 data2, _Out_opt_ nfInt32 &param);
		void deleteMatch(_In_ nfInt32 data1, _In_ nfInt32 data2);
	};

}

#endif // __NMR_PAIRMATCHINGTREE




