/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CLib3MFSlice

*/


#ifndef __LIB3MF_LIB3MFSLICE
#define __LIB3MF_LIB3MFSLICE

#include "lib3mf_interfaces.hpp"


// Include custom headers here.
#include "Model/Classes/NMR_ModelSlice.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CLib3MFSlice 
**************************************************************************************************************************/

class CLib3MFSlice : public virtual ILib3MFSlice {
private:

	/**
	* Put private members here.
	*/
	NMR::PSlice m_pSlice;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CLib3MFSlice(NMR::PSlice);

	/**
	* Public member functions to implement.
	*/

	void SetVertices (const Lib3MF_uint64 nVerticesBufferSize, const sLib3MFPosition2D * pVerticesBuffer);

	void GetVertices (Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, sLib3MFPosition2D * pVerticesBuffer);

	Lib3MF_uint64 GetVertexCount ();

	Lib3MF_uint64 GetPolygonCount();

	void SetPolygonIndices (const Lib3MF_uint64 nIndex, const Lib3MF_uint64 nIndicesBufferSize, const Lib3MF_uint32 * pIndicesBuffer);

	void GetPolygonIndices (const Lib3MF_uint64 nIndex, Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, Lib3MF_uint32 * pIndicesBuffer);

	void GetPolygonIndexCount (const Lib3MF_uint64 nIndex, Lib3MF_uint64 & nCount);

	double GetZTop();
};

} // namespace Impl
} // namespace Lib3MF

#endif // __LIB3MF_LIB3MFSLICE
