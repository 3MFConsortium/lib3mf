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

NMR_MeshInformationHandler.cpp implements the Mesh Information Handler Class.
It allows to include different kinds of information in one mesh (like Textures AND colors).

--*/

#include "Common/MeshInformation/NMR_MeshInformationHandler.h" 
#include "Common/Math/NMR_Vector.h" 
#include "Common/NMR_Exception.h" 
#include <math.h>

namespace NMR {

	CMeshInformationHandler::CMeshInformationHandler()
	{
		nfInt32 eType;
		for (eType = emiAbstract; eType < emiLastType; eType++) {
			m_pLookup[eType] = NULL;
		}
	}

	void CMeshInformationHandler::addInformation(_In_ PMeshInformation pInformation)
	{
		__NMRASSERT(pInformation.get());

		eMeshInformationType eType = pInformation->getType();

		m_pInformations.push_back(pInformation);
		m_pLookup[eType] = pInformation.get();
	}

	void CMeshInformationHandler::addFace(_In_ nfUint32 nNewFaceCount)
	{
		std::vector<PMeshInformation>::iterator iter = m_pInformations.begin();

		while (iter != m_pInformations.end()) {
			MESHINFORMATIONFACEDATA * pData = (*iter)->addFaceData(nNewFaceCount);
			if (pData)
				(*iter)->invalidateFace(pData);

			iter++;
		}
	}

	CMeshInformation * CMeshInformationHandler::getInformationIndexed(_In_ nfUint32 nIdx)
	{
		if (nIdx >= (nfUint32)m_pInformations.size())
			throw CNMRException(NMR_ERROR_INVALIDMESHINFORMATIONINDEX);

		return m_pInformations[nIdx].get();
	}

	CMeshInformation * CMeshInformationHandler::getInformationByType(_In_ nfUint32 nChannel, _In_ eMeshInformationType eType)
	{
		return m_pLookup[eType];
	}

	nfUint32 CMeshInformationHandler::getInformationCount()
	{
		return (nfUint32)m_pInformations.size();
	}

	void CMeshInformationHandler::addInfoTableFrom(_In_ CMeshInformationHandler * pOtherInfoHandler, _In_ nfUint32 nCurrentFaceCount)
	{
		nfInt32 eType;
		for (eType = emiAbstract; eType < emiLastType; eType++) {
			if ((pOtherInfoHandler->m_pLookup[eType]) && (!m_pLookup[eType]))
				addInformation(pOtherInfoHandler->m_pLookup[eType]->cloneInstance(nCurrentFaceCount));
			if ((pOtherInfoHandler->m_pLookup[eType]) && (m_pLookup[eType]))
				m_pLookup[eType]->mergeInformationFrom(pOtherInfoHandler->m_pLookup[eType]);

		}
	}

	void CMeshInformationHandler::cloneFaceInfosFrom(_In_ nfUint32 nFaceIdx, _In_ CMeshInformationHandler * pOtherInfoHandler, _In_ nfUint32 nOtherFaceIndex)
	{
		nfInt32 eType;
		for (eType = emiAbstract; eType < emiLastType; eType++) {
			if ((pOtherInfoHandler->m_pLookup[eType]) && (m_pLookup[eType]))
				m_pLookup[eType]->cloneFaceInfosFrom(nFaceIdx, pOtherInfoHandler->m_pLookup[eType], nOtherFaceIndex);

		}
	}

	void CMeshInformationHandler::permuteNodeInformation(_In_ nfUint32 nFaceIdx, _In_ nfUint32 nNodeIndex1, _In_ nfUint32 nNodeIndex2, _In_ nfUint32 nNodeIndex3)
	{
		std::vector<PMeshInformation>::iterator iter = m_pInformations.begin();

		while (iter != m_pInformations.end()) {
			(*iter)->permuteNodeInformation(nFaceIdx, nNodeIndex1, nNodeIndex2, nNodeIndex3);
			iter++;
		}
	}

	void CMeshInformationHandler::resetFaceInformation(_In_ nfUint32 nFaceIdx)
	{
		std::vector<PMeshInformation>::iterator iter = m_pInformations.begin();

		while (iter != m_pInformations.end()) {
			(*iter)->resetFaceInformation(nFaceIdx);
			iter++;
		}
	}

	void CMeshInformationHandler::removeInformation(eMeshInformationType eType)
	{
		nfUint32 nCount = (nfUint32)m_pInformations.size();
		nfUint32 nIdx;
		nfUint32 nNewIdx = 0;

		m_pLookup[eType] = NULL;

		for (nIdx = 0; nIdx < nCount; nIdx++) {
			eMeshInformationType eCurrentType = m_pInformations[nIdx]->getType();
			if (eCurrentType != eType) {
				m_pInformations[nNewIdx] = m_pInformations[nIdx];
				nNewIdx++;
			}
		}

		if (nNewIdx < nCount) {
			for (nIdx = nNewIdx; nIdx < nCount; nIdx++)
				m_pInformations.pop_back();
		}
	}

}
