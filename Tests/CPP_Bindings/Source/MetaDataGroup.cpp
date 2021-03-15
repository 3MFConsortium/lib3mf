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

UnitTest_MetaDataGroup.cpp: Defines Unittests for the MetaDataGroup class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class MetaDataGroup : public Lib3MFTest {
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
	};

	std::string MetaDataGroup::m_sFolderName("TestOutput");

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
		ASSERT_TRUE(metaDatum->GetNameSpace() == metaDatum->GetNameSpace());

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

	void VerifyMetaDataGroup(PMetaDataGroup metaDataGroup)
	{
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 8);

		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "Copyright")->GetValue() == "(C) Microsoft Corporation 2013");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "Application")->GetValue() == "Microsoft 3D Builder");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "LicenseTerms")->GetValue() == "All rights reserved");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "Title")->GetValue() == "Cube");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "Designer")->GetValue() == "Microsoft Corporation");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "CreationDate")->GetValue() == "2013-10-07");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "ModificationDate")->GetValue() == "2014-03-27");
		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("", "Description")->GetValue() == "Cube");
	}

	void VerifyVendorMetaDataGroup(PMetaDataGroup metaDataGroup)
	{
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 1);

		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("http://www.vendorwwebsite.com/3mf/vendor13mfextension/2017/01", "CustomMetadata1")->GetValue()
			== "Value of CustomMetadata1");
	}

	void VerifyFailedVendorMetaDataGroup(PMetaDataGroup metaDataGroup)
	{
		ASSERT_EQ(metaDataGroup->GetMetaDataCount(), 1);

		ASSERT_TRUE(metaDataGroup->GetMetaDataByKey("vendor1", "CustomMetadata1")->GetValue()
			== "Value of CustomMetadata1");
	}

	TEST_F(MetaDataGroup, Read_Model_DefaultNS)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Model_DefaultNS.3mf");
		CheckReaderWarnings(reader, 0);

		auto metaDataGroup = model->GetMetaDataGroup();
		
		VerifyMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_BuildItem_DefaultNS)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_BuildItem_DefaultNS.3mf");
		CheckReaderWarnings(reader, 0);

		auto buildItems = model->GetBuildItems();
		ASSERT_TRUE(buildItems->MoveNext());

		auto buildItem = buildItems->GetCurrent();

		auto metaDataGroup = buildItem->GetMetaDataGroup();

		VerifyMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_Object_DefaultNS)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Object_DefaultNS.3mf");
		CheckReaderWarnings(reader, 0);

		auto metaDataGroup = model->GetMeshObjectByID(2)->GetMetaDataGroup();

		VerifyMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_Model_DefaultNS_Fail)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Model_DefaultNS_Fail.3mf");
		CheckReaderWarnings(reader, 1);
	}

	TEST_F(MetaDataGroup, Read_Object_DefaultNS_Fail)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Object_DefaultNS_Fail.3mf");
		CheckReaderWarnings(reader, 1);
	}

	TEST_F(MetaDataGroup, Read_Model_VendorNS)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Model_VendorNS.3mf");
		CheckReaderWarnings(reader, 0);
		auto metaDataGroup = model->GetMetaDataGroup();
		VerifyVendorMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_Object_VendorNS)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Object_VendorNS.3mf");
		CheckReaderWarnings(reader, 0);
		auto metaDataGroup = model->GetMeshObjectByID(2)->GetMetaDataGroup();
		VerifyVendorMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_Model_VendorNS_Fail)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Model_VendorNS_Fail.3mf");
		CheckReaderWarnings(reader, 1);
		auto metaDataGroup = model->GetMetaDataGroup();
		VerifyFailedVendorMetaDataGroup(metaDataGroup);
	}

	TEST_F(MetaDataGroup, Read_Object_VendorNS_Fail)
	{
		reader->ReadFromFile(sTestFilesPath + "/MetaData/" + "MetaData_Object_VendorNS_Fail.3mf");
		CheckReaderWarnings(reader, 1);
		auto metaDataGroup = model->GetMeshObjectByID(2)->GetMetaDataGroup();
		VerifyFailedVendorMetaDataGroup(metaDataGroup);
	}

	void CompareMetaDataGroups(PMetaDataGroup A, PMetaDataGroup B)
	{
		ASSERT_EQ(A->GetMetaDataCount(), B->GetMetaDataCount());
		
		for (Lib3MF_uint32 i = 0; i < A->GetMetaDataCount(); i++) {
			auto metadataA = A->GetMetaData(i);
			auto metadataB = B->GetMetaData(i);

			EXPECT_EQ(metadataA->GetKey(), metadataB->GetKey());
			EXPECT_EQ(metadataA->GetValue(), metadataB->GetValue());
			EXPECT_EQ(metadataA->GetType(), metadataB->GetType());
			EXPECT_EQ(metadataA->GetMustPreserve(), metadataB->GetMustPreserve());
		}
	}

	TEST_F(MetaDataGroup, Write_DefaultNS_Model)
	{
		std::string nameSpace = "ANameSpace";
		std::string name = "Title with \"<> escape characters";
		std::string key = nameSpace + ":" + name;
		std::string value = "TheValue with \n \\ all sorts of \n crazy characters\\t  >	<Ti/!:\r\n  h  ' \"\" ";
		std::string type = "AType";
		bool mustPreserve = true;

		auto metaDataGroup = model->GetMetaDataGroup();
		metaDataGroup->AddMetaData(nameSpace, name, value, type, mustPreserve);

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto newModel = wrapper->CreateModel();
		{
			auto newReader = newModel->QueryReader("3mf");
			newReader->ReadFromBuffer(buffer);
			CheckReaderWarnings(newReader, 0);
		}
		CompareMetaDataGroups(metaDataGroup, newModel->GetMetaDataGroup());
	}

	TEST_F(MetaDataGroup, Write_DefaultNS_Object)
	{
		std::string nameSpace = "ANameSpace";
		std::string name = "AName";
		std::string key = nameSpace + ":" + name;
		std::string value = "AValue";
		std::string type = "AType";
		bool mustPreserve = true;

		auto componentsObject = model->AddComponentsObject();
		auto metaDataGroup = componentsObject->GetMetaDataGroup();
		metaDataGroup->AddMetaData(nameSpace, name, value, type, mustPreserve);

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto newModel = wrapper->CreateModel();
		{
			auto newReader = newModel->QueryReader("3mf");
			newReader->ReadFromBuffer(buffer);
			CheckReaderWarnings(newReader, 0);
		}
		CompareMetaDataGroups(metaDataGroup, newModel->GetComponentsObjectByID(1)->GetMetaDataGroup());
	}

}
