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

#ifndef __GCC
#include "Model/Reader/NMR_ModelReader_3MF_OPC.h"
#include "Model/Writer/NMR_ModelWriter_3MF_OPC.h"
#else
#include "Model/Reader/NMR_ModelReader_3MF_GCC.h"
#include "Model/Writer/NMR_ModelWriter_3MF_GCC.h"
#endif

#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Reader/NMR_ModelReader_STL.h"
#include "Model/Writer/NMR_ModelWriter_STL.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception_Windows.h"

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

			std::wstring sLanguage(pwszLanguage);
			m_pModel->setLanguage(pwszLanguage);

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
			fnWStringToBufferSafe(m_pModel->getLanguage(), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModel::QueryWriter(_In_z_ LPCWSTR pwszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter)
	{
		try {
			if ((!ppWriter) || (!pwszWriterClass))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Create Writer Object
			PModelWriter pWriter = nullptr;

			// Create specified writer instance
			if (wcscmp(pwszWriterClass, MODELWRITERCLASS_3MF) == 0) {
#ifndef __GCC
				pWriter = std::make_shared<CModelWriter_3MF_OPC>(m_pModel);
#else
				pWriter = std::make_shared<CModelWriter_3MF_GCC>(m_pModel);
#endif
			}
			if (wcscmp(pwszWriterClass, MODELWRITERCLASS_STL) == 0)
				pWriter = std::make_shared<CModelWriter_STL>(m_pModel);

			if (!pWriter)
				throw CNMRException(NMR_ERROR_UNKNOWNWRITERCLASS);

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelWriter> * pCOMObject = new CCOMObject<CCOMModelWriter>();
			pCOMObject->setWriter(pWriter);
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

	LIB3MFMETHODIMP CCOMModel::QueryReader(_In_z_ LPCWSTR pwszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader)
	{
		try {
			if ((!ppReader) || (!pwszReaderClass))
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Create Reader Object
			PModelReader pReader = nullptr;

			// Create specified writer instance
			if (wcscmp(pwszReaderClass, MODELREADERCLASS_3MF) == 0) {
#ifndef __GCC
				pReader = std::make_shared<CModelReader_3MF_OPC>(m_pModel);
#else
				pReader = std::make_shared<CModelReader_3MF_GCC>(m_pModel);
#endif // __GCC
			}
			if (wcscmp(pwszReaderClass, MODELREADERCLASS_STL) == 0)
				pReader = std::make_shared<CModelReader_STL>(m_pModel);

			if (!pReader)
				throw CNMRException(NMR_ERROR_UNKNOWNREADERCLASS);

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelReader> * pCOMObject = new CCOMObject<CCOMModelReader>();
			pCOMObject->setReader(pReader);
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


	LIB3MFMETHODIMP CCOMModel::GetBuildItems(_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject<CCOMModelBuildItemIterator> * pResult = new CCOMObject<CCOMModelBuildItemIterator>();

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
			ModelResourceID nObjectID = 0;
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
			*ppMergedModel = pNewModelInterface;

			// Copy relevant resources to new model!
			CModel * pNewModel = pNewModelInterface->getModel();
			if (pNewModel == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDMODEL);

			pNewModel->mergeTextureStreams(m_pModel.get());
			pNewModel->mergeTextures2D(m_pModel.get());
			pNewModel->mergeBaseMaterials(m_pModel.get());
			pNewModel->mergeMetaData(m_pModel.get());


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

	LIB3MFMETHODIMP CCOMModel::GetThumbnails(_Outptr_ ILib3MFModelThumbnailIterator ** ppIterator)
	{
		try {
			if (!ppIterator)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
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

	LIB3MFMETHODIMP CCOMModel::AddTexture2D(_In_z_ LPCWSTR pwszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance)
	{
		try {
			if (pwszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (!ppTextureInstance)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PModelTexture2DResource pResource = std::make_shared<CModelTexture2DResource>(m_pModel->generateResourceID(), m_pModel.get());
			m_pModel->addResource(pResource);

			std::wstring sPath(pwszPath);
			pResource->setPath(sPath);

			CCOMObject<CCOMModelTexture2D> * pCOMObject = new CCOMObject<CCOMModelTexture2D>();
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

	LIB3MFMETHODIMP CCOMModel::GetTextureStreamCount(_Out_ DWORD * pnCount)
	{
		try {
			if (!pnCount)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			
			*pnCount = m_pModel->getTextureStreamCount();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModel::GetTextureStreamSize(_In_ DWORD nIndex, _Out_ UINT64 * pnSize)
	{
		try {
			if (!pnSize)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			nfUint32 nCount = m_pModel->getTextureStreamCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			PImportStream pStream = m_pModel->getTextureStream(nIndex);
			if (pStream.get() != nullptr) {
				*pnSize = pStream->retrieveSize();
			}
			else {
				*pnSize = 0;
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

	LIB3MFMETHODIMP CCOMModel::GetTextureStreamPath(_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getTextureStreamCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::wstring sPath = m_pModel->getTextureStreamPath(nIndex);
			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(sPath, pwszBuffer, cbBufferSize, &nNeededChars);

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

			std::wstring sName;
			std::wstring sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);
	
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

	LIB3MFMETHODIMP CCOMModel::GetMetaDataValue (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			nfUint32 nCount = m_pModel->getMetaDataCount();
			if (nIndex > nCount)
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			std::wstring sName;
			std::wstring sValue;
			m_pModel->getMetaData(nIndex, sName, sValue);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(sValue, pwszBuffer, cbBufferSize, &nNeededChars);

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

			std::wstring sName (pszwKey);
			std::wstring sValue (pszwValue);

			m_pModel->addMetaData(sName, sValue);
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



}
