/*++

Copyright (C) 2015 Microsoft Corporation
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


#include "Model/COM/NMR_COMInterface_ModelTexture2D.h"
#include "Model/COM/NMR_COMInterface_ModelAttachment.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ExportStream_Callback.h"

#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Model/Classes/NMR_ModelConstants.h"

#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_ImportStream_COM.h"
#endif

namespace NMR {

	CCOMModelTexture2D::CCOMModelTexture2D()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	_Ret_notnull_ CModelTexture2DResource * CCOMModelTexture2D::getTexture2D()
	{
		if (m_pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

		CModelTexture2DResource * pTextureResource = dynamic_cast<CModelTexture2DResource *> (m_pResource.get());
		if (pTextureResource == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDTEXTURE);

		return pTextureResource;
	}


	void CCOMModelTexture2D::setResource(_In_ PModelResource pModelResource)
	{
		m_pResource = pModelResource;
	}

	LIB3MFRESULT CCOMModelTexture2D::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelTexture2D::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelTexture2D::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelTexture2D::GetResourceID(_Out_ DWORD * pnResourceID)
	{
		try {
			if (pnResourceID == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pResource.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			*pnResourceID = m_pResource->getResourceID()->getUniqueID();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetAttachment(_Out_ ILib3MFModelAttachment ** ppTextureAttachment)
	{
		try {
			if (ppTextureAttachment == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			// Retrieve Path
			std::string sPath = pTextureResource->getPath();

			CModel * pModel = pTextureResource->getModel();
			__NMRASSERT(pModel);
			PModelAttachment pAttachment = pModel->findModelAttachment(sPath);
			
			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppTextureAttachment = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::SetAttachment(_In_ ILib3MFModelAttachment * pTextureAttachment)
	{
		try {
			if (pTextureAttachment == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			// check relationship type
			std::string sRelationshipType;
			ULONG cbNeededChars;
			pTextureAttachment->GetRelationshipTypeUTF8(nullptr, 0, &cbNeededChars);
			sRelationshipType.resize(cbNeededChars);
			pTextureAttachment->GetRelationshipTypeUTF8(&(sRelationshipType[0]), cbNeededChars + 1, &cbNeededChars);
			if (!(sRelationshipType == PACKAGE_TEXTURE_RELATIONSHIP_TYPE))
				throw CNMRException(NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE);

			std::string sPath;
			pTextureAttachment->GetPathUTF8(nullptr, 0, &cbNeededChars);
			sPath.resize(cbNeededChars);
			pTextureAttachment->GetPathUTF8(&(sPath[0]), cbNeededChars+1, &cbNeededChars);

			pTextureResource->setPath(sPath);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetPath(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			// Retrieve Path
			std::string sPath = pTextureResource->getPath();
			std::wstring wPath = fnUTF8toUTF16(sPath);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(wPath, pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelTexture2D::GetPathUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			// Retrieve Path
			std::string sUTF8Path = pTextureResource->getPath();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Path, pszBuffer, cbBufferSize, &nNeededChars);

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


	LIB3MFMETHODIMP CCOMModelTexture2D::SetPath(_In_z_ LPCWSTR pwszPath)
	{
		try {
			if (pwszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			std::wstring sPath(pwszPath);
			pTextureResource->setPath(fnUTF16toUTF8(sPath));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelTexture2D::SetPathUTF8(_In_z_ LPCSTR pszPath)
	{
		try {
			if (pszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			std::string sUTF8Path(pszPath);
			pTextureResource->setPath(sUTF8Path);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetContentType(_Out_ eModelTexture2DType * peContentType)
	{
		try {
			if (peContentType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			*peContentType = pTextureResource->getContentType();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::SetContentType(_In_ eModelTexture2DType eContentType)
	{
		try {
			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			pTextureResource->setContentType(eContentType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetTileStyleUV(_Out_ eModelTextureTileStyle * peTileStyleU, _Out_ eModelTextureTileStyle * peTileStyleV)
	{
		try {
			if ((peTileStyleU == nullptr) || (peTileStyleU == nullptr))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			*peTileStyleU = pTextureResource->getTileStyleU();
			*peTileStyleV = pTextureResource->getTileStyleV();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::SetTileStyleUV(_In_ eModelTextureTileStyle eTileStyleU, _In_ eModelTextureTileStyle eTileStyleV)
	{
		try {
			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			pTextureResource->setTileStyleU(eTileStyleU);
			pTextureResource->setTileStyleV(eTileStyleV);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetBox2D(_Out_ FLOAT * pfU, _Out_ FLOAT * pfV, _Out_ FLOAT * pfWidth, _Out_ FLOAT * pfHeight)
	{
		try {
			if ((pfU == nullptr) || (pfV == nullptr) || (pfWidth == nullptr) || (pfHeight == nullptr))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			nfFloat fU, fV, fWidth, fHeight;
			if (pTextureResource->getBox2D(fU, fV, fWidth, fHeight)) {
				*pfU = fU;
				*pfV = fV;
				*pfWidth = fWidth;
				*pfHeight = fHeight;
			}
			else {
				*pfU = 0.0f;
				*pfV = 0.0f;
				*pfWidth = 1.0f;
				*pfHeight = 1.0f;
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

	LIB3MFMETHODIMP CCOMModelTexture2D::SetBox2D(_In_ FLOAT fU, _In_ FLOAT fV, _In_ FLOAT fWidth, _In_ FLOAT fHeight)
	{
		try {
			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			pTextureResource->setBox2D(fU, fV, fWidth, fHeight);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelTexture2D::ClearBox2D()
	{
		try {
			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			pTextureResource->clearBox2D();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelTexture2D::GetStreamSize(_Out_ ULONG64 * pcbStreamSize)
	{
		try {
			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pTextureAttachment = pTextureResource->getTextureStream();

			if (pTextureAttachment.get() != nullptr) {
				*pcbStreamSize = pTextureAttachment->retrieveSize();
			}
			else {
				*pcbStreamSize = 0;
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

	LIB3MFMETHODIMP CCOMModelTexture2D::WriteToFile(_In_z_ LPCWSTR pwszFilename)
	{
		try {
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pTextureAttachment = pTextureResource->getTextureStream();

			if (pTextureAttachment.get() != nullptr) {
				pTextureAttachment->writeToFile(pwszFilename);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelTexture2D::WriteToFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pTextureAttachment = pTextureResource->getTextureStream();

			if (pTextureAttachment.get() != nullptr) {
				std::string sUTF8FileName(pszFilename);
				std::wstring sUTF16FileName = fnUTF8toUTF16(sUTF8FileName);

				pTextureAttachment->writeToFile(sUTF16FileName.c_str());
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelTexture2D::WriteToBuffer(_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pTextureAttachment = pTextureResource->getTextureStream();

			if (pTextureAttachment.get() != nullptr) {
				nfUint64 cbStreamSize = pTextureAttachment->retrieveSize();
				if (cbStreamSize > cbBufferSize)
					throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

				pTextureAttachment->seekPosition(0, true);
				pTextureAttachment->readBuffer(pBuffer, cbStreamSize, true);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelTexture2D::WriteToCallback(_In_ void * pWriteCallback, _In_opt_ void * pUserData)
	{
		try {
			if (pWriteCallback == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pTextureAttachment = pTextureResource->getTextureStream();
			ExportStream_WriteCallbackType pTypedWriteCallback = (ExportStream_WriteCallbackType)pWriteCallback;

			if (pTextureAttachment.get() != nullptr) {
				PExportStream pExportStream = std::make_shared<CExportStream_Callback>(pTypedWriteCallback, nullptr, pUserData);
				pExportStream->copyFrom(pTextureAttachment.get(), pTextureAttachment->retrieveSize(), MODELTEXTURE2D_BUFFERSIZE);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelTexture2D::ReadFromFile(_In_z_ LPCWSTR pwszFilename)
	{
		try {
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			std::string sUTF8FileName = fnUTF16toUTF8(pwszFilename);
			PImportStream pImportStream = fnCreateImportStreamInstance(sUTF8FileName.c_str());

			CModel * pModel = pTextureResource->getModel();
			__NMRASSERT(pModel);
			
			pModel->removeAttachment(pTextureResource->getPath());
			pModel->addAttachment(pTextureResource->getPath(), PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelTexture2D::ReadFromFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			std::string sUTF8FileName(pszFilename);
			PImportStream pImportStream = fnCreateImportStreamInstance(sUTF8FileName.c_str());

			CModel * pModel = pTextureResource->getModel();
			__NMRASSERT(pModel);

			pModel->removeAttachment(pTextureResource->getPath());
			pModel->addAttachment(pTextureResource->getPath(), PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}


	LIB3MFMETHODIMP CCOMModelTexture2D::ReadFromBuffer(_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CModelTexture2DResource * pTextureResource = getTexture2D();
			__NMRASSERT(pTextureResource);

			PImportStream pImportStream = std::make_shared<CImportStream_Memory>(pBuffer, cbBufferSize);

			CModel * pModel = pTextureResource->getModel();
			__NMRASSERT(pModel);

			pModel->removeAttachment(pTextureResource->getPath());
			pModel->addAttachment(pTextureResource->getPath(), PACKAGE_TEXTURE_RELATIONSHIP_TYPE, pImportStream);

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
