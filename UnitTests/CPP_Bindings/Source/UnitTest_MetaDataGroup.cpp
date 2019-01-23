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

UnitTest_MetaDataGroup.cpp: Defines Unittests for the MetaDataGroup class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class MetaDataGroup : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
		}
		virtual void TearDown() {
			model.reset();
		}

		static PLib3MFModel model;
	};

	PLib3MFModel MetaDataGroup::model;

	TEST_F(MetaDataGroup, ModelGet)
	{
		auto metaDataGroup = model->GetMetaDataGroup();
	}

	TEST_F(MetaDataGroup, GenericGroup)
	{
		auto metaDataGroup = model->GetMetaDataGroup();
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 0);
		auto metaDatum = metaDataGroup->AddMetaData("NameSpace", "Name", "TheName", "string", true);
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 1);
		// ASSERT_EQ(metaDatum->GetNameSpace() == metaDatum->GetNameSpace());

		metaDataGroup->GetMetaData(0);
		metaDataGroup->GetMetaDataByKey("NameSpace", "Name");
	}

	TEST_F(MetaDataGroup, GenericGroupRemoveByIndex)
	{
		auto metaDataGroup = model->GetMetaDataGroup();
		auto metaDatum = metaDataGroup->AddMetaData("NameSpace", "Name", "TheName", "string", true);

		metaDataGroup->RemoveMetaDataByIndex(0);
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 0);
	}

	TEST_F(MetaDataGroup, GenericGroupRemove)
	{
		auto metaDataGroup = model->GetMetaDataGroup();
		auto metaDatum = metaDataGroup->AddMetaData("NameSpace", "Name", "TheName", "string", true);

		metaDataGroup->RemoveMetaData(metaDatum.get());
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 0);
	}

	TEST_F(MetaDataGroup, GenericGroupRemoveByKey)
	{
		auto metaDataGroup = model->GetMetaDataGroup();
		auto metaDatum = metaDataGroup->AddMetaData("NameSpace", "Name", "TheName", "string", true);

		metaDataGroup->RemoveMetaData(metaDataGroup->GetMetaDataByKey("NameSpace", "Name").get());
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 0);
	}



	//TEST_F(MetaDataGroup, ModelRead)
	//{
	//	ASSERT_FALSE(true);
	//}

	//TEST_F(MetaDataGroup, ModelWriteRead)
	//{
	//	ASSERT_FALSE(true);
	//}

	//TEST_F(MetaDataGroup, BuildItemRead)
	//{
	//	ASSERT_FALSE(true);
	//}

	//TEST_F(MetaDataGroup, BuildItemWrite)
	//{
	//	ASSERT_FALSE(true);
	//}

	//TEST_F(MetaDataGroup, ObjectRead)
	//{
	//	ASSERT_FALSE(true);
	//}

	//TEST_F(MetaDataGroup, ObjectWrite)
	//{
	//	ASSERT_FALSE(true);
	//}
}
