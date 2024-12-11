/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CSlice

*/


#ifndef __LIB3MF_SLICE
#define __LIB3MF_SLICE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CSlice 
**************************************************************************************************************************/

class CSlice : public virtual ISlice, public virtual CBase {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	void SetVertices(const Lib3MF_uint64 nVerticesBufferSize, const Lib3MF::sPosition2D * pVerticesBuffer) override;

	void GetVertices(Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, Lib3MF::sPosition2D * pVerticesBuffer) override;

	Lib3MF_uint64 GetVertexCount() override;

	Lib3MF_uint64 AddPolygon(const Lib3MF_uint64 nIndicesBufferSize, const Lib3MF_uint32 * pIndicesBuffer) override;

	Lib3MF_uint64 GetPolygonCount() override;

	void SetPolygonIndices(const Lib3MF_uint64 nIndex, const Lib3MF_uint64 nIndicesBufferSize, const Lib3MF_uint32 * pIndicesBuffer) override;

	void GetPolygonIndices(const Lib3MF_uint64 nIndex, Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, Lib3MF_uint32 * pIndicesBuffer) override;

	Lib3MF_uint64 GetPolygonIndexCount(const Lib3MF_uint64 nIndex) override;

	Lib3MF_double GetZTop() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_SLICE
