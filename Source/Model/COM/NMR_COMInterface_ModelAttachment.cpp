/*++

Copyright (C)  Microsoft Corporation.
Copyright (C) netfabb GmbH (Original Author)

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

COM Interface Implementation for Model Classes

--*/


#include "Model/COM/NMR_COMInterface_ModelAttachment.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ExportStream_Callback.h"

#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "Common/Platform/NMR_ImportStream_Memory.h"

#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_ImportStream_COM.h"
#else
#include "Common/Platform/NMR_ImportStream_GCC_Native.h"
#endif

#ifndef __GNUC__
#include <atlbase.h>
#endif

namespace NMR {

	CCOMModelAttachment::CCOMModelAttachment()
	{
		m_nErrorCode = NMR_SUCCESS;
	}

	LIB3MFRESULT CCOMModelAttachment::handleSuccess()
	{
		m_nErrorCode = NMR_SUCCESS;
		return LIB3MF_OK;
	}

	LIB3MFRESULT CCOMModelAttachment::handleNMRException(_In_ CNMRException * pException)
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

	LIB3MFRESULT CCOMModelAttachment::handleGenericException()
	{
		m_nErrorCode = NMR_ERROR_GENERICEXCEPTION;
		m_sErrorMessage = NMR_GENERICEXCEPTIONSTRING;
		return LIB3MF_FAIL;
	}

	LIB3MFMETHODIMP CCOMModelAttachment::GetLastError(_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage)
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

	
	void CCOMModelAttachment::setAttachment(_In_ PModelAttachment pModelAttachment)
	{
		m_pModelAttachment = pModelAttachment;
	}

	LIB3MFMETHODIMP CCOMModelAttachment::GetPath(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (!m_pModelAttachment.get()) 
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Retrieve Path
			std::string sPath = m_pModelAttachment->getPathURI();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(sPath), pwszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelAttachment::GetPathUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Retrieve Path
			std::string sUTF8Path = m_pModelAttachment->getPathURI();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Path, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelAttachment::SetPath(_In_z_ LPCWSTR pwszPath)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pwszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			std::string sPathUTF8 = fnUTF16toUTF8(pwszPath);
			CModel * pModel = m_pModelAttachment->getModel();
			PImportStream pStream = m_pModelAttachment->getStream();
			if (pModel->getPackageThumbnail() == m_pModelAttachment) {
				// different handling for package-wide attachment
				pModel->removePackageThumbnail();
				m_pModelAttachment = pModel->addPackageThumbnail(sPathUTF8, pStream);
			}
			else {
				std::string sRelationshipType = m_pModelAttachment->getRelationShipType();
				pModel->removeAttachment(m_pModelAttachment->getPathURI());
				m_pModelAttachment = pModel->addAttachment(sPathUTF8, sRelationshipType, pStream);
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

	LIB3MFMETHODIMP CCOMModelAttachment::SetPathUTF8(_In_z_ LPCSTR pszPath)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pszPath == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			std::string sUTF8Path(pszPath);

			CModel * pModel = m_pModelAttachment->getModel();
			PImportStream pStream = m_pModelAttachment->getStream();
			if (pModel->getPackageThumbnail() == m_pModelAttachment) {
				// different handling for package-wide attachment
				pModel->removePackageThumbnail();
				m_pModelAttachment = pModel->addPackageThumbnail(sUTF8Path, pStream);
			}
			else {
				std::string sRelationshipType = m_pModelAttachment->getRelationShipType();
				pModel->removeAttachment(m_pModelAttachment->getPathURI());
				m_pModelAttachment = pModel->addAttachment(sUTF8Path, sRelationshipType, pStream);
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

	LIB3MFMETHODIMP CCOMModelAttachment::GetRelationshipType(_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Retrieve Path
			std::string sType = m_pModelAttachment->getRelationShipType();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(fnUTF8toUTF16(sType), pwszBuffer, cbBufferSize, &nNeededChars);

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
	

	LIB3MFMETHODIMP CCOMModelAttachment::GetRelationshipTypeUTF8(_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

			// Retrieve Path
			std::string sUTF8Type = m_pModelAttachment->getRelationShipType();

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnStringToBufferSafe(sUTF8Type, pszBuffer, cbBufferSize, &nNeededChars);

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

	LIB3MFMETHODIMP CCOMModelAttachment::SetRelationshipType(_In_z_ LPCWSTR pwszRelationShipType)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pwszRelationShipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModelAttachment->setRelationShipType(fnUTF16toUTF8(pwszRelationShipType));

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelAttachment::SetRelationshipTypeUTF8(_In_z_ LPCSTR pszRelationShipType)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pszRelationShipType == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			m_pModelAttachment->setRelationShipType(pszRelationShipType);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelAttachment::GetStreamSize(_Out_ ULONG64 * pcbStreamSize)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			PImportStream pStream = m_pModelAttachment->getStream();

			if (pStream.get() != nullptr) {
				*pcbStreamSize = pStream->retrieveSize();
			}
			else {
				*pcbStreamSize = 0;
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

	LIB3MFMETHODIMP CCOMModelAttachment::WriteToFile(_In_z_ LPCWSTR pwszFilename)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);


			PImportStream pStream = m_pModelAttachment->getStream();

			if (pStream.get() != nullptr) {
				pStream->writeToFile(pwszFilename);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelAttachment::WriteToFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pStream = m_pModelAttachment->getStream();

			if (pStream.get() != nullptr) {
				std::string sUTF8FileName(pszFilename);
				std::wstring sUTF16FileName = fnUTF8toUTF16(sUTF8FileName);

				pStream->writeToFile(sUTF16FileName.c_str());
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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
	
	LIB3MFMETHODIMP CCOMModelAttachment::WriteToBuffer(_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pStream = m_pModelAttachment->getStream();

			if (pStream.get() != nullptr) {
				nfUint64 cbStreamSize = pStream->retrieveSize();
				if (cbStreamSize > cbBufferSize)
					throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

				pStream->seekPosition(0, true);
				pStream->readBuffer(pBuffer, cbStreamSize, true);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

#ifdef NMR_COM_NATIVE
	LIB3MFMETHODIMP CCOMModelAttachment::WriteToStream(_In_ IStream * pCOMStream)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pCOMStream == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pStream = m_pModelAttachment->getStream();

			if (pStream.get() != nullptr) {
				nfUint64 cbStreamSize = pStream->retrieveSize();

				pStream->seekPosition(0, true);
				std::array<nfByte, NMR_IMPORTSTREAM_COPYBUFFERSIZE> pBuffer;

				nfUint64 cbBytesLeft = cbStreamSize;
				while (cbBytesLeft > 0) {
					nfUint64 cbLength = cbBytesLeft;
					if (cbLength > NMR_IMPORTSTREAM_COPYBUFFERSIZE)
						cbLength = NMR_IMPORTSTREAM_COPYBUFFERSIZE;

					ULONG cbWrittenBytes = 0;
					pStream->readBuffer(&pBuffer[0], cbLength, true);
					HRESULT hResult = pCOMStream->Write(&pBuffer[0], (nfUint32)cbLength, &cbWrittenBytes);
					if (hResult != S_OK)
						throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, hResult);

					if (cbWrittenBytes != cbLength)
						throw CNMRException(NMR_ERROR_COULDNOTWRITEFULLDATA);
					cbBytesLeft -= cbLength;
				}

			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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
#endif// NMR_COM_NATIVE

	LIB3MFMETHODIMP CCOMModelAttachment::WriteToCallback(_In_ void * pWriteCallback, _In_opt_ void * pUserData)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pWriteCallback == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pStream = m_pModelAttachment->getStream();
			ExportStream_WriteCallbackType pTypedWriteCallback = (ExportStream_WriteCallbackType)pWriteCallback;

			if (pStream.get() != nullptr) {
				PExportStream pExportStream = std::make_shared<CExportStream_Callback>(pTypedWriteCallback, nullptr, pUserData);
				pExportStream->copyFrom(pStream.get(), pStream->retrieveSize(), MODELATTACHMENT_BUFFERSIZE);
			}
			else {
				throw CNMRException(NMR_ERROR_NOTEXTURESTREAM);
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

	LIB3MFMETHODIMP CCOMModelAttachment::ReadFromFile(_In_z_ LPCWSTR pwszFilename)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pwszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pImportStream = fnCreateImportStreamInstance(pwszFilename);

			m_pModelAttachment->setStream(pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

	LIB3MFMETHODIMP CCOMModelAttachment::ReadFromFileUTF8(_In_z_ LPCSTR pszFilename)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

			if (pszFilename == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			std::string sUTF8FileName(pszFilename);
			std::wstring sUTF16FileName = fnUTF8toUTF16(sUTF8FileName);
			PImportStream pImportStream = fnCreateImportStreamInstance(sUTF16FileName.c_str());

			m_pModelAttachment->setStream(pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}


	}

	LIB3MFMETHODIMP CCOMModelAttachment::ReadFromBuffer(_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pBuffer == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			PImportStream pImportStream = std::make_shared<CImportStream_Memory>(pBuffer, cbBufferSize);
			m_pModelAttachment->setStream(pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}

#ifdef NMR_COM_NATIVE
	LIB3MFMETHODIMP CCOMModelAttachment::ReadFromStream(_In_ IStream * pStream)
	{
		try {
			if (!m_pModelAttachment.get())
				throw CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
			if (pStream == nullptr)
				throw CNMRException(NMR_ERROR_INVALIDPOINTER);

			HRESULT hResult;

			CComPtr<IStream> pMemoryStream = nullptr;
			hResult = CreateStreamOnHGlobal(nullptr, true, &pMemoryStream);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTCREATESTREAM, hResult);

			// Seek Stream
			LARGE_INTEGER nOriginPosition;
			ULARGE_INTEGER nNewPosition;
			nOriginPosition.HighPart = 0;
			nOriginPosition.LowPart = 0;
			hResult = pStream->Seek(nOriginPosition, STREAM_SEEK_END, &nNewPosition);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

			nfUint64 cbStreamSize = nNewPosition.QuadPart;

			hResult = pStream->Seek(nOriginPosition, STREAM_SEEK_SET, &nNewPosition);
			if (hResult != S_OK)
				throw CNMRException_Windows(NMR_ERROR_COULDNOTSEEKSTREAM, hResult);

			std::array<nfByte, NMR_IMPORTSTREAM_COPYBUFFERSIZE> pBuffer;

			nfUint64 cbBytesLeft = cbStreamSize;
			while (cbBytesLeft > 0) {
				nfUint64 cbLength = cbBytesLeft;
				if (cbLength > NMR_IMPORTSTREAM_COPYBUFFERSIZE)
					cbLength = NMR_IMPORTSTREAM_COPYBUFFERSIZE;

				ULONG cbReadBytes = 0;
				ULONG cbWrittenBytes = 0;
				hResult = pStream->Read(&pBuffer[0], (nfUint32)cbLength, &cbReadBytes);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_COULDNOTREADSTREAM, hResult);

				if (cbReadBytes != cbLength)
					throw CNMRException(NMR_ERROR_COULDNOTREADFULLDATA);

				hResult = pMemoryStream->Write(&pBuffer[0], (nfUint32)cbLength, &cbWrittenBytes);
				if (hResult != S_OK)
					throw CNMRException_Windows(NMR_ERROR_COULDNOTWRITESTREAM, hResult);

				if (cbWrittenBytes != cbLength)
					throw CNMRException(NMR_ERROR_COULDNOTWRITEFULLDATA);
				cbBytesLeft -= cbLength;
			}

			PImportStream pImportStream = std::make_shared<CImportStream_COM>(pMemoryStream);

			m_pModelAttachment->setStream(pImportStream);

			return handleSuccess();
		}
		catch (CNMRException & Exception) {
			return handleNMRException(&Exception);
		}
		catch (...) {
			return handleGenericException();
		}

	}
#endif //NMR_COM_NATIVE


	}
