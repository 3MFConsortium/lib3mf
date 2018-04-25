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

#ifndef __NMR_COMINTERFACE_MODELTEXTURE2D
#define __NMR_COMINTERFACE_MODELTEXTURE2D

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelTexture2D.h" 

#define MODELTEXTURE2D_BUFFERSIZE 65536

namespace NMR {

	class CCOMModelTexture2D : public ILib3MFModelTexture2D {
	protected:
		PModelResource m_pResource;
		_Ret_notnull_ CModelTexture2DResource * getTexture2D();

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelTexture2D)
		
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);
		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnVertexCount);

		LIB3MFMETHOD(GetAttachment) (_Out_ ILib3MFModelAttachment ** ppTextureAttachment);
		LIB3MFMETHOD(SetAttachment) (_In_ ILib3MFModelAttachment * pTextureAttachment);

		LIB3MFMETHOD(GetPath) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetPath) (_In_z_ LPCWSTR pwszPath);
		LIB3MFMETHOD(SetPathUTF8) (_In_z_ LPCSTR pszPath);
		LIB3MFMETHOD(GetContentType) (_Out_ eModelTexture2DType * peContentType);
		LIB3MFMETHOD(SetContentType) (_In_ eModelTexture2DType eContentType);
		LIB3MFMETHOD(GetTileStyleUV) (_Out_ eModelTextureTileStyle * peTileStyleU, _Out_ eModelTextureTileStyle * peTileStyleV);
		LIB3MFMETHOD(SetTileStyleUV) (_In_ eModelTextureTileStyle eTileStyleU, _In_ eModelTextureTileStyle eTileStyleV);
		LIB3MFMETHOD(GetBox2D) (_Out_ FLOAT * pfU, _Out_ FLOAT * pfV, _Out_ FLOAT * pfWidth, _Out_ FLOAT * pfHeight);
		LIB3MFMETHOD(SetBox2D) (_In_ FLOAT fU, _In_ FLOAT fV, _In_ FLOAT fWidth, _In_ FLOAT fHeight);
		LIB3MFMETHOD(ClearBox2D) ();
		LIB3MFMETHOD(GetStreamSize) (_Out_ ULONG64 * pcbStreamSize);
		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pszFilename);
		LIB3MFMETHOD(WriteToBuffer) (_Out_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);
		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pUserData);

#ifdef NMR_COM_NATIVE
		LIB3MFMETHOD(WriteToStream) (_In_ IStream * pStream);
#endif// NMR_COM_NATIVE

		LIB3MFMETHOD(ReadFromFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(ReadFromFileUTF8) (_In_z_ LPCSTR pszFilename);
		LIB3MFMETHOD(ReadFromBuffer) (_In_ BYTE * pBuffer, _In_ ULONG64 cbBufferSize);

#ifdef NMR_COM_NATIVE
		LIB3MFMETHOD(ReadFromStream) (_In_ IStream * pStream);
#endif //NMR_COM_NATIVE

		CCOMModelTexture2D();

		void setResource(_In_ PModelResource pModelResource);
	};

}

#endif // __NMR_COMINTERFACE_MODELTEXTURE2D
