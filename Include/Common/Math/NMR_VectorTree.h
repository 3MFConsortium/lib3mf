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

NMR_VectorTree.h defines a n*log(n) lookup tree class to identify vectors by
their position.

--*/

#ifndef __NMR_VECTORTREE
#define __NMR_VECTORTREE

#include "Common/Math/NMR_Geometry.h" 
#include "Common/NMR_Types.h" 

#include <map>

namespace NMR {

	typedef struct {
		NVEC3I m_position;
	} VECTORTREEENTRY;

	bool operator< (_In_ const VECTORTREEENTRY & entry1, _In_ const VECTORTREEENTRY & entry2);

	class CVectorTree {
	private:
		nfFloat m_fUnits;
		std::map <VECTORTREEENTRY, nfUint32> m_entries;
	public:
		CVectorTree();
		CVectorTree(_In_ nfFloat fUnits);

		nfFloat getUnits();
		void setUnits(_In_ nfFloat fUnits);

		_Success_(return) nfBool findVector2(_In_ NVEC2 vVector, _Out_opt_ nfUint32 & value);
		_Success_(return) nfBool findVector3(_In_ NVEC3 vVector, _Out_opt_ nfUint32 & value);
		_Success_(return) nfBool findIntVector2(_In_ NVEC2I vVector, _Out_opt_ nfUint32 & value);
		_Success_(return) nfBool findIntVector3(_In_ NVEC3I vVector, _Out_opt_ nfUint32 & value);

		void addVector2(_In_ NVEC2 vVector, _In_ nfUint32 value);
		void addVector3(_In_ NVEC3 vVector, _In_ nfUint32 value);
		void addIntVector2(_In_ NVEC2I vVector, _In_ nfUint32 value);
		void addIntVector3(_In_ NVEC3I vVector, _In_ nfUint32 value);

		void removeVector2(_In_ NVEC2 vVector);
		void removeVector3(_In_ NVEC3 vVector);
		void removeIntVector2(_In_ NVEC2I vVector);
		void removeIntVector3(_In_ NVEC3I vVector);
	};

}

#endif // __NMR_VECTORTREE
