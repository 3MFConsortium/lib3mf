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

UnitTest_Attachments.cpp: Defines Unittests for handling of attachments

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class AttachmentsT : public ::testing::Test {
	protected:
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sRelationShipPath;
		const std::string m_sRelationShipPath_file;
		const std::string m_sAttachmetType;
		const std::string m_sAttachmetPayload;
		const std::string m_sAttachmentPath;
	protected:
		AttachmentsT() :
			m_sFolderName("TestOutput"), m_sFilenameReadWrite("output_attachment.3mf"),
			// m_sRelationShipPath("/Attachments/test.xml"),
			m_sRelationShipPath("/Attachments/test"),
			m_sRelationShipPath_file("/Attachments/test fromfile.xml"),
			m_sAttachmetType("http://schemas.autodesk.com/dmg/testattachment/2017/08"),
			m_sAttachmetPayload("PAYLOAD"),
			m_sAttachmentPath(std::string (TESTFILESPATH)+"/Resources/attachment.xml")
		{
		}

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
	PWrapper AttachmentsT::wrapper;

	TEST_F(AttachmentsT, AddRemove)
	{
		auto attachment = model->AddAttachment(m_sRelationShipPath + ".xml", m_sAttachmetType);
		model->RemoveAttachment(attachment.get());

		try {
			model->FindAttachment(m_sRelationShipPath + ".xml");
			ASSERT_FALSE(true);
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
	}

	TEST_F(AttachmentsT, AddAttachmentBuffer)
	{
		auto attachment = model->AddAttachment(m_sRelationShipPath + ".xml", m_sAttachmetType);

		attachment->ReadFromBuffer(CInputVector<Lib3MF_uint8>((Lib3MF_uint8*)m_sAttachmetPayload.data(), m_sAttachmetPayload.size()));

		std::vector<Lib3MF_uint8> buffer;
		attachment->WriteToBuffer(buffer);

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), m_sAttachmetPayload.begin());
		ASSERT_TRUE(bAreEqual);
	}

	TEST_F(AttachmentsT, AddAttachmentFile)
	{
		auto attachment = model->AddAttachment(m_sRelationShipPath + ".xml", m_sAttachmetType);

		attachment->ReadFromFile(m_sAttachmentPath);

		std::vector<Lib3MF_uint8> buffer;
		attachment->WriteToBuffer(buffer);

		bool bAreEqual = std::equal(buffer.begin(), buffer.end(), m_sAttachmetPayload.begin());
		ASSERT_TRUE(bAreEqual);
	}

	TEST_F(AttachmentsT, FindAttachment)
	{
		std::string sRelationShipPath1("/Attachments/test1.xml");
		std::string sAttachmetType1("http://schemas.autodesk.com/dmg/testattachment/2017/08/1");
		std::string sRelationShipPath2("/Attachments/test2.xml");
		std::string sAttachmetType2("http://schemas.autodesk.com/dmg/testattachment/2017/08/2");

		auto attachment1 = model->AddAttachment(sRelationShipPath1, sAttachmetType1);
		auto attachment2 = model->AddAttachment(sRelationShipPath2, sAttachmetType2);

		try {
			model->FindAttachment("NotAnAttachmentURI");
			ASSERT_FALSE(true);
		}
		catch (...) {
			ASSERT_TRUE(true);
		}

		auto foundAttachment1 = model->FindAttachment(sRelationShipPath1);
		foundAttachment1->GetPath() == attachment1->GetPath();
		foundAttachment1->GetRelationShipType() == attachment1->GetRelationShipType();

		auto foundAttachment2 = model->FindAttachment(sRelationShipPath2);
		foundAttachment2->GetPath() == attachment2->GetPath();
		foundAttachment2->GetRelationShipType() == attachment1->GetRelationShipType();
	}

	TEST_F(AttachmentsT, WriteReadAttachment)
	{
		{
			for (int i = 0; i < 2; i++) {
				auto attachment = model->AddAttachment(m_sRelationShipPath + std::to_string(i) + ".xml", m_sAttachmetType);
				attachment->ReadFromBuffer(CLib3MFInputVector<Lib3MF_uint8>((Lib3MF_uint8*)m_sAttachmetPayload.data(), m_sAttachmetPayload.size()));
			}
			model->AddCustomContentType("xml", "application/xml");

			auto writer = model->QueryWriter("3mf");

			ASSERT_TRUE(CreateDir(m_sFolderName.c_str())) << L"Could not create folder.";
			writer->WriteToFile(m_sFolderName + "/" + m_sFilenameReadWrite);
		}
		
		{
			auto readModel = wrapper->CreateModel();
			auto reader = readModel->QueryReader("3mf");

			reader->ReadFromFile(m_sFolderName + "/" + m_sFilenameReadWrite);
			Lib3MF_uint32 count = readModel->GetAttachmentCount();
			ASSERT_EQ(count, 0);
		}
		
		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->AddRelationToRead(m_sAttachmetType);
		reader->ReadFromFile(m_sFolderName + "/" + m_sFilenameReadWrite);
		Lib3MF_uint32 count = readModel->GetAttachmentCount();

		ASSERT_EQ(count, 2);
		for (Lib3MF_uint32 i = 0; i<count; i++) {
			auto attachment = readModel->GetAttachment(i);
			ASSERT_EQ(m_sAttachmetType.compare(attachment->GetRelationShipType()), 0);
			ASSERT_EQ((m_sRelationShipPath + std::to_string(i) + ".xml").compare(attachment->GetPath()), 0);

			std::vector<Lib3MF_uint8> buffer;
			attachment->WriteToBuffer(buffer);

			bool bAreEqual = std::equal(buffer.begin(), buffer.end(), m_sAttachmetPayload.begin());
			ASSERT_TRUE(bAreEqual);
		}
	}

}


