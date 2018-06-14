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

NMR_MeshInformation_NodeColors.cpp implements the Node Color Mesh Information
Class. This class specializes the abstract Mesh Information Class to "colors
defined per node". It implements functions to interpolate and reconstruct colors
while the mesh topology is changing.

--*/

#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h" 
#include "Common/NMR_Exception.h" 
#include <cmath>

namespace NMR {

	CMeshInformation_NodeColors::CMeshInformation_NodeColors() : CMeshInformation()
	{
		m_pContainer = std::make_shared<CMeshInformationContainer>(0, (nfUint32) sizeof(MESHINFORMATION_NODECOLOR));
	}

	CMeshInformation_NodeColors::CMeshInformation_NodeColors(nfUint32 nCurrentFaceCount)
	{
		nfUint32 nIdx;
		m_pContainer = std::make_shared<CMeshInformationContainer>(nCurrentFaceCount, (nfUint32) sizeof(MESHINFORMATION_NODECOLOR));
		for (nIdx = 0; nIdx < nCurrentFaceCount; nIdx++)
			invalidateFace(m_pContainer->getFaceData(nIdx));
	}

	void CMeshInformation_NodeColors::invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData)
	{
		nfUint32 j;

		MESHINFORMATION_NODECOLOR * pNodeData = (MESHINFORMATION_NODECOLOR *)pData;
		if (pNodeData) {
			for (j = 0; j < 3; j++)
				pNodeData->m_cColors[j] = 0x00000000;
		}
	}

	eMeshInformationType CMeshInformation_NodeColors::getType()
	{
		return emiNodeColors;
	}

	void CMeshInformation_NodeColors::cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex)
	{
		__NMRASSERT(pOtherInformation);

		nfUint32 j;

		MESHINFORMATION_NODECOLOR * pTargetFaceData = (MESHINFORMATION_NODECOLOR*)getFaceData(nFaceIndex);
		MESHINFORMATION_NODECOLOR * pSourceFaceData = (MESHINFORMATION_NODECOLOR*)pOtherInformation->getFaceData(nOtherFaceIndex);
		if (pTargetFaceData && pSourceFaceData) {
			for (j = 0; j < 3; j++)
				pTargetFaceData->m_cColors[j] = pSourceFaceData->m_cColors[j];
		}
	}

	PMeshInformation CMeshInformation_NodeColors::cloneInstance(_In_ nfUint32 nCurrentFaceCount)
	{
		return std::make_shared<CMeshInformation_NodeColors>(nCurrentFaceCount);
	}

	void CMeshInformation_NodeColors::permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR*)getFaceData(nFaceIndex);
		if (pFaceData && (nNodeIndex1 < 3) && (nNodeIndex2 < 3) && (nNodeIndex3 < 3)) {
			MESHINFORMATION_NODECOLOR FaceDataCopy = *pFaceData;

			pFaceData->m_cColors[0] = FaceDataCopy.m_cColors[nNodeIndex1];
			pFaceData->m_cColors[1] = FaceDataCopy.m_cColors[nNodeIndex2];
			pFaceData->m_cColors[2] = FaceDataCopy.m_cColors[nNodeIndex3];
		}
	}

	nfUint32 CMeshInformation_NodeColors::getBackupSize()
	{
		return (nfUint32) sizeof (MESHINFORMATION_NODECOLOR);
	}

	void CMeshInformation_NodeColors::mergeInformationFrom(_In_ CMeshInformation * pInformation)
	{
		// nothing to merge
	}

	nfBool CMeshInformation_NodeColors::faceHasData(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATION_NODECOLOR * pFaceData = (MESHINFORMATION_NODECOLOR*)getFaceData(nFaceIndex);
		if (pFaceData) {
			return ((pFaceData->m_cColors[0] != 0) || (pFaceData->m_cColors[1] != 0) ||
				(pFaceData->m_cColors[2] != 0));
		}

		return false;
	}

}
