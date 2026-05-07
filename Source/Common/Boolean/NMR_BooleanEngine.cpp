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

Winding-number based boolean field construction and CSG composition.

--*/

#include "Common/Boolean/NMR_BooleanEngine.h"

#include "Common/Boolean/NMR_BooleanDistanceField.h"
#include "Common/Boolean/NMR_BooleanSurfacePostProcess.h"
#include "Common/Boolean/NMR_MarchingCubes.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	void CBooleanEngine::evaluate(
		_In_ CMesh * pBaseMesh,
		_In_ const std::vector<PMesh> & operandMeshes,
		_In_ eModelBooleanOperation operation,
		_In_ CMesh * pResultMesh,
		_In_ nfUint32 nGridResolution)
	{
		if (!pResultMesh)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		const auto fieldData = Boolean::buildCSGField(pBaseMesh, operandMeshes, operation, nGridResolution);
		Boolean::extractIsoSurfaceMarchingCubes(
			pResultMesh,
			fieldData.values,
			fieldData.resolution,
			{ fieldData.minCorner.x, fieldData.minCorner.y, fieldData.minCorner.z },
			{ fieldData.maxCorner.x, fieldData.maxCorner.y, fieldData.maxCorner.z });
		Boolean::smoothAndProjectExtractedSurface(pResultMesh, fieldData);
	}

}
