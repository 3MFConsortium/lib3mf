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

UnitTest_Model.cpp: Defines Unittests for the model class

--*/

#include "UnitTest_Utilities.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h"
#include "Common/Mesh/NMR_Mesh.h"
#include "Common/MeshExport/NMR_MeshExporter_STL.h"
#include "Model/Classes/NMR_ModelBuildItem.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelTextureAttachment.h"
#include "Common/NMR_Exception.h"

#ifdef NMR_COM_NATIVE
#include "Model/Reader/NMR_ModelReader_3MF_OPC.h"
#include "Model/Writer/NMR_ModelWriter_3MF_OPC.h"
#include "Common/Platform/NMR_ExportStream_COM.h"
#include "Common/Platform/NMR_ImportStream_COM.h"
#define IMPORTSTREAM CImportStream_COM
#define EXPORTSTREAM CExportStream_COM
#define WRITER CModelWriter_3MF_OPC
#define READER CModelReader_3MF_OPC
#else
#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#include "Model/Writer/NMR_ModelWriter_3MF_Native.h"
#include "Common/Platform/NMR_ExportStream_GCC_Native.h"
#include "Common/Platform/NMR_ImportStream_GCC_Native.h"
#define IMPORTSTREAM CImportStream_GCC_Native
#define EXPORTSTREAM CExportStream_GCC_Native
#define WRITER CModelWriter_3MF_Native
#define READER CModelReader_3MF_Native
#endif

namespace NMR
{
	const std::wstring sInPath = sTestFilesPath + L"/CPP_UnitTests/";
	const std::wstring sOutPath = sOutFilesPath;

	const double EPS_TOL = 1e-5;

	TEST(Model, 3MFBox)
	{
		PModel pModel = std::make_shared<CModel> ();
		PModelReader pModelReader = std::make_shared<READER> (pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"Box.3mf").c_str());
		pModelReader->readStream(pImportStream);

		CMesh Mesh;
		pModel->mergeToMesh(&Mesh);

		PExportStream pExportStream(new EXPORTSTREAM((sOutPath + L"output_3mfbox.stl").c_str()));
		PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
		pExporter->exportMesh(&Mesh, NULL);
	}

	TEST(Model, 3MFBox10)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"cube10.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfcube10.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFModel093)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"track093.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_track093.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFColor093)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"Color_093.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_color093.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFTexture093)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"Texture_093.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_texture093.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase1)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath+L"3mfbase1_cube.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase1.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase2)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath+L"3mfbase2_material093.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase2.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	//TEST(Model, 3MFBase3)
	//{
	//	PModel pModel = std::make_shared<CModel>();
	//	PModelReader pModelReader = std::make_shared<READER>(pModel);
	//	PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase3_MaterialAndTexture093.3mf").c_str());
	//	pModelReader->readStream(pImportStream);

	//	PModelWriter pModelWriter = std::make_shared<CModelWriter_3MF_OPC>(pModel);
	//	PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase3.3mf").c_str());
	//	pModelWriter->exportToStream(pExportStream);
	//}

	TEST(Model, 3MFBase4)
	{
		PModel pModel = std::make_shared<CModel>();
#ifdef NMR_COM_NATIVE
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase4_Mesh1.3mf").c_str());
#else
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase4_Mesh1.3mf").c_str());
#endif
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase4.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase5)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase5_texture093.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase5.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}


	TEST(Model, 3MFBase8)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase8_VertexTet1.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase8.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase9)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase9_ladybug.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase9.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase10)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase10_box.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase10.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase11)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase11_AdjMount.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase11.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase12)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase12_holed_cube.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase12.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase13)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase13_holed_cube_support.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase13.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFBase14)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"3mfbase14_materialandcolor2.3mf").c_str());
		pModelReader->readStream(pImportStream);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_3mfbase14.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}


	TEST(Model, 3MFModel100_TexCube)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"texcube.3mf").c_str());
		pModelReader->readStream(pImportStream);

		nfUint32 nAttachmentCount = pModel->getAttachmentCount();
		ASSERT_TRUE(nAttachmentCount == 2);
			
		PImportStream pTexStream = pModel->getModelAttachment(0)->getStream();
		ASSERT_TRUE(pTexStream.get() != nullptr);

		nfUint64 nTexture0Size = pTexStream->retrieveSize();
		ASSERT_TRUE(nTexture0Size == 0x158d);

		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream pExportStream = std::make_shared<EXPORTSTREAM>((sOutPath + L"output_texcube.3mf").c_str());
		pModelWriter->exportToStream(pExportStream);
	}

	TEST(Model, 3MFTexture)
	{
		PModel pModel = std::make_shared<CModel>();
		PModelReader_3MF pModelReader = std::make_shared<READER>(pModel);
		PImportStream pImportStream = std::make_shared<IMPORTSTREAM>((sInPath + L"Texture.3mf").c_str());
		pModelReader->readStream(pImportStream);

		CMesh Mesh;
		pModel->mergeToMesh(&Mesh);

		PExportStream pExportStream(new EXPORTSTREAM((sOutPath + L"output_3mftexture.stl").c_str()));
		PMeshExporter pExporter(new CMeshExporter_STL(pExportStream));
		pExporter->exportMesh(&Mesh, NULL);


		PModelWriter pModelWriter = std::make_shared<WRITER>(pModel);
		PExportStream p3MFExportStream(new EXPORTSTREAM((sOutPath + L"output_3mftexture.3mf").c_str()));
		pModelWriter->exportToStream(p3MFExportStream);
	}

}