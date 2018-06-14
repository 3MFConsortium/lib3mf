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

NMR_COMInterface_Slice.h: defines COM interface for the slice class

--*/

#ifndef __NMR_COMINTERFACE_SLICE
#define __NMR_COMINTERFACE_SLICE

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelSliceResource.h"

namespace NMR {
	class CCOMSlice : public ILib3MFSlice {
	protected:
		PSlice m_pSlice;

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFSlice)

		LIB3MFMETHOD(AddVertex)(_In_ MODELSLICEVERTEX * pVertex, _Out_opt_ DWORD * pnIndex);

		LIB3MFMETHOD(BeginPolygon)(_Out_ DWORD *nPolygonIndex);

		LIB3MFMETHOD(AddNode)(_In_ float x, _In_ float y, _Out_ DWORD * nNodeIndex);

		LIB3MFMETHOD(AddPolygonIndices)(_In_ DWORD nPolygonIndex, _In_ DWORD *pnVertexIndices, _In_ DWORD nBufferSize, _Out_ DWORD * nPolygonVertexIndex);

		LIB3MFMETHOD(AddPolygonIndex)(_In_ DWORD nPolygonIndex, _In_ DWORD nVertexIndex);

		LIB3MFMETHOD(GetPolygonCount)(_Out_ DWORD *nPolygonCount);

		LIB3MFMETHOD(GetTopZ)(_Out_ float * nTopZ);

		LIB3MFMETHOD(SetTopZ)(_In_ float nTopZ);

		LIB3MFMETHOD(GetVertexCount) (_Out_ DWORD * pnVertexCount);

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage);

		void setSlice(_In_ PSlice pSlice);

		LIB3MFMETHOD(GetIndexCountOfPolygon) (_In_ DWORD nPolygonIndex, _Out_ DWORD *pnPolygonCount);

		LIB3MFMETHOD(GetPolygonIndices) (_In_ DWORD nPolygonIndex, _Out_ DWORD *pPolygonIndices, _In_ DWORD nBufferCount);

		LIB3MFMETHOD(GetVertex) (_In_ DWORD nIndex, _Out_ MODELSLICEVERTEX *pVertex);

		CCOMSlice();
		CSlice *getSlice();
	};

	class CCOMSliceStack : public ILib3MFSliceStack {
	protected:
		PModelResource m_pResource;
		_Ret_notnull_ CSliceStack * getSliceStack();

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFSliceStack)

		LIB3MFMETHOD(AddSlice) (_In_ FLOAT fTopZ, _Outptr_ ILib3MFSlice ** ppSliceObject);

		LIB3MFMETHOD(GetSliceCount) (_Out_ DWORD * pnSliceCount);

		LIB3MFMETHOD(GetSlice) (_In_ DWORD nSliceIndex, _Outptr_ ILib3MFSlice **ppSliceObject);

		LIB3MFMETHOD(GetBottomZ) (_Out_ FLOAT * pfBottomZ);

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage);

		LIB3MFMETHOD(GetResourceID) (_Out_ DWORD * pnResourceID);

		LIB3MFMETHOD(SetUsesSliceRef) (_In_ BOOL bUsesSliceRef);

		LIB3MFMETHOD(GetUsesSliceRef) (_Out_ BOOL *pbUsesSliceRef);

		CCOMSliceStack();

		void setResource(_In_ PModelResource pModelResource);
	};
}

#endif

