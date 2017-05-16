/*++

Copyright (C) 2017 Autodesk, Inc.
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

UnitTest_DLLInterfaces.cpp: Defines Unittests for the shared library interface

--*/

#include "Model/COM/NMR_DLLInterfaces.h"
#include <memory>
#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"

#ifdef __GNUC__
	#define S_OK ((DWORD)(0))
#endif


bool CreateDir(std::string sPath) {
	system( (std::string("mkdir \"")+ sPath + "\"").c_str() );
	return true;
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

	
	
	class DLLInterfacesTester
	{
	private:
		std::vector<MODELMESHVERTEX> m_vertices;
		MODELMESHTRIANGLE m_triangle;
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sPartNumber;
		const std::string m_sPartName;
		
		const double EPS_TOL = 1e-5;
		
		static HRESULT ObtainMeshObjectInformation(_In_ NMR::PLib3MFModelMeshObject * pMeshObject, DWORD & nVertexCount, DWORD & nTriangleCount)
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

			return S_OK;
		}		
		
	public:
		DLLInterfacesTester() :m_vertices(3), m_sFolderName("TestOutput"), 
			m_sFilenameReadWrite("output_dllwriteread.3mf"),
			m_sPartNumber("PartNumber"), m_sPartName("PartName") {
			for (size_t i = 0; i<m_vertices.size(); i++) {
				for (size_t j = 0; j < 3; j++)
					m_vertices[i].m_fPosition[j] = ((i + j) == 1)*1.f + ((i + j) == 2)*2.f;
				m_triangle.m_nIndices[i] = (DWORD)i;
			}
		}

		void Write3MF(bool writeTriangles) {
			bool writeVertices = writeTriangles;
			CustomLib3MFBase pModel;

			// Create Model Instance
			ASSERT_EQ(NMR::lib3mf_createmodel(&pModel.get(), true), S_OK) << L"Could not create model";
			
			CustomLib3MFBase pMeshObject;
			ASSERT_EQ(lib3mf_model_addmeshobject(pModel.get(), &pMeshObject.get()), S_OK) << L"Could not add mesh object";
			
			if (writeVertices) {
				for (size_t i = 0; i < m_vertices.size(); i++)
					ASSERT_EQ(lib3mf_meshobject_addvertex(pMeshObject.get(), &m_vertices[i], NULL), S_OK) << L"Could not add vertex";
			}

			if (writeTriangles) {
				ASSERT_EQ(lib3mf_meshobject_addtriangle(pMeshObject.get(), &m_triangle, NULL), S_OK) << L"Could not add triangle";
			}

			ASSERT_EQ(lib3mf_object_setpartnumberutf8(pMeshObject.get(), m_sPartNumber.c_str()), S_OK) << L"Could not set part number";

			ASSERT_EQ(lib3mf_object_setnameutf8(pMeshObject.get(), m_sPartName.c_str()), S_OK) << L"Could not set part name";

			CustomLib3MFBase pBuildItem;
			ASSERT_EQ(lib3mf_model_addbuilditem(pModel.get(), pMeshObject.get(), NULL, &pBuildItem.get()), S_OK) << L"Could not add build item";

			CustomLib3MFBase p3MFWriter;
			ASSERT_EQ(lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << L"Could create Model Writer";
			
			ASSERT_TRUE(CreateDir(m_sFolderName.c_str())) << L"Could not create folder.";
			ASSERT_EQ(lib3mf_writer_writetofileutf8(p3MFWriter.get(),
				(std::string("TestOutput") + separator() + m_sFilenameReadWrite).c_str()),
				S_OK) << L"Could not write 3MF file.";
		}


		void Read3MF (bool readTriangles){
			bool readVertices = readTriangles;
			CustomLib3MFBase pModel;
			ASSERT_EQ(lib3mf_createmodel(&pModel.get(), false), S_OK) << L"Could not create 3MF model";
			{
				CustomLib3MFBase p3MFReader;
				ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
				ASSERT_EQ(lib3mf_reader_readfromfileutf8(p3MFReader.get(), 
					(std::string("TestOutput") + separator() + m_sFilenameReadWrite).c_str()),
					S_OK) << L"Could not read 3MF file.";
			}

			CustomLib3MFBase pBuildItemIterator;
			ASSERT_EQ(lib3mf_model_getbuilditems(pModel.get(), &pBuildItemIterator.get()), S_OK) << L"Could not get build item iterator.";

			// iterate build items
			BOOL bHasNext;
			ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			while (bHasNext) {
				CustomLib3MFBase pBuildItem;
				ASSERT_EQ(lib3mf_builditemiterator_getcurrent(pBuildItemIterator.get(), &pBuildItem.get()), S_OK) << L"Could not retrieve build item.";

				CustomLib3MFBase pObjectResource;
				ASSERT_EQ(NMR::lib3mf_builditem_getobjectresource(pBuildItem.get(), &pObjectResource.get()), S_OK) << L"Ccould not get build item resource";

				BOOL bIsMeshObject;
				ASSERT_EQ(NMR::lib3mf_object_ismeshobject(pObjectResource.get(), &bIsMeshObject), S_OK) << L"Could not determine whether build item is mesh object";

				EXPECT_TRUE(bIsMeshObject) << L"Object is not a mesh!";

				// check mesh metadata
				{
					std::string sNameBuffer = "";
					ULONG cbNeededChars = 0;

					ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part number string";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_object_getpartnumberutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part number";
					EXPECT_TRUE(sNameBuffer == m_sPartNumber) << L"Part numbers do not match.";

					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), NULL, 0, &cbNeededChars), S_OK) << L"Could not get length of part name string";
					sNameBuffer.resize(cbNeededChars);
					ASSERT_EQ(lib3mf_object_getnameutf8(pObjectResource.get(), &sNameBuffer[0], cbNeededChars + 1, &cbNeededChars), S_OK) << L"Could not get part name";
					EXPECT_TRUE(sNameBuffer == m_sPartName) << L"Part names do not match.";
				}

				// check geometry and connectivity
				DWORD nVertexCount, nTriangleCount;
				ASSERT_EQ(ObtainMeshObjectInformation(pObjectResource.get(), nVertexCount, nTriangleCount), S_OK) << L"Could not obtain mesh object information";

				if (readVertices) {
					ASSERT_TRUE(nVertexCount == m_vertices.size()) << L"Vertex count does not match.";
					for (size_t i = 0; i < m_vertices.size(); i++) {
						MODELMESHVERTEX vertex;
						ASSERT_EQ(lib3mf_meshobject_getvertex(pObjectResource.get(), (DWORD)i, &vertex), S_OK) << L"Could not get vertex";
						for (int j = 0; j < 3; j++)
							EXPECT_TRUE(std::fabs(m_vertices[i].m_fPosition[j] - vertex.m_fPosition[j]) < EPS_TOL) << L"Vertex position does not match.";
					}
				}

				if (readTriangles) {
					ASSERT_EQ(nTriangleCount, 1) << L"Incorrect triangle count";
					MODELMESHTRIANGLE triangle;
					ASSERT_EQ(lib3mf_meshobject_gettriangle(pObjectResource.get(), 0, &triangle), S_OK) << L"Could not get triangle";
					for (int j = 0; j < 3; j++)
						EXPECT_TRUE(triangle.m_nIndices[j] == m_triangle.m_nIndices[j]) << L"Triangle indices do not match.";
				}
				
				ASSERT_EQ(lib3mf_builditemiterator_movenext(pBuildItemIterator.get(), &bHasNext), S_OK) << L"Could not get next build item.";
			}
		}
	};
	
	TEST(DLLInterfacesTest, VersionNumber)
	{
		DWORD nMajorVersion;
		DWORD nMinorVersion;
		EXPECT_EQ(lib3mf_getspecversion(&nMajorVersion, &nMinorVersion), S_OK);
		EXPECT_EQ(nMajorVersion, (DWORD)(1));
	}

	TEST(DLLInterfacesTest, InterfaceVersion)
	{
		DWORD nInterfaceVersion;
		EXPECT_EQ(NMR::lib3mf_getinterfaceversion(&nInterfaceVersion), S_OK) << L"Failed to obtain interface version.";
		EXPECT_EQ(nInterfaceVersion, (DWORD)(NMR_APIVERSION_INTERFACE)) << std::wstring(L"invalid 3MF interface version: " + std::to_wstring(NMR_APIVERSION_INTERFACE)).c_str();
		
		CustomLib3MFBase pModel;
		// Create Model Instance
		EXPECT_EQ( NMR::lib3mf_createmodel(&pModel.get(), false), S_OK) << L"Failed to create model.";
	}
		
	TEST(DLLInterfacesTest, Extensions)
	{
		DWORD nAPIInterfaceVersion;
		BOOL bIsSupported;
		std::wstring errString;
		
		// We do not support the mustfail spec
		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/thisspecmustfail/2000/00", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check MustFail-Spec";
		EXPECT_FALSE(bIsSupported) << L"MustFail-Spec IS supported";
		
		
		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/material/2015/02", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check Materials-Spec";
		EXPECT_TRUE(bIsSupported) << L"Materials-Spec not supported";	
		
		errString = std::wstring(L"invalid 3MF API interface version for Materials-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MATERIALSPEC));
		EXPECT_EQ(nAPIInterfaceVersion, (DWORD)(NMR_APIVERSION_INTERFACE_MATERIALSPEC)) << errString;
	}

	TEST(DLLInterfacesTest, WriteMesh)
	{
		DLLInterfacesTester interfaceTester;
		interfaceTester.Write3MF(true);
	}

	TEST(DLLInterfacesTest, ReadMesh)
	{
		DLLInterfacesTester interfaceTester;
		interfaceTester.Read3MF(true);
	}
	
}

