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

COM Interface Implementation for Model Build Item Classes

--*/

#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Model/COM/NMR_COMInterface_ModelBuildItem.h"
#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelBuildItem::CCOMModelBuildItem()
	{
		m_pModelBuildItem = nullptr;
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelBuildItem::setItem(_In_ PModelBuildItem pModelBuildItem)
	{
		m_pModelBuildItem = pModelBuildItem;
	}

	LIB3MFRESULT CCOMModelBuildItem::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelBuildItem::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelBuildItem::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelBuildItem::HasObjectTransform(_Out_ BOOL * pbHasTransform)
	{
		try {
			if (!pbHasTransform)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			*pbHasTransform = m_pModelBuildItem->hasTransform();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectResourceID(_Out_ DWORD * pnID)
	{
		try {
			if (!pnID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			*pnID = m_pModelBuildItem->getObjectID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}
	
	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectResource(_Outptr_ ILib3MFModelObjectResource ** ppObject)
	{
		try {
			if (!ppObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			// Retrieve Object of Build Item
			CModelObject * pObject = m_pModelBuildItem->getObject();
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
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			// Return result
			*ppObject = pResourceInterface;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetUUIDUTF8(_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer)
	{
		try {
			if ( (!pbHasUUID) || (!pszBuffer) )
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			PUUID pUUID = m_pModelBuildItem->uuid();
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

	LIB3MFMETHODIMP CCOMModelBuildItem::SetUUIDUTF8(_In_ LPCSTR pszUUID)
	{
		try
		{
			if (!pszUUID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			PUUID pUUID = std::make_shared<CUUID>(pszUUID);
			m_pModelBuildItem->setUUID(pUUID);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectTransform(_Out_ MODELTRANSFORM * pmTransform)
	{
		try {
			if (!pmTransform)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			NMATRIX3 mMatrix = m_pModelBuildItem->getTransform();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					pmTransform->m_fFields[i][j] = mMatrix.m_fields[i][j];
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

	LIB3MFMETHODIMP CCOMModelBuildItem::SetObjectTransform(_In_ MODELTRANSFORM * pmTransform)
	{
		try {
			if (!pmTransform)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			NMATRIX3 mMatrix = fnMATRIX3_identity ();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					mMatrix.m_fields[i][j] = pmTransform->m_fFields[i][j];
				}
			}

			m_pModelBuildItem->setTransform(mMatrix);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		try	{
			if (!pwszPartNumber)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			m_pModelBuildItem->setPartNumber(fnUTF16toUTF8(pwszPartNumber));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::SetPartNumberUTF8(_In_z_ LPCSTR pszPartNumber)
	{
		try {
			if (!pszPartNumber)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			m_pModelBuildItem->setPartNumber(pszPartNumber);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModelBuildItem::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try	{
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(m_pModelBuildItem->getPartNumber()), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelBuildItem::GetPartNumberUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;

			fnStringToBufferSafe(m_pModelBuildItem->getPartNumber(), pszBuffer, cbBufferSize, &nNeededChars);

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


	LIB3MFMETHODIMP CCOMModelBuildItem::GetHandle(_Outptr_ DWORD * pHandle)
	{
		try	{
			if (pHandle == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			*pHandle = m_pModelBuildItem->getHandle();

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

