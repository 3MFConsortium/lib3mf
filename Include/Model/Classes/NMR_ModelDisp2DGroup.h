/*++

Copyright (C) 2026 3MF Consortium

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

In-memory representation and serialization support for the 3MF Displacement extension.

--*/

#ifndef __NMR_MODELDISP2DGROUP
#define __NMR_MODELDISP2DGROUP

#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelDisplacement2D.h"
#include "Model/Classes/NMR_ModelNormVectorGroup.h"

namespace NMR {

	struct MODELDISPLACEMENT2DCOORDINATE {
		nfDouble m_dU;
		nfDouble m_dV;
		nfUint32 m_nNormalVectorIndex;
		nfDouble m_dDisplacementFactor;
	};

	class CModelDisp2DGroupResource : public CModelResource {
	private:
		PModelDisplacement2DResource m_pDisplacement2D;
		PModelNormVectorGroupResource m_pNormVectorGroup;
		nfDouble m_dHeight;
		nfDouble m_dOffset;
		std::vector<MODELDISPLACEMENT2DCOORDINATE> m_Coordinates;

		void validateCoordinate(_In_ const MODELDISPLACEMENT2DCOORDINATE & sCoordinate) const;

	public:
		CModelDisp2DGroupResource() = delete;
		CModelDisp2DGroupResource(_In_ ModelResourceID nID, _In_ CModel * pModel,
			_In_ PModelDisplacement2DResource pDisplacement2D,
			_In_ PModelNormVectorGroupResource pNormVectorGroup,
			_In_ nfDouble dHeight, _In_ nfDouble dOffset);

		PModelDisplacement2DResource getDisplacement2D() const;
		PModelNormVectorGroupResource getNormVectorGroup() const;
		nfDouble getHeight() const;
		void setHeight(_In_ nfDouble dHeight);
		nfDouble getOffset() const;
		void setOffset(_In_ nfDouble dOffset);

		nfUint32 getCount() const;
		nfUint32 addCoordinate(_In_ const MODELDISPLACEMENT2DCOORDINATE & sCoordinate);
		MODELDISPLACEMENT2DCOORDINATE getCoordinate(_In_ nfUint32 nIndex) const;
		void setCoordinate(_In_ nfUint32 nIndex, _In_ const MODELDISPLACEMENT2DCOORDINATE & sCoordinate);

		ResourceDependencies getDependencies() override;
	};

	typedef std::shared_ptr<CModelDisp2DGroupResource> PModelDisp2DGroupResource;
}

#endif
