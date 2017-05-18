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
#include "Model/Classes/NMR_ModelSliceResource.h"

namespace NMR {

	class CCOMModel : public ILib3MFModel	{
	protected:
		PModel m_pModel;
		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModel)
		
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(SetUnit) (_In_ DWORD Unit);
		LIB3MFMETHOD(GetUnit) (_Out_ DWORD * pUnit);

		LIB3MFMETHOD(SetLanguage) (_In_z_ LPCWSTR pwszLanguage);
		LIB3MFMETHOD(SetLanguageUTF8) (_In_z_ LPCSTR pszLanguage);
		LIB3MFMETHOD(GetLanguage) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetLanguageUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_opt_ ULONG * pcbNeededChars);

		LIB3MFMETHOD(QueryWriter) (_In_z_ LPCSTR pszWriterClass, _Outptr_ ILib3MFModelWriter ** ppWriter);
		LIB3MFMETHOD(QueryReader) (_In_z_ LPCSTR pszReaderClass, _Outptr_ ILib3MFModelReader ** ppReader);

		LIB3MFMETHOD(GetResourceByID) (_In_ DWORD ResourceID, _Outptr_ ILib3MFModelResource ** ppResource);
		LIB3MFMETHOD(GetTexture2DByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelTexture2D ** ppTexture);
		LIB3MFMETHOD(GetBaseMaterialByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelBaseMaterial ** ppMaterial);
		LIB3MFMETHOD(GetMeshObjectByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelMeshObject ** ppMeshObject);
		LIB3MFMETHOD(GetComponentsObjectByID) (_In_ DWORD nResourceID, _Outptr_ ILib3MFModelComponentsObject ** ppComponentsObject);

		LIB3MFMETHOD(GetBuildUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer);
		LIB3MFMETHOD(SetBuildUUIDUTF8) (_In_ LPCSTR pszUUID);
		LIB3MFMETHOD(GetBuildItems) (_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator);
		LIB3MFMETHOD(GetResources) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetMeshObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetComponentsObjects) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);

		LIB3MFMETHOD(Get2DTextures) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);
		LIB3MFMETHOD(GetBaseMaterials) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);

		LIB3MFMETHOD(MergeToModel) (_Outptr_ ILib3MFModel ** ppMergedModel);

		LIB3MFMETHOD(AddMeshObject) (_Outptr_ ILib3MFModelMeshObject ** ppMeshObject);
		LIB3MFMETHOD(AddComponentsObject) (_Outptr_ ILib3MFModelComponentsObject ** ppComponentObject);

		LIB3MFMETHOD(AddTexture2DFromAttachment) (_In_z_ ILib3MFModelAttachment* pTextureAttachment, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance);
		LIB3MFMETHOD(AddTexture2D) (_In_z_ LPCWSTR pwszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance);
		LIB3MFMETHOD(AddTexture2DUTF8) (_In_z_ LPCSTR pszPath, _Outptr_ ILib3MFModelTexture2D ** ppTextureInstance);
		LIB3MFMETHOD(AddBaseMaterialGroup) (_Outptr_ ILib3MFModelBaseMaterial ** ppBaseMaterialInstance);

		LIB3MFMETHOD(AddBuildItem) (_In_ ILib3MFModelObjectResource * pObject, _In_opt_ MODELTRANSFORM * pTransform, _Outptr_ ILib3MFModelBuildItem ** ppBuildItem);
		LIB3MFMETHOD(RemoveBuildItem) (_In_ ILib3MFModelBuildItem * pBuildItem);

		LIB3MFMETHOD(GetMetaDataCount) (_Out_ DWORD * pnCount);
		LIB3MFMETHOD(GetMetaDataKey) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetMetaDataKeyUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetMetaDataValue) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetMetaDataValueUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(AddMetaData) (_In_ LPCWSTR pszwKey, _In_ LPCWSTR pszwValue);
		LIB3MFMETHOD(AddMetaDataUTF8) (_In_ LPCSTR pszKey, _In_ LPCSTR pszValue);
		LIB3MFMETHOD(RemoveMetaData) (_In_ DWORD nIndex);

		LIB3MFMETHOD(AddAttachment) (_In_z_ LPWSTR pwszURI, _In_z_ LPWSTR pwszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(AddAttachmentUTF8) (_In_z_ LPSTR pszURI, _In_z_ LPSTR pszRelationShipType, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(GetAttachment) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(FindAttachment) (_In_z_ LPWSTR pwszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(FindAttachmentUTF8) (_In_z_ LPSTR pwszURI, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(GetAttachmentCount) (_Out_ DWORD * pnCount);
		LIB3MFMETHOD(GetAttachmentSize) (_In_ DWORD nIndex, _Out_ UINT64 * pnSize);
		LIB3MFMETHOD(GetAttachmentPath) (_In_ DWORD nIndex, _Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetAttachmentPathUTF8) (_In_ DWORD nIndex, _Out_opt_ LPSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetPackageThumbnailAttachment) (_In_ BOOL bCreateIfNotExisting, _Outptr_ ILib3MFModelAttachment ** ppAttachmentInstance);
		LIB3MFMETHOD(RemovePackageThumbnailAttachment) ();

		LIB3MFMETHOD(AddCustomContentType) (_In_ LPCWSTR pszwExtension, _In_ LPCWSTR pszwContentType);
		LIB3MFMETHOD(AddCustomContentTypeUTF8) (_In_ LPCSTR pszExtension, _In_ LPCSTR pszContentType);
		LIB3MFMETHOD(RemoveCustomContentType) (_In_ LPCWSTR pszwExtension);
		LIB3MFMETHOD(RemoveCustomContentTypeUTF8) (_In_ LPCSTR pszExtension);
		LIB3MFMETHOD(AddSliceStack) (_In_opt_ FLOAT nBottomZ, _Out_opt_ ILib3MFSliceStack ** pSliceStack);

		CCOMModel();
		CModel * getModel();
	};

}

#endif // __NMR_COMINTERFACE_MODEL
