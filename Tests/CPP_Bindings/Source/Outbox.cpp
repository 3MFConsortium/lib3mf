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

UnitTest_Object.cpp: Defines Unittests for the object class and its descendants

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Outbox : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + "/Models/PartsForBoundingBox.3mf");
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
	PWrapper Outbox::wrapper;

	TEST_F(Outbox, CheckMesh)
	{
		auto meshes = model->GetMeshObjects();
		meshes->MoveNext();
		auto mesh = meshes->GetCurrentMeshObject();

		Lib3MF::sBox sOutbox = mesh->GetOutbox();

		//ASSERT_FLOAT_EQ();
	}

	TEST_F(Outbox, CheckComponent)
	{
		auto components = model->GetComponentsObjects();
		components->MoveNext();
		auto mesh = components->GetCurrentComponentsObject();

		Lib3MF::sBox sOutbox = mesh->GetOutbox();
	}

	TEST_F(Outbox, CheckBuildItemWithMesh)
	{
		auto buildItems = model->GetBuildItems();
		buildItems->MoveNext();
		auto buildItem = buildItems->GetCurrent();
		Lib3MF::sBox sOutbox = buildItem->GetOutbox();
	}

	TEST_F(Outbox, CheckBuildItemWithComponent)
	{
		auto buildItems = model->GetBuildItems();
		buildItems->MoveNext();
		buildItems->MoveNext();
		auto buildItem = buildItems->GetCurrent();
		Lib3MF::sBox sOutbox = buildItem->GetOutbox();
	}

	TEST_F(Outbox, CheckModel)
	{
		Lib3MF::sBox sOutbox = model->GetOutbox();
	}
}


