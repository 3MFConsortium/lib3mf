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

COM Interface Implementation for Model Class

--*/

#ifndef __NMR_COMINTERFACE_MODEL
#define __NMR_COMINTERFACE_MODEL

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_Model.h" 

namespace NMR {

	class CCOMModel : public ILib3MFModel	{
	protected:
		PModel m_pModel;
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModel)
	
		LIB3MFMETHOD(SetUnit) (_In_ DWORD Unit);
		LIB3MFMETHOD(GetUnit) (_Out_ DWORD * pUnit);

		LIB3MFMETHOD(SetLanguage) (_In_z_ LPCWSTR pwszLanguage);
		LIB3MFMETHOD(GetLanguage) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		LIB3MFMETHOD(QueryWriter) (_In_z_ LPCWSTR pwszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter);
		LIB3MFMETHOD(QueryReader) (_In_z_ LPCWSTR pwszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader);

		LIB3MFMETHOD(GetResourceByID) (_In_ DWORD ResourceID, _Outptr_ ILib3MFModelResource ** ppResource);

		LIB3MFMETHOD(GetBuildItems) (_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator);
		LIB3MFMETHOD(GetResources) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetMeshObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetComponentsObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);

		LIB3MFMETHOD(MergeToModel) (_Outptr_ ILib3MFModel ** ppMergedModel);

		LIB3MFMETHOD(AddMeshObject) (_Outptr_ ILib3MFModelMeshObject ** ppMeshObject);
		LIB3MFMETHOD(AddComponentsObject) (_Outptr_ ILib3MFModelComponentsObject ** ppComponentObject);

		LIB3MFMETHOD(AddBuildItem) (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ ILib3MFModelBuildItem ** ppBuildItem);
		LIB3MFMETHOD(RemoveBuildItem) (_In_ ILib3MFModelBuildItem * pBuildItem);

		CCOMModel();
	};
	
}

#endif // __NMR_COMINTERFACE_MODEL
