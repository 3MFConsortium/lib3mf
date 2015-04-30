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

COM Interface Implementation for Model Component Object Classes

--*/

#ifndef __NMR_COMINTERFACE_MODELCOMPONENTSOBJECT
#define __NMR_COMINTERFACE_MODELCOMPONENTSOBJECT

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 

namespace NMR {

	class CCOMModelComponentsObject : public ILib3MFModelComponentsObject {
	protected:
		PModelResource m_pResource;
		_Ret_notnull_ CModelComponentsObject * getComponentsObject();
		_Ret_notnull_ CMesh * getMesh();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelComponentsObject)

			LIB3MFMETHOD(AddComponent) (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pmTransform, _Outptr_ ILib3MFModelComponent ** ppComponent);
		LIB3MFMETHOD(GetComponent) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelComponent ** ppComponent);
		LIB3MFMETHOD(GetComponentCount) (_Out_ DWORD * pComponentCount);
		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID);

		LIB3MFMETHOD(GetName) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetName) (_In_z_ LPCWSTR pwszName);
		LIB3MFMETHOD(GetPartNumber) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetPartNumber) (_In_z_ LPCWSTR pwszPartNumber);
		LIB3MFMETHOD(GetType) (_Out_ DWORD * pObjectType);
		LIB3MFMETHOD(SetType) (_In_ DWORD ObjectType);

			CCOMModelComponentsObject();

		void setResource(_In_ PModelResource pModelResource);
	};

}

#endif // __NMR_COMINTERFACE_MODELCOMPONENTSOBJECT
