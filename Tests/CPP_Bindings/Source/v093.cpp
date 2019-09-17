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

UnitTest_v093.cpp: Defines Unittests for the reading the 093-version of 3mf.

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class ReaderV093 : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
		}
		virtual void TearDown() {
			model.reset();
			reader.reset();
		}
	
		PModel model;
		PReader reader;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ReaderV093::wrapper;

	TEST_F(ReaderV093, 3MFReadFromFile_Geometry)
	{
		reader->ReadFromFile(sTestFilesPath + "/v093/" + "Track_093.3mf");
		CheckReaderWarnings(reader, 2);

		auto buildItems = model->GetBuildItems();
		ASSERT_EQ(buildItems->Count(),1);

		auto resources = model->GetResources();
		ASSERT_EQ(resources->Count(), 1);

		auto meshObjects = model->GetMeshObjects();
		ASSERT_EQ(meshObjects->Count(), 1);

		ASSERT_TRUE(meshObjects->MoveNext());
		auto mesh = meshObjects->GetCurrentMeshObject();

		EXPECT_EQ(mesh->GetTriangleCount(), 5282);
		EXPECT_EQ(mesh->GetVertexCount(), 2643);

		auto mdg = model->GetMetaDataGroup();
		EXPECT_EQ(mdg->GetMetaDataCount(), 8);
	}

	TEST_F(ReaderV093, 3MFReadFromFile_Texture)
	{
		reader->ReadFromFile(sTestFilesPath + "/v093/" + "Texture_093.3mf");
		CheckReaderWarnings(reader, 1);

		auto writer = model->QueryWriter("3mf");
		writer->WriteToFile("v093reout.3mf");

	}

	TEST_F(ReaderV093, 3MFReadFromFile_Material)
	{
		reader->ReadFromFile(sTestFilesPath + "/v093/" + "Material_093.3mf");
		CheckReaderWarnings(reader, 1);

		auto materialGroups = model->GetBaseMaterialGroups();
		ASSERT_EQ(materialGroups->Count(), 3);

		ASSERT_TRUE(materialGroups->MoveNext());
		auto pMaterialGroup = materialGroups->GetCurrentBaseMaterialGroup();
		ASSERT_EQ(pMaterialGroup->GetCount(), 1);

		sColor sColorExpected;
		sColorExpected.m_Alpha = 0xff;
		sColorExpected.m_Red = 0xff;
		sColorExpected.m_Green = 0xff;
		sColorExpected.m_Blue = 0xff;
		CompareColors(sColorExpected, pMaterialGroup->GetDisplayColor(1));

		ASSERT_TRUE(materialGroups->MoveNext());
		pMaterialGroup = materialGroups->GetCurrentBaseMaterialGroup();
		ASSERT_EQ(pMaterialGroup->GetCount(), 1);
		sColorExpected.m_Red = 0xfd;
		sColorExpected.m_Green = 0x08;
		sColorExpected.m_Blue = 0x0e;
		CompareColors(sColorExpected, pMaterialGroup->GetDisplayColor(1));

		ASSERT_TRUE(materialGroups->MoveNext());
		pMaterialGroup = materialGroups->GetCurrentBaseMaterialGroup();
		ASSERT_EQ(pMaterialGroup->GetCount(), 1);
		sColorExpected.m_Red = 0x8a;
		sColorExpected.m_Green = 0x74;
		sColorExpected.m_Blue = 0xff;
		CompareColors(sColorExpected, pMaterialGroup->GetDisplayColor(1));
	}

}
