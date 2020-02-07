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
		virtual void SetUp() {
			model = wrapper->CreateModel();
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
		partToChange->Set("/3D/boxPathChanged.model");

		auto newPart = model->FindOrCreatePackagePart("/3D/boxPathNew.model");

		while (resources->MoveNext()) {
			auto resource = resources->GetCurrent();
			std::cout << resource->PackagePart()->Get() << ":" << resource->GetModelResourceID() << std::endl;
			if (resource->PackagePart()->Get() == "/3D/box1.model") {
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

}
