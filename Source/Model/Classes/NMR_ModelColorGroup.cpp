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

NMR_ModelColorGroup.cpp implements the Model Color Group Resource Class.
A color group is an in memory representation of the 3MF color groups node.

--*/

#include "Model/Classes/NMR_ModelColorGroup.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>

namespace NMR {

	CModelColorGroupResource::CModelColorGroupResource(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource(sID, pModel)
	{
		m_nNextPropertyID = 1;
	}

	nfUint32 CModelColorGroupResource::addColor( _In_ nfColor cColor)
	{
		nfUint32 nID = m_nNextPropertyID;
		if (getCount() >= XML_3MF_MAXRESOURCEINDEX) {
			throw CNMRException(NMR_ERROR_TOOMANYCOLORS);
		}

		m_pColors.insert(std::make_pair(nID, cColor));

		m_nNextPropertyID++;

		clearResourceIndexMap();

		return nID;
	}

	nfUint32 CModelColorGroupResource::getCount()
	{
		return (nfUint32)m_pColors.size();
	}

	nfColor CModelColorGroupResource::getColor(_In_ ModelPropertyID nPropertyID)
	{
		auto iIterator = m_pColors.find(nPropertyID);
		if (iIterator != m_pColors.end()) {
			return iIterator->second;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelColorGroupResource::setColor(_In_ ModelPropertyID nPropertyID, _In_ nfColor cColor)
	{
		auto iIterator = m_pColors.find(nPropertyID);
		if (iIterator != m_pColors.end()) {
			iIterator->second = cColor;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelColorGroupResource::removeColor(_In_ ModelPropertyID nPropertyID)
	{
		m_pColors.erase(nPropertyID);
		clearResourceIndexMap();
	}

	void CModelColorGroupResource::mergeFrom(_In_ CModelColorGroupResource * pSourceColorGroup)
	{
		if (pSourceColorGroup == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
		nfUint32 nCount = pSourceColorGroup->getCount();

		pSourceColorGroup->buildResourceIndexMap();
		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			ModelPropertyID nPropertyID;
			pSourceColorGroup->mapResourceIndexToPropertyID(nIndex, nPropertyID);
			nfColor color = pSourceColorGroup->getColor(nPropertyID);
			addColor(color);
		}
		clearResourceIndexMap();
	}

	void CModelColorGroupResource::buildResourceIndexMap()
	{
		m_ResourceIndexMap.clear();
		m_ResourceIndexMap.reserve(m_pColors.size());

		auto iIterator = m_pColors.begin();
		while (iIterator != m_pColors.end()) {
			m_ResourceIndexMap.push_back(iIterator->first);
			iIterator++;
		}

		m_bHasResourceIndexMap = true;
	}

}

