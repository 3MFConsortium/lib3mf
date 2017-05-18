/*++

Copyright (C) 2017 Autodesk Inc.

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

COM Interface Implementation for Model MeshBeam Set

--*/

#include "Model/COM/NMR_COMInterface_ModelMeshBeamSet.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCOMModelMeshBeamSet::CCOMModelMeshBeamSet()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	CCOMModelMeshBeamSet::~CCOMModelMeshBeamSet() {
		m_nErrorCode = NMR_SUCCESS;
	}
	
	void CCOMModelMeshBeamSet::setBeamSet(_In_ PBEAMSET pBeamSet)
	{
		if (!pBeamSet)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_pBeamSet = pBeamSet;
	}

	BEAMSET* CCOMModelMeshBeamSet::getBeamSet()
	{
		if (m_pBeamSet.get() == NULL)
			throw CNMRException(NMR_ERROR_INVALIDMESH);

		return m_pBeamSet.get();
	}
	
	LIB3MFRESULT CCOMModelMeshBeamSet::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelMeshBeamSet::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelMeshBeamSet::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}


	LIB3MFMETHODIMP CCOMModelMeshBeamSet::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::SetName(_In_z_ LPCWSTR pwszName) {
		BEAMSET* pBeamSet = getBeamSet();
		__NMRASSERT(pBeamSet);

		pBeamSet->m_sName = fnUTF16toUTF8(pwszName);
		return LIB3MF_OK;
	}
	LIB3MFMETHODIMP CCOMModelMeshBeamSet::SetNameUTF8(_In_z_ LPCSTR pszName) {
		BEAMSET* pBeamSet = getBeamSet();
		__NMRASSERT(pBeamSet);

		pBeamSet->m_sName = pszName;
		return LIB3MF_OK;
	}

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::SetIdentifier(_In_z_ LPCWSTR pwszIdentifier) {
		BEAMSET* pBeamSet = getBeamSet();
		__NMRASSERT(pBeamSet);

		pBeamSet->m_sIdentifier = fnUTF16toUTF8(pwszIdentifier);
		return LIB3MF_OK;
	}
	LIB3MFMETHODIMP CCOMModelMeshBeamSet::SetIdentifierUTF8(_In_z_ LPCSTR pszIdentifier) {
		BEAMSET* pBeamSet = getBeamSet();
		__NMRASSERT(pBeamSet);

		pBeamSet->m_sIdentifier = pszIdentifier;
		return LIB3MF_OK;
	}

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::GetNameUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			BEAMSET* pBeamSet = getBeamSet();
			__NMRASSERT(pBeamSet);

			std::string sUTF8Name = pBeamSet->m_sName;

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

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::GetIdentifierUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		try {
			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			BEAMSET* pBeamSet = getBeamSet();
			__NMRASSERT(pBeamSet);

			std::string sUTF8Id = pBeamSet->m_sIdentifier;

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Id, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::GetRefCount(_Out_ DWORD * pnCount)
	{
		try {
			if (!pnCount)
				return LIB3MF_POINTER;

			BEAMSET* pBeamSet = getBeamSet();
			__NMRASSERT(pBeamSet);

			*pnCount = (DWORD)(pBeamSet->m_Refs.size());

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::SetRefs(_In_ DWORD * pRefs, _In_ DWORD nRefCount)
	{
		try {
			if (!pRefs)
				return LIB3MF_POINTER;

			BEAMSET* pBeamSet = getBeamSet();
			__NMRASSERT(pBeamSet);

			pBeamSet->m_Refs.resize(nRefCount);
			for (DWORD i = 0; i < nRefCount; i++) {
				m_pBeamSet->m_Refs[i] = nfUint32(pRefs[i]);
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

	LIB3MFMETHODIMP CCOMModelMeshBeamSet::GetRefs(_Out_ DWORD * pRefs, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnRefCount)
	{
		try {
			if (!pRefs)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			BEAMSET* pBeamSet = getBeamSet();
			__NMRASSERT(pBeamSet);

			// Check Buffer size
			DWORD nRefCount = (DWORD)pBeamSet->m_Refs.size();
			if (pnRefCount)
				*pnRefCount = nRefCount;

			if (nBufferSize < nRefCount)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			for (DWORD nIndex = 0; nIndex < nRefCount; nIndex++) {
				pRefs[nIndex] = pBeamSet->m_Refs[nIndex];
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

}
