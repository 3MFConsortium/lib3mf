/*++

Copyright (C) 2017 Autodesk Inc.
Copyright (C) 2015 Microsoft Corporation (Original Author)
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

UnitTest_MeshFormat_STL.cpp: Defines Unittests for the STL mesh format

--*/

#include "CppUnitTest.h"
#include "Common\Math\NMR_Vector.h"
#include "Common\Math\NMR_Matrix.h"
#include "Common\NMR_Exception.h"
#include "Common\Mesh\NMR_Mesh.h"
#include "Common\MeshImport\NMR_MeshImporter_STL.h"
#include "Common\MeshExport\NMR_MeshExporter_STL.h"
#ifdef NMR_COM_NATIVE
#include "Common\Platform\NMR_ImportStream_COM.h"
#include "Common\Platform\NMR_ExportStream_COM.h"
#else
#include "Common\Platform\NMR_ImportStream_GCC_Native.h"
#include "Common\Platform\NMR_ExportStream_GCC_Native.h"
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace NMR
{

	TEST_CLASS(MeshFormatTest_STL)
	{
	private:
		const static std::wstring sInPath;
		const static std::wstring sOutPath;
	public:

		TEST_METHOD(MeshImport_STL_Box)
		{
			CMesh mesh;
#ifdef NMR_COM_NATIVE
			PImportStream stream(new CImportStream_COM((sInPath + L"Box.stl").c_str()));
#else
			PImportStream stream(new CImportStream_GCC_Native((sInPath + L"Box.stl").c_str()));
#endif
			PMeshImporter importer(new CMeshImporter_STL(stream, 0.0001f));
			importer->loadMesh(&mesh, NULL);
			Assert::IsTrue(mesh.getNodeCount() == 8);
			Assert::IsTrue(mesh.getFaceCount() == 12);
			Assert::IsTrue(mesh.checkSanity());
		}

		TEST_METHOD(MeshImport_STL_Sphere)
		{
			CMesh Mesh;
#ifdef NMR_COM_NATIVE
			PImportStream pStream(new CImportStream_COM((sInPath + L"Sphere.stl").c_str()));
#else
			PImportStream pStream(new CImportStream_GCC_Native((sInPath + L"Sphere.stl").c_str()));
#endif
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 1442);
			Assert::IsTrue(Mesh.getFaceCount() == 2880);
			Assert::IsTrue(Mesh.checkSanity());
		}

		TEST_METHOD(MeshExport_STL_Box)
		{
			CMesh Mesh;
#ifdef NMR_COM_NATIVE
			PImportStream pStream(new CImportStream_COM((sInPath + L"Box.stl").c_str()));
#else
			PImportStream pStream(new CImportStream_GCC_Native((sInPath + L"Box.stl").c_str()));
#endif
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 8);
			Assert::IsTrue(Mesh.getFaceCount() == 12);

#ifdef NMR_COM_NATIVE
			PExportStream pExportStream(new CExportStream_COM((sOutPath + L"output_box.stl").c_str()));
#else
			PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"output_box.stl").c_str()));
#endif
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);
		}

		TEST_METHOD(MeshExport_STL_Sphere)
		{
			CMesh Mesh;
#ifdef NMR_COM_NATIVE
			PImportStream pStream(new CImportStream_COM((sInPath + L"Sphere.stl").c_str()));
#else
			PImportStream pStream(new CImportStream_GCC_Native((sInPath + L"Sphere.stl").c_str()));
#endif
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 1442);
			Assert::IsTrue(Mesh.getFaceCount() == 2880);

#ifdef NMR_COM_NATIVE
			PExportStream pExportStream(new CExportStream_COM((sOutPath + L"output_sphere.stl").c_str()));
#else
			PExportStream pExportStream(new CExportStream_GCC_Native((sOutPath + L"output_sphere.stl").c_str()));
#endif
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);
		}

	};

	const std::wstring MeshFormatTest_STL::sInPath = L"../../TestFiles/VS_UnitTests/";
	const std::wstring MeshFormatTest_STL::sOutPath = L"../TestOutput/";
}
