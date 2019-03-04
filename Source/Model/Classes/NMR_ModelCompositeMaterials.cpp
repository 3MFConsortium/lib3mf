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

NMR_ModelCompositeMaterials.cpp implements the Model Composite Materials Resource Class.
A model composite materials resource is an in memory representation of the 3MF
composite materials resource object.

--*/

#include "Model/Classes/NMR_ModelCompositeMaterials.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>

namespace NMR {

	;
	CModelCompositeMaterialsResource::CModelCompositeMaterialsResource(_In_ const ModelResourceID sID, _In_ CModel * pModel,
		_In_ PModelBaseMaterialResource pBaseMaterialResource)
		: CModelResource(sID, pModel), m_pBaseMaterialResource(pBaseMaterialResource)
	{
		m_nNextPropertyID = 1;
		if (!pBaseMaterialResource.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	nfUint32 CModelCompositeMaterialsResource::addComposite(_In_ PModelComposite pComposite)
	{
		nfUint32 nID = m_nNextPropertyID;
		if (getCount() >= XML_3MF_MAXRESOURCEINDEX) {
			throw CNMRException(NMR_ERROR_TOOMANYCOMPOSITES);
		}
		
		// check for valid PropertyID
		for (auto constituent : *pComposite) {
			m_pBaseMaterialResource->getBaseMaterial(constituent.m_nPropertyID);
			if ((constituent.m_dMixingRatio < 0) || constituent.m_dMixingRatio > 1.0) {
				throw CNMRException(NMR_ERROR_MIXINGRATIO_OUTOFRANGE);
			}
		}
		

		m_pComposites.insert(std::make_pair(nID, pComposite));

		m_nNextPropertyID++;

		clearResourceIndexMap();

		return nID;
	}

	nfUint32 CModelCompositeMaterialsResource::getCount()
	{
		return (nfUint32)m_pComposites.size();
	}

	PModelComposite CModelCompositeMaterialsResource::getComposite(_In_ ModelPropertyID nPropertyID)
	{
		auto iIterator = m_pComposites.find(nPropertyID);
		if (iIterator != m_pComposites.end()) {
			return iIterator->second;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelCompositeMaterialsResource::setComposite(_In_ ModelPropertyID nPropertyID, _In_ PModelComposite pComposite)
	{
		auto iIterator = m_pComposites.find(nPropertyID);
		if (iIterator != m_pComposites.end()) {
			iIterator->second = pComposite;
		}
		else {
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		}
	}

	void CModelCompositeMaterialsResource::removeComposite(_In_ ModelPropertyID nPropertyID)
	{
		m_pComposites.erase(nPropertyID);
		clearResourceIndexMap();
	}

	void CModelCompositeMaterialsResource::mergeFrom(_In_ CModelCompositeMaterialsResource * pSourceCompositesMaterials)
	{
		if (pSourceCompositesMaterials == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
		nfUint32 nCount = pSourceCompositesMaterials->getCount();

		for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
			PModelComposite pCmposite = pSourceCompositesMaterials->getComposite(nIndex);
			addComposite(pCmposite);
		}
		clearResourceIndexMap();
	}

	void CModelCompositeMaterialsResource::buildResourceIndexMap()
	{
		m_ResourceIndexMap.clear();
		m_ResourceIndexMap.reserve(m_pComposites.size());

		auto iIterator = m_pComposites.begin();
		while (iIterator != m_pComposites.end()) {
			m_ResourceIndexMap.push_back(iIterator->first);
			iIterator++;
		}

		m_bHasResourceIndexMap = true;
	}

	PModelBaseMaterialResource CModelCompositeMaterialsResource::getBaseMaterialResource()
	{
		return m_pBaseMaterialResource;
	}
}

