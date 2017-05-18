/*++

Copyright (C) 2017 Autodesk Inc.

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

COM Interface Implementation for Model MeshBeam Set

--*/

#ifndef __NMR_COMINTERFACE_MODELMESHBEAMSET
#define __NMR_COMINTERFACE_MODELMESHBEAMSET

#include "Model/COM/NMR_COMInterfaces.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

namespace NMR {

	class CCOMModelMeshBeamSet : public ILib3MFModelMeshBeamSet {
	protected:
		PBEAMSET m_pBeamSet;

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelMeshBeamSet)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		CCOMModelMeshBeamSet();

		LIB3MFMETHOD(SetName) (_In_z_ LPCWSTR pwszName);
		LIB3MFMETHOD(SetIdentifier) (_In_z_ LPCWSTR pwszIdentifier);
		LIB3MFMETHOD(SetNameUTF8) (_In_z_ LPCSTR pszName);
		LIB3MFMETHOD(SetIdentifierUTF8) (_In_z_ LPCSTR pszIdentifier);
		LIB3MFMETHOD(GetNameUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetIdentifierUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		LIB3MFMETHOD(GetRefCount) (_Out_ DWORD * pnCount);
		LIB3MFMETHOD(SetRefs) (_In_ DWORD * pRefs, _In_ DWORD nRefCount);
		LIB3MFMETHOD(GetRefs) (_Out_ DWORD * pRefs, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnRefCount);

		void setBeamSet(_In_ PBEAMSET pBeamSet);
		BEAMSET* getBeamSet();
		~CCOMModelMeshBeamSet();
	};

}

#endif // __NMR_COMINTERFACE_MODELMESHBEAMSET
