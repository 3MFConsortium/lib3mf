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

COM Interface Implementation for Model Reader Class

--*/

#include "Model/COM/NMR_COMInterface_ModelReader.h" 
#include "Model/Reader/NMR_ModelReader.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_Platform.h" 
#include "Common/NMR_StringUtils.h" 
#include <locale.h>

#ifndef __GNUC__
#include "Common/Platform/NMR_ImportStream_COM.h" 
#endif // __GNUC__

namespace NMR {

	CCOMModelReader::CCOMModelReader()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelReader::setReader(_In_ PModelReader pModelReader)
	{
		m_pModelReader = pModelReader;
	}


	LIB3MFRESULT CCOMModelReader::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelReader::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelReader::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}


	LIB3MFMETHODIMP CCOMModelReader::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelReader::ReadFromFile(_In_z_ LPCWSTR pwszFilename)
	{
		try {
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			setlocale (LC_ALL, "C");

			PImportStream pStream = fnCreateImportStreamInstance(pwszFilename);
			m_pModelReader->readStream(pStream);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::ReadFromFileUTF8(_In_z_ LPCSTR pwszFilename)
	{
		try {
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			setlocale (LC_ALL, "C");

			// Convert to UTF16
			std::string sUTF8FileName(pwszFilename);
			std::wstring sUTF16FileName = fnUTF8toUTF16(sUTF8FileName);

			PImportStream pStream = fnCreateImportStreamInstance(sUTF16FileName.c_str());
			m_pModelReader->readStream(pStream);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::GetWarningCount(_Out_ DWORD * pnWarningCount)
	{

		try {
			if (pnWarningCount == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			PModelReaderWarnings pWarnings = m_pModelReader->getWarnings();
			if (pWarnings.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			*pnWarningCount = pWarnings->getWarningCount();
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::GetWarning(_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{

		try {
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			PModelReaderWarnings pWarnings = m_pModelReader->getWarnings();
			if (pWarnings.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			PModelReaderWarning pWarning = pWarnings->getWarning(nIndex);
			__NMRASSERT(pWarning.get() != nullptr);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pWarning->getMessage(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars != nullptr)
				*pcbNeededChars = nNeededChars;
			if (pErrorCode == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			*pErrorCode = pWarning->getErrorCode();

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

#ifndef __GNUC__
	LIB3MFMETHODIMP CCOMModelReader::ReadFromStream(_In_ IStream * pStream)
	{
		try {
			if (pStream == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			setlocale (LC_ALL, "C");
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(pStream);
			m_pModelReader->readStream(pImportStream);
			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}
#endif // __GNUC__


}
