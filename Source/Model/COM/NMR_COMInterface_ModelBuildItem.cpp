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
	}

	void CCOMModelBuildItem::setItem(_In_ PModelBuildItem pModelBuildItem)
	{
		m_pModelBuildItem = pModelBuildItem;
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::HasObjectTransform(_Out_ BOOL * pbHasTransform)
	{
		if (!pbHasTransform)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			*pbHasTransform = m_pModelBuildItem->hasTransform();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectResourceID(_Out_ DWORD * pnID)
	{
		if (!pnID)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			*pnID = m_pModelBuildItem->getObjectID();

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectResource(_Outptr_ ILib3MFModelObjectResource ** ppObject)
	{
		if (!ppObject)
			return LIB3MF_POINTER;

		try {
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
				return LIB3MF_FAIL;

			// Return result
			*ppObject = pResourceInterface;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetObjectTransform(_Out_ MODELTRANSFORM * pmTransform)
	{
		if (!pmTransform)
			return LIB3MF_POINTER;

		try {
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			NMATRIX3 mMatrix = m_pModelBuildItem->getTransform();
			int i, j;

			for (i = 0; i < 3; i++) {
				for (j = 0; j < 4; j++) {
					pmTransform->m_fFields[i][j] = mMatrix.m_fields[i][j];
				}
			}

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::SetObjectTransform(_In_ MODELTRANSFORM * pmTransform)
	{
		if (!pmTransform)
			return LIB3MF_POINTER;

		try {
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

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::SetPartNumber(_In_z_ LPCWSTR pwszPartNumber)
	{
		if (!pwszPartNumber)
			return LIB3MF_POINTER;

		try	{
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			std::wstring sPartNumber(pwszPartNumber);
			m_pModelBuildItem->setPartNumber(sPartNumber);

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItem::GetPartNumber(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try	{
			if (!m_pModelBuildItem.get())
				throw CNMRException(NMR_ERROR_INVALIDBUILDITEM);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(m_pModelBuildItem->getPartNumber(), pwszBuffer, cbBufferSize, &nNeededChars);

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
