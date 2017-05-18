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

#ifndef __NMR_COMINTERFACE_MODELREADER
#define __NMR_COMINTERFACE_MODELREADER

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Reader/NMR_ModelReader.h" 

namespace NMR {

	class CCOMModelReader : public ILib3MFModelReader {
	protected:
		PModelReader m_pModelReader;
		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelReader)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pwszFilename);
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		LIB3MFMETHOD(GetWarningCount) (_Out_ DWORD * pnWarningCount);
		LIB3MFMETHOD(GetWarning) (_In_ DWORD nIndex, _Out_ DWORD * pErrorCode, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		LIB3MFMETHOD(AddRelationToRead) (_In_z_ LPCWSTR pwszRelationshipType);
		LIB3MFMETHOD(RemoveRelationToRead) (_In_z_ LPCWSTR pwszRelationshipType);
		LIB3MFMETHOD(AddRelationToReadUTF8) (_In_z_ LPCSTR pszRelationshipType);
		LIB3MFMETHOD(RemoveRelationToReadUTF8) (_In_z_ LPCSTR pszRelationshipType);

		LIB3MFMETHOD(SetStrictModeActive) (_In_ BOOL bStrictModeActive);
		LIB3MFMETHOD(GetStrictModeActive) (_Out_ BOOL * pbStrictModeActive);

#ifdef NMR_COM_NATIVE
		LIB3MFMETHOD(ReadFromStream) (_In_ IStream * pStream);
#endif // NMR_COM_NATIVE

		LIB3MFMETHOD(ReadFromCallback) (_In_ void * pReadCallback, _In_ nfUint64 nStreamSize, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData);

		CCOMModelReader();

		void setReader(_In_ PModelReader pModelReader);
	};

}

#endif // __NMR_COMINTERFACE_MODELREADER
