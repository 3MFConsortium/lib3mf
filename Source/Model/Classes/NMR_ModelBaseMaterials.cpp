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

NMR_ModelBaseMaterials.cpp implements the Model Base Material Resource Class.
A base material is an in memory representation of the 3MF basematerials node.

--*/

#include "Model/Classes/NMR_ModelBaseMaterials.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h" 
#include <sstream>

namespace NMR {

	CModelBaseMaterialResource::CModelBaseMaterialResource(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelResource(sID, pModel)
	{
	}

	nfUint32 CModelBaseMaterialResource::addBaseMaterial(_In_ const std::string sName, _In_ nfColor cDisplayColor)
	{
		nfUint32 nNewIndex = getCount();
		if (nNewIndex >= XML_3MF_MAXRESOURCEINDEX) {
			throw CNMRException(NMR_ERROR_TOOMANYMATERIALS);
		}

		m_pMaterials.push_back(std::make_shared<CModelBaseMaterial> (sName, cDisplayColor));

		return nNewIndex;
	}

	nfUint32 CModelBaseMaterialResource::getCount()
	{
		return (nfUint32)m_pMaterials.size();
	}

	PModelBaseMaterial CModelBaseMaterialResource::getBaseMaterial(_In_ nfUint32 nIndex)
	{
		if (nIndex >= (nfUint32)m_pMaterials.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_pMaterials[nIndex];
	}

	void CModelBaseMaterialResource::removeMaterial(_In_ nfUint32 nIndex)
	{
		if (nIndex >= (nfUint32)m_pMaterials.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		m_pMaterials.erase(m_pMaterials.begin() + nIndex);
	}

	void CModelBaseMaterialResource::mergeFrom(_In_ CModelBaseMaterialResource * pSourceMaterial)
	{
		if (pSourceMaterial == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
		nfUint32 nCount = pSourceMaterial->getCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			PModelBaseMaterial pMaterial = pSourceMaterial->getBaseMaterial(nIndex);
			addBaseMaterial(pMaterial->getName(), pMaterial->getDisplayColor());
		}

	}

}

