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

UnitTest_Utilities.h: Utilities for UnitTests

--*/

#ifndef __NMR_UNITTEST_UTILITIES
#define __NMR_UNITTEST_UTILITIES

#include "Model/COM/NMR_DLLInterfaces.h"
#include <memory>
#include <vector>
#include <string>
#include <cmath>

#include "gtest/gtest.h"

#ifdef __GNUC__
	#define S_OK ((HRESULT)(0))
#endif

#ifdef TESTFILESPATH
const std::string sTestFilesPath = TESTFILESPATH;
#else
const std::string sTestFilesPath = "TestFiles";
#endif

inline bool CreateDir(std::string sPath) {
	return (system( (std::string("mkdir \"")+ sPath + "\"").c_str() ) != -1);
}

inline char separator()
{
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}

namespace NMR {
	
	class CustomLib3MFBase {
		public:
			CustomLib3MFBase(): m_pLib3MFBase(nullptr)
			{	
			}
			~CustomLib3MFBase()
			{
				if (m_pLib3MFBase) {
					EXPECT_EQ(NMR::lib3mf_release(m_pLib3MFBase), S_OK) << L"Failed releasing NMR::PLib3MFBase*.";
					m_pLib3MFBase = nullptr;
				}
			}
			NMR::PLib3MFBase* & get() {
				return m_pLib3MFBase;
			}
		private:
			NMR::PLib3MFBase* m_pLib3MFBase;
	};
}

inline HRESULT ObtainMeshObjectInformation(_In_ NMR::PLib3MFModelMeshObject * pMeshObject, DWORD & nVertexCount, DWORD & nTriangleCount, DWORD & nBeamCount)
{
	HRESULT hResult;
	// Retrieve Mesh Vertex Count
	hResult = NMR::lib3mf_meshobject_getvertexcount(pMeshObject, &nVertexCount);
	if (hResult != S_OK)
		return hResult;

	// Retrieve Mesh triangle Count
	hResult = NMR::lib3mf_meshobject_gettrianglecount(pMeshObject, &nTriangleCount);
	if (hResult != S_OK)
		return hResult;

	// Retrieve Mesh Beam Count
	hResult = NMR::lib3mf_meshobject_getbeamcount(pMeshObject, &nBeamCount);
	if (hResult != S_OK)
		return hResult;

	return S_OK;
}


// Utility functions to create vertices and triangles
inline NMR::MODELMESHVERTEX fnCreateVertex(float x, float y, float z)
{
	NMR::MODELMESHVERTEX result;
	result.m_fPosition[0] = x;
	result.m_fPosition[1] = y;
	result.m_fPosition[2] = z;
	return result;
}

inline NMR::MODELMESHTRIANGLE fnCreateTriangle(int v0, int v1, int v2)
{
	NMR::MODELMESHTRIANGLE result;
	result.m_nIndices[0] = v0;
	result.m_nIndices[1] = v1;
	result.m_nIndices[2] = v2;
	return result;
}

inline int FillMeshObject(NMR::CustomLib3MFBase &pMeshObject)
{
	using namespace NMR;
	// Create mesh structure of a cube
	MODELMESHVERTEX pVertices[8];
	MODELMESHTRIANGLE pTriangles[12];
	float fSizeX = 100.0f;
	float fSizeY = 100.0f;
	float fSizeZ = 100.0f;

	// Manually create vertices
	pVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	pVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	pVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	pVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	pVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	pVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	pVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	pVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);


	// Manually create triangles
	pTriangles[0] = fnCreateTriangle(2, 1, 0);
	pTriangles[1] = fnCreateTriangle(0, 3, 2);
	pTriangles[2] = fnCreateTriangle(4, 5, 6);
	pTriangles[3] = fnCreateTriangle(6, 7, 4);
	pTriangles[4] = fnCreateTriangle(0, 1, 5);
	pTriangles[5] = fnCreateTriangle(5, 4, 0);
	pTriangles[6] = fnCreateTriangle(2, 3, 7);
	pTriangles[7] = fnCreateTriangle(7, 6, 2);
	pTriangles[8] = fnCreateTriangle(1, 2, 6);
	pTriangles[9] = fnCreateTriangle(6, 5, 1);
	pTriangles[10] = fnCreateTriangle(3, 0, 4);
	pTriangles[11] = fnCreateTriangle(4, 7, 3);

	return lib3mf_meshobject_setgeometry(pMeshObject.get(), pVertices, 8, pTriangles, 12);
}

#endif //__NMR_UNITTEST_UTILITIES
