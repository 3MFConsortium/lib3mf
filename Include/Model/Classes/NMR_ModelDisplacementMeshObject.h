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

#ifndef __NMR_MODELDISPLACEMENTMESHOBJECT
#define __NMR_MODELDISPLACEMENTMESHOBJECT

#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelDisp2DGroup.h"
#include <set>

namespace NMR {

	struct MODELMESHTRIANGLEDISPLACEMENT {
		PModelDisp2DGroupResource m_pGroup;
		nfUint32 m_nIndices[3];
	};

	class CModelDisplacementMeshObject : public CModelMeshObject {
	private:
		std::map<nfUint32, MODELMESHTRIANGLEDISPLACEMENT> m_TriangleDisplacements;

	public:
		CModelDisplacementMeshObject() = delete;
		CModelDisplacementMeshObject(_In_ ModelResourceID nID, _In_ CModel * pModel);

		nfBool hasTriangleDisplacement(_In_ nfUint32 nTriangleIndex) const;
		void setTriangleDisplacement(_In_ nfUint32 nTriangleIndex, _In_ PModelDisp2DGroupResource pGroup,
			_In_ nfUint32 nD1, _In_ nfUint32 nD2, _In_ nfUint32 nD3);
		MODELMESHTRIANGLEDISPLACEMENT getTriangleDisplacement(_In_ nfUint32 nTriangleIndex) const;
		void clearTriangleDisplacement(_In_ nfUint32 nTriangleIndex);
		nfUint32 getDisplacedTriangleCount() const;

		void setObjectType(_In_ eModelObjectType ObjectType) override;
		nfBool isValid() override;
		void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix) override;
		ResourceDependencies getDependencies() override;
	};

	typedef std::shared_ptr<CModelDisplacementMeshObject> PModelDisplacementMeshObject;
}

#endif
