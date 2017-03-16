/*++

Copyright (C) Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

COM Interface Implementation for Model Classes

--*/


#include "Model/COM/NMR_COMInterface_ModelBaseMaterial.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <math.h>

namespace NMR {

	CCOMModelBaseMaterial::CCOMModelBaseMaterial()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	_Ret_notnull_ CModelBaseMaterialResource * CCOMModelBaseMaterial::getBaseMaterials()
	{
		if (m_pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

		CModelBaseMaterialResource * pMaterialResource = dynamic_cast<CModelBaseMaterialResource *> (m_pResource.get());
		if (pMaterialResource == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDBASEMATERIAL);

		return pMaterialResource;
	}

	void CCOMModelBaseMaterial::setResource(_In_ PModelResource pModelResource)
	{
		m_pResource = pModelResource;
	}

	LIB3MFRESULT CCOMModelBaseMaterial::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelBaseMaterial::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelBaseMaterial::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetResourceID(_Out_ DWORD * pnResourceID)
	{
		try {
			if (pnResourceID == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			*pnResourceID = m_pResource->getResourceID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetCount(_Out_ DWORD * pcbCount)
	{
		try {
			if (pcbCount == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();

			*pcbCount = pMaterialResource->getCount();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::AddMaterial(_In_z_ LPCWSTR pwszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_opt_ DWORD * pnResourceIndex)
	{
		try {
			if (pwszName == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();

			std::wstring sName(pwszName);
			nfUint32 nRed = bRed;
			nfUint32 nGreen = bGreen;
			nfUint32 nBlue = bBlue;
			nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16);

			nfUint32 nNewResourceIndex = pMaterialResource->getCount();
			pMaterialResource->addBaseMaterial(sName, cColor);

			if (pnResourceIndex != nullptr)
				*pnResourceIndex = nNewResourceIndex;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::AddMaterialUTF8(_In_z_ LPCSTR pszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_opt_ DWORD * pnResourceIndex)
	{
		try {
			if (pszName == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();

			std::string sUTF8Name(pszName);
			std::wstring sUTF16Name = fnUTF8toUTF16(sUTF8Name);
			nfUint32 nRed = bRed;
			nfUint32 nGreen = bGreen;
			nfUint32 nBlue = bBlue;
			nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16);

			nfUint32 nNewResourceIndex = pMaterialResource->getCount();
			pMaterialResource->addBaseMaterial(sUTF16Name, cColor);

			if (pnResourceIndex != nullptr)
				*pnResourceIndex = nNewResourceIndex;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::RemoveMaterial (_In_ DWORD nIndex)
	{
		try {
			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			pMaterialResource->removeMaterial(nIndex);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetName(_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			// Retrieve Name
			std::wstring sName = pMaterial->getName();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(sName, pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetNameUTF8(_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			// Retrieve Name
			std::wstring sUTF16Name = pMaterial->getName();
			std::string sUTF8Name = fnUTF16toUTF8(sUTF16Name);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Name, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetName(_In_ DWORD nIndex, _In_z_ LPCWSTR pwszName)
	{
		try {
			if (pwszName == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::wstring sName(pwszName);
			pMaterial->setName(sName);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetNameUTF8(_In_ DWORD nIndex, _In_z_ LPCSTR pszName)
	{
		try {
			if (pszName == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::string sUTF8Name(pszName);
			std::wstring sUTF16Name = fnUTF8toUTF16(sUTF8Name);
			pMaterial->setName(sUTF16Name);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetDisplayColorRGB(_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue)
	{
		return SetDisplayColorRGBA(nIndex, bRed, bGreen, bBlue, 255);

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetDisplayColorRGBA(_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha)
	{
		try {
			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			nfUint32 nRed = bRed;
			nfUint32 nGreen = bGreen;
			nfUint32 nBlue = bBlue;
			nfUint32 nAlpha = bAlpha;
			nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			pMaterial->setColor(cColor);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetDisplayColorFloatRGB(_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue)
	{
		return SetDisplayColorFloatRGBA(nIndex, fRed, fGreen, fBlue, 1.0f);
	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::SetDisplayColorFloatRGBA(_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha)
	{
		if (fRed < 0.0f)
			fRed = 0.0f;
		if (fRed > 1.0f)
			fRed = 1.0f;
		if (fGreen < 0.0f)
			fGreen = 0.0f;
		if (fGreen > 1.0f)
			fGreen = 1.0f;
		if (fBlue < 0.0f)
			fBlue = 0.0f;
		if (fBlue > 1.0f)
			fBlue = 1.0f;
		if (fAlpha < 0.0f)
			fAlpha = 0.0f;
		if (fAlpha > 1.0f)
			fAlpha = 1.0f;

		nfInt32 nRed = (nfInt32) roundf (fRed * 255.0f);
		nfInt32 nGreen = (nfInt32)roundf(fGreen * 255.0f);
		nfInt32 nBlue = (nfInt32)roundf(fBlue * 255.0f);
		nfInt32 nAlpha = (nfInt32)roundf(fAlpha * 255.0f);

		return SetDisplayColorRGBA(nIndex, nRed, nGreen, nBlue, nAlpha);

	}

	LIB3MFMETHODIMP CCOMModelBaseMaterial::GetDisplayColor (_In_ DWORD nIndex, _Out_ BYTE* pbRed, _Out_ BYTE* pbGreen, _Out_ BYTE* pbBlue, _Out_ BYTE* pbAlpha)
	{
		try {
			if ((pbRed == nullptr) || (pbGreen == nullptr) || (pbBlue == nullptr) || (pbAlpha == nullptr)) {
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			}

			CModelBaseMaterialResource * pMaterialResource = getBaseMaterials();
			__NMRASSERT(pMaterialResource);

			PModelBaseMaterial pMaterial = pMaterialResource->getBaseMaterial(nIndex);
			if (!pMaterial.get())
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			nfColor cColor = pMaterial->getDisplayColor();
			nfUint32 nRed = (cColor) & 0xff;
			nfUint32 nGreen = (cColor >> 8)& 0xff;
			nfUint32 nBlue = (cColor >> 16)& 0xff;
			nfUint32 nAlpha = (cColor >> 24)& 0xff;

			*pbRed = nRed;
			*pbGreen = nGreen;
			*pbBlue = nBlue;
			*pbAlpha = nAlpha;


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


