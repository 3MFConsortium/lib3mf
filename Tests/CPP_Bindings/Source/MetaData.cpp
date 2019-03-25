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

UnitTest_MetaData.cpp: Defines Unittests for the MetaData class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class MetaData : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			model = CWrapper::CreateModel();
			metaDataGroup = model->GetMetaDataGroup();
			metaData = metaDataGroup->AddMetaData("", "Title", "TheTitle", "xs:string", true);
		}
		virtual void TearDown() {
			model.reset();
		}

		static PModel model;
		static PMetaDataGroup metaDataGroup;
		static PMetaData metaData;
	};

	PModel MetaData::model;
	PMetaDataGroup MetaData::metaDataGroup;
	PMetaData MetaData::metaData;

	TEST_F(MetaData, DefaultNameSpace)
	{
		auto md = metaDataGroup->AddMetaData("", "Designer", "SomeDesigner", "xs:string", true);
		try {
			metaDataGroup->AddMetaData("", "NoStandardName", "TheTitle", "xs:string", true);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(MetaData, OtherNameSpace)
	{
		metaDataGroup->AddMetaData("ANameSpace", "NoStandardName", "TheTitle", "xs:string", true);
	}

	TEST_F(MetaData, GetProperties)
	{
		std::string nameSpace = "ANameSpace";
		std::string name = "AName";
		std::string key = nameSpace + ":" + name;
		std::string value = "AValue";
		std::string type = "AType";
		bool mustPreserve = true;

		auto md = metaDataGroup->AddMetaData(nameSpace, name, value, type, mustPreserve);

		ASSERT_EQ(md->GetNameSpace(), nameSpace);
		ASSERT_EQ(md->GetName(), name);
		ASSERT_EQ(md->GetKey(), key);
		ASSERT_EQ(md->GetValue(), value);
		ASSERT_EQ(md->GetType(), type);
		ASSERT_EQ(md->GetMustPreserve(), mustPreserve);
	}

	TEST_F(MetaData, SetProperties)
	{
		std::string nameSpace = "AnotherNameSpace";
		std::string name = "AnotherName";
		std::string key = nameSpace + ":" + name;
		std::string value = "AnotherValue";
		std::string type = "AnotherType";
		bool mustPreserve = false;

		metaData->SetNameSpace(nameSpace);
		metaData->SetName(name);
		metaData->SetValue(value);
		metaData->SetType(type);
		metaData->SetMustPreserve(mustPreserve);

		ASSERT_EQ(metaData->GetNameSpace(), nameSpace);
		ASSERT_EQ(metaData->GetName(), name);
		ASSERT_EQ(metaData->GetKey(), key);
		ASSERT_EQ(metaData->GetValue(), value);
		ASSERT_EQ(metaData->GetType(), type);
		ASSERT_EQ(metaData->GetMustPreserve(), mustPreserve);
	}

	TEST_F(MetaData, DuplicateMetaData)
	{
		try {
			metaDataGroup->AddMetaData("", "Title", "TheTitle", "xs:string", true);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(MetaData, InvalidNameInDefaultNamespace1)
	{
		auto md = metaDataGroup->AddMetaData("", "Designer", "TheTitle", "xs:string", true);
		try {
			md->SetName("INVALIDNAME");
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(MetaData, InvalidNameInDefaultNamespace2)
	{
		auto md = metaDataGroup->AddMetaData("ANameSpace", "SomeName", "TheTitle", "xs:string", true);
		try {
			md->SetNameSpace("");
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(MetaData, EmtpyStrings)
	{
		try {
			auto md = metaDataGroup->AddMetaData("ANameSpace", "", "TheTitle", "xs:string", true);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		try {
			auto md = metaDataGroup->AddMetaData("ANameSpace", "SomeName", "TheTitle", "", true);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		auto md = metaDataGroup->AddMetaData("ANameSpace", "SomeName", "TheTitle", "xs:string", true);
		try {
			md->SetName("");
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}

		try {
			md->SetType("");
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}
}

