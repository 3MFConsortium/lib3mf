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

COM Interface Implementation for Model Default Property Handler

--*/

#include "Model/COM/NMR_COMInterface_ModelDefaultPropertyHandler.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

#include "Common/MeshInformation/NMR_MeshInformation_BaseMaterials.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"

#include "Model/Classes/NMR_ModelDefaultProperty_BaseMaterial.h"
#include "Model/Classes/NMR_ModelDefaultProperty_Color.h"
#include "Model/Classes/NMR_ModelDefaultProperty_TexCoord2D.h"

#include <cmath>

namespace NMR {

	CCOMModelDefaultPropertyHandler::CCOMModelDefaultPropertyHandler()
	{
		m_nChannel = 0;
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelDefaultPropertyHandler::setResource(_In_ PModelResource pResource)
	{
		m_pModelResource = pResource;
	}

	void CCOMModelDefaultPropertyHandler::setChannel(_In_ DWORD nChannel)
	{
		m_nChannel = nChannel;
	}

	_Ret_notnull_ CModelObject * CCOMModelDefaultPropertyHandler::getObject()
	{
		CModelObject * pObject = dynamic_cast<CModelObject *> (m_pModelResource.get());

		if (pObject == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		return pObject;
	}


	LIB3MFRESULT CCOMModelDefaultPropertyHandler::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelDefaultPropertyHandler::handleNMRException(_In_ CNMRException * pException)
	{
		__NMRASSERT(pException);

		m_nErrorCode = pException->getErrorCode();
		m_sErrorMessage = std::string(pException->what());

		CNMRException_Windows * pWinException = dynamic_cast<CNMRException_Windows *> (pException);
		if (pWinException != nullptr) {
			return pWinException->getHResult();
		}
		else {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFRESULT CCOMModelDefaultPropertyHandler::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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


	LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::RemoveProperty()
	{
		try {

			CModelObject * pObject = getObject();
			pObject->setDefaultProperty(nullptr);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::GetPropertyType (_Out_ eModelPropertyType * pnPropertyType) 
	 {
		 try {

			 if (pnPropertyType == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			 CModelObject * pObject = getObject();
			 PModelDefaultProperty pDefaultProperty = pObject->getDefaultProperty();

			 *pnPropertyType = MODELPROPERTYTYPE_NONE;

			 CModelDefaultProperty_BaseMaterial * pMaterialProperty = dynamic_cast<CModelDefaultProperty_BaseMaterial *> (pDefaultProperty.get());
			 if (pMaterialProperty != nullptr) {
				 *pnPropertyType = MODELPROPERTYTYPE_BASEMATERIALS;
			 }
			 CModelDefaultProperty_Color * pColorProperty = dynamic_cast<CModelDefaultProperty_Color *> (pDefaultProperty.get());
			 if (pColorProperty != nullptr) {
				 *pnPropertyType = MODELPROPERTYTYPE_COLOR;
			 }

			 CModelDefaultProperty_TexCoord2D * pTexCoordProperty = dynamic_cast<CModelDefaultProperty_TexCoord2D *> (pDefaultProperty.get());
			 if (pTexCoordProperty != nullptr) {
				 *pnPropertyType = MODELPROPERTYTYPE_TEXCOORD2D;
			 }
			 
			 // MODELPROPERTYTYPE_COMPOSITE is not supported as default property

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }
	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::GetBaseMaterial (_Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex)
	 {
		 try {

			 if (pnMaterialGroupID == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			 if (pnMaterialIndex == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			 CModelObject * pObject = getObject();
			 PModelDefaultProperty pDefaultProperty = pObject->getDefaultProperty();

			 *pnMaterialGroupID = 0;
			 *pnMaterialIndex = 0;

			 CModelDefaultProperty_BaseMaterial * pMaterialProperty = dynamic_cast<CModelDefaultProperty_BaseMaterial *> (pDefaultProperty.get());
			
			 if (pMaterialProperty != nullptr) {
				 *pnMaterialGroupID = pMaterialProperty->getResourceID();
				 *pnMaterialIndex = pMaterialProperty->getResourceIndex();
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

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetBaseMaterial (_In_ ModelResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex)
	 {
		 try {

			 CModelObject * pObject = getObject();
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_BaseMaterial>(nMaterialGroupID, nMaterialIndex));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::GetColor (_In_ DWORD nIndex, _Out_ MODELMESHCOLOR_SRGB * pColor) 
	 {
		 try {

			 if (pColor == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			 CModelObject * pObject = getObject();
			 PModelDefaultProperty pDefaultProperty = pObject->getDefaultProperty();

			 pColor->m_Red = 255;
			 pColor->m_Green = 255;
			 pColor->m_Blue = 255;
			 pColor->m_Alpha = 255;

			 CModelDefaultProperty_Color * pColorProperty = dynamic_cast<CModelDefaultProperty_Color *> (pDefaultProperty.get());
			 if (pColorProperty != nullptr) {
				 nfColor cColor = pColorProperty->getColor();
				 pColor->m_Red = cColor & 0xff;
				 pColor->m_Green = (cColor >> 8) & 0xff;
				 pColor->m_Blue = (cColor >> 16) & 0xff;
				 pColor->m_Alpha = (cColor >> 24) & 0xff;
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

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetColor (_In_ MODELMESHCOLOR_SRGB * pColor) 
	 {
		 try {
			 if (pColor == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			 CModelObject * pObject = getObject();
			 nfUint32 nRed = pColor->m_Red;
			 nfUint32 nGreen = pColor->m_Green;
			 nfUint32 nBlue = pColor->m_Blue;
			 nfUint32 nAlpha = pColor->m_Alpha;

			 nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetColorRGB (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue) 
	 {
		 try {

			 CModelObject * pObject = getObject();
			 nfUint32 nRed = bRed;
			 nfUint32 nGreen = bGreen;
			 nfUint32 nBlue = bBlue;
			 nfUint32 nAlpha = 255;

			 nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetColorRGBA (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha)
	 {
		 try {

			 CModelObject * pObject = getObject();
			 nfUint32 nRed = bRed;
			 nfUint32 nGreen = bGreen;
			 nfUint32 nBlue = bBlue;
			 nfUint32 nAlpha = bAlpha;

			 nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetFloatColorRGB (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue) 
	 {
		 try {

			 CModelObject * pObject = getObject();
			 nfUint32 nRed = (nfInt32)round(fRed * 255.0f);
			 nfUint32 nGreen = (nfInt32)round(fGreen * 255.0f);
			 nfUint32 nBlue = (nfInt32)round(fBlue * 255.0f);
			 nfUint32 nAlpha = 255;

			 if (nRed > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);
			 if (nGreen > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);
			 if (nBlue > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);

			 nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetFloatColorRGBA (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha) 
	 {
		 try {

			 CModelObject * pObject = getObject();
			 nfUint32 nRed = (nfInt32)round(fRed * 255.0f);
			 nfUint32 nGreen = (nfInt32)round(fGreen * 255.0f);
			 nfUint32 nBlue = (nfInt32)round(fBlue * 255.0f);
			 nfUint32 nAlpha = (nfInt32)round(fAlpha * 255.0f);

			 if (nRed > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);
			 if (nGreen > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);
			 if (nBlue > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);
			 if (nAlpha > 255)
				 throw CNMRException(NMR_ERROR_INVALIDPARAM);

			 nfColor cColor = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_Color>(cColor));

			 return handleSuccess();
		 }
		 catch (CNMRException & Exception) {
			 return handleNMRException(&Exception);
		 }
		 catch (...) {
			 return handleGenericException();
		 }

	 }

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::GetTexture (_Out_ ModelResourceID * pnTextureID, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV)
	 {
		 try {

			 if (pnTextureID == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			 if (pfU == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			 if (pfV == nullptr)
				 throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			 CModelObject * pObject = getObject();
			 PModelDefaultProperty pDefaultProperty = pObject->getDefaultProperty();

			 *pnTextureID = 0;
			 *pfU = 0.0f;
			 *pfV = 0.0f;

			 CModelDefaultProperty_TexCoord2D * pTexCoordProperty = dynamic_cast<CModelDefaultProperty_TexCoord2D *> (pDefaultProperty.get());

			 if (pTexCoordProperty != nullptr) {
				 *pnTextureID = pTexCoordProperty->getTextureID();
				 *pfU = pTexCoordProperty->getU();
				 *pfV = pTexCoordProperty->getV();
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

	 LIB3MFMETHODIMP CCOMModelDefaultPropertyHandler::SetTexture (_In_ ModelResourceID nTextureID, _In_ FLOAT fU, _In_ FLOAT fV) 
	 {
		 try {

			 CModelObject * pObject = getObject();
			 pObject->setDefaultProperty(std::make_shared<CModelDefaultProperty_TexCoord2D>(nTextureID, fU, fV));

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
