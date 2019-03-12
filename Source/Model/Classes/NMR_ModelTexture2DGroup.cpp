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

NMR_ModelTexture2DGroup.cpp implements the Model Color Group Resource Class.
A color group is an in memory representation of the 3MF color groups node.

--*/

#include "Model/Classes/NMR_ModelTexture2DGroup.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>

namespace NMR {

	CModelTexture2DGroupResource::CModelTexture2DGroupResource(_In_ const ModelResourceID sID,
		_In_ CModel * pModel, _In_ PModelTexture2DResource pTexture2D)
		: CModelResource(sID, pModel)
	{
		m_nNextPropertyID = 1;
		if (!pTexture2D.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pTexture2D = pTexture2D;
	}

	nfUint32 CModelTexture2DGroupResource::addUVCoordinate(_In_ MODELTEXTURE2DCOORDINATE UV)
	{
		nfUint32 nID = m_nNextPropertyID;
		if (getCount() >= XML_3MF_MAXRESOURCEINDEX) {
			throw CNMRException(NMR_ERROR_TOOMANYCOLORS);
		}

		m_pUVCoordinates.insert(std::make_pair(nID, UV));

		m_nNextPropertyID++;

		clearResourceIndexMap();

		return nID;
	}

	nfUint32 CModelTexture2DGroupResource::getCount()
	{
		return (nfUint32)m_pUVCoordinates.size();
	}

	void CModelTexture2DGroupResource::setUVCoordinate(_In_ ModelPropertyID nPropertyID, _In_ MODELTEXTURE2DCOORDINATE sCoordinate)
	{
		auto iIterator = m_pUVCoordinates.find(nPropertyID);
		if (iIterator != m_pUVCoordinates.end()) {
			iIterator->second = sCoordinate;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelTexture2DGroupResource::removePropertyID(_In_ ModelPropertyID nPropertyID)
	{
		m_pUVCoordinates.erase(nPropertyID);
		clearResourceIndexMap();
	}

	MODELTEXTURE2DCOORDINATE CModelTexture2DGroupResource::getUVCoordinate(_In_ ModelPropertyID nPropertyID)
	{
		auto iIterator = m_pUVCoordinates.find(nPropertyID);
		if (iIterator != m_pUVCoordinates.end()) {
			return iIterator->second;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelTexture2DGroupResource::mergeFrom(_In_ CModelTexture2DGroupResource * pSourceTexture2DGroup)
	{
		if (pSourceTexture2DGroup == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
		nfUint32 nCount = pSourceTexture2DGroup->getCount();
		pSourceTexture2DGroup->buildResourceIndexMap();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			ModelPropertyID nPropertyID;
			pSourceTexture2DGroup->mapResourceIndexToPropertyID(nIndex, nPropertyID);
			MODELTEXTURE2DCOORDINATE UVCoordinate = pSourceTexture2DGroup->getUVCoordinate(nPropertyID);
			addUVCoordinate(UVCoordinate);
		}
		clearResourceIndexMap();
	}

	void CModelTexture2DGroupResource::buildResourceIndexMap()
	{
		m_ResourceIndexMap.clear();
		m_ResourceIndexMap.reserve(m_pUVCoordinates.size());

		auto iIterator = m_pUVCoordinates.begin();
		while (iIterator != m_pUVCoordinates.end()) {
			m_ResourceIndexMap.push_back(iIterator->first);
			iIterator++;
		}

		m_bHasResourceIndexMap = true;
	}


	PModelTexture2DResource CModelTexture2DGroupResource::getTexture2D()
	{
		return m_pTexture2D;
	}
}

