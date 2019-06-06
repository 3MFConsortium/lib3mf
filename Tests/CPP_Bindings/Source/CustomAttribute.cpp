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

UnitTest_CustomAttribute.cpp: Defines Unittests for the Attribute class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class CustomAttribute : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();
			customAttributes = model->GetCustomAttributes();
			attribute = customAttributes->AddAttribute("", "Title", "TheTitle");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PCustomAttributes customAttributes;
		PCustomAttribute attribute;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper CustomAttribute::wrapper;

	TEST_F(CustomAttribute, OtherNameSpace)
	{
		customAttributes->AddAttribute("ANameSpace", "NoStandardName", "TheTitle");
	}

	TEST_F(CustomAttribute, GetProperties)
	{
		std::string nameSpace = "ANameSpace";
		std::string name = "AName";
		std::string key = nameSpace + ":" + name;
		std::string value = "AValue";

		auto att = customAttributes->AddAttribute(nameSpace, name, value);

		ASSERT_EQ(att->GetNameSpace(), nameSpace);
		ASSERT_EQ(att->GetName(), name);
		ASSERT_EQ(att->GetKey(), key);
		ASSERT_EQ(att->GetValue(), value);
	}

	TEST_F(CustomAttribute, SetProperties)
	{
		std::string nameSpace = "AnotherNameSpace";
		std::string name = "AnotherName";
		std::string key = nameSpace + ":" + name;
		std::string value = "AnotherValue";

		attribute->SetNameSpace(nameSpace);
		attribute->SetName(name);
		attribute->SetValue(value);

		ASSERT_EQ(attribute->GetNameSpace(), nameSpace);
		ASSERT_EQ(attribute->GetName(), name);
		ASSERT_EQ(attribute->GetKey(), key);
		ASSERT_EQ(attribute->GetValue(), value);
	}

	TEST_F(CustomAttribute, DuplicateMetaData)
	{
		try {
			customAttributes->AddAttribute("", "Title", "TheTitle");
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException) {
			ASSERT_TRUE(true);
		}
	}
}

