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

COM Interface Implementation for Slice and Slicestack Classes

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
#include "Model/COM/NMR_COMInterface_Slice.h"

#ifdef NMR_COM_NATIVE
#include "Model/Reader/NMR_ModelReader_3MF_OPC.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h"
#else
#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h"
#endif

#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelComponentsObject.h"
#include "Model/Reader/NMR_ModelReader_STL.h"
#include "Model/Writer/NMR_ModelWriter_STL.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"
#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_COM_Native.h"
#else
#include "Common/Platform/NMR_COM_Emulation.h"
#endif

#include <string.h>

namespace NMR {

	_Ret_notnull_ CSliceStack * CCOMSliceStack::getSliceStack()
	{
		if (m_pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

		CModelSliceStackResource * pSliceStackResource = dynamic_cast<CModelSliceStackResource *> (m_pResource.get());
		if (pSliceStackResource == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSLICESTACK);

		CSliceStack* pSliceStack = pSliceStackResource->getSliceStack().get();
		if (pSliceStackResource == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDSLICESTACK);
		return pSliceStack;
	}

	LIB3MFMETHODIMP CCOMSliceStack::AddSlice(_In_ FLOAT fTopZ, _Outptr_ ILib3MFSlice ** ppSliceObject) {
		try {
			if (!ppSliceObject)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PSlice pSlice = std::make_shared<CSlice>();
			pSlice->setTopZ(fTopZ);

			CCOMObject<CCOMSlice> *pResult = new CCOMObject<CCOMSlice>();
			getSliceStack()->addSlice(pSlice);
			pResult->AddRef();
			pResult->setSlice(pSlice);

			*ppSliceObject = pResult;

			return LIB3MF_OK;
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSliceStack::GetSliceCount(_Out_ DWORD * pnSliceCount) {
		try {
			*pnSliceCount = getSliceStack()->getSliceCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSliceStack::GetSlice(_In_ DWORD nSliceIndex, _Outptr_ ILib3MFSlice **ppSliceObject) {
		try {
			CCOMObject<CCOMSlice> *pResult = new CCOMObject<CCOMSlice>();
			pResult->AddRef();
			pResult->setSlice(getSliceStack()->getSlice(nSliceIndex));
			*ppSliceObject = pResult;
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSliceStack::GetBottomZ(_Out_ FLOAT * pfBottomZ) {
		try {
			*pfBottomZ = getSliceStack()->getBottomZ();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}
	
	LIB3MFMETHODIMP CCOMSliceStack::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage) {
		try {
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSliceStack::GetResourceID(_Out_ DWORD * pnResourceID) {
		try {
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

	LIB3MFMETHODIMP CCOMSliceStack::SetUsesSliceRef(_In_ BOOL bUsesSliceRef) {
		try {
			getSliceStack()->setUsesSliceRef(bUsesSliceRef!=0);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSliceStack::GetUsesSliceRef(_Out_ BOOL* pbUsesSliceRef) {
		try {
			if (!pbUsesSliceRef)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);
			*pbUsesSliceRef = getSliceStack()->usesSliceRef();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	void CCOMSliceStack::setResource(_In_ PModelResource pModelResource)
	{
		m_pResource = pModelResource;
	}

	CCOMSliceStack::CCOMSliceStack() {
		m_nErrorCode = NMR_SUCCESS;
	}

	LIB3MFRESULT CCOMSliceStack::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMSliceStack::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMSliceStack::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}



	LIB3MFRESULT CCOMSlice::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMSlice::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMSlice::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMSlice::AddVertex(_In_ MODELSLICEVERTEX * pVertex, _Out_opt_ DWORD * pnIndex) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*pnIndex = m_pSlice->addVertex(pVertex->m_fPosition[0], pVertex->m_fPosition[1]);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::BeginPolygon(_Out_ DWORD *nPolygonIndex) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*nPolygonIndex = m_pSlice->beginPolygon();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::AddNode(_In_ float x, _In_ float y, _Out_ DWORD * nNodeIndex) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*nNodeIndex = m_pSlice->addVertex(x, y);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::AddPolygonIndices(_In_ DWORD nPolygonIndex, _In_ DWORD *pnVertexIndices, _In_ DWORD nBufferSize, _Out_ DWORD * nPolygonVertexIndex) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pSlice->getPolygonCount() < nPolygonIndex)
				throw CNMRException(NMR_ERROR_INVALIDSLICEPOLYGON);

			*nPolygonVertexIndex = m_pSlice->getPolygonIndexCount(nPolygonIndex);
			for (nfUint32 i = 0; i < nBufferSize; i++) {
				m_pSlice->addPolygonIndex(nPolygonIndex, (nfUint32)pnVertexIndices[i]);
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

	LIB3MFMETHODIMP CCOMSlice::AddPolygonIndex(_In_ DWORD nPolygonIndex, _In_ DWORD nVertexIndex) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pSlice->getPolygonCount() < nPolygonIndex)
				throw CNMRException(NMR_ERROR_INVALIDSLICEPOLYGON);

			m_pSlice->addPolygonIndex(nPolygonIndex, nVertexIndex);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::GetPolygonCount(_Out_ DWORD *nPolygonCount) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*nPolygonCount = m_pSlice->getPolygonCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::GetTopZ(_Out_ float * nTopZ) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			*nTopZ = m_pSlice->getTopZ();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::SetTopZ(_In_ float nTopZ) {
		try {
			if (!m_pSlice)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pSlice->setTopZ(nTopZ);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage) {
		try {
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	void CCOMSlice::setSlice(_In_ PSlice pSlice) {
		m_pSlice = pSlice;
	}

	CCOMSlice::CCOMSlice() {

	}

	LIB3MFMETHODIMP CCOMSlice::GetIndexCountOfPolygon(_In_ DWORD nPolygonIndex, _Out_ DWORD *pnPolygonCount) {
		try {
			if (nPolygonIndex >= m_pSlice->getPolygonCount())
				throw CNMRException(LIB3MF_INVALIDARG);

			*pnPolygonCount = m_pSlice->getPolygonIndexCount(nPolygonIndex);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::GetPolygonIndices(_In_ DWORD nPolygonIndex, _Out_ DWORD *pPolygonIndices, _In_ DWORD nBufferSize) {
		try {
			if (nPolygonIndex >= m_pSlice->getPolygonCount())
				throw CNMRException(LIB3MF_INVALIDARG);

			for (nfUint32 i = 0; i < nBufferSize && i < m_pSlice->getPolygonIndexCount((nfUint32)nPolygonIndex); i++) {
				pPolygonIndices[i] = (DWORD)m_pSlice->getPolygonIndex(nPolygonIndex, i);
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


	LIB3MFMETHODIMP CCOMSlice::GetVertex(_In_ DWORD nIndex, _Out_ MODELSLICEVERTEX *pVertex) {
		try {
			m_pSlice->getVertex(nIndex, &pVertex->m_fPosition[0], &pVertex->m_fPosition[1]);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMSlice::GetVertexCount(_Out_ DWORD *pnVertexCount) {
		try {
			*pnVertexCount = m_pSlice->getVertexCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	CSlice *CCOMSlice::getSlice() {
		return m_pSlice.get();
	}

}
