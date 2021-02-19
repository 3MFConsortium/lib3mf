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

UnitTest_TextureProperties.cpp: Defines Unittests for texture properties

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class TextureProperty : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());

			std::string sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			std::string sPath_Texture = "/3D/Textures/MyTexture.png";

			textureAttachment = model->AddAttachment(sPath_Texture, sRelationshipType_Texture);
			textureAttachment->ReadFromFile(sTestFilesPath + "Textures/tex1.png");
			texture2D = model->AddTexture2DFromAttachment(textureAttachment.get());
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;
		PAttachment textureAttachment;
		PTexture2D texture2D;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper TextureProperty::wrapper;

	TEST_F(TextureProperty, SetGet_Texture2DGroup)
	{
		std::vector<sTex2Coord> coords(0);

		auto texture2DGroup = model->AddTexture2DGroup(texture2D.get());
		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = texture2DGroup->GetResourceID();

			coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
			properties[i].m_PropertyIDs[0] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
			coords.push_back({ 1.0*((uint64_t) i + 1) / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
			properties[i].m_PropertyIDs[1] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
			coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*((uint64_t)i + 1) / mesh->GetTriangleCount() });
			properties[i].m_PropertyIDs[2] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
		}
		mesh->SetAllTriangleProperties(properties);

		std::vector<sTriangleProperties> obtainedProperties;
		mesh->GetAllTriangleProperties(obtainedProperties);
		int count = 0;
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(obtainedProperties[i].m_ResourceID, properties[i].m_ResourceID);
			for (size_t j = 0; j < 3; j++) {
				EXPECT_EQ(obtainedProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}

			sTriangleProperties currentProperty;
			mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
			auto currentTexture2DGroup = model->GetTexture2DGroupByID(currentProperty.m_ResourceID);
			EXPECT_EQ(currentProperty.m_ResourceID, properties[i].m_ResourceID);
			for (size_t j = 0; j < 3; j++) {
				EXPECT_EQ(currentProperty.m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
				sTex2Coord uvcoord = currentTexture2DGroup->GetTex2Coord(currentProperty.m_PropertyIDs[j]);
				EXPECT_DOUBLE_EQ(uvcoord.m_U, coords[count].m_U);
				EXPECT_DOUBLE_EQ(uvcoord.m_V, coords[count++].m_V);
			}
		}

		std::vector<Lib3MF_uint32> properties2D;
		texture2DGroup->GetAllPropertyIDs(properties2D);
	}

	TEST_F(TextureProperty, WriteRead)
	{
		auto texture2DGroup = model->AddTexture2DGroup(texture2D.get());
		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = texture2DGroup->GetResourceID();
			properties[i].m_PropertyIDs[0] = texture2DGroup->AddTex2Coord({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
			properties[i].m_PropertyIDs[1] = texture2DGroup->AddTex2Coord({ 1.0*(i + 1) / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
			properties[i].m_PropertyIDs[2] = texture2DGroup->AddTex2Coord({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*(i + 1) / mesh->GetTriangleCount() });
		}
		mesh->SetAllTriangleProperties(properties);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		// writer->WriteToFile("Texture_Out.3mf");

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		// reader->ReadFromFile("Texture_Out.3mf");
		reader->ReadFromBuffer(buffer);

		int texture2DGroupCount = 0;
		auto iterator = readModel->GetTexture2DGroups();
		while (iterator->MoveNext())
		{
			auto texture2Dgroup = iterator->GetCurrentTexture2DGroup();
			ASSERT_EQ(texture2Dgroup->GetCount(), properties.size()*3);
			texture2DGroupCount++;
			auto localTexture2D = texture2Dgroup->GetTexture2D();
			ASSERT_EQ(localTexture2D->GetAttachment()->GetStreamSize(), texture2D->GetAttachment()->GetStreamSize());
		}
		ASSERT_EQ(texture2DGroupCount, 1);
	}
	
}
