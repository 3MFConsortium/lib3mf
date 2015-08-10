/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

#include "Common/MeshInformation/NMR_MeshInformation_BaseMaterials.h" 
#include "Common/NMR_Exception.h" 
#include <math.h>

namespace NMR {

	CMeshInformation_BaseMaterials::CMeshInformation_BaseMaterials() : CMeshInformation()
	{
		m_pContainer = std::make_shared<CMeshInformationContainer>(0, (nfUint32) sizeof(MESHINFORMATION_BASEMATERIAL));
	}

	CMeshInformation_BaseMaterials::CMeshInformation_BaseMaterials(nfUint32 nCurrentFaceCount)
	{
		nfUint32 nIdx;
		m_pContainer = std::make_shared<CMeshInformationContainer>(nCurrentFaceCount, (nfUint32) sizeof(MESHINFORMATION_BASEMATERIAL));
		for (nIdx = 0; nIdx < nCurrentFaceCount; nIdx++)
			invalidateFace(m_pContainer->getFaceData(nIdx));
	}

	void CMeshInformation_BaseMaterials::invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData)
	{
		MESHINFORMATION_BASEMATERIAL * pNodeData = (MESHINFORMATION_BASEMATERIAL *)pData;
		if (pNodeData) {
			pNodeData->m_nMaterialGroupID = 0;
			pNodeData->m_nMaterialIndex = 0;
		}
	}

	eMeshInformationType CMeshInformation_BaseMaterials::getType()
	{
		return emiBaseMaterials;
	}

	void CMeshInformation_BaseMaterials::cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex)
	{
		__NMRASSERT(pOtherInformation);

		MESHINFORMATION_BASEMATERIAL * pTargetFaceData = (MESHINFORMATION_BASEMATERIAL*)getFaceData(nFaceIndex);
		MESHINFORMATION_BASEMATERIAL * pSourceFaceData = (MESHINFORMATION_BASEMATERIAL*)pOtherInformation->getFaceData(nOtherFaceIndex);
		if (pTargetFaceData && pSourceFaceData) {
			pTargetFaceData->m_nMaterialGroupID = pSourceFaceData->m_nMaterialGroupID;
			pTargetFaceData->m_nMaterialIndex = pSourceFaceData->m_nMaterialIndex;
		}
	}

	PMeshInformation CMeshInformation_BaseMaterials::cloneInstance(_In_ nfUint32 nCurrentFaceCount)
	{
		return std::make_shared<CMeshInformation_BaseMaterials>(nCurrentFaceCount);
	}

	void CMeshInformation_BaseMaterials::permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		// Nothing to do
	}

	nfUint32 CMeshInformation_BaseMaterials::getBackupSize()
	{
		return (nfUint32) sizeof (MESHINFORMATION_BASEMATERIAL);
	}

	void CMeshInformation_BaseMaterials::mergeInformationFrom(_In_ CMeshInformation * pInformation)
	{
		// nothing to merge
	}

	nfBool CMeshInformation_BaseMaterials::faceHasData(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATION_BASEMATERIAL * pFaceData = (MESHINFORMATION_BASEMATERIAL*)getFaceData(nFaceIndex);
		if (pFaceData)
			return (pFaceData->m_nMaterialGroupID != 0);

		return false;
	}

}


