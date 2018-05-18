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

COM Interface Implementation for Model Class

--*/

#include "Model/COM/NMR_COMInterface_Model.h"
#include "Model/COM/NMR_COMInterface_ModelReader.h"
#include "Model/COM/NMR_COMInterface_ModelWriter.h"
#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h"
#include "Model/COM/NMR_COMInterface_ModelResourceIterator.h"
#include "Model/COM/NMR_COMInterface_ModelBuildItem.h"
#include "Model/COM/NMR_COMInterface_ModelBuildItemIterator.h"
#include "Model/COM/NMR_COMInterface_ModelMeshObject.h"
#include "Model/COM/NMR_COMInterface_ModelComponentsObject.h"
#include "Model/COM/NMR_COMInterface_ModelTexture2D.h"
#include "Model/COM/NMR_COMInterface_ModelBaseMaterial.h"
#include "Model/COM/NMR_COMInterface_ModelAttachment.h"
// TODO #include "Model/COM/NMR_COMInterface_ModelTextureAttachment.h"
#include "Model/COM/NMR_COMInterface_Slice.h"


#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Reader/NMR_ModelReader_STL.h"
#include "Model/Writer/NMR_ModelWriter_STL.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"

#ifndef NMR_COM_NATIVE
#include "Common/Platform/NMR_COM_Emulation.h"
#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h"
#else
#include "Common/Platform/NMR_COM_Native.h"
#include "Model/Reader/NMR_ModelReader_3MF_OPC.h"
#include "Model/Writer/NMR_ModelWriter_3MF_OPC.h"
#endif


#include <string.h>

namespace NMR {

	CCOMModel::CCOMModel()
	{
		m_pModel = std::make_shared<CModel>();
		m_nErrorCode = NMR_SUCCESS;
	}

	CModel * CCOMModel::getModel()
	{
		return m_pModel.get();
	}

	LIB3MFRESULT CCOMModel::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModel::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModel::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModel::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModel::SetUnit(_In_ DWORD Unit)
	{
		try {
			m_pModel->setUnit((eModelUnit)Unit);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetUnit(_Out_ DWORD * pUnit)
	{

		try {
			if (!pUnit)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pUnit = (DWORD)m_pModel->getUnit();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::SetLanguage(_In_z_ LPCWSTR pwszLanguage)
	{
		try {
			if (!pwszLanguage)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModel->setLanguage(fnUTF16toUTF8(pwszLanguage));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::SetLanguageUTF8(_In_z_ LPCSTR pszLanguage)
	{
		try {
			if (!pszLanguage)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModel->setLanguage(pszLanguage);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetLanguage(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(m_pModel->getLanguage()), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::GetLanguageUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			std::string sUTF8Language = m_pModel->getLanguage();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Language, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::QueryWriter(_In_z_ LPCSTR pszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter)
	{
		try {
			if ((!ppWriter) || (!pszWriterClass))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Create Writer Object
			PModelWriter pWriter = nullptr;

			// Create specified writer instance
			if (strcmp(pszWriterClass, MODELWRITERCLASS_3MF) == 0) {
#ifdef NMR_COM_NATIVE
				pWriter = std::make_shared<CModelWriter_3MF_OPC>(m_pModel);
#else
				pWriter = std::make_shared<CModelWriter_3MF_Native>(m_pModel);
#endif
			}
			if (strcmp(pszWriterClass, MODELWRITERCLASS_STL) == 0)
				pWriter = std::make_shared<CModelWriter_STL>(m_pModel);

			if (!pWriter)
				throw CNMRException(NMR_ERROR_UNKNOWNWRITERCLASS);

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelWriter> * pCOMObject = new CCOMObject<CCOMModelWriter>();
			pCOMObject->setWriter(pWriter);
			pCOMObject->AddRef();
			*ppWriter = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::QueryReader(_In_z_ LPCSTR pszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader)
	{
		try {
			if ((!ppReader) || (!pszReaderClass))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Create Reader Object
			PModelReader pReader = nullptr;

			// Create specified writer instance
			if (strcmp(pszReaderClass, MODELREADERCLASS_3MF) == 0) {
#ifdef NMR_COM_NATIVE
				pReader = std::make_shared<CModelReader_3MF_OPC>(m_pModel);
#else
				pReader = std::make_shared<CModelReader_3MF_Native>(m_pModel);
#endif
			}
			if (strcmp(pszReaderClass, MODELREADERCLASS_STL) == 0)
				pReader = std::make_shared<CModelReader_STL>(m_pModel);

			if (!pReader)
				throw CNMRException(NMR_ERROR_UNKNOWNREADERCLASS);

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelReader> * pCOMObject = new CCOMObject<CCOMModelReader>();
			pCOMObject->setReader(pReader);
			pCOMObject->AddRef();
			*ppReader = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetResourceByID(_In_ DWORD ResourceID, _Outptr_ ILib3MFModelResource ** ppResource)
	{

		try {
			if (!ppResource)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelResource pResource = m_pModel->findResource(ResourceID);
			if (!pResource)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			fnCreateModelResourceFromClass(pResource, true, ppResource);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetTexture2DByID(_In_ DWORD nResourceID, _Outptr_ ILib3MFModelTexture2D ** ppTexture)
	{
		try {
			if (!ppTexture)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelResource pResource = m_pModel->findResource(nResourceID);
			if (!pResource)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelTexture2DResource * pTexture = dynamic_cast<CModelTexture2DResource *>(pResource.get());
			if (pTexture != nullptr) {
				ILib3MFModelResource * pResourceInterface;
				fnCreateModelResourceFromClass(pResource, true, &pResourceInterface);

					*ppTexture = (ILib3MFModelTexture2D*) pResourceInterface;
			}
			else {
				*ppTexture = nullptr;
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

	LIB3MFMETHODIMP CCOMModel::GetBaseMaterialByID(_In_ DWORD nResourceID, _Outptr_ ILib3MFModelBaseMaterial ** ppMaterial)
	{
		try {
			if (!ppMaterial)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelResource pResource = m_pModel->findResource(nResourceID);
			if (!pResource)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelBaseMaterialResource * pBaseMaterial = dynamic_cast<CModelBaseMaterialResource *>(pResource.get());
			if (pBaseMaterial != nullptr) {
				ILib3MFModelResource * pResourceInterface;
				fnCreateModelResourceFromClass(pResource, true, &pResourceInterface);

				*ppMaterial = (ILib3MFModelBaseMaterial*)pResourceInterface;
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

	LIB3MFMETHODIMP CCOMModel::GetMeshObjectByID(_In_ DWORD nResourceID, _Outptr_ ILib3MFModelMeshObject ** ppMeshObject)
	{
		try {
			if (!ppMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelResource pResource = m_pModel->findResource(nResourceID);
			if (!pResource)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelMeshObject * pMeshObject = dynamic_cast<CModelMeshObject *>(pResource.get());
			if (pMeshObject != nullptr) {
				ILib3MFModelResource * pResourceInterface;
				fnCreateModelResourceFromClass(pResource, true, &pResourceInterface);

				*ppMeshObject = (ILib3MFModelMeshObject*)pResourceInterface;
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

	LIB3MFMETHODIMP CCOMModel::GetComponentsObjectByID(_In_ DWORD nResourceID, _Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject)
	{
		try {
			if (!ppComponentsObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelResource pResource = m_pModel->findResource(nResourceID);
			if (!pResource)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			CModelComponentsObject * pComponentsObject = dynamic_cast<CModelComponentsObject *>(pResource.get());
			if (pComponentsObject != nullptr) {
				ILib3MFModelResource * pResourceInterface;
				fnCreateModelResourceFromClass(pResource, true, &pResourceInterface);

				*ppComponentsObject = (ILib3MFModelComponentsObject*)pResourceInterface;
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

	LIB3MFMETHODIMP CCOMModel::GetBuildUUIDUTF8(_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer)
	{
		try {
			if ((!pbHasUUID) || (!pszBuffer))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pbHasUUID = (m_pModel->buildUUID().get() != nullptr);
			if (*pbHasUUID) {
				std::string sUUID = m_pModel->buildUUID()->toString();
				// Safely call StringToBuffer
				nfUint32 nNeededChars = 0;
				fnStringToBufferSafe(sUUID, pszBuffer, 37, &nNeededChars);
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

	LIB3MFMETHODIMP CCOMModel::SetBuildUUIDUTF8(_In_ LPCSTR pszUUID)
	{
		try
		{
			if (!pszUUID)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PUUID pUUID = std::make_shared<CUUID>(pszUUID);
			m_pModel->setBuildUUID(pUUID);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetBuildItems(_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelBuildItemIterator> * pResult = new CCOMObject<CCOMModelBuildItemIterator>();
			pResult->AddRef();

			nfUint32 nBuildItemCount = m_pModel->getBuildItemCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nBuildItemCount; nIdx++)
				pResult->addBuildItem(m_pModel->getBuildItem(nIdx));

			*ppIterator = pResult;

			return handleSuccess();

		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetResources(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nResourceCount = m_pModel->getResourceCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nResourceCount; nIdx++)
				pResult->addResource(m_pModel->getResource(nIdx));

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++)
				pResult->addResource(m_pModel->getObjectResource(nIdx));

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetMeshObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++) {
				PModelResource pResource = m_pModel->getObjectResource(nIdx);
				if (dynamic_cast<CModelMeshObject *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetComponentsObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++) {
				PModelResource pResource = m_pModel->getObjectResource(nIdx);
				if (dynamic_cast<CModelComponentsObject *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddMeshObject(_Outptr_ ILib3MFModelMeshObject ** ppMeshObject)
	{
		try {
			if (!ppMeshObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			ModelResourceID NewResourceID = m_pModel->generateResourceID();
			PMesh pNewMesh = std::make_shared<CMesh>();
			PModelMeshObject pNewResource = std::make_shared<CModelMeshObject>(NewResourceID, m_pModel.get(), pNewMesh);

			m_pModel->addResource(pNewResource);

			CCOMObject<CCOMModelMeshObject> * pResult = new CCOMObject<CCOMModelMeshObject>();
			pResult->AddRef();
			pResult->setResource(pNewResource);
			*ppMeshObject = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddComponentsObject(_Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject)
	{
		try {
			if (!ppComponentsObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			ModelResourceID NewResourceID = m_pModel->generateResourceID();
			PModelComponentsObject pNewResource = std::make_shared<CModelComponentsObject>(NewResourceID, m_pModel.get());
			m_pModel->addResource(pNewResource);

			CCOMObject<CCOMModelComponentsObject> * pResult = new CCOMObject<CCOMModelComponentsObject>();
			pResult->AddRef();
			pResult->setResource(pNewResource);
			*ppComponentsObject = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddBuildItem(_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ ILib3MFModelBuildItem ** ppBuildItem)
	{
		try {
			if ((!pObject) || (!ppBuildItem))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Get Resource ID
			PackageResourceID nObjectID = 0;
			HRESULT hResult = pObject->GetResourceID(&nObjectID);
			if (hResult != LIB3MF_OK)
				return hResult;

			// Find class instance
			CModelObject * pObject = m_pModel->findObject(nObjectID);
			if (pObject == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			// Create Build item instance
			PModelBuildItem pModelBuildItem = std::make_shared<CModelBuildItem>(pObject, m_pModel->createHandle());
			m_pModel->addBuildItem(pModelBuildItem);

			// Create COM Wrapper
			CCOMObject<CCOMModelBuildItem> * pResult = new CCOMObject<CCOMModelBuildItem>();
			pResult->AddRef();
			pResult->setItem(pModelBuildItem);
			*ppBuildItem = pResult;

			// Set transform, if necessary
			if (pTransform)
				pResult->SetObjectTransform(pTransform);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::RemoveBuildItem(_In_ ILib3MFModelBuildItem * pBuildItem)
	{
		try {
			if (!pBuildItem)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			DWORD nHandle = 0;
			LIB3MFRESULT hResult = pBuildItem->GetHandle(&nHandle);
			if (hResult != LIB3MF_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTGETHANDLE, hResult);

			m_pModel->removeBuildItem(nHandle, true);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::MergeToModel(_Outptr_ ILib3MFModel ** ppMergedModel)
	{
		try {
			if (!ppMergedModel)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Create merged mesh
			PMesh pMesh = std::make_shared<CMesh>();
			m_pModel->mergeToMesh(pMesh.get());

			CCOMObject<CCOMModel> * pNewModelInterface = new CCOMObject<CCOMModel>();
			pNewModelInterface->AddRef();
			*ppMergedModel = pNewModelInterface;

			// Copy relevant resources to new model!
			CModel * pNewModel = pNewModelInterface->getModel();
			if (pNewModel == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDMODEL);

			pNewModel->mergeModelAttachments(m_pModel.get());
			pNewModel->mergeTextures2D(m_pModel.get());
			pNewModel->mergeBaseMaterials(m_pModel.get());
			pNewModel->mergeMetaData(m_pModel.get());

			pNewModel->setUnit(m_pModel->getUnit());
			pNewModel->setLanguage(m_pModel->getLanguage());

			PModelMeshObject pMeshObject = std::make_shared<CModelMeshObject>(pNewModel->generateResourceID(), pNewModel, pMesh);
			pNewModel->addResource(pMeshObject);

			PModelBuildItem pBuildItem = std::make_shared<CModelBuildItem>(pMeshObject.get(), m_pModel->createHandle());
			pNewModel->addBuildItem(pBuildItem);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::Get2DTextures(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nResourceCount = m_pModel->getResourceCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nResourceCount; nIdx++) {
				PModelResource pResource = m_pModel->getResource(nIdx);
				if (dynamic_cast<CModelTexture2DResource *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}


	LIB3MFMETHODIMP CCOMModel::GetBaseMaterials(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();
			pResult->AddRef();

			nfUint32 nResourceCount = m_pModel->getResourceCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nResourceCount; nIdx++) {
				PModelResource pResource = m_pModel->getResource(nIdx);
				if (dynamic_cast<CModelBaseMaterialResource *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::AddTexture2DFromAttachment(_In_z_ ILib3MFModelAttachment* pTextureAttachment, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance)
	{
		try {
			if (pTextureAttachment == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!ppTextureInstance)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			
			// check relationship type
			std::string sRelationshipType;
			ULONG cbNeededChars;
			pTextureAttachment->GetRelationshipTypeUTF8(nullptr, 0, &cbNeededChars);
			sRelationshipType.resize(cbNeededChars);
			pTextureAttachment->GetRelationshipTypeUTF8(&(sRelationshipType[0]), cbNeededChars + 1, &cbNeededChars);
			if ( !(sRelationshipType == PACKAGE_TEXTURE_RELATIONSHIP_TYPE) )
				throw CNMRException(NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE);

			std::string sPath;
			pTextureAttachment->GetPathUTF8(nullptr, 0, &cbNeededChars);
			sPath.resize(cbNeededChars);
			pTextureAttachment->GetPathUTF8(&(sPath[0]), cbNeededChars+1, &cbNeededChars);

			PModelTexture2DResource pResource = std::make_shared<CModelTexture2DResource>(m_pModel->generateResourceID(), m_pModel.get());
			m_pModel->addResource(pResource);
			pResource->setPath(sPath);

			CCOMObject<CCOMModelTexture2D> * pCOMObject = new CCOMObject<CCOMModelTexture2D>();
			pCOMObject->setResource(pResource);
			pCOMObject->AddRef();
			*ppTextureInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddTexture2D(_In_z_ LPCWSTR pwszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance)
	{
		try {
			if (pwszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!ppTextureInstance)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelTexture2DResource pResource = std::make_shared<CModelTexture2DResource>(m_pModel->generateResourceID(), m_pModel.get());
			m_pModel->addResource(pResource);

			pResource->setPath(fnUTF16toUTF8(pwszPath));

			CCOMObject<CCOMModelTexture2D> * pCOMObject = new CCOMObject<CCOMModelTexture2D>();
			pCOMObject->setResource(pResource);
			pCOMObject->AddRef();
			*ppTextureInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


	LIB3MFMETHODIMP CCOMModel::AddTexture2DUTF8(_In_z_ LPCSTR pszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance)
	{
		try {
			if (pszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!ppTextureInstance)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelTexture2DResource pResource = std::make_shared<CModelTexture2DResource>(m_pModel->generateResourceID(), m_pModel.get());
			m_pModel->addResource(pResource);

			pResource->setPath(pszPath);

			CCOMObject<CCOMModelTexture2D> * pCOMObject = new CCOMObject<CCOMModelTexture2D>();
			pCOMObject->AddRef();
			pCOMObject->setResource(pResource);
			*ppTextureInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::AddBaseMaterialGroup(_Outptr_ ILib3MFModelBaseMaterial ** ppBaseMaterialInstance)
	{
		try {
			if (!ppBaseMaterialInstance)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelBaseMaterialResource pResource = std::make_shared<CModelBaseMaterialResource>(m_pModel->generateResourceID(), m_pModel.get());
			m_pModel->addResource(pResource);

			CCOMObject<CCOMModelBaseMaterial> * pCOMObject = new CCOMObject<CCOMModelBaseMaterial>();
			pCOMObject->AddRef();
			pCOMObject->setResource(pResource);
			*ppBaseMaterialInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::GetMetaDataCount (_Out_ DWORD * pnCount)
	{
		try {
			if (!pnCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			*pnCount = nCount;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::GetMetaDataKey (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::string sName;
			std::string sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(sName), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::GetMetaDataKeyUTF8(_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::string sName;
			std::string sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sName, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::GetMetaDataValue (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::string sName;
			std::string sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(sValue), pwszBuffer, cbBufferSize, &nNeededChars);

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


	LIB3MFMETHODIMP CCOMModel::GetMetaDataValueUTF8(_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::string sName;
			std::string sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);


			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sValue, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::AddMetaData (_In_ LPCWSTR pszwKey, _In_ LPCWSTR pszwValue)
	{
		try
		{
			if (!pszwKey)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!pszwValue)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModel->addMetaData(fnUTF16toUTF8(pszwKey), fnUTF16toUTF8(pszwValue));
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::AddMetaDataUTF8(_In_ LPCSTR pszKey, _In_ LPCSTR pszValue)
	{
		try
		{
			if (!pszKey)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!pszValue)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModel->addMetaData(pszKey, pszValue);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::RemoveMetaData (_In_ DWORD nIndex)
	{
		try {

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			m_pModel->removeMetaData(nIndex);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::AddAttachment(_In_z_ LPWSTR pwszURI, _In_z_ LPWSTR pwszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {
			if (ppAttachmentInstance == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pwszURI == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pwszRelationShipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PImportStream pStream = std::make_shared<CImportStream_Memory> ();
			PModelAttachment pAttachment = m_pModel->addAttachment(fnUTF16toUTF8(pwszURI), fnUTF16toUTF8(pwszRelationShipType), pStream);

			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppAttachmentInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::AddAttachmentUTF8(_In_z_ LPSTR pszURI, _In_z_ LPSTR pszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {

			if (ppAttachmentInstance == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pszURI == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pszRelationShipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PImportStream pStream = std::make_shared<CImportStream_Memory>();
			PModelAttachment pAttachment = m_pModel->addAttachment(pszURI, pszRelationShipType, pStream);

			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppAttachmentInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::GetAttachment(_In_ DWORD nIndex, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {

			nfUint32 nCount = m_pModel->getAttachmentCount();
			if (nIndex >= nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			if (ppAttachmentInstance == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PModelAttachment pAttachment = m_pModel->getModelAttachment(nIndex);

			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppAttachmentInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::FindAttachment(_In_z_ LPWSTR pwszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {

			if (pwszURI == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			std::wstring sURI(pwszURI);

			PModelAttachment pAttachment = m_pModel->findModelAttachment(fnUTF16toUTF8(sURI));

			if (pAttachment.get() == nullptr)
				throw CNMRException(NMR_ERROR_ATTACHMENTNOTFOUND);

			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppAttachmentInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::FindAttachmentUTF8(_In_z_ LPSTR pszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {

			if (pszURI == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PModelAttachment pAttachment = m_pModel->findModelAttachment(pszURI);

			if (pAttachment.get() == nullptr)
				throw CNMRException(NMR_ERROR_ATTACHMENTNOTFOUND);

			CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
			pCOMObject->AddRef();
			pCOMObject->setAttachment(pAttachment);
			*ppAttachmentInstance = pCOMObject;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::GetAttachmentCount(_Out_ DWORD * pnCount)
	{
		try {
			if (!pnCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			nfUint32 nCount = m_pModel->getAttachmentCount();
			*pnCount = nCount;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::GetAttachmentSize(_In_ DWORD nIndex, _Out_ UINT64 * pnSize)
	{
		try {

			if (pnSize == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			nfUint32 nCount = m_pModel->getAttachmentCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			PModelAttachment pAttachment = m_pModel->getModelAttachment(nIndex);

			if (pAttachment.get() == nullptr)
				throw CNMRException(NMR_ERROR_ATTACHMENTNOTFOUND);

			PImportStream pStream = pAttachment->getStream();
			*pnSize = pStream->retrieveSize();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::GetAttachmentPath(_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getAttachmentCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			PModelAttachment pAttachment = m_pModel->getModelAttachment(nIndex);

			if (pAttachment.get() == nullptr)
				throw CNMRException(NMR_ERROR_ATTACHMENTNOTFOUND);

			std::string sValue = pAttachment->getPathURI();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(sValue), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::GetAttachmentPathUTF8(_In_ DWORD nIndex, _Out_opt_ LPSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getAttachmentCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			PModelAttachment pAttachment = m_pModel->getModelAttachment(nIndex);

			if (pAttachment.get() == nullptr)
				throw CNMRException(NMR_ERROR_ATTACHMENTNOTFOUND);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(pAttachment->getPathURI(), pwszBuffer, cbBufferSize, &nNeededChars);

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

	
	LIB3MFMETHODIMP CCOMModel::GetPackageThumbnailAttachment(_In_ BOOL bCreateIfNotExisting, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance)
	{
		try {
			if (ppAttachmentInstance == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PModelAttachment pAttachment;
			if (m_pModel->getPackageThumbnail() != nullptr)
			{
				pAttachment = m_pModel->getPackageThumbnail();
			}
			else
			{
				if (bCreateIfNotExisting)
					pAttachment = m_pModel->addPackageThumbnail();
			}
			if (pAttachment.get() != nullptr) {
				CCOMObject<CCOMModelAttachment> * pCOMObject = new CCOMObject<CCOMModelAttachment>();
				pCOMObject->AddRef();
				pCOMObject->setAttachment(pAttachment);
				*ppAttachmentInstance = pCOMObject;
			}
			else
				*ppAttachmentInstance = nullptr;
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModel::RemovePackageThumbnailAttachment()
	{
		try {
			m_pModel->removePackageThumbnail();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}
	

	LIB3MFMETHODIMP CCOMModel::AddCustomContentType(_In_ LPCWSTR pszwExtension, _In_ LPCWSTR pszwContentType)
	{

		try {
			if (pszwExtension == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pszwContentType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			std::wstring sExtension(pszwExtension);
			std::wstring sContentType(pszwContentType);

			std::string sUTF8Extension = fnUTF16toUTF8(sExtension);
			std::string sUTF8ContentType = fnUTF16toUTF8(sContentType);

			m_pModel->addCustomContentType(sUTF8Extension, sUTF8ContentType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::AddCustomContentTypeUTF8 (_In_ LPCSTR pszExtension, _In_ LPCSTR pszContentType) 
	{
		try {
			if (pszExtension == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			if (pszContentType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			m_pModel->addCustomContentType(pszExtension, pszContentType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::RemoveCustomContentType (_In_ LPCWSTR pszwExtension)
	{
		try {
			if (pszwExtension == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			m_pModel->removeCustomContentType(fnUTF16toUTF8(pszwExtension));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModel::RemoveCustomContentTypeUTF8 (_In_ LPCSTR pszExtension)
	{
		try {
			if (pszExtension == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			m_pModel->removeCustomContentType(pszExtension);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

  LIB3MFMETHODIMP CCOMModel::AddSliceStack(_In_opt_ FLOAT nBottomZ, _Out_opt_ ILib3MFSliceStack ** ppSliceStack)
  {
    try {
      if (!ppSliceStack)
        throw CNMRException(NMR_ERROR_INVALIDPOINTER);

      ModelResourceID NewResourceID = m_pModel->generateResourceID();
      PSliceStack pSliceStack = std::make_shared<CSliceStack>();
      pSliceStack->setBottomZ(nBottomZ);
      PModelSliceStackResource pNewResource = std::make_shared<CModelSliceStackResource>(NewResourceID, m_pModel.get(), pSliceStack);

      m_pModel->addResource(pNewResource);
      CCOMObject<CCOMSliceStack> * pResult = new CCOMObject<CCOMSliceStack>();
      pResult->AddRef();
      pResult->setResource(pNewResource);
      *ppSliceStack = pResult;

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
