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

NMR_ModelSliceStack.h: defines the Model Slice Stack Class.
A Model Slice Stack represents a ordered list of slices.

--*/

#ifndef __NMR_MODELSLICESTACK
#define __NMR_MODELSLICESTACK

#include "Common/NMR_Types.h"
#include "Common/Mesh/NMR_MeshTypes.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelSlice.h"

#include <vector>

namespace NMR {


	class CModelSliceStack : public CModelResource {
	private:
		nfDouble m_dZBottom;

		std::vector<PModelSliceStack> m_pSliceRefs;
		std::vector<PSlice> m_pSlices;
	public:
		bool AllowsGeometry() const;
		bool AllowsReferences() const;

	public:
		CModelSliceStack() = delete;
		CModelSliceStack(_In_ const ModelResourceID sID, _In_ CModel * pModel, nfDouble dZBottom);

		~CModelSliceStack();

		PSlice AddSlice(const nfDouble dZTop);
		void AddSliceRef(PModelSliceStack);

		nfUint32 getSliceCount();
		PSlice getSlice(nfUint32 nIndex);

		nfUint32 getSliceRefCount();
		PModelSliceStack getSliceRef(nfUint32 nIndex);

		nfDouble getZBottom();
		void setZBottom(nfDouble dBottomZ);

		nfDouble getHighestZ() const;

		void CollapseSliceReferences();

		std::string OwnPath();
		void SetOwnPath(std::string);

		bool areAllPolygonsClosed();
	};

	typedef std::shared_ptr<CModelSliceStack> PModelSliceStack;
}

#endif // __NMR_MODELSLICESTACK
