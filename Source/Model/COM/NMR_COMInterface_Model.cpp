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
	}

	LIB3MFMETHODIMP CCOMModel::SetUnit(_In_ DWORD Unit)
	{
		try {
			m_pModel->setUnit((eModelUnit) Unit);
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetUnit(_Out_ DWORD * pUnit)
	{
		if (!pUnit)
			return LIB3MF_POINTER;

		try {
			*pUnit = (DWORD) m_pModel->getUnit();
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::SetLanguage(_In_z_ LPCWSTR pwszLanguage)
	{
		if (!pwszLanguage)
			return LIB3MF_POINTER;

		try {
			std::wstring sLanguage(pwszLanguage);
			m_pModel->setLanguage(pwszLanguage);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetLanguage(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(m_pModel->getLanguage(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars)
				*pcbNeededChars = nNeededChars;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::QueryWriter(_In_z_ LPCWSTR pwszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter)
	{
		if ((!ppWriter) || (!pwszWriterClass))
			return LIB3MF_POINTER;

		try {
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
				return LIB3MF_INVALIDARG;

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelWriter> * pCOMObject = new CCOMObject<CCOMModelWriter>();
			pCOMObject->setWriter(pWriter);
			*ppWriter = pCOMObject;

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::QueryReader(_In_z_ LPCWSTR pwszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader)
	{
		if ((!ppReader) || (!pwszReaderClass))
			return LIB3MF_POINTER;

		try {
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
				return LIB3MF_INVALIDARG;

			// Create COM Object, and set writer property
			CCOMObject<CCOMModelReader> * pCOMObject = new CCOMObject<CCOMModelReader>();
			pCOMObject->setReader(pReader);
			*ppReader = pCOMObject;

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetResourceByID(_In_ DWORD ResourceID, _Outptr_ ILib3MFModelResource ** ppResource)
	{
		if (!ppResource)
			return LIB3MF_POINTER;

		try {
			PModelResource pResource = m_pModel->findResource(ResourceID);
			if (!pResource)
				return LIB3MF_FAIL;

			fnCreateModelResourceFromClass(pResource, true, ppResource);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetBuildItems(_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator)
	{
		if (!ppIterator)
			return LIB3MF_POINTER;

		try {
			CCOMObject<CCOMModelBuildItemIterator> * pResult = new CCOMObject<CCOMModelBuildItemIterator>();

			nfUint32 nBuildItemCount = m_pModel->getBuildItemCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nBuildItemCount; nIdx++)
				pResult->addBuildItem(m_pModel->getBuildItem(nIdx));

			*ppIterator = pResult;

			return LIB3MF_OK;

		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetResources(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		if (!ppIterator)
			return LIB3MF_POINTER;

		try {
			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();

			nfUint32 nResourceCount = m_pModel->getResourceCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nResourceCount; nIdx++)
				pResult->addResource(m_pModel->getResource(nIdx));

			*ppIterator = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		if (!ppIterator)
			return LIB3MF_POINTER;

		try {
			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++)
				pResult->addResource(m_pModel->getObjectResource(nIdx));

			*ppIterator = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetMeshObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		if (!ppIterator)
			return LIB3MF_POINTER;

		try {
			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++) {
				PModelResource pResource = m_pModel->getObjectResource(nIdx);
				if (dynamic_cast<CModelMeshObject *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::GetComponentsObjects(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{
		if (!ppIterator)
			return LIB3MF_POINTER;

		try {
			CCOMObject<CCOMModelResourceIterator> * pResult = new CCOMObject<CCOMModelResourceIterator>();

			nfUint32 nObjectCount = m_pModel->getObjectCount();
			nfUint32 nIdx;

			for (nIdx = 0; nIdx < nObjectCount; nIdx++) {
				PModelResource pResource = m_pModel->getObjectResource(nIdx);
				if (dynamic_cast<CModelComponentsObject *> (pResource.get()) != nullptr)
					pResult->addResource(pResource);
			}

			*ppIterator = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddMeshObject(_Outptr_ ILib3MFModelMeshObject ** ppMeshObject)
	{
		if (!ppMeshObject)
			return LIB3MF_POINTER;

		try {
			ModelResourceID NewResourceID = m_pModel->generateResourceID ();
			PMesh pNewMesh = std::make_shared<CMesh>();
			PModelMeshObject pNewResource = std::make_shared<CModelMeshObject>(NewResourceID, m_pModel.get(), pNewMesh);

			m_pModel->addResource(pNewResource);

			CCOMObject<CCOMModelMeshObject> * pResult = new CCOMObject<CCOMModelMeshObject>();
			pResult->setResource(pNewResource);
			*ppMeshObject = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddComponentsObject (_Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject)
	{
		if (!ppComponentsObject)
			return LIB3MF_POINTER;

		try {
			ModelResourceID NewResourceID = m_pModel->generateResourceID();
			PModelComponentsObject pNewResource = std::make_shared<CModelComponentsObject>(NewResourceID, m_pModel.get());

			m_pModel->addResource(pNewResource);

			CCOMObject<CCOMModelComponentsObject> * pResult = new CCOMObject<CCOMModelComponentsObject>();
			pResult->setResource(pNewResource);
			*ppComponentsObject = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::AddBuildItem (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform,_Outptr_ ILib3MFModelBuildItem ** ppBuildItem)
	{
		if ((!pObject) || (!ppBuildItem))
			return LIB3MF_POINTER;

		try {
			// Get Resource ID
			ModelResourceID nObjectID;
			HRESULT hResult = pObject->GetResourceID(&nObjectID);
			if (hResult != LIB3MF_OK)
				return hResult;

			// Find class instance
			CModelObject * pObject = m_pModel->findObject(nObjectID);
			if (pObject == nullptr)
				return LIB3MF_FAIL;

			// Create Build item instance
			PModelBuildItem pModelBuildItem= std::make_shared<CModelBuildItem>(pObject);
			m_pModel->addBuildItem(pModelBuildItem);

			// Create COM Wrapper
			CCOMObject<CCOMModelBuildItem> * pResult = new CCOMObject<CCOMModelBuildItem>();
			pResult->setItem(pModelBuildItem);
			*ppBuildItem = pResult;

			// Set transform, if necessary
			if (pTransform)
				pResult->SetObjectTransform(pTransform);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::RemoveBuildItem(_In_ ILib3MFModelBuildItem * pBuildItem)
	{
		if (!pBuildItem)
			return LIB3MF_POINTER;

		try {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModel::MergeToModel(_Outptr_ ILib3MFModel ** ppMergedModel)
	{
		if (!ppMergedModel)
			return LIB3MF_POINTER;

		try {
			// Create merged mesh
			PMesh pMesh = std::make_shared<CMesh>();
			m_pModel->mergeToMesh(pMesh.get());

			ILib3MFModel * pNewModel = new CCOMObject<CCOMModel>();
			*ppMergedModel = pNewModel;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

}
