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

UnitTest_CustomAttributes.cpp: Defines Unittests for the CustomAttributes class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class CustomAttributes : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();
			reader = model->QueryReader("3mf");
			writer = model->QueryWriter("3mf");
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PReader reader;
		PWriter writer;
		static std::string m_sFolderName;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper CustomAttributes::wrapper;

	std::string CustomAttributes::m_sFolderName("TestOutput");

	TEST_F(CustomAttributes, ModelGet)
	{
		auto CustomAttributes = model->GetCustomAttributes();
	}

	TEST_F(CustomAttributes, GenericAttributes)
	{
		auto customAttributes = model->GetCustomAttributes();
		ASSERT_EQ(customAttributes->GetAttributeCount(), 0);
		auto attr = customAttributes->AddAttribute("NameSpace", "Name", "TheName");
		ASSERT_EQ(customAttributes->GetAttributeCount(), 1);
		ASSERT_TRUE(attr->GetNameSpace() == attr->GetNameSpace());

		customAttributes->GetAttribute(0);
		customAttributes->GetAttributeByKey("NameSpace", "Name");
	}

	TEST_F(CustomAttributes, GenericAttributesRemoveByIndex)
	{
		auto customAttributes = model->GetCustomAttributes();
		auto attr = customAttributes->AddAttribute("NameSpace", "Name", "TheName");

		customAttributes->RemoveAttributeByIndex(0);
		ASSERT_EQ(customAttributes->GetAttributeCount(), 0);
	}

	TEST_F(CustomAttributes, GenericAttributesRemove)
	{
		auto customAttributes = model->GetCustomAttributes();
		auto metaDatum = customAttributes->AddAttribute("NameSpace", "Name", "TheName");

		customAttributes->RemoveAttribute(metaDatum.get());
		ASSERT_EQ(customAttributes->GetAttributeCount(), 0);
	}

	TEST_F(CustomAttributes, GenericAttributesRemoveByKey)
	{
		auto customAttributes = model->GetCustomAttributes();
		auto metaDatum = customAttributes->AddAttribute("NameSpace", "Name", "TheName");

		customAttributes->RemoveAttribute(customAttributes->GetAttributeByKey("NameSpace", "Name").get());
		ASSERT_EQ(customAttributes->GetAttributeCount(), 0);
	}

	void VerifyAttributes(PCustomAttributes customAttributes)
	{
		ASSERT_EQ(customAttributes->GetAttributeCount(), 3);

		ASSERT_TRUE(customAttributes->GetAttributeByKey("http://www.vendorwwebsite.com/3mf/vendor13mfextension/2017/01", "CustomData1")->GetValue() == "Value of CustomData1");
		ASSERT_TRUE(customAttributes->GetAttributeByKey("http://www.vendorwwebsite.com/3mf/vendor13mfextension/2017/01", "CustomData2")->GetValue() == "Value of CustomData2");
		ASSERT_TRUE(customAttributes->GetAttributeByKey("http://www.vendorwwebsite.com/3mf/vendor23mfextension/2017/01", "CustomData3")->GetValue() == "Value of CustomData3");
	}

	TEST_F(CustomAttributes, Read_Model)
	{
		reader->ReadFromFile(sTestFilesPath + "/CustomAttribute/" + "CustomAttribute_Model.3mf");
		CheckReaderWarnings(reader, 0);

		auto customAttributes = model->GetCustomAttributes();
		
		VerifyAttributes(customAttributes);
	}

	TEST_F(CustomAttributes, Read_BuildItem)
	{
		reader->ReadFromFile(sTestFilesPath + "/CustomAttribute/" + "CustomAttribute_BuildItem.3mf");
		CheckReaderWarnings(reader, 0);

		auto buildItems = model->GetBuildItems();
		ASSERT_TRUE(buildItems->MoveNext());

		auto buildItem = buildItems->GetCurrent();

		auto customAttributes = buildItem->GetCustomAttributes();
		
		VerifyAttributes(customAttributes);
	}

	TEST_F(CustomAttributes, Read_Object)
	{
		reader->ReadFromFile(sTestFilesPath + "/CustomAttribute/" + "CustomAttribute_Object.3mf");
		CheckReaderWarnings(reader, 0);

		auto customAttributes = model->GetMeshObjectByID(2)->GetCustomAttributes();

		VerifyAttributes(customAttributes);
	}

	void CompareCustomAttributes(PCustomAttributes A, PCustomAttributes B)
	{
		ASSERT_EQ(A->GetAttributeCount(), B->GetAttributeCount());
		
		for (Lib3MF_uint32 i = 0; i < A->GetAttributeCount(); i++) {
			auto attrA = A->GetAttribute(i);
			auto attrB = B->GetAttribute(i);

			EXPECT_EQ(attrA->GetKey(), attrB->GetKey());
			EXPECT_EQ(attrA->GetValue(), attrB->GetValue());
		}
	}

	TEST_F(CustomAttributes, Write_Model)
	{
		std::string nameSpace = "ANameSpace";
		std::string locale = "AVendorLocale";
		std::string name = "AName";
		std::string value = "TheValue with \n \\ all sorts of \n crazy characters\\t  >	<Ti/!:\r\n  h  ' \"\" ";

		model->AddCustomNameSpace(nameSpace, locale);
		auto customAttributes = model->GetCustomAttributes();
		customAttributes->AddAttribute(nameSpace, name, value);

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto newModel = wrapper->CreateModel();
		{
			auto newReader = newModel->QueryReader("3mf");
			newReader->ReadFromBuffer(buffer);
			CheckReaderWarnings(newReader, 0);
		}
		CompareCustomAttributes(customAttributes, newModel->GetCustomAttributes());
	}

	TEST_F(CustomAttributes, Write_Object)
	{
		std::string nameSpace = "ANameSpace";
		std::string locale = "AVendorLocale";
		std::string name = "AName";
		std::string value = "AValue";

		model->AddCustomNameSpace(nameSpace, locale);
		auto componentsObject = model->AddComponentsObject();
		auto customAttributes = componentsObject->GetCustomAttributes();
		customAttributes->AddAttribute(nameSpace, name, value);

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto newModel = wrapper->CreateModel();
		{
			auto newReader = newModel->QueryReader("3mf");
			newReader->ReadFromBuffer(buffer);
			CheckReaderWarnings(newReader, 0);
		}
		CompareCustomAttributes(customAttributes, newModel->GetComponentsObjectByID(1)->GetCustomAttributes());
	}

}
