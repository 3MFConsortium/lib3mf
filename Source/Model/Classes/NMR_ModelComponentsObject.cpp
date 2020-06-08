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

NMR_ModelComponentsObject.cpp implements the Model Component Class.
A model component object is an in memory representation of the 3MF
component object.

--*/

#include "Model/Classes/NMR_ModelComponentsObject.h" 
#include "Common/NMR_Exception.h"

namespace NMR {
	
	CModelComponentsObject::CModelComponentsObject(_In_ const ModelResourceID sID, _In_ CModel * pModel)
		: CModelObject(sID, pModel)
	{
		// empty on purpose
	}

	CModelComponentsObject::~CModelComponentsObject()
	{
		m_Components.clear();
	}

	void CModelComponentsObject::addComponent(_In_ PModelComponent pComponent)
	{
		if (!pComponent)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		CModel * pModel = getModel();
		CModelObject * pModelObject = pComponent->getObject();

		if (pModel != pModelObject->getModel())
			throw CNMRException(NMR_ERROR_MODELMISMATCH);

		if (pComponent->getObject()->getPackageResourceID() == this->getPackageResourceID() )
			throw CNMRException(NMR_ERROR_MODELMISMATCH);

		m_Components.push_back(pComponent);
	}

	nfUint32 CModelComponentsObject::getComponentCount()
	{
		return (nfUint32)m_Components.size();
	}

	PModelComponent CModelComponentsObject::getComponent(_In_ nfUint32 nIdx)
	{
		if (nIdx >= (nfUint32)m_Components.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		
		return m_Components[nIdx];
	}

	void CModelComponentsObject::mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix)
	{
		__NMRASSERT(pMesh);
		for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++)
			(*iIterator)->mergeToMesh(pMesh, mMatrix);
	}

	nfBool CModelComponentsObject::isValid()
	{
		if (m_Components.size() == 0)
			return false;

		for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++) {
			CModelObject * pObject = (*iIterator)->getObject();
			__NMRASSERT(pObject);

			if (!pObject->isValid())
				return false;
		}

		return true;
	}

	nfBool CModelComponentsObject::hasSlices(nfBool bRecursive)
	{
		if (bRecursive) {
			if (this->getSliceStack().get())
				return true;
			for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++) {
				CModelObject * pObject = (*iIterator)->getObject();
				__NMRASSERT(pObject);
				if (pObject->hasSlices(bRecursive))
					return true;
			}
			return false;
		}
		else {
			return (this->getSliceStack().get() != nullptr);
		}
	}

	nfBool CModelComponentsObject::isValidForSlices(const NMATRIX3& totalParentMatrix)
	{
		if (this->getSliceStack().get()) {
			if (!fnMATRIX3_isplanar(totalParentMatrix)) {
				return false;
			}
		}

		for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++) {
			CModelObject * pObject = (*iIterator)->getObject();
			__NMRASSERT(pObject);
			NMATRIX3 curMat = fnMATRIX3_multiply(totalParentMatrix, (*iIterator)->getTransform());
			if (!pObject->isValidForSlices(curMat))
				return false;
		}

		return true;
	}

	void CModelComponentsObject::calculateComponentDepthLevel(nfUint32 nLevel)
	{
		CModelObject::calculateComponentDepthLevel(nLevel);
		for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++) {
			CModelObject * pObject = (*iIterator)->getObject();
			pObject->calculateComponentDepthLevel(nLevel + 1);
		}
	}

	void CModelComponentsObject::extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix)
	{
		for (auto iIterator = m_Components.begin(); iIterator != m_Components.end(); iIterator++) {
			(*iIterator)->getObject()->extendOutbox(vOutBox, fnMATRIX3_multiply(mAccumulatedMatrix, (*iIterator)->getTransform()));
		}
	}
}
