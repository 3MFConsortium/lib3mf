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

NMR_MeshInformation_Nurbs.cpp implements the Texture Mesh Information Class.

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

		MESHINFORMATION_NURBS * pNurbsInformation = (MESHINFORMATION_NURBS *)pData;
		if (pNurbsInformation) {
			pNurbsInformation->m_nResourceID = 0;
			for (j = 0; j < 3; j++) {
				pNurbsInformation->m_nUVIDs[j] = 0;
				pNurbsInformation->m_nEdgeIDs[j] = 0;
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
			for (j = 0; j < 3; j++) {
				pTargetFaceData->m_nUVIDs[j] = pSourceFaceData->m_nUVIDs[j];
				pTargetFaceData->m_nEdgeIDs[j] = pSourceFaceData->m_nEdgeIDs[j];
			}

			pTargetFaceData->m_nResourceID = pSourceFaceData->m_nResourceID;
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

			__NMRASSERT(nNodeIndex1 != nNodeIndex2);
			__NMRASSERT(nNodeIndex1 != nNodeIndex3);
			__NMRASSERT(nNodeIndex2 != nNodeIndex2);

			pFaceData->m_nUVIDs[0] = FaceDataCopy.m_nUVIDs[nNodeIndex1];
			pFaceData->m_nUVIDs[1] = FaceDataCopy.m_nUVIDs[nNodeIndex2];
			pFaceData->m_nUVIDs[2] = FaceDataCopy.m_nUVIDs[nNodeIndex3];

			int nEdgeIndex1 = 0;
			int nEdgeIndex2 = 0;
			int nEdgeIndex3 = 0;

			if (nNodeIndex1 == 0) {
				if (nNodeIndex2 == 1) {
					// 0/1/2
					nEdgeIndex1 = 0;
					nEdgeIndex2 = 1;
					nEdgeIndex3 = 2;
				}
				else {
					// 0/2/1
					nEdgeIndex1 = 2;
					nEdgeIndex2 = 1;
					nEdgeIndex3 = 0;
				}

			}
			else if (nNodeIndex1 == 1) {
				if (nNodeIndex2 == 0) {
					// 1/0/2
					nEdgeIndex1 = 0;
					nEdgeIndex2 = 2;
					nEdgeIndex3 = 1;

				}
				else {
					// 1/2/0
					nEdgeIndex1 = 1;
					nEdgeIndex2 = 2;
					nEdgeIndex3 = 0;
				}
			}
			else {
				if (nNodeIndex2 == 0) {
					// 2/0/1
					nEdgeIndex1 = 2;
					nEdgeIndex2 = 0;
					nEdgeIndex3 = 1;

				}
				else {
					// 2/1/0
					nEdgeIndex1 = 1;
					nEdgeIndex2 = 0;
					nEdgeIndex3 = 2;
				}

			}

			pFaceData->m_nEdgeIDs[0] = FaceDataCopy.m_nUVIDs[nEdgeIndex1];
			pFaceData->m_nEdgeIDs[1] = FaceDataCopy.m_nUVIDs[nEdgeIndex2];
			pFaceData->m_nEdgeIDs[2] = FaceDataCopy.m_nUVIDs[nEdgeIndex3];
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
			return (pFaceData->m_nResourceID != 0);

		return false;
	}


}
