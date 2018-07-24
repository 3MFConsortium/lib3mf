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

COM Interface Implementation for Mesh Object Classes

--*/

#ifndef __NMR_COMINTERFACE_MODELMESHOBJECT
#define __NMR_COMINTERFACE_MODELMESHOBJECT

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 

namespace NMR {

	class CCOMModelMeshObject : public ILib3MFModelMeshObject {
	protected:
		PModelResource m_pResource;
		_Ret_notnull_ CModelMeshObject * getMeshObject();
		_Ret_notnull_ CMesh * getMesh();

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelMeshObject)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(GetVertexCount) (_Out_ DWORD * pnVertexCount);
		LIB3MFMETHOD(GetTriangleCount) (_Out_ DWORD * pnTriangleCount);
		LIB3MFMETHOD(GetBeamCount) (_Out_ DWORD * pnBeamCount);

		LIB3MFMETHOD(GetVertex) (_In_ DWORD nIndex, _Out_ MODELMESHVERTEX * pVertex);
		LIB3MFMETHOD(SetVertex) (_In_ DWORD nIndex, _In_ MODELMESHVERTEX * pVertex);
		LIB3MFMETHOD(AddVertex) (_In_ MODELMESHVERTEX * pVertex, _Out_opt_ DWORD * pnIndex);

		LIB3MFMETHOD(GetTriangle) (_In_ DWORD nIndex, _Out_ MODELMESHTRIANGLE * pTriangle);
		LIB3MFMETHOD(SetTriangle) (_In_ DWORD nIndex, _In_ MODELMESHTRIANGLE * pTriangle);
		LIB3MFMETHOD(AddTriangle) (_In_ MODELMESHTRIANGLE * pTriangle, _Out_opt_ DWORD * pnIndex);

		LIB3MFMETHOD(GetBeamLatticeMinLength) (_Out_ DOUBLE * pdMinLength);
		LIB3MFMETHOD(SetBeamLatticeMinLength) (_In_ DOUBLE dMinLength);
		LIB3MFMETHOD(GetBeamLatticeRadius) (_Out_ DOUBLE * pdRadius);
		LIB3MFMETHOD(SetBeamLatticeRadius) (_In_ DOUBLE dRadius);
		LIB3MFMETHOD(GetBeamLatticeCapMode) (_Out_ eModelBeamLatticeCapMode * peCapMode);
		LIB3MFMETHOD(SetBeamLatticeCapMode) (_In_ eModelBeamLatticeCapMode eCapMode);
		LIB3MFMETHODIMP GetBeamLatticeClipping(_Out_ eModelBeamLatticeClipMode * peClipMode, _Out_ DWORD *pnResourceID);
		LIB3MFMETHODIMP SetBeamLatticeClipping(_In_ eModelBeamLatticeClipMode eClipMode, _In_ DWORD nResourceID);
		LIB3MFMETHODIMP SetBeamLatticeRepresentation(_In_ DWORD nResourceID);
		LIB3MFMETHODIMP GetBeamLatticeRepresentation(_Out_ BOOL *pbHasRepresentation, _Out_ DWORD *pnResourceID);

		LIB3MFMETHOD(GetBeam) (_In_ DWORD nIndex, _Out_ MODELMESHBEAM * pBeam);
		LIB3MFMETHOD(SetBeam) (_In_ DWORD nIndex, _In_ MODELMESHBEAM * pBeam);
		LIB3MFMETHOD(AddBeam) (_In_ MODELMESHBEAM * pBeam, _Out_opt_ DWORD * pnIndex);

		LIB3MFMETHOD(GetVertices) (_Out_ MODELMESHVERTEX * pVertices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnVertexCount);
		LIB3MFMETHOD(GetTriangleIndices) (_Out_ MODELMESHTRIANGLE * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnTriangleCount);

		LIB3MFMETHOD(GetThumbnailPathUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetThumbnailPathUTF8) (_In_z_ LPCSTR pszName);

		LIB3MFMETHOD(GetUUIDUTF8) (_Out_ BOOL * pbHasUUID, _Out_ LPSTR pszBuffer);
		LIB3MFMETHOD(SetUUIDUTF8) (_In_z_ LPCSTR pszUUID);

		LIB3MFMETHOD(SetBeamIndices) (_In_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize);
		LIB3MFMETHOD(GetBeamIndices) (_Out_ MODELMESHBEAM * pIndices, _In_ DWORD nBufferSize, _Out_opt_ DWORD * pnBeamCount);
		
		LIB3MFMETHOD(GetBeamSetCount) (_Out_ DWORD * pnBeamSetCount);
		LIB3MFMETHOD(AddBeamSet) (_Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet);
		LIB3MFMETHOD(GetBeamSet) (_In_ DWORD nIndex, _Outptr_ ILib3MFModelMeshBeamSet ** ppBeamSet);

		LIB3MFMETHOD(SetGeometry) (_In_ MODELMESHVERTEX * pVertices, _In_ DWORD nVertexCount, _In_ MODELMESHTRIANGLE * pTriangles, _In_ DWORD nTriangleCount);

		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID);

		LIB3MFMETHOD(GetName) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetNameUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetName) (_In_z_ LPCWSTR pwszName);
		LIB3MFMETHOD(SetNameUTF8) (_In_z_ LPCSTR pszName);
		LIB3MFMETHOD(GetPartNumber) (_Out_opt_ LPWSTR pwszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(GetPartNumberUTF8) (_Out_opt_ LPSTR pszBuffer, _In_ ULONG cbBufferSize, _Out_ ULONG * pcbNeededChars);
		LIB3MFMETHOD(SetPartNumber) (_In_z_ LPCWSTR pwszPartNumber);
		LIB3MFMETHOD(SetPartNumberUTF8) (_In_z_ LPCSTR pszPartNumber);
		LIB3MFMETHOD(GetType) (_Out_ DWORD * pObjectType);
		LIB3MFMETHOD(SetType) (_In_ DWORD ObjectType);
		LIB3MFMETHOD(IsMeshObject) (_Out_ BOOL * pbIsMeshObject);
		LIB3MFMETHOD(IsComponentsObject) (_Out_ BOOL * pbIsComponentsObject);
		LIB3MFMETHOD(IsValidObject) (_Out_ BOOL * pbIsValid);
		LIB3MFMETHOD(CreateDefaultPropertyHandler) (_Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler);
		LIB3MFMETHOD(CreateDefaultMultiPropertyHandler) (_In_ DWORD nChannel, _Outptr_ ILib3MFDefaultPropertyHandler ** ppPropertyHandler);

		LIB3MFMETHOD(CreatePropertyHandler) (_Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler);
		LIB3MFMETHOD(CreateMultiPropertyHandler) (_In_ DWORD nChannel, _Outptr_ ILib3MFPropertyHandler ** ppPropertyHandler);

		LIB3MFMETHOD(IsManifoldAndOriented) (_Out_ BOOL * pbIsOrientedAndManifold);

		LIB3MFMETHOD(SetSliceStackId)(_In_ DWORD nSliceStackId);
		LIB3MFMETHOD(GetSliceStackId)(_Out_ DWORD *pnSliceStackId);
		LIB3MFMETHOD(SetSlicesMeshResolution)(_In_ eModelSlicesMeshResolution eSlicesMeshResolution);
		LIB3MFMETHOD(GetSlicesMeshResolution)(_Out_ eModelSlicesMeshResolution *peSlicesMeshResolution);

		CCOMModelMeshObject();

		void setResource(_In_ PModelResource pModelResource);
	};

}

#endif // __NMR_COMINTERFACE_MODELMESHOBJECT
