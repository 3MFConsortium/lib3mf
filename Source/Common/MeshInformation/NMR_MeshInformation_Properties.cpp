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

NMR_MeshInformation_Properties.cpp implements the Texture Mesh Information Class.

--*/

#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"
#include "Common/NMR_Exception.h"
#include "Common/Math/NMR_Vector.h"
#include <cmath>

namespace NMR {


	CMeshInformation_PropertyIndexMapping::CMeshInformation_PropertyIndexMapping()
	{
	}

	nfUint32 CMeshInformation_PropertyIndexMapping::registerPropertyID(UniqueResourceID nUniqueResourceID, ModelPropertyID nPropertyID, nfUint32 nResourceIndex)
	{
		if (nUniqueResourceID == 0)
			throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);

		m_IDMap.insert(std::make_pair(std::make_pair(nUniqueResourceID, nPropertyID), nResourceIndex));

		return nResourceIndex;
	}


	nfUint32 CMeshInformation_PropertyIndexMapping::mapPropertyIDToIndex(UniqueResourceID nUniqueResourceID, ModelPropertyID nPropertyID)
	{
		if (nUniqueResourceID == 0)
			throw CNMRException(NMR_ERROR_INVALIDPROPERTYRESOURCEID);

		auto iIterator = m_IDMap.find(std::make_pair(nUniqueResourceID, nPropertyID));
		if (iIterator == m_IDMap.end())
			throw CNMRException(NMR_ERROR_PROPERTYIDNOTFOUND);

		return iIterator->second;
	}


/*	void CMeshInformation_PropertyIndexMapping::findDefaultProperties()
	{
		nfUint32 nMaxCount = 0;
		auto iIterator = m_IDMap.begin();
		while (iIterator != m_IDMap.end()) {
			nfUint32 nCurrentCount = iIterator->second.second;
			if (nCurrentCount > nMaxCount) {
				m_nDefaultResourceID = iIterator->first.first;
				m_nDefaultResourceIndex = iIterator->second.first;

				nMaxCount = nCurrentCount;
			}

			iIterator++;
		}
	} */


	CMeshInformation_Properties::CMeshInformation_Properties() : CMeshInformation()
	{
		m_pContainer = std::make_shared<CMeshInformationContainer>(0, (nfUint32) sizeof(MESHINFORMATION_PROPERTIES));
	}

	CMeshInformation_Properties::CMeshInformation_Properties(nfUint32 nCurrentFaceCount)
	{
		nfUint32 nIdx;
		m_pContainer = std::make_shared<CMeshInformationContainer>(nCurrentFaceCount, (nfUint32) sizeof(MESHINFORMATION_PROPERTIES));
		for (nIdx = 0; nIdx < nCurrentFaceCount; nIdx++)
			invalidateFace(m_pContainer->getFaceData(nIdx));
	}

	void CMeshInformation_Properties::invalidateFace(_In_ MESHINFORMATIONFACEDATA * pData)
	{
		nfUint32 j;

		MESHINFORMATION_PROPERTIES * pProperties = (MESHINFORMATION_PROPERTIES *)pData;
		if (pProperties) {
			pProperties->m_nUniqueResourceID = 0;
			for (j = 0; j < 3; j++) {
				pProperties->m_nPropertyIDs[j] = 0;
			}
		}
	}

	eMeshInformationType CMeshInformation_Properties::getType()
	{
		return emiProperties;
	}

	void CMeshInformation_Properties::cloneDefaultInfosFrom(_In_ CMeshInformation * pOtherInformation)
	{
		MESHINFORMATION_PROPERTIES * pTargetDefaultData = (MESHINFORMATION_PROPERTIES*)getDefaultData();
		if (!pTargetDefaultData) {
			setDefaultData((MESHINFORMATIONFACEDATA*)new MESHINFORMATION_PROPERTIES);
			pTargetDefaultData = (MESHINFORMATION_PROPERTIES*)getDefaultData();
		}
		MESHINFORMATION_PROPERTIES * pSourceDefaultData = (MESHINFORMATION_PROPERTIES*)pOtherInformation->getDefaultData();
		if (pTargetDefaultData && pSourceDefaultData) {
			for (nfUint32 j = 0; j < 3; j++)
				pTargetDefaultData->m_nPropertyIDs[j] = pSourceDefaultData->m_nPropertyIDs[j];

			pTargetDefaultData->m_nUniqueResourceID = pSourceDefaultData->m_nUniqueResourceID;
		}
	}
	

	void CMeshInformation_Properties::cloneFaceInfosFrom(_In_ nfUint32 nFaceIndex, _In_ CMeshInformation * pOtherInformation, _In_ nfUint32 nOtherFaceIndex)
	{
		__NMRASSERT(pOtherInformation);

		MESHINFORMATION_PROPERTIES * pTargetFaceData = (MESHINFORMATION_PROPERTIES*)getFaceData(nFaceIndex);
		MESHINFORMATION_PROPERTIES * pSourceFaceData = (MESHINFORMATION_PROPERTIES*)pOtherInformation->getFaceData(nOtherFaceIndex);

		if (pTargetFaceData && pSourceFaceData) {
			for (nfUint32 j = 0; j < 3; j++)
				pTargetFaceData->m_nPropertyIDs[j] = pSourceFaceData->m_nPropertyIDs[j];

			pTargetFaceData->m_nUniqueResourceID = pSourceFaceData->m_nUniqueResourceID;
		}
	}

	PMeshInformation CMeshInformation_Properties::cloneInstance(_In_ nfUint32 nCurrentFaceCount)
	{
		return std::make_shared<CMeshInformation_Properties>(nCurrentFaceCount);
	}

	void CMeshInformation_Properties::permuteNodeInformation(_In_ nfUint32 nFaceIndex, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		MESHINFORMATION_PROPERTIES * pFaceData = (MESHINFORMATION_PROPERTIES*)getFaceData(nFaceIndex);
		if (pFaceData && (nNodeIndex1 < 3) && (nNodeIndex2 < 3) && (nNodeIndex3 < 3)) {
			MESHINFORMATION_PROPERTIES FaceDataCopy = *pFaceData;

			pFaceData->m_nPropertyIDs[0] = FaceDataCopy.m_nPropertyIDs[nNodeIndex1];
			pFaceData->m_nPropertyIDs[1] = FaceDataCopy.m_nPropertyIDs[nNodeIndex2];
			pFaceData->m_nPropertyIDs[2] = FaceDataCopy.m_nPropertyIDs[nNodeIndex3];
		}
	}

	nfUint32 CMeshInformation_Properties::getBackupSize()
	{
		return (nfUint32) sizeof (MESHINFORMATION_PROPERTIES);
	}

	void CMeshInformation_Properties::mergeInformationFrom(_In_ CMeshInformation * pInformation)
	{
		__NMRASSERT(pInformation);

	}

	nfBool CMeshInformation_Properties::faceHasData(_In_ nfUint32 nFaceIndex)
	{
		MESHINFORMATION_PROPERTIES * pFaceData = (MESHINFORMATION_PROPERTIES*)getFaceData(nFaceIndex);
		if (pFaceData)
			return (pFaceData->m_nUniqueResourceID != 0);

		return false;
	}

	void CMeshInformation_Properties::setDefaultData(MESHINFORMATIONFACEDATA* pData)
	{
		MESHINFORMATION_PROPERTIES * pProperties = (MESHINFORMATION_PROPERTIES *)pData;

		m_pDefaultProperty.reset(pProperties);
	}

	MESHINFORMATIONFACEDATA* CMeshInformation_Properties::getDefaultData()
	{
		return (MESHINFORMATIONFACEDATA*)m_pDefaultProperty.get();
	}
}
