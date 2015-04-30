/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for Model Component Class

--*/

#include "Model/COM/NMR_COMInterface_ModelComponent.h" 
#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h"
#include "Model/Classes/NMR_ModelComponent.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_Platform.h" 

namespace NMR {

	CCOMModelComponent::CCOMModelComponent()
	{
		// empty on purpose
	}

	void CCOMModelComponent::setComponent(_In_ PModelComponent pModelComponent)
	{
		m_pModelComponent = pModelComponent;
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetObjectResource(_Outptr_ ILib3MFModelObjectResource ** ppObjectResource)
	{
		if (!ppObjectResource)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			// Retrieve Object of Build Item
			CModelObject * pObject = m_pModelComponent->getObject();
			if (!pObject)
				throw CNMRException(NMR_ERROR_INVALIDOBJECT);

			// Retrieve Model of Build Item
			CModel * pModel = pObject->getModel();
			if (!pModel)
				throw CNMRException(NMR_ERROR_INVALIDMODEL);

			// Retrieve corresponding Model Resource
			PModelResource pResource = pModel->findResource(pObject->getResourceID());
			if (!pResource.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);

			// Resource interface
			ILib3MFModelObjectResource * pResourceInterface = nullptr;

			// Return Mesh COM Object
			fnCreateModelObjectResourceFromClass(pResource, false, &pResourceInterface);

			// We have not found a suitable object class to return..
			if (pResourceInterface == nullptr)
				return LIB3MF_FAIL;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetTransform(_Out_ MODELTRANSFORM * pTransformation)
	{
		if (!pTransformation)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			NMATRIX3 mMatrix = m_pModelComponent->getTransform();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					pTransformation->m_fFields[i][j] = mMatrix.m_fields[i][j];
				}
			}

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::SetTransform(_In_opt_ MODELTRANSFORM * pTransformation)
	{
		if (!pTransformation)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			NMATRIX3 mMatrix = fnMATRIX3_identity();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					mMatrix.m_fields[i][j] = pTransformation->m_fFields[i][j];
				}
			}

			m_pModelComponent->setTransform(mMatrix);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetObjectResourceID(_Out_ DWORD * pnResourceID)
	{
		if (!pnResourceID)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			*pnResourceID = m_pModelComponent->getObjectID();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::HasTransform(_Out_ BOOL * pbHasTransform)
	{
		if (!pbHasTransform)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			*pbHasTransform = m_pModelComponent->hasTransform();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

}
