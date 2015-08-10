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
#include "Model/COM/NMR_COMInterface_ModelDefaultPropertyHandler.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelComponentsObject::CCOMModelComponentsObject()
	{
		m_nErrorCode = NMR_SUCCESS;
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

	LIB3MFRESULT CCOMModelComponentsObject::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelComponentsObject::handleNMRException(_In_ CNMRException * pException)
	{
		__NMRASSERT(pException);

		m_nErrorCode = pException->getErrorCode();
		m_sErrorMessage = std::string(pException->what());

		CNMRException_Windows * pWinException = dynamic_cast<CNMRException_Windows *> (pException);
		if (pWinException != nullptr) {
			return pWinException->getHResult();
		}
		else {
			if (m_nErrorCode == NMR_ERROR_INVALIDPOINTER)
				return LIB3MF_POINTER;
			if (m_nErrorCode == NMR_ERROR_INVALIDPARAM)
				return LIB3MF_INVALIDARG;

			return LIB3MF_FAIL;
		}
	}

	LIB3MFRESULT CCOMModelComponentsObject::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
	{
		if (!pErrorCode)
			return LIB3MF_POINTER;

		*pErrorCode = m_nErrorCode;
		if (pErrorMessage) {
			if (m_nErrorCode != NMR_SUCCESS) {
				*pErrorMessage = m_sErrorMessage.c_str();
			}
			else {
				*pErrorMessage = nullptr;
			}
		}

		return LIB3MF_OK;
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::AddComponent(_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ ILib3MFModelComponent ** ppComponent)
	{

		try {
			if (!pObject)
				throw CNMRException (NMR_ERROR_INVALIDPOINTER);

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
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

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

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetComponent(_In_ DWORD nIndex, _Outptr_ ILib3MFModelComponent ** ppComponent)
	{
		try {
			if (!ppComponent)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pComponentsObject = getComponentsObject();
			PModelComponent pModelComponent = pComponentsObject->getComponent(nIndex);

			CCOMObject<CCOMModelComponent> * pResult = new CCOMObject<CCOMModelComponent>();
			pResult->setComponent(pModelComponent);
			*ppComponent = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetComponentCount(_Out_ DWORD * pComponentCount)
	{
		try {
			if (!pComponentCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pComponentsObject = getComponentsObject();
			*pComponentCount = pComponentsObject->getComponentCount();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetResourceID(_Out_ DWORD * pnResourceID)
	{
		try {
			if (!pnResourceID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pComponentsObject = getComponentsObject();
			*pnResourceID = pComponentsObject->getResourceID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetType(_Out_ DWORD * pObjectType)
	{
		try {
			if (pObjectType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			*pObjectType = (DWORD)pObject->getObjectType();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetType(_In_ DWORD ObjectType)
	{
		try {
			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			pObject->setObjectType((eModelObjectType)ObjectType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		try {
			if (!pwszPartNumber)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			std::wstring sPartNumber(pwszPartNumber);
			pObject->setPartNumber(pwszPartNumber);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getPartNumber(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::SetName(_In_z_ LPCWSTR pwszName)
	{
		try {
			if (!pwszName)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			std::wstring sName(pwszName);
			pObject->setName(pwszName);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::GetName(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pObject->getName(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::IsMeshObject(_Out_ BOOL * pbIsMeshObject)
	{
		try {
			if (!pbIsMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pbIsMeshObject = false;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::IsComponentsObject(_Out_ BOOL * pbIsComponentsObject)
	{
		try {
			if (!pbIsComponentsObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pbIsComponentsObject = true;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::IsValidObject(_Out_ BOOL * pbIsValid)
	{
		try {
			if (!pbIsValid)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			*pbIsValid = pObject->isValid();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::CreateDefaultPropertyHandler (_Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		try {
			if (!ppPropertyHandler)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			CCOMObject<CCOMModelDefaultPropertyHandler> * pNewPropertyHandler = new CCOMObject<CCOMModelDefaultPropertyHandler>();
			pNewPropertyHandler->setChannel(0);
			pNewPropertyHandler->setResource(m_pResource);
			*ppPropertyHandler = pNewPropertyHandler;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponentsObject::CreateDefaultMultiPropertyHandler(_In_ DWORD nChannel, _Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler)
	{
		try {
			if (!ppPropertyHandler)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelComponentsObject * pObject = getComponentsObject();
			__NMRASSERT(pObject);

			CCOMObject<CCOMModelDefaultPropertyHandler> * pNewPropertyHandler = new CCOMObject<CCOMModelDefaultPropertyHandler>();
			pNewPropertyHandler->setChannel(nChannel);
			pNewPropertyHandler->setResource(m_pResource);
			*ppPropertyHandler = pNewPropertyHandler;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


}
