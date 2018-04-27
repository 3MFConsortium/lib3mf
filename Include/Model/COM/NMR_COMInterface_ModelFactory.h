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

COM Interface Implementation for the COM Model Factory class.

--*/


#ifndef __NMR_COM_MODELFACTORY
#define __NMR_COM_MODELFACTORY

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_Model.h" 
#include "Common/3MF_ProgressTypes.h" 

namespace NMR {

	class CCOMModelFactory : public ILib3MFModelFactory
	{
	protected:
		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelFactory);

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(CreateModel) (_Outptr_ ILib3MFModel ** ppModel);

		LIB3MFMETHOD(GetSpecVersion) (_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion);
		LIB3MFMETHOD(GetInterfaceVersion) (_Out_ DWORD * pInterfaceVersionMajor, _Out_ DWORD * pInterfaceVersionMinor, _Out_ DWORD * pInterfaceVersionMicro);
		LIB3MFMETHOD(QueryExtension) (_In_z_ LPCWSTR pwszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);
		LIB3MFMETHOD(QueryExtensionUTF8) (_In_z_ LPCSTR pszExtensionUrl, _Out_ BOOL * pbIsSupported, _Out_opt_ DWORD * pExtensionInterfaceVersion);

		LIB3MFMETHOD(RetrieveProgressMessage) (_In_ int progressIdentifier, _Out_ LPCSTR * progressMessage);

		CCOMModelFactory();
	};

	
}

#endif // __NMR_COM_MODELFACTORY






