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

NMR_MeshInformation_TexCoords.cpp implements the Texture Mesh Information Class.
This class specializes the abstract Mesh Information Class to "textures". It
implements functions to interpolate and reconstruct texture coordinates while
the mesh topology is changing.

--*/

#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"
#include "Common/NMR_Exception.h"
#include "Common/Math/NMR_Vector.h"
#include <cmath>

namespace NMR {

	CMeshInformation_TexCoords::CMeshInformation_TexCoords() : CMeshInformation()
	{
		m_pContainer = std::make_shared<CMeshInformationContainer>(0, (nfUint32) sizeof(MESHINFORMATION_TEXCOORDS));
	}

	CMeshInformation_TexCoords::CMeshInformation_TexCoords(nfUint32 nCurrentFaceCount)
	{
		nfUint32 nIdx;
		m_pContainer = std::make_shared<CMeshInformationContainer>(nCurrentFaceCount, (nfUint32) sizeof(MESHINFORMATION_TEXCOORDS));
		for (nIdx = 0; nIdx < nCurrentFaceCount; nIdx++)
			invalidateFace(m_pContainer->getFaceData(nIdx));
	}

	void CMeshInformation_TexCoords::invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData)
	{
		nfUint32 j;

		MESHINFORMATION_TEXCOORDS * pTexCoordData = (MESHINFORMATION_TEXCOORDS *)pData;
		if (pTexCoordData) {
			pTexCoordData->m_TextureID = 0;
			for (j = 0; j < 3; j++) {
				pTexCoordData->m_vCoords[j] = fnVEC2_make(0.0f, 0.0f);
			}
		}
	}

	eMeshInformationType CMeshInformation_TexCoords::getType()
	{
		return emiTexCoords;
	}

	void CMeshInformation_TexCoords::cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex)
	{
		nfUint32 j;

		__NMRASSERT(pOtherInformation);

		MESHINFORMATION_TEXCOORDS * pTargetFaceData = (MESHINFORMATION_TEXCOORDS*)getFaceData(nFaceIndex);
		MESHINFORMATION_TEXCOORDS * pSourceFaceData = (MESHINFORMATION_TEXCOORDS*)pOtherInformation->getFaceData(nOtherFaceIndex);

		if (pTargetFaceData && pSourceFaceData) {
			for (j = 0; j < 3; j++)
				pTargetFaceData->m_vCoords[j] = pSourceFaceData->m_vCoords[j];

			pTargetFaceData->m_TextureID = pSourceFaceData->m_TextureID;
		}
	}

	PMeshInformation CMeshInformation_TexCoords::cloneInstance(_In_ nfUint32 nCurrentFaceCount)
	{
		return std::make_shared<CMeshInformation_TexCoords>(nCurrentFaceCount);
	}

	void CMeshInformation_TexCoords::permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS*)getFaceData(nFaceIndex);
		if (pFaceData && (nNodeIndex1 < 3) && (nNodeIndex2 < 3) && (nNodeIndex3 < 3)) {
			MESHINFORMATION_TEXCOORDS FaceDataCopy = *pFaceData;

			pFaceData->m_vCoords[0] = FaceDataCopy.m_vCoords[nNodeIndex1];
			pFaceData->m_vCoords[1] = FaceDataCopy.m_vCoords[nNodeIndex2];
			pFaceData->m_vCoords[2] = FaceDataCopy.m_vCoords[nNodeIndex3];
		}
	}

	nfUint32 CMeshInformation_TexCoords::getBackupSize()
	{
		return (nfUint32) sizeof (MESHINFORMATION_TEXCOORDS);
	}

	void CMeshInformation_TexCoords::mergeInformationFrom(_In_ CMeshInformation * pInformation)
	{
		__NMRASSERT(pInformation);

		//CMeshInformation_TexCoords * pTexCoords = dynamic_cast<CMeshInformation_TexCoords*>(pInformation);
		//__NMRASSERT(pTexCoords);
	}

	nfBool CMeshInformation_TexCoords::faceHasData(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATION_TEXCOORDS * pFaceData = (MESHINFORMATION_TEXCOORDS*)getFaceData(nFaceIndex);
		if (pFaceData)
			return (pFaceData->m_TextureID != 0);

		return false;
	}

}
