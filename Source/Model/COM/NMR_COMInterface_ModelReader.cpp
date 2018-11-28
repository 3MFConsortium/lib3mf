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

COM Interface Implementation for Model Reader Class

--*/

#include "Model/COM/NMR_COMInterface_ModelReader.h" 
#include "Model/Reader/NMR_ModelReader.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/Platform/NMR_Platform.h" 
#include "Common/NMR_StringUtils.h" 
#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Common/Platform/NMR_ImportStream_Callback.h" 
#include <locale.h>

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

			std::string sUTF8FileName = fnUTF16toUTF8(pwszFilename);
			PImportStream pStream = fnCreateImportStreamInstance(sUTF8FileName.c_str());
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

	LIB3MFMETHODIMP CCOMModelReader::ReadFromFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			setlocale (LC_ALL, "C");

			std::string sUTF8FileName(pszFilename);

			PImportStream pStream = fnCreateImportStreamInstance(sUTF8FileName.c_str());
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

	LIB3MFMETHODIMP CCOMModelReader::ReadFromBuffer(_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			PImportStream pStream = std::make_shared<CImportStream_Memory>(pBuffer, cbBufferSize);
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

	LIB3MFMETHODIMP CCOMModelReader::ReadFromCallback (_In_ void * pReadCallback, _In_ nfUint64 nStreamSize, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData)
	{
		try {
			if (pReadCallback == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			setlocale(LC_ALL, "C");

			PImportStream pStream = std::make_shared<CImportStream_Callback> (
				reinterpret_cast<ImportStream_ReadCallbackType>(pReadCallback), 
				reinterpret_cast<ImportStream_SeekCallbackType>(pSeekCallback), pUserData, nStreamSize);
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


	LIB3MFMETHODIMP CCOMModelReader::AddRelationToRead(_In_z_ LPCWSTR pwszRelationshipType)
	{
		try {
			if (pwszRelationshipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			std::wstring sRelationShipType(pwszRelationshipType);
			m_pModelReader->addRelationToRead(fnUTF16toUTF8(sRelationShipType));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::RemoveRelationToRead (_In_z_ LPCWSTR pwszRelationshipType)
	{
		try {
			if (pwszRelationshipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			std::wstring sRelationShipType(pwszRelationshipType);
			m_pModelReader->removeRelationToRead(fnUTF16toUTF8(sRelationShipType));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::AddRelationToReadUTF8(_In_z_ LPCSTR pszRelationshipType)
	{
		try {
			if (pszRelationshipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			std::string sRelationShipTypeUTF8(pszRelationshipType);
			m_pModelReader->addRelationToRead(sRelationShipTypeUTF8);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::RemoveRelationToReadUTF8(_In_z_ LPCSTR pszRelationshipType)
	{
		try {
			if (pszRelationshipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			std::string sRelationShipTypeUTF8(pszRelationshipType);
			m_pModelReader->removeRelationToRead(sRelationShipTypeUTF8);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::SetStrictModeActive(_In_ BOOL bStrictModeActive)
	{
		try {
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			if (m_pModelReader->getWarnings() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADERWARNINGSOBJECT);

			if (bStrictModeActive)
				m_pModelReader->getWarnings()->setCriticalWarningLevel(mrwInvalidOptionalValue);
			else
				m_pModelReader->getWarnings()->setCriticalWarningLevel(mrwFatal);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::GetStrictModeActive(_In_ BOOL *pbStrictModeActive)
	{
		try {
			if (pbStrictModeActive == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			if (m_pModelReader->getWarnings() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADERWARNINGSOBJECT);
			
			*pbStrictModeActive = m_pModelReader->getWarnings()->getCriticalWarningLevel() == mrwInvalidOptionalValue;

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
			fnWStringToBufferSafe(fnUTF8toUTF16(pWarning->getMessage()), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelReader::GetWarningUTF8(_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
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
			fnStringToBufferSafe(pWarning->getMessage(), pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelReader::SetProgressCallback(_In_ void * callback, _In_ void* userData)
	{
		try
		{
			if (m_pModelReader.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			if (!callback)
				m_pModelReader->SetProgressCallback(nullptr, nullptr);
			else
				m_pModelReader->SetProgressCallback(reinterpret_cast<Lib3MFProgressCallback>(callback), userData);
			return  handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

}
