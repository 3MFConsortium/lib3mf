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

COM Interface Implementation for Model Build Item Iterator Classes

--*/

#include "Model/COM/NMR_COMInterface_ModelBuildItemIterator.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CCOMModelBuildItemIterator::CCOMModelBuildItemIterator()
	{
		m_nCurrentIndex = -1;
		m_nErrorCode = NMR_SUCCESS;
	}

	LIB3MFRESULT CCOMModelBuildItemIterator::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelBuildItemIterator::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelBuildItemIterator::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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


	void CCOMModelBuildItemIterator::addBuildItem(_In_ PModelBuildItem pBuildItem)
	{
		if (pBuildItem.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_pBuildItems.size() >= XML_3MF_MAXBUILDITEMCOUNT)
			throw CNMRException(NMR_ERROR_INVALIDBUILDITEMCOUNT);

		m_pBuildItems.push_back(pBuildItem);
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::MoveNext(_Out_ BOOL * pbHasNext)
	{
		try {
			if (pbHasNext == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Get Resource Count
			nfInt32 nBuildItemCount = (nfInt32)m_pBuildItems.size();
			m_nCurrentIndex++;

			// Check new Index
			if (m_nCurrentIndex >= nBuildItemCount) {
				*pbHasNext = false;
				m_nCurrentIndex = nBuildItemCount;
			}
			else {
				*pbHasNext = true;
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

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::MovePrevious(_Out_ BOOL * pbHasPrevious)
	{
		try {
			if (pbHasPrevious == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Get Resource Count
			m_nCurrentIndex--;

			// Check new Index
			if (m_nCurrentIndex <= -1) {
				*pbHasPrevious = false;
				m_nCurrentIndex = -1;
			}
			else {
				*pbHasPrevious = true;
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

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::GetCurrent(_Outptr_ ILib3MFModelBuildItem ** ppResultBuildItem)
	{
		try {
			if (ppResultBuildItem == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Get Resource Count
			nfInt32 nBuildItemCount = (nfInt32)m_pBuildItems.size();
			if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nBuildItemCount))
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			// Create specific COM classes
			PModelBuildItem pBuildItem = m_pBuildItems[m_nCurrentIndex];

			CCOMObject<CCOMModelBuildItem> * pResult = new CCOMObject<CCOMModelBuildItem>();
			pResult->AddRef();
			pResult->setItem(pBuildItem);

			// Return result
			*ppResultBuildItem = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::Clone(_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator)
	{
		try {
			if (ppIterator == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject <CCOMModelBuildItemIterator> * pComObject = new CCOMObject <CCOMModelBuildItemIterator>();
			pComObject->AddRef();
			for (auto iIterator = m_pBuildItems.begin(); iIterator != m_pBuildItems.end(); iIterator++)
				pComObject->addBuildItem(*iIterator);

			*ppIterator = pComObject;

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
