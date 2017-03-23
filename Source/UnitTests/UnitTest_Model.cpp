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

UnitTest_Model.cpp: Defines Unittests for the model class

--*/

#include "CppUnitTest.h"
#include "Model\Classes\NMR_Model.h"
#include "Model\Reader\NMR_ModelReader_3MF_OPC.h"
#include "Model\Writer\NMR_ModelWriter_3MF_OPC.h"
#include "Model\Writer\NMR_ModelWriter_3MF_Native.h"
#include "Common\Mesh\NMR_Mesh.h"
#include "Common\MeshExport\NMR_MeshExporter_STL.h"
#include "Common\Platform\NMR_ExportStream_COM.h"
#include "Common\Platform\NMR_ImportStream_COM.h"
#include "Model\Classes\NMR_ModelBuildItem.h"
#include "Model\Classes\NMR_ModelMeshObject.h"
#include "Model\Classes\NMR_ModelObject.h"
#include "Common\NMR_Exception.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace NMR
{

	TEST_CLASS(ModelTest)
	{
	private:


	public:
		const double EPS_TOL = 1e-5;

		TEST_METHOD(Model_3MFBox)
		{
			PModel pModel = std::make_shared<CModel> ();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC> (pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/Box.3mf");
			pModelReader->readStream(pImportStream);

			CMesh Mesh;
			pModel->mergeToMesh(&Mesh);

			PExportStream pExportStream(new CExportStream_COM(L"../TestOutput/output_3mfbox.stl"));
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);
		}

		TEST_METHOD(Model_3MFBox10)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/cube10.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfcube10.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFModel093)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/track093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_track093.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFColor093)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/Color_093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_color093.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFTexture093)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/Texture_093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_texture093.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase1)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase1_cube.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase1.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase2)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase2_material093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase2.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		//TEST_METHOD(Model_3MFBase3)
		//{
		//	PModel pModel = std::make_shared<CModel>();
		//	PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
		//	PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase3_MaterialAndTexture093.3mf");
		//	pModelReader->readStream(pImportStream);

		//	PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_OPC>(pModel);
		//	PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase3.3mf");
		//	pModelWriter->exportToStream(pExportStream);
		//}

		TEST_METHOD(Model_3MFBase4)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase4_Mesh1.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase4.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase5)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase5_texture093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase5.3mf");
			pModelWriter->exportToStream(pExportStream);
		}


		TEST_METHOD(Model_3MFBase6)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase6_VertexAlex093.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase6.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase7)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase7_VertexEmmett1.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase7.3mf");
			pModelWriter->exportToStream(pExportStream);
		}


		TEST_METHOD(Model_3MFBase8)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase8_VertexTet1.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase8.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase9)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase9_ladybug.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase9.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase10)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase10_box.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase10.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase11)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase11_AdjMount.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase11.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase12)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase12_holed_cube.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase12.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase13)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase13_holed_cube_support.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase13.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFBase14)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/3mfbase14_materialandcolor2.3mf");
			pModelReader->readStream(pImportStream);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_3mfbase14.3mf");
			pModelWriter->exportToStream(pExportStream);
		}
		

		TEST_METHOD(Model_3MFModel100_TexCube)
		{
			PModel pModel = std::make_shared<CModel>();
			PModelReader pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/texcube.3mf");
			pModelReader->readStream(pImportStream);

			nfUint32 nTextureCount = pModel->getTextureStreamCount();
			Assert::IsTrue(nTextureCount == 2);

			PImportStream pTexStream = pModel->getTextureStream(0);
			Assert::IsTrue(pTexStream.get() != nullptr);

			nfUint64 nTexture0Size = pTexStream->retrieveSize();
			Assert::IsTrue(nTexture0Size ==	0x158d);

			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream pExportStream = std::make_shared<CExportStream_COM>(L"../TestOutput/output_texcube.3mf");
			pModelWriter->exportToStream(pExportStream);
		}

		TEST_METHOD(Model_3MFTexture)
		{
			PModel pModel = std::make_shared<CModel> ();
			PModelReader_3MF_OPC pModelReader = std::make_shared<CModelReader_3MF_OPC>(pModel);
			PImportStream pImportStream = std::make_shared<CImportStream_COM>(L"../../TestFiles/Texture.3mf");
			pModelReader->readStream(pImportStream);

			CMesh Mesh;
			pModel->mergeToMesh(&Mesh);

			PExportStream pExportStream(new CExportStream_COM(L"../TestOutput/output_3mftexture.stl"));
			PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
			pExporter->exportMesh(&Mesh, NULL);


			PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_Native>(pModel);
			PExportStream p3MFExportStream(new CExportStream_COM(L"../TestOutput/output_3mftexture.3mf"));
			pModelWriter->exportToStream(p3MFExportStream);
		}

	};

}
