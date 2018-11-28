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

COM Interface Implementation for Model Component Class

--*/

#include "Model/COM/NMR_COMInterface_ModelComponent.h"
#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h"
#include "Model/Classes/NMR_ModelComponent.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelComponent::CCOMModelComponent()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelComponent::setComponent(_In_ PModelComponent pModelComponent)
	{
		m_pModelComponent = pModelComponent;
	}

	LIB3MFRESULT CCOMModelComponent::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelComponent::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelComponent::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelComponent::GetObjectResource(_Outptr_ ILib3MFModelObjectResource ** ppObjectResource)
	{
		try {
			if (!ppObjectResource)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			// Return result
			*ppObjectResource = pResourceInterface;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetUUIDUTF8(_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer)
	{
		try {
			if ((!pbHasUUID) || (!pszBuffer))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			PUUID pUUID = m_pModelComponent->uuid();
			*pbHasUUID = (pUUID.get() != nullptr);
			nfUint32 nNeededChars = 0;
			if (*pbHasUUID)
				fnStringToBufferSafe(pUUID->toString(), pszBuffer, 37, &nNeededChars);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::SetUUIDUTF8(_In_ LPCSTR pszUUID)
	{
		try
		{
			if (!pszUUID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			PUUID pUUID = std::make_shared<CUUID>(pszUUID);
			m_pModelComponent->setUUID(pUUID);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetTransform(_Out_ MODELTRANSFORM * pTransformation)
	{
		try {
			if (!pTransformation)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			NMATRIX3 mMatrix = m_pModelComponent->getTransform();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					pTransformation->m_fFields[i][j] = mMatrix.m_fields[i][j];
				}
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

	LIB3MFMETHODIMP CCOMModelComponent::SetTransform(_In_opt_ MODELTRANSFORM * pTransformation)
	{
		try {
			if (!pTransformation)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

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

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::GetObjectResourceID(_Out_ DWORD * pnResourceID)
	{
		try {
			if (!pnResourceID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			*pnResourceID = m_pModelComponent->getObjectID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelComponent::HasTransform(_Out_ BOOL * pbHasTransform)
	{
		try {
			if (!pbHasTransform)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelComponent.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELCOMPONENT);

			*pbHasTransform = m_pModelComponent->hasTransform();

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
