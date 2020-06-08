/*++

Copyright (C) 2019 3MF Consortium

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

UnitTest_ProductionExtension.cpp: Defines Unittests for the Production extension
classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class ProductionExtension : public ::testing::Test {
	protected:
		sPosition pVertices[8];
		sTriangle pTriangles[12];

		virtual void SetUp() {
			model = wrapper->CreateModel();
			float fSizeX = 100.0f;
			float fSizeY = 200.0f;
			float fSizeZ = 300.0f;

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
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ProductionExtension::wrapper;

	TEST_F(ProductionExtension, ManipulatePaths)
	{
		auto rootPart = model->RootModelPart();
		auto otherPart = model->FindOrCreatePackagePart("/somepart");
		ASSERT_SPECIFIC_THROW(otherPart->SetPath(rootPart->GetPath()), ELib3MFException);
	}

	TEST_F(ProductionExtension, ReadInspect)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Production/" + "2ProductionBoxes.3mf");
		auto reader3MF = model->QueryReader("3mf");
		reader3MF->ReadFromBuffer(buffer);
		CheckReaderWarnings(reader3MF, 0);
	}

	//TEST_F(ProductionExtension, ReadWrite)
	//{
	//}

	//TEST_F(ProductionExtension, ReadWriteReadInspect)
	//{
	//}

	TEST_F(ProductionExtension, ReadModifyWrite)
	{
		auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/Production/" + "2ProductionBoxes.3mf");
		auto reader3MF = model->QueryReader("3mf");
		reader3MF->ReadFromBuffer(buffer);
		CheckReaderWarnings(reader3MF, 0);

		auto resources = model->GetResources();
		auto partToChange = model->FindOrCreatePackagePart("/3D/box2.model");
		partToChange->SetPath("/3D/boxPathChanged.model");

		auto newPart = model->FindOrCreatePackagePart("/3D/boxPathNew.model");

		while (resources->MoveNext()) {
			auto resource = resources->GetCurrent();
			std::cout << resource->PackagePart()->GetPath() << ":" << resource->GetModelResourceID() << std::endl;
			if (resource->PackagePart()->GetPath() == "/3D/box1.model") {
				resource->SetPackagePart(newPart.get());
			}
		}
		auto writer = model->QueryWriter("3mf");
		writer->WriteToFile("outProduction.3mf");
	}

	//TEST_F(ProductionExtension, ReadModifySplitWriteReadInspect)
	//{
	//}

	//TEST_F(ProductionExtension, ReadModifyJoinWriteReadInspect)
	//{
	//}

	TEST_F(ProductionExtension, SetupWrite)
	{
		std::vector<Lib3MF::sPosition> vctVertices;
		std::vector<Lib3MF::sTriangle> vctTriangles;

		auto mesh1 = model->AddMeshObject();
		fnCreateBox(vctVertices, vctTriangles);
		mesh1->SetGeometry(vctVertices, vctTriangles);

		auto component1 = model->AddComponentsObject();
		component1->AddComponent(mesh1.get(), wrapper->GetIdentityTransform());

		model->AddBuildItem(component1.get(), wrapper->GetIdentityTransform());
		auto writer = model->QueryWriter("3mf");
		writer->WriteToFile("SetupWriteRead1.3mf");

		auto newPart = model->FindOrCreatePackagePart("/3D/outsourced.model");
		mesh1->SetPackagePart(newPart.get());
		writer->WriteToFile("SetupWriteRead2.3mf");

		component1->SetPackagePart(newPart.get());
		writer->WriteToFile("SetupWriteRead3.3mf");

		mesh1->SetPackagePart(model->RootModelPart().get());
		ASSERT_SPECIFIC_THROW(writer->WriteToFile("SetupWriteRead4.3mf"), ELib3MFException);

		auto newPart2 = model->FindOrCreatePackagePart("/3D/outsourced2.model");
		mesh1->SetPackagePart(newPart2.get());
		ASSERT_SPECIFIC_THROW(writer->WriteToFile("SetupWriteRead5.3mf"), ELib3MFException);
	}

	//TEST_F(ProductionExtension, SetupWriteReadInspect)
	//{

	//}

	TEST_F(ProductionExtension, ProductionWriteExternalModel) {
		//create the attachment to be secured
		auto lModel = wrapper->CreateModel();
		auto meshObject = lModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		sTransform transformation = wrapper->GetIdentityTransform();
		
		auto part1 = lModel->FindOrCreatePackagePart("/3D/nonrootmodel1.model");
		lModel->AddBuildItem(meshObject.get(), transformation);
		meshObject->SetPackagePart(part1.get());

		meshObject = lModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		transformation = wrapper->GetTranslationTransform(0.0, 250.0, 0.0);
		meshObject->SetPackagePart(part1.get());
		lModel->AddBuildItem(meshObject.get(), transformation);
		
		meshObject = lModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		transformation = wrapper->GetTranslationTransform(0.0, 250.0, 0.0);
		lModel->AddBuildItem(meshObject.get(), transformation);
		auto part2 = lModel->FindOrCreatePackagePart("/3D/nonrootmodel2.model");
		meshObject->SetPackagePart(part2.get());


		auto writer = lModel->QueryWriter("3mf");
		//std::vector<Lib3MF_uint8> buffer;
		writer->WriteToFile("nonrootmodels.3mf");
		//writer->WriteToBuffer(buffer);

		auto modelAssert = wrapper->CreateModel();
		auto reader = modelAssert->QueryReader("3mf");
		reader->ReadFromFile("nonrootmodels.3mf");
		//reader->ReadFromBuffer(buffer);
		ASSERT_EQ(3, modelAssert->GetObjects()->Count());
		ASSERT_EQ(3, modelAssert->GetBuildItems()->Count());
	}
}
