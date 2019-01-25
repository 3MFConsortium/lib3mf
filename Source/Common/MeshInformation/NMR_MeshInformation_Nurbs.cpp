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

NMR_MeshInformation_Nurbs.cpp defines the Nurbs Information Class.

--*/

#include "Common/MeshInformation/NMR_MeshInformation_Nurbs.h"
#include "Common/NMR_Exception.h"
#include "Common/Math/NMR_Vector.h"
#include <cmath>

namespace NMR {

	CMeshInformation_Nurbs::CMeshInformation_Nurbs() : CMeshInformation()
	{
		m_pContainer = std::make_shared<CMeshInformationContainer>(0, (nfUint32) sizeof(MESHINFORMATION_NURBS));
	}

	CMeshInformation_Nurbs::CMeshInformation_Nurbs(nfUint32 nCurrentFaceCount)
	{
		nfUint32 nIdx;
		m_pContainer = std::make_shared<CMeshInformationContainer>(nCurrentFaceCount, (nfUint32) sizeof(MESHINFORMATION_NURBS));
		for (nIdx = 0; nIdx < nCurrentFaceCount; nIdx++)
			invalidateFace(m_pContainer->getFaceData(nIdx));
	}

	void CMeshInformation_Nurbs::invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData)
	{
		nfUint32 j;

		MESHINFORMATION_NURBS * pNurbsData = (MESHINFORMATION_NURBS *)pData;
		if (pNurbsData) {
			pNurbsData->m_ResourceID = 0;
			for (j = 0; j < 3; j++) {
				pNurbsData->m_UVCoordIDs[j] = 0;
			}
		}
	}

	eMeshInformationType CMeshInformation_Nurbs::getType()
	{
		return emiNurbs;
	}

	void CMeshInformation_Nurbs::cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex)
	{
		nfUint32 j;

		__NMRASSERT(pOtherInformation);

		MESHINFORMATION_NURBS * pTargetFaceData = (MESHINFORMATION_NURBS*)getFaceData(nFaceIndex);
		MESHINFORMATION_NURBS * pSourceFaceData = (MESHINFORMATION_NURBS*)pOtherInformation->getFaceData(nOtherFaceIndex);

		if (pTargetFaceData && pSourceFaceData) {
			for (j = 0; j < 3; j++)
				pTargetFaceData->m_UVCoordIDs[j] = pSourceFaceData->m_UVCoordIDs[j];

			pTargetFaceData->m_ResourceID = pSourceFaceData->m_ResourceID;
		}
	}

	PMeshInformation CMeshInformation_Nurbs::cloneInstance(_In_ nfUint32 nCurrentFaceCount)
	{
		return std::make_shared<CMeshInformation_Nurbs>(nCurrentFaceCount);
	}

	void CMeshInformation_Nurbs::permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		MESHINFORMATION_NURBS * pFaceData = (MESHINFORMATION_NURBS*)getFaceData(nFaceIndex);
		if (pFaceData && (nNodeIndex1 < 3) && (nNodeIndex2 < 3) && (nNodeIndex3 < 3)) {
			MESHINFORMATION_NURBS FaceDataCopy = *pFaceData;

			pFaceData->m_UVCoordIDs[0] = FaceDataCopy.m_UVCoordIDs[nNodeIndex1];
			pFaceData->m_UVCoordIDs[1] = FaceDataCopy.m_UVCoordIDs[nNodeIndex2];
			pFaceData->m_UVCoordIDs[2] = FaceDataCopy.m_UVCoordIDs[nNodeIndex3];
		}
	}

	nfUint32 CMeshInformation_Nurbs::getBackupSize()
	{
		return (nfUint32) sizeof (MESHINFORMATION_NURBS);
	}

	void CMeshInformation_Nurbs::mergeInformationFrom(_In_ CMeshInformation * pInformation)
	{
		__NMRASSERT(pInformation);

	}

	nfBool CMeshInformation_Nurbs::faceHasData(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATION_NURBS * pFaceData = (MESHINFORMATION_NURBS*)getFaceData(nFaceIndex);
		if (pFaceData)
			return (pFaceData->m_ResourceID != 0);

		return false;
	}

}


