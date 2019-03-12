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

UnitTest_TextureResources.cpp: Defines Unittests for the Texture Resource class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Model_TextureResource : public ::testing::Test {
	protected:

		virtual void SetUp() {
			m_sRelationshipType = "http://schemas.autodesk.com/dmg/testattachment/2017/08";
			m_sPath = "/Resources/attachment.xml";
			m_sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			m_sPath_Texture = "/3D/Textures/MyTexture.png";

			model = CLib3MFWrapper::CreateModel();
			otherAttachment = model->AddAttachment(m_sPath, m_sRelationshipType);
			textureAttachment = model->AddAttachment(m_sPath_Texture, m_sRelationshipType_Texture);
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFAttachment otherAttachment;
		PLib3MFAttachment textureAttachment;
		std::string m_sPath;
		std::string m_sRelationshipType;
		std::string m_sRelationshipType_Texture;
		std::string m_sPath_Texture;
	};

	TEST_F(Model_TextureResource, AddTexture)
	{
		ASSERT_SPECIFIC_THROW(model->AddTexture2DFromAttachment(otherAttachment.get()), ELib3MFException);

		auto texture = model->AddTexture2DFromAttachment(textureAttachment.get());
	}

	TEST_F(Model_TextureResource, GetSetAttachment)
	{
		auto texture = model->AddTexture2DFromAttachment(textureAttachment.get());

		auto outAttachment = texture->GetAttachment();
		ASSERT_EQ(outAttachment->GetPath(), textureAttachment->GetPath());

		ASSERT_SPECIFIC_THROW(texture->SetAttachment(otherAttachment.get()), ELib3MFException);
	}

	class Model_TextureResources : public ::testing::Test {
	protected:

		virtual void SetUp() {
			std::string m_sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			m_sPath_Texture = "/3D/Textures/MyTexture_";

			model = CLib3MFWrapper::CreateModel();
			for (int i = 0; i < 3; i++) {
				auto attachment = model->AddAttachment(m_sPath_Texture+std::to_string(i)+".png", m_sRelationshipType_Texture);
				model->AddTexture2DFromAttachment(attachment.get());
				model->AddTexture2DFromAttachment(attachment.get());
			}
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		std::string m_sPath_Texture;
	};

	TEST_F(Model_TextureResources, Iterator)
	{
		auto iterator = model->GetTexture2Ds();
		int i = 0;
		while (iterator->MoveNext()) {
			auto texture = iterator->GetCurrentTexture2D();
			EXPECT_EQ(texture->GetAttachment()->GetPath(), m_sPath_Texture + std::to_string(i/2) + ".png");
			i++;

			Lib3MF_uint32 nID = texture->GetResourceID();
			auto textureById = model->GetTexture2DByID(nID);
			EXPECT_EQ(texture->GetAttachment()->GetPath(), textureById->GetAttachment()->GetPath());
		}
	}

	TEST_F(Model_TextureResource, WriteRead)
	{
		auto writeModel = CLib3MFWrapper::CreateModel();
		std::string sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
		std::string sPath_Texture = "/3D/Textures/MyTexture";
		std::vector<Lib3MF_uint8> buffer;
		{
			auto textureAttachment1 = writeModel->AddAttachment(m_sPath_Texture + "0.png", sRelationshipType_Texture);
			auto textureAttachment2 = writeModel->AddAttachment(m_sPath_Texture + "1.png", sRelationshipType_Texture);
			auto textureAttachment3 = writeModel->AddAttachment(m_sPath_Texture + "2.png", sRelationshipType_Texture);

			auto texture1 = writeModel->AddTexture2DFromAttachment(textureAttachment1.get());
			auto texture2 = writeModel->AddTexture2DFromAttachment(textureAttachment2.get());
			auto texture3 = writeModel->AddTexture2DFromAttachment(textureAttachment3.get());

			textureAttachment1->ReadFromFile(sTestFilesPath + "Textures/tex1.png");
			textureAttachment2->ReadFromFile(sTestFilesPath + "Textures/tex2.png");
			textureAttachment3->ReadFromFile(sTestFilesPath + "Textures/tex3.png");

			auto writer = writeModel->QueryWriter("3mf");
			writer->WriteToBuffer(buffer);
		}
		
		auto readModel = CLib3MFWrapper::CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);

		ASSERT_EQ(readModel->GetAttachmentCount(), 3);

		auto textures = readModel->GetTexture2Ds();
		int count = 0;
		while (textures->MoveNext()) {
			auto texture = textures->GetCurrentTexture2D();
			count++;
		}
		ASSERT_EQ(count, 3);
	}

	class TextureResource : public ::testing::Test {
	protected:

		virtual void SetUp() {
			m_sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			m_sPath_Texture = "/3D/Textures/MyTexture.png";

			model = CLib3MFWrapper::CreateModel();
			textureAttachment = model->AddAttachment(m_sPath_Texture, m_sRelationshipType_Texture);
			texture = model->AddTexture2DFromAttachment(textureAttachment.get());
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFAttachment textureAttachment;
		PLib3MFTexture2D texture;
		std::string m_sRelationshipType_Texture;
		std::string m_sPath_Texture;
	};

	TEST_F(TextureResource, Properties)
	{
		EXPECT_EQ(texture->GetContentType(), eLib3MFTextureType::eTextureTypePNG);
		texture->SetContentType(eLib3MFTextureType::eTextureTypeJPEG);
		EXPECT_EQ(texture->GetContentType(), eLib3MFTextureType::eTextureTypeJPEG);

		eLib3MFTextureTileStyle u, v, u1, v1;
		texture->GetTileStyleUV(u, v);
		EXPECT_EQ(u, eLib3MFTextureTileStyle::eTextureTileStyleWrap);
		EXPECT_EQ(v, eLib3MFTextureTileStyle::eTextureTileStyleWrap);
		u1 = eLib3MFTextureTileStyle::eTextureTileStyleMirror;
		v1 = eLib3MFTextureTileStyle::eTextureTileStyleNoTileStyle;
		texture->SetTileStyleUV(u1, v1);
		texture->GetTileStyleUV(u, v);
		EXPECT_EQ(u, u1);
		EXPECT_EQ(v, v1);


		EXPECT_EQ(texture->GetFilter(), eLib3MFTextureFilter::eTextureFilterAuto);
		texture->SetFilter(eLib3MFTextureFilter::eTextureFilterNearest);
		EXPECT_EQ(texture->GetFilter(), eLib3MFTextureFilter::eTextureFilterNearest);
		texture->SetFilter(eLib3MFTextureFilter::eTextureFilterLinear);
		EXPECT_EQ(texture->GetFilter(), eLib3MFTextureFilter::eTextureFilterLinear);
	}

}

