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

NMR_ModelSlice.h: defines the Model Slice Class.
A Model Slice represents a single slice as vertices and polygons

--*/

#ifndef __NMR_MODELSLICE
#define __NMR_MODELSLICE

#include "Common/NMR_Types.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Model/Classes/NMR_ModelResource.h"

#include <vector>

namespace NMR {
	class CSlice {
	private:
		std::vector<SLICENODE> m_Vertices;
		std::vector<std::vector<nfUint32>> m_Polygons;

		nfDouble m_dZTop;

	public:
		CSlice() = delete;
		CSlice(nfDouble dZTop);
		CSlice(CSlice&);
		~CSlice();

		nfUint32 addVertex(nfFloat x, nfFloat y);

		void getVertex(nfUint32 nIndex, nfFloat *x, nfFloat *y);

		nfUint32 beginPolygon();

		void addPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndex);

		nfUint32 getPolygonCount();

		nfDouble getTopZ();

		void setTopZ(nfDouble dZTop);

		_Ret_notnull_ SLICENODE *getNode(nfUint32 nIndex);

		nfUint32 getNumberOfPolygons();

		nfUint32 getPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndexOfIndex);

		nfUint32 getPolygonIndexCount(nfUint32 nPolygonIndex);

		nfUint32 getVertexCount();

		bool allPolygonsAreClosed();

		bool isPolygonValid(nfUint32 nPolygonIndex);
	};

	typedef std::shared_ptr <CSlice> PSlice;
}

#endif // __NMR_MODELSLICE
