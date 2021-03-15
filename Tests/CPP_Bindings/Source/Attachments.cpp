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
	class AttachmentsT : public Lib3MFTest {
	protected:
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sRelationShipPath;
		const std::string m_sRelationShipPath_file;
		const std::string m_sAttachmetType;
		const std::string m_sAttachmetPayload;
		const std::string m_sAttachmentPath;
		const std::string m_sThumbnailPath;
	protected:
		AttachmentsT() :
			m_sFolderName("TestOutput"), m_sFilenameReadWrite("output_attachment.3mf"),
			// m_sRelationShipPath("/Attachments/test.xml"),
			m_sRelationShipPath("/Attachments/test"),
			m_sRelationShipPath_file("/Attachments/test fromfile.xml"),
			m_sAttachmetType("http://schemas.autodesk.com/dmg/testattachment/2017/08"),
			m_sAttachmetPayload("PAYLOAD"),
			m_sAttachmentPath(std::string(TESTFILESPATH) + "/Resources/attachment.xml"),
			m_sThumbnailPath(std::string(TESTFILESPATH) + "/Attachments/thumbnail.png")
		{
		}

		virtual void SetUp() {
			model = wrapper->CreateModel();
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
	};

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
		EXPECT_TRUE(foundAttachment1->GetPath() == attachment1->GetPath());
		EXPECT_TRUE(foundAttachment1->GetRelationShipType() == attachment1->GetRelationShipType());

		auto foundAttachment2 = model->FindAttachment(sRelationShipPath2);
		EXPECT_TRUE(foundAttachment2->GetPath() == attachment2->GetPath());
		EXPECT_TRUE(foundAttachment2->GetRelationShipType() == attachment2->GetRelationShipType());
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
		for (Lib3MF_uint32 i = 0; i < count; i++) {
			auto attachment = readModel->GetAttachment(i);
			ASSERT_EQ(m_sAttachmetType.compare(attachment->GetRelationShipType()), 0);
			ASSERT_EQ((m_sRelationShipPath + std::to_string(i) + ".xml").compare(attachment->GetPath()), 0);

			std::vector<Lib3MF_uint8> buffer;
			attachment->WriteToBuffer(buffer);

			bool bAreEqual = std::equal(buffer.begin(), buffer.end(), m_sAttachmetPayload.begin());
			ASSERT_TRUE(bAreEqual);
		}
	}

	void CheckPackageThumbnailAreEqual(PModel pModel1, PModel pModel2)
	{
		ASSERT_EQ(pModel1->HasPackageThumbnailAttachment(), pModel2->HasPackageThumbnailAttachment());
		if (pModel1->HasPackageThumbnailAttachment()) {
			auto attachment1 = pModel1->GetPackageThumbnailAttachment();

			ASSERT_TRUE(pModel2->HasPackageThumbnailAttachment());
			auto attachment2 = pModel2->GetPackageThumbnailAttachment();

			std::vector<Lib3MF_uint8> vctAttachmentBufferIn, vctAttachmentBufferOut;
			attachment1->WriteToBuffer(vctAttachmentBufferIn);
			attachment2->WriteToBuffer(vctAttachmentBufferOut);

			bool bAreEqual = std::equal(vctAttachmentBufferIn.begin(), vctAttachmentBufferIn.end(), vctAttachmentBufferOut.begin());
			ASSERT_TRUE(bAreEqual);
		}
	}

	TEST_F(AttachmentsT, WriteReadPackageThumbnail)
	{
		auto attachment = model->CreatePackageThumbnailAttachment();
		attachment->ReadFromFile(m_sThumbnailPath);

		std::vector<Lib3MF_uint8> vctFileBuffer;
		{
			auto writer = model->QueryWriter("3mf");
			writer->WriteToBuffer(vctFileBuffer);
		}
		auto readModel = wrapper->CreateModel();
		{
			auto reader = readModel->QueryReader("3mf");
			reader->ReadFromBuffer(vctFileBuffer);
		}
		CheckPackageThumbnailAreEqual(model, readModel);
	}

	TEST_F(AttachmentsT, ManipulatePackageThumbnail)
	{
		ASSERT_FALSE(model->HasPackageThumbnailAttachment());

		auto attachment = model->CreatePackageThumbnailAttachment();
		ASSERT_TRUE(model->HasPackageThumbnailAttachment());

		model->RemovePackageThumbnailAttachment();
		ASSERT_FALSE(model->HasPackageThumbnailAttachment());
		ASSERT_TRUE(model->GetPackageThumbnailAttachment() == nullptr);

		attachment = model->CreatePackageThumbnailAttachment();
		ASSERT_TRUE(model->HasPackageThumbnailAttachment());
	}

	TEST_F(AttachmentsT, ReadPackageThumbnail)
	{
		auto reader = model->QueryReader("3mf");
		reader->ReadFromFile(std::string(TESTFILESPATH) + "/Attachments/withPackageThumbnail.3mf");

		ASSERT_TRUE(model->HasPackageThumbnailAttachment());
		auto attachment = model->GetPackageThumbnailAttachment();
		
		std::vector<Lib3MF_uint8> vctThumbnailBuffer, vctFileBuffer;
		attachment->WriteToBuffer(vctThumbnailBuffer);
		
		attachment->ReadFromFile(m_sThumbnailPath);
		attachment->WriteToBuffer(vctFileBuffer);

		bool bAreEqual = std::equal(vctThumbnailBuffer.begin(), vctThumbnailBuffer.end(), vctFileBuffer.begin());
		ASSERT_TRUE(bAreEqual);
	}

}


