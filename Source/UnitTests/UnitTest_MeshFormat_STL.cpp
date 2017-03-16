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
#include "Common\Platform\NMR_ImportStream_COM.h"
#include "Common\MeshExport\NMR_MeshExporter_STL.h"
// #include "Common\MeshExport\NMR_MeshExporter_Debug.h"
#include "Common\Platform\NMR_ExportStream_COM.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace NMR
{

	TEST_CLASS(MeshFormatTest_STL)
	{
	public:

		TEST_METHOD(MeshImport_STL_Box)
		{
			CMesh mesh;
			PImportStream stream(new CImportStream_COM(L"../../TestFiles/Box.stl"));
			PMeshImporter importer(new CMeshImporter_STL(stream, 0.0001f));
			importer->loadMesh(&mesh, NULL);
			Assert::IsTrue(mesh.getNodeCount() == 8);
			Assert::IsTrue(mesh.getFaceCount() == 12);
			Assert::IsTrue(mesh.checkSanity());
		}

		TEST_METHOD(MeshImport_STL_Sphere)
		{
			CMesh Mesh;
			PImportStream pStream(new CImportStream_COM(L"../../TestFiles/Sphere.stl"));
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 1442);
			Assert::IsTrue(Mesh.getFaceCount() == 2880);
			Assert::IsTrue(Mesh.checkSanity());
		}

		TEST_METHOD(MeshExport_STL_Box)
		{
			CMesh Mesh;
			PImportStream pStream(new CImportStream_COM(L"../../TestFiles/Box.stl"));
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 8);
			Assert::IsTrue(Mesh.getFaceCount() == 12);

			PExportStream pExportStream(new CExportStream_COM(L"../TestOutput/output_box.stl"));
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);
		}

		TEST_METHOD(MeshExport_STL_Sphere)
		{
			CMesh Mesh;
			PImportStream pStream(new CImportStream_COM(L"../../TestFiles/Sphere.stl"));
			PMeshImporter pImporter(new CMeshImporter_STL(pStream, 0.0001f));
			pImporter->loadMesh(&Mesh, NULL);
			Assert::IsTrue(Mesh.getNodeCount() == 1442);
			Assert::IsTrue(Mesh.getFaceCount() == 2880);

			PExportStream pExportStream(new CExportStream_COM(L"../TestOutput/output_sphere.stl"));
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);
		}

	};

}
