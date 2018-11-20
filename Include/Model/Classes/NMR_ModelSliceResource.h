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

NMR_ModelSliceResource.h: defines the resource object for slices

--*/

#ifndef __NMR_SLICERESOURCE
#define __NMR_SLICERESOURCE

#include "Common/NMR_Types.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Model/Classes/NMR_ModelResource.h"

#include <vector>

namespace NMR {
	class CSlice {
	private:
		std::vector<SLICENODE> m_Vertices;
		std::vector<std::vector<nfUint32> > m_Polygons;

		nfFloat m_topZ;

	public:
		CSlice();
		~CSlice();

		nfUint32 addVertex(nfFloat x, nfFloat y);

		void getVertex(nfUint32 nIndex, nfFloat *x, nfFloat *y);

		nfUint32 beginPolygon();

		void addPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndex);

		nfUint32 getPolygonCount();

		nfFloat getTopZ();

		void setTopZ(nfFloat nTopZ);

		_Ret_notnull_ SLICENODE *getNode(nfUint32 nIndex);

		nfUint32 getNumberOfPolygons();

		nfUint32 getPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndexOfIndex);

		nfUint32 getPolygonIndexCount(nfUint32 nPolygonIndex);

		nfUint32 getVertexCount();

		bool allPolygonsAreClosed();

		bool isPolygonValid(nfUint32 nPolygonIndex);
	};

	typedef std::shared_ptr <CSlice> PSlice;

	class CSliceStack {
	private:
		nfFloat  m_BottomZ;
		std::vector<PSlice> m_Slices;
		nfBool m_bUsesSliceRef;

	public:
		CSliceStack();
		~CSliceStack();

		_Ret_notnull_ PSlice getSlice(nfUint32 nIndex);

		nfUint32 getSliceCount();
		nfUint32 addSlice(PSlice pSlice);
		void mergeSliceStack(PSliceStack pSliceStack);

		nfBool usesSliceRef();

		nfFloat getBottomZ();
		void setBottomZ(nfFloat nBottomZ);
		void setUsesSliceRef(nfBool bUsesSliceRef);
		bool areAllPolygonsClosed();
	};

	typedef std::shared_ptr<CSliceStack> PSliceStack;

	class CModelSliceStackResource : public CModelResource {
	private:
		PSliceStack m_pSliceStack;
		nfUint32 m_nNumSliceRefsToMe;	// number of sliceref-references to this resource
	public:
		CModelSliceStackResource() = delete;
		CModelSliceStackResource(_In_ const ModelResourceID sID, _In_ CModel * pModel, PSliceStack pSliceStack);
		~CModelSliceStackResource();

		void setSliceStack(PSliceStack pSliceStack);
		_Ret_notnull_ PSliceStack getSliceStack();
		std::string sliceRefPath();
		nfUint32& NumSliceRefsToMe();
	};

	typedef std::shared_ptr<CModelSliceStackResource> PModelSliceStackResource;
}

#endif
