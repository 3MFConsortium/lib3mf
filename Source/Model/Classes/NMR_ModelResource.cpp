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

NMR_ModelResource.cpp implements the Model Resource Class.
A model resource is an in memory representation of the 3MF
resource object.

--*/

#include "Model/Classes/NMR_Model.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Common/NMR_Exception.h" 

namespace NMR {

	CModelResource::CModelResource(_In_ const ModelResourceID sResourceID, _In_ CModel * pModel)
	{
		if (!pModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pModel = pModel;
		m_pPackageResourceID = m_pModel->generatePackageResourceID(pModel->currentPath(), sResourceID);
		m_bHasResourceIndexMap = false;
	}

	CModelResource::~CModelResource()
	{
		m_pModel = NULL;
	}

	CModel * CModelResource::Model()
	{
		return m_pModel;
	}

	PPackageResourceID CModelResource::getPackageResourceID()
	{
		return m_pPackageResourceID;
	}

	void CModelResource::setPackageResourceID(PPackageResourceID pID)
	{
		m_pModel->updateUniqueResourceID(m_pPackageResourceID->getUniqueID(), pID->getUniqueID());
		m_pPackageResourceID = pID;
	}

	_Ret_notnull_ CModel * CModelResource::getModel()
	{
		return m_pModel;
	}


	void CModelResource::clearResourceIndexMap()
	{
		m_ResourceIndexMap.clear();
		m_bHasResourceIndexMap = false;
	}

	void CModelResource::buildResourceIndexMap()
	{
		clearResourceIndexMap();
		m_bHasResourceIndexMap = true;
	}

	nfBool CModelResource::hasResourceIndexMap()
	{
		return m_bHasResourceIndexMap;
	}

	bool CModelResource::mapResourceIndexToPropertyID(_In_ ModelResourceIndex nPropertyIndex, _Out_ ModelPropertyID & nPropertyID)
	{
		if (nPropertyIndex < m_ResourceIndexMap.size()) {
			nPropertyID = m_ResourceIndexMap[nPropertyIndex];
			return true;
		}

		nPropertyID = 0;
		return false;
	}

}
