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

COM Interface Implementation for Model Build Item Iterator Classes

--*/

#include "Model/COM/NMR_COMInterface_ModelBuildItemIterator.h" 
#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception_Windows.h" 

namespace NMR {

	CCOMModelBuildItemIterator::CCOMModelBuildItemIterator()
	{
		m_nCurrentIndex = -1;
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
		if (pbHasNext == nullptr)
			return LIB3MF_POINTER;

		try {
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

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::MovePrevious(_Out_ BOOL * pbHasPrevious)
	{
		if (pbHasPrevious == nullptr)
			return LIB3MF_POINTER;

		try {
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

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::GetCurrent(_Outptr_ ILib3MFModelBuildItem ** ppResultBuildItem)
	{
		if (ppResultBuildItem == nullptr)
			return LIB3MF_POINTER;

		try {
			// Get Resource Count
			nfInt32 nBuildItemCount = (nfInt32)m_pBuildItems.size();
			if ((m_nCurrentIndex < 0) || (m_nCurrentIndex >= nBuildItemCount))
				return LIB3MF_FAIL;

			// Create specific COM classes
			PModelBuildItem pBuildItem = m_pBuildItems[m_nCurrentIndex];

			CCOMObject<CCOMModelBuildItem> * pResult = new CCOMObject<CCOMModelBuildItem>();
			pResult->setItem(pBuildItem);

			// Return result
			*ppResultBuildItem = pResult;

			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelBuildItemIterator::Clone(_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator)
	{
		if (ppIterator == nullptr)
			return LIB3MF_POINTER;

		try {
			CCOMObject <CCOMModelBuildItemIterator> * pComObject = new CCOMObject <CCOMModelBuildItemIterator>();
			for (auto iIterator = m_pBuildItems.begin(); iIterator != m_pBuildItems.end(); iIterator++)
				pComObject->addBuildItem(*iIterator);

			*ppIterator = pComObject;
			
			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

}
