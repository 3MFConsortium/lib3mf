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

NMR_ModelComponent.cpp implements the Model Component Class.
A model component is an in memory representation of the 3MF component.

--*/

#include "Model/Classes/NMR_ModelComponent.h" 
#include "Common/NMR_Exception.h" 

namespace NMR {

	CModelComponent::CModelComponent(_In_ CModelObject * pObject)
	{
		if (!pObject)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pObject = pObject;
		m_mTransform = fnMATRIX3_identity();

		m_UUID = std::make_shared<CUUID>();
	}

	CModelComponent::CModelComponent(_In_ CModelObject * pObject, _In_ const NMATRIX3 mTransform)
	{
		if (!pObject)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pObject = pObject;
		m_mTransform = mTransform;

		m_UUID = std::make_shared<CUUID>();
	}

	CModelComponent::~CModelComponent()
	{
		m_pObject = NULL;
	}

	CModelObject * CModelComponent::getObject()
	{
		return m_pObject;
	}

	CModel* CModelComponent::getModel()
	{
		return m_pObject->getModel();
	}

	NMATRIX3 CModelComponent::getTransform()
	{
		return m_mTransform;
	}

	void CModelComponent::setTransform(_In_ const NMATRIX3 mTransform)
	{
		m_mTransform = mTransform;
	}

	nfBool CModelComponent::hasTransform()
	{
		return (!fnMATRIX3_isIdentity(m_mTransform));
	}

	std::string CModelComponent::getTransformString()
	{
		return fnMATRIX3_toString(m_mTransform);
	}

	PUUID CModelComponent::uuid()
	{
		return m_UUID;
	}

	void CModelComponent::setUUID(PUUID uuid)
	{
		getObject()->getModel()->registerUUID(uuid);
		getObject()->getModel()->unRegisterUUID(m_UUID);
		m_UUID = uuid;
	}

	void CModelComponent::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		__NMRASSERT(pMesh);
		NMATRIX3 mLocalMatrix = fnMATRIX3_multiply(mMatrix, m_mTransform);
		m_pObject->mergeToMesh(pMesh, mLocalMatrix);
	}

}
