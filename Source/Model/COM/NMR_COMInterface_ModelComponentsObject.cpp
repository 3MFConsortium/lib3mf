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

COM Interface Implementation for Model Component Object Classes

--*/

#include "Model/COM/NMR_COMInterface_ModelComponentsObject.h"
#include "Model/COM/NMR_COMInterface_ModelComponent.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelComponentsObject::CCOMModelComponentsObject()
	{
		// empty on purpose
	}

	_Ret_notnull_ CModelComponentsObject * CCOMModelComponentsObject::getComponentsObject()
	{
		if (m_pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		CModelComponentsObject * pComponentsObject = dynamic_cast<CModelComponentsObject *> (m_pResource.get());
		if (pComponentsObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDCOMPONENT);

		return pComponentsObject;
	}

	void CCOMModelComponentsObject::setResource(_In_ PModelResource pModelResource)
	{
		m_pResource = pModelResource;
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::AddComponent(_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ ILib3MFModelComponent ** ppComponent)
	{
		if (!pObject)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pComponentsObject = getComponentsObject();
			CModel * pModel = pComponentsObject->getModel();
			__NMRASSERT(pModel);

			// Get Resource ID
			ModelResourceID nObjectID;
			HRESULT hResult = pObject->GetResourceID(&nObjectID);
			if (hResult != LIB3MF_OK)
				return hResult;

			// Find class instance
			CModelObject * pObject = pModel->findObject(nObjectID);
			if (pObject == nullptr)
				return LIB3MF_FAIL;

			// Convert Transform, if given
			NMATRIX3 mMatrix = fnMATRIX3_identity();
			if (pmTransform) {
				int i, j;

				for (i = 0; i < 3; i++) {
					for (j = 0; j < 4; j++) {
						mMatrix.m_fields[i][j] = pmTransform->m_fFields[i][j];
					}
				}
			}

			// Create Model component
			PModelComponent pNewComponent = std::make_shared<CModelComponent>(pObject, mMatrix);
			pComponentsObject->addComponent(pNewComponent);

			// Create Component COM Wrapper
			if (ppComponent) {
				CCOMObject<CCOMModelComponent> * pResult = new CCOMObject<CCOMModelComponent>();
				pResult->setComponent(pNewComponent);
				*ppComponent = pResult;
			}

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetComponent(_In_ DWORD nIndex, _Outptr_ ILib3MFModelComponent ** ppComponent)
	{
		if (!ppComponent)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pComponentsObject = getComponentsObject();
			PModelComponent pModelComponent = pComponentsObject->getComponent(nIndex);

			CCOMObject<CCOMModelComponent> * pResult = new CCOMObject<CCOMModelComponent>();
			pResult->setComponent(pModelComponent);
			*ppComponent = pResult;

			return LIB3MF_OK;

		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetComponentCount(_Out_ DWORD * pComponentCount)
	{
		if (!pComponentCount)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pComponentsObject = getComponentsObject();
			*pComponentCount = pComponentsObject->getComponentCount();

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetResourceID(_Out_ DWORD * pnResourceID)
	{
		if (!pnResourceID)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pComponentsObject = getComponentsObject();
			*pnResourceID = pComponentsObject->getResourceID();

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetType(_Out_ DWORD * pObjectType)
	{
		if (pObjectType == nullptr)
			return LIB3MF_POINTER;

		if (m_pResource.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			*pObjectType = (DWORD)pObject->getObjectType();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetType(_In_ DWORD ObjectType)
	{
		if (m_pResource.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			pObject->setObjectType((eModelObjectType)ObjectType);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		if (!pwszPartNumber)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			std::wstring sPartNumber(pwszPartNumber);
			pObject->setPartNumber(pwszPartNumber);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getPartNumber(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetName(_In_z_ LPCWSTR pwszName)
	{
		if (!pwszName)
			return LIB3MF_POINTER;

		try {
			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			std::wstring sName(pwszName);
			pObject->setName(pwszName);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetName(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getName(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

}
