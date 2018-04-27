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

COM Interface Implementation for Model Writer Class

--*/

#include <cstring>

#include "Model/COM/NMR_COMInterface_ModelWriter.h"
#include "Model/Writer/NMR_ModelWriter.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_ExportStream_COM.h"
#endif

#include "Common/Platform/NMR_ExportStream_Memory.h"
#include "Common/Platform/NMR_ExportStream_Dummy.h"
#include "Common/Platform/NMR_ExportStream_Callback.h"
#include <locale.h>


namespace NMR {

	CCOMModelWriter::CCOMModelWriter()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	void CCOMModelWriter::setWriter(_In_ PModelWriter pModelWriter)
	{
		m_pModelWriter = pModelWriter;
	}

	LIB3MFRESULT CCOMModelWriter::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelWriter::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelWriter::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}


	LIB3MFMETHODIMP CCOMModelWriter::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	LIB3MFMETHODIMP CCOMModelWriter::WriteToFile(_In_z_ LPCWSTR pwszFilename)
	{

		try {
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			setlocale (LC_ALL, "C");
			PExportStream pStream = fnCreateExportStreamInstance(pwszFilename);
			m_pModelWriter->exportToStream(pStream);

			return handleSuccess();
		}
		catch (CNMRException_Windows & WinException) {
			return handleNMRException(&WinException);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelWriter::WriteToFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			// Convert to UTF16
			std::string sUTF8FileName(pszFilename);
			std::wstring sUTF16FileName = fnUTF8toUTF16(sUTF8FileName);

			setlocale (LC_ALL, "C");

			PExportStream pStream = fnCreateExportStreamInstance(sUTF16FileName.c_str());
			m_pModelWriter->exportToStream(pStream);

			return handleSuccess();
		}
		catch (CNMRException_Windows & WinException) {
			return handleNMRException(&WinException);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelWriter::GetStreamSize(_Out_ ULONG64 * pcbStreamSize)
	{
		try {
			if (pcbStreamSize == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			setlocale(LC_ALL, "C");

			// Write to a special dummy stream just to calculate the size
			PExportStreamDummy pStream = std::make_shared<CExportStreamDummy>();
			m_pModelWriter->exportToStream(pStream);

			*pcbStreamSize = pStream->getDataSize();

			return handleSuccess();
		}
		catch (CNMRException_Windows & WinException) {
			return handleNMRException(&WinException);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelWriter::WriteToBuffer(_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			setlocale(LC_ALL, "C");

			PExportStreamMemory pStream = std::make_shared<CExportStreamMemory>();
			m_pModelWriter->exportToStream(pStream);

			ULONG64 cbStreamSize = pStream->getDataSize();
			if (cbBufferSize < cbStreamSize)
				throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

			// TODO eliminate this copy, perhaps by allowing CExportStreamMemory to use existing buffers
			memcpy(pBuffer, pStream->getData(), cbStreamSize);

			return handleSuccess();
		}
		catch (CNMRException_Windows & WinException) {
			return handleNMRException(&WinException);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}

	LIB3MFMETHODIMP CCOMModelWriter::WriteToCallback(_In_ void * pWriteCallback, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData)
	{
		try {
			if (pWriteCallback == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPARAM);

			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			setlocale (LC_ALL, "C");

			ExportStream_WriteCallbackType pTypedWriteCallback = (ExportStream_WriteCallbackType) pWriteCallback;
			ExportStream_SeekCallbackType pTypedSeekCallback = (ExportStream_SeekCallbackType)pSeekCallback;

			PExportStream pStream = std::make_shared<CExportStream_Callback> (pTypedWriteCallback, pTypedSeekCallback, pUserData);
			m_pModelWriter->exportToStream(pStream);
		
			return handleSuccess();
		}
		catch (CNMRException_Windows & WinException) {
			return handleNMRException(&WinException);
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}
	}


#ifdef NMR_COM_NATIVE
	LIB3MFMETHODIMP CCOMModelWriter::WriteToStream(_In_ IStream * pStream)
	{

		try {
			if (pStream == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

			setlocale (LC_ALL, "C");

			PExportStream pExportStream = std::make_shared<CExportStream_COM>(pStream);
			m_pModelWriter->exportToStream(pExportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}
#endif // NMR_COM_NATIVE
	
	LIB3MFMETHODIMP CCOMModelWriter::SetProgressCallback(_In_ void * callback, _In_ void* userData)
	{
		try
		{
			if (m_pModelWriter.get() == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDREADEROBJECT);

			if (!callback)
				m_pModelWriter->SetProgressCallback(nullptr, nullptr);
			else
				m_pModelWriter->SetProgressCallback(reinterpret_cast<Lib3MFProgressCallback>(callback), userData);
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
