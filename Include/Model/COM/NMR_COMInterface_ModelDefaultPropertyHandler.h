/*++

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

COM Interface Implementation for Model Property Handler Class

--*/

#ifndef __NMR_COMINTERFACE_MODELDEFAULTPROPERTYHANDLER
#define __NMR_COMINTERFACE_MODELDEFAULTPROPERTYHANDLER

#include "Model/COM/NMR_COMInterfaces.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

namespace NMR {

	class CCOMModelDefaultPropertyHandler : public ILib3MFDefaultPropertyHandler {
	protected:
		PModelResource m_pModelResource;
		nfUint32 m_nChannel;

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFDefaultPropertyHandler)

			LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(RemoveProperty) ();
		LIB3MFMETHOD(GetPropertyType) (_Out_ eModelPropertyType * pnPropertyType);
		LIB3MFMETHOD(GetBaseMaterial) (_Out_ DWORD * pnMaterialGroupID, _Out_ DWORD * pnMaterialIndex);
		LIB3MFMETHOD(SetBaseMaterial) (_In_ ModelResourceID nMaterialGroupID, _In_ DWORD nMaterialIndex);
		LIB3MFMETHOD(GetColor) (_In_ DWORD nIndex, _Out_ MODELMESHCOLOR_SRGB * pColor);
		LIB3MFMETHOD(SetColor) (_In_ MODELMESHCOLOR_SRGB * pColor);
		LIB3MFMETHOD(SetColorRGB) (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue);
		LIB3MFMETHOD(SetColorRGBA) (_In_ BYTE bRed, _In_ BYTE bGreen, _In_ BYTE bBlue, _In_ BYTE bAlpha);
		LIB3MFMETHOD(SetFloatColorRGB) (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue);
		LIB3MFMETHOD(SetFloatColorRGBA) (_In_ FLOAT fRed, _In_ FLOAT fGreen, _In_ FLOAT fBlue, _In_ FLOAT fAlpha);
		LIB3MFMETHOD(GetTexture) (_Out_ ModelResourceID * pnTextureID, _Out_ FLOAT * pfU, _Out_ FLOAT * pfV);
		LIB3MFMETHOD(SetTexture) (_In_ ModelResourceID nTextureID, _In_ FLOAT fU, _In_ FLOAT fV);

		CCOMModelDefaultPropertyHandler();
		void setResource(_In_ PModelResource pResource);
		void setChannel(_In_ DWORD nChannel);
		_Ret_notnull_ CModelObject * getObject();

	};

}

#endif // __NMR_COMINTERFACE_MODELDEFAULTPROPERTYHANDLER
