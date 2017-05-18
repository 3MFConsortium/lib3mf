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

#ifndef __NMR_COMINTERFACE_MODELCOMPONENT
#define __NMR_COMINTERFACE_MODELCOMPONENT

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelComponent.h" 

namespace NMR {

	class CCOMModelComponent : public ILib3MFModelComponent {
	protected:
		PModelComponent m_pModelComponent;

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelComponent)
		
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(GetObjectResource) (_Outptr_ ILib3MFModelObjectResource ** ppObjectResource);
		LIB3MFMETHOD(GetUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer);
		LIB3MFMETHOD(SetUUIDUTF8) (_In_z_ LPCSTR pszUUID);
		LIB3MFMETHOD(GetTransform) (_Out_ MODELTRANSFORM * pTransformation);
		LIB3MFMETHOD(SetTransform) (_In_opt_ MODELTRANSFORM * pTransformation);
		LIB3MFMETHOD(GetObjectResourceID) (_Out_ DWORD * nResourceID);
		LIB3MFMETHOD(HasTransform) (_Out_ BOOL * pbHasTransform);

		CCOMModelComponent();
		void setComponent(_In_ PModelComponent pModelComponent);
	};

}

#endif // __NMR_COMINTERFACE_MODELCOMPONENT
