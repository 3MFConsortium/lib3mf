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

#ifndef __NMR_COMINTERFACE_MODELATTACHMENT
#define __NMR_COMINTERFACE_MODELATTACHMENT

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelAttachment.h" 

#define MODELATTACHMENT_BUFFERSIZE 65536

namespace NMR {

	class CCOMModelAttachment : public ILib3MFModelAttachment {
	protected:

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		PModelAttachment m_pModelAttachment;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelAttachment)
		
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);


		CCOMModelAttachment();

		void setAttachment(_In_ PModelAttachment pModelAttachment);
		LIB3MFMETHOD(GetPath) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars) ;
		LIB3MFMETHOD(GetPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetPath) (_In_z_ LPCWSTR pwszPath);
		LIB3MFMETHOD(SetPathUTF8) (_In_z_ LPCSTR pszPath);
		LIB3MFMETHOD(GetRelationshipType) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetRelationshipTypeUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetRelationshipType) (_In_z_ LPCWSTR pwszRelationShipType);
		LIB3MFMETHOD(SetRelationshipTypeUTF8) (_In_z_ LPCSTR pszRelationShipType);
		LIB3MFMETHOD(GetStreamSize) (_Out_ ULONG64 * pcbStreamSize);
		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pszFilename);
		LIB3MFMETHOD(WriteToBuffer) (_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pUserData);
		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pwszFilename);
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);
	};

}

#endif // __NMR_COMINTERFACE_MODELATTACHMENT
