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

#ifndef __GCC
#include "Common/Platform/NMR_ImportStream_COM.h" 
#endif // __GCC

namespace NMR {

	LIB3MFMETHODIMP CCOMModelReader::ReadFromFile(_In_z_ LPCWSTR pwszFilename)
	{
		if (pwszFilename == nullptr)
			return LIB3MF_POINTER;
		if (m_pModelReader.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PImportStream pStream = fnCreateImportStreamInstance (pwszFilename);
			m_pModelReader->readStream(pStream);
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::GetWarningCount(_Out_ DWORD * pnWarningCount)
	{
		if (pnWarningCount == nullptr)
			return LIB3MF_POINTER;
		if (m_pModelReader.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PModelReaderWarnings pWarnings = m_pModelReader->getWarnings();
			if (pWarnings.get() == nullptr)
				return LIB3MF_FAIL;

			*pnWarningCount = pWarnings->getWarningCount();
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

	LIB3MFMETHODIMP CCOMModelReader::GetWarning(_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars)
	{
		if (m_pModelReader.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PModelReaderWarnings pWarnings = m_pModelReader->getWarnings();
			if (pWarnings.get() == nullptr)
				return LIB3MF_FAIL;

			PModelReaderWarning pWarning = pWarnings->getWarning(nIndex);
			__NMRASSERT(pWarning.get() != nullptr);

			if (cbBufferSize > MODEL_MAXSTRINGBUFFERLENGTH)
				return LIB3MF_FAIL;

			// Safely call StringToBuffer
			nfUint32 nNeededChars = 0;
			fnWStringToBufferSafe(pWarning->getMessage(), pwszBuffer, cbBufferSize, &nNeededChars);

			// Return length if needed
			if (pcbNeededChars != nullptr)
				*pcbNeededChars = nNeededChars;
			if (pErrorCode != nullptr)
				return LIB3MF_POINTER;

			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}

#ifndef __GCC
	LIB3MFMETHODIMP CCOMModelReader::ReadFromStream(_In_ IStream * pStream)
	{
		if (pStream == nullptr)
			return LIB3MF_INVALIDARG;
		if (m_pModelReader.get() == nullptr)
			return LIB3MF_FAIL;

		try {
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(pStream);
			m_pModelReader->readStream(pImportStream);
			return LIB3MF_OK;
		}
		catch (CNMRException_Windows & WinException) {
			return WinException.getHResult();
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}
#endif // __GCC

	void CCOMModelReader::setReader(_In_ PModelReader pModelReader)
	{
		m_pModelReader = pModelReader;
	}

}
