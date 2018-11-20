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

COM Interface Implementation for Model Resource Iterator Class

--*/

#include "Model/COM/NMR_COMInterface_ModelResourceIterator.h" 
#include "Model/COM/NMR_COMInterface_ModelResourceFactory.h" 
#include "Common/NMR_Exception_Windows.h" 

namespace NMR {

	CCOMModelResourceIterator::CCOMModelResourceIterator()
	{
		m_nCurrentIndex = -1;
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelResourceIterator::addResource(_In_ PModelResource pResource)
	{
		if (pResource.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pResources.push_back(pResource);
	}

	LIB3MFRESULT CCOMModelResourceIterator::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelResourceIterator::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelResourceIterator::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}


	LIB3MFMETHODIMP CCOMModelResourceIterator::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelResourceIterator::MoveNext (_Out_ BOOL * pbHasNext)
	{
		
		try {
			if (pbHasNext == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			// Get Resource Count
			nfInt32 nResourceCount = (nfInt32)m_pResources.size();
			m_nCurrentIndex++;

			// Check new Index
			if (m_nCurrentIndex >= nResourceCount) {
				*pbHasNext = false;
				m_nCurrentIndex = nResourceCount;
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

	LIB3MFMETHODIMP CCOMModelResourceIterator::MovePrevious(_Out_ BOOL * pbHasPrevious)
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

	LIB3MFMETHODIMP CCOMModelResourceIterator::GetCurrent(_Outptr_ ILib3MFModelResource ** ppResultResource)
	{

		try {
			if (ppResultResource == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);


			// default return 
			ILib3MFModelResource * pResult = nullptr;

			// Get Resource Count
			nfInt32 nResourceCount = (nfInt32)m_pResources.size();
			if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nResourceCount))
				throw CNMRException(NMR_ERROR_INVALIDINDEX);

			// Cast to specific COM classes
			PModelResource pResource = m_pResources[m_nCurrentIndex];

			// Return Mesh COM Object
			fnCreateModelResourceFromClass(pResource, false, &pResult);

			// We have not found a suitable object class to return..
			if (pResult == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			// Return result
			*ppResultResource = pResult;

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelResourceIterator::Clone(_Outptr_ ILib3MFModelResourceIterator ** ppIterator)
	{

		try {
			if (ppIterator == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			CCOMObject <CCOMModelResourceIterator> * pComObject = new CCOMObject <CCOMModelResourceIterator>();
			pComObject->AddRef();

			for (auto iIterator = m_pResources.begin(); iIterator != m_pResources.end(); iIterator++)
				pComObject->addResource(*iIterator);

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

