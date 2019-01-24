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

UnitTest_BuildItems.cpp: Defines Unittests for the BuildItems class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class BuildItems : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			auto reader3MF= model->QueryReader("3mf");
			reader3MF->ReadFromFile(InFolder + "WagonWithWheels.3mf");
		}
		virtual void TearDown() {
			model.reset();
		}
	
		static PLib3MFModel model;
		static std::string InFolder;
	};

	PLib3MFModel BuildItems::model;
	std::string BuildItems::InFolder(sTestFilesPath + "/BuildItems/");

	TEST_F(BuildItems, TestIterating)
	{
		auto buildItems = BuildItems::model->GetBuildItems();
		Lib3MF_uint64 nBuildItems = 0;
		while (buildItems->MoveNext()) {
			auto buildItem = buildItems->GetCurrent();
			nBuildItems++;
		}
		ASSERT_EQ(nBuildItems, 2);
	}

	TEST_F(BuildItems, TestObjectID)
	{
		auto buildItems = BuildItems::model->GetBuildItems();
		Lib3MF_uint64 nBuildItems = 0;
		while (buildItems->MoveNext()) {
			auto buildItem = buildItems->GetCurrent();
			Lib3MF_uint32 objectID = buildItem->GetObjectResource()->GetResourceID();
			ASSERT_EQ(buildItem->GetObjectResourceID(), objectID);
		}
	}

	TEST_F(BuildItems, TestUUID)
	{
		auto buildItems = BuildItems::model->GetBuildItems();
		ASSERT_TRUE(buildItems->MoveNext());
		auto buildItem1 = buildItems->GetCurrent();

		bool bHasUUID = false;
		auto uuidString = buildItem1->GetUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		//try {
		//	buildItem1->SetUUID(uuidString);
		//	ASSERT_TRUE(true) << "Self assignment of UUIDs should work";
		//}
		//catch (ELib3MFException) {
		//	ASSERT_TRUE(false);
		//}

		ASSERT_TRUE(buildItems->MoveNext());
		auto buildItem2 = buildItems->GetCurrent();
		try {
			buildItem2->SetUUID(uuidString);
			ASSERT_TRUE(false) << "Duplicate UUIDs must fail";
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(BuildItems, TestTransformations)
	{
		auto buildItems = BuildItems::model->GetBuildItems();
		ASSERT_TRUE(buildItems->MoveNext());
		auto buildItem = buildItems->GetCurrent();
			
		sLib3MFTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}

		buildItem->SetObjectTransform(transformation);
		ASSERT_TRUE(buildItem->HasObjectTransform());
		auto newTransformation = buildItem->GetObjectTransform();

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 3; j++)
				ASSERT_EQ(transformation.m_Fields[i][j], newTransformation.m_Fields[i][j]);
	}

	TEST_F(BuildItems, TestItem)
	{
		auto buildItems = BuildItems::model->GetBuildItems();
		ASSERT_TRUE(buildItems->MoveNext());
		{
			auto buildItem = buildItems->GetCurrent();

			auto componentsObject = buildItem->GetObjectResource();
			ASSERT_FALSE(componentsObject->IsMeshObject());
			ASSERT_TRUE(componentsObject->IsComponentsObject());

			ASSERT_FALSE(buildItem->HasObjectTransform());

			std::string partNumber("TestComponent");
			buildItem->SetPartNumber(partNumber);
			auto newPartNumber = buildItem->GetPartNumber();
			ASSERT_TRUE(partNumber == newPartNumber);
		}

		ASSERT_TRUE(buildItems->MoveNext());
		{
			auto buildItem = buildItems->GetCurrent();

			auto meshObject = buildItem->GetObjectResource();
			ASSERT_TRUE(meshObject->IsMeshObject());
			ASSERT_FALSE(meshObject->IsComponentsObject());

			ASSERT_TRUE(buildItem->HasObjectTransform());

			auto partNumber = buildItem->GetPartNumber();
			ASSERT_TRUE(partNumber == "spare wheel");
		}
		
		ASSERT_FALSE(buildItems->MoveNext());
	}

}
