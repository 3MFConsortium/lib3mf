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

Abstract: COM Interface Implementation for Model Classes

--*/

#ifndef __NMR_COMINTERFACE_MODELBASEMATERIAL
#define __NMR_COMINTERFACE_MODELBASEMATERIAL

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelBaseMaterials.h" 

namespace NMR {

	class CCOMModelBaseMaterial : public ILib3MFModelBaseMaterial {
	protected:
		PModelResource m_pResource;
		_Ret_notnull_ CModelBaseMaterialResource * getBaseMaterials();

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelReader)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);
		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID);

		LIB3MFMETHOD(GetCount) (_Out_ DWORD * pcbCount);
		LIB3MFMETHOD(AddMaterial) (_In_z_ LPCWSTR pwszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_opt_ DWORD * pnResourceIndex);
		LIB3MFMETHOD(AddMaterialUTF8) (_In_z_ LPCSTR pszName, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_opt_ DWORD * pnResourceIndex);
		LIB3MFMETHOD(RemoveMaterial) (_In_ DWORD nIndex);
		LIB3MFMETHOD(GetName) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetNameUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetName) (_In_ DWORD nIndex, _In_z_ LPCWSTR pwszName);
		LIB3MFMETHOD(SetNameUTF8) (_In_ DWORD nIndex, _In_z_ LPCSTR pszName);
		LIB3MFMETHOD(SetDisplayColorRGB) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);
		LIB3MFMETHOD(SetDisplayColorRGBA) (_In_ DWORD nIndex, _In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);
		LIB3MFMETHOD(SetDisplayColorFloatRGB) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);
		LIB3MFMETHOD(SetDisplayColorFloatRGBA) (_In_ DWORD nIndex, _In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha);
		LIB3MFMETHOD(GetDisplayColor) (_In_ DWORD nIndex, _Out_ BYTE* pbRed, _Out_ BYTE* pbGreen, _Out_ BYTE* pbBlue, _Out_ BYTE* pbAlpha);

		CCOMModelBaseMaterial();

		void setResource(_In_ PModelResource pModelResource);
	};

}

#endif // __NMR_COMINTERFACE_MODELBASEMATERIAL
