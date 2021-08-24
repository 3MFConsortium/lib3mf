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
	class Outbox : public Lib3MFTest {
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
	};

	void CompareBoxes(Lib3MF::sBox boxA, Lib3MF::sBox boxB) {
		for (int i = 0; i < 3; i++) {
			EXPECT_FLOAT_EQ(boxA.m_MinCoordinate[i], boxB.m_MinCoordinate[i]);
			EXPECT_FLOAT_EQ(boxA.m_MaxCoordinate[i], boxB.m_MaxCoordinate[i]);
		}
	}


	TEST_F(Outbox, CheckMesh)
	{
		auto meshes = model->GetMeshObjects();
		meshes->MoveNext();
		auto mesh = meshes->GetCurrentMeshObject();

		Lib3MF::sBox sOutbox = mesh->GetOutbox();

		Lib3MF::sBox sExpectedOutbox;
		sExpectedOutbox.m_MinCoordinate[0] = 15.2786398f;
		sExpectedOutbox.m_MinCoordinate[1] = 3.91547990f;
		sExpectedOutbox.m_MinCoordinate[2] = 0.f;
		sExpectedOutbox.m_MaxCoordinate[0] = 160.000000f;
		sExpectedOutbox.m_MaxCoordinate[1] = 156.084518f;
		sExpectedOutbox.m_MaxCoordinate[2] = 100.000000f;

		CompareBoxes(sOutbox, sExpectedOutbox);
	}

	TEST_F(Outbox, CheckComponent)
	{
		auto components = model->GetComponentsObjects();
		components->MoveNext();
		auto component = components->GetCurrentComponentsObject();

		Lib3MF::sBox sOutbox = component->GetOutbox();

		Lib3MF::sBox sExpectedOutbox;
		sExpectedOutbox.m_MinCoordinate[0] = 64.9831238f;
		sExpectedOutbox.m_MinCoordinate[1] = -103.887222f;
		sExpectedOutbox.m_MinCoordinate[2] = -39.4011765f;
		sExpectedOutbox.m_MaxCoordinate[0] = 307.725159f;
		sExpectedOutbox.m_MaxCoordinate[1] = 101.911346f;
		sExpectedOutbox.m_MaxCoordinate[2] = 279.938263f;

		CompareBoxes(sOutbox, sExpectedOutbox);
	}

	TEST_F(Outbox, CheckBuildItemWithMesh)
	{
		auto buildItems = model->GetBuildItems();
		buildItems->MoveNext();
		auto buildItem = buildItems->GetCurrent();
		Lib3MF::sBox sOutbox = buildItem->GetOutbox();
		
		Lib3MF::sBox sExpectedOutbox;
		sExpectedOutbox.m_MinCoordinate[0] = 64.9831238f;
		sExpectedOutbox.m_MinCoordinate[1] = -103.887222f;
		sExpectedOutbox.m_MinCoordinate[2] = -39.4011765f;
		sExpectedOutbox.m_MaxCoordinate[0] = 307.725159f;
		sExpectedOutbox.m_MaxCoordinate[1] = 101.911346f;
		sExpectedOutbox.m_MaxCoordinate[2] = 279.938263f;

		CompareBoxes(sOutbox, sExpectedOutbox);
	}

	TEST_F(Outbox, CheckBuildItemWithComponent)
	{
		auto buildItems = model->GetBuildItems();
		buildItems->MoveNext();
		buildItems->MoveNext();
		auto buildItem = buildItems->GetCurrent();
		Lib3MF::sBox sOutbox = buildItem->GetOutbox();

		Lib3MF::sBox sExpectedOutbox;
		sExpectedOutbox.m_MinCoordinate[0] = 1.25774193f;
		sExpectedOutbox.m_MinCoordinate[1] = -7.59726334f;
		sExpectedOutbox.m_MinCoordinate[2] = -21.2018776f;
		sExpectedOutbox.m_MaxCoordinate[0] = 146.953094f;
		sExpectedOutbox.m_MaxCoordinate[1] = 139.430359f;
		sExpectedOutbox.m_MaxCoordinate[2] = 95.4953918f;

		CompareBoxes(sOutbox, sExpectedOutbox);
	}

	TEST_F(Outbox, CheckModel)
	{
		Lib3MF::sBox sOutbox = model->GetOutbox();

		Lib3MF::sBox sExpectedOutbox;
		sExpectedOutbox.m_MinCoordinate[0] = 1.25774193f;
		sExpectedOutbox.m_MinCoordinate[1] = -103.887222f;
		sExpectedOutbox.m_MinCoordinate[2] = -39.4011765f;
		sExpectedOutbox.m_MaxCoordinate[0] = 307.725159f;
		sExpectedOutbox.m_MaxCoordinate[1] = 139.430359f;
		sExpectedOutbox.m_MaxCoordinate[2] = 279.938263f;

		CompareBoxes(sOutbox, sExpectedOutbox);
	}
}


