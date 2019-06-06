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

UnitTest_Properties.cpp: Defines Unittests for the color and basematerial properties

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Properties : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;

		static void SetUpTestCase() {
			wrapper = wrapper->loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Properties::wrapper;


	TEST_F(Properties, GetProperties)
	{
		std::vector<sTriangleProperties> properties;
		mesh->GetAllTriangleProperties(properties);

		for (Lib3MF_uint64 i=0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(properties[i].m_ResourceID, 0);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(properties[i].m_PropertyIDs[j], 0);
			}
		}
	}

	TEST_F(Properties, DISABLED_Set_BaseMaterial_Fail)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		sTriangleProperties prop;
		prop.m_ResourceID = baseMaterialGroup->GetResourceID();
		prop.m_PropertyIDs[0] = someMaterial;
		prop.m_PropertyIDs[1] = someMaterial;
		prop.m_PropertyIDs[2] = anotherMaterial;

		ASSERT_SPECIFIC_THROW(mesh->SetTriangleProperties(0, prop), ELib3MFException);
	}

	TEST_F(Properties, SetGet_BaseMaterial)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = baseMaterialGroup->GetResourceID();
			auto material = someMaterial;
			if (i % 2 == 0) {
				material = anotherMaterial;
			}
			properties[i].m_PropertyIDs[0] = material;
			properties[i].m_PropertyIDs[1] = material;
			properties[i].m_PropertyIDs[2] = material;
		}
		mesh->SetAllTriangleProperties(properties);

		std::vector<sTriangleProperties> gottenProperties;
		mesh->GetAllTriangleProperties(gottenProperties);
		for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(gottenProperties[i].m_ResourceID, properties[i].m_ResourceID);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(gottenProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}

			sTriangleProperties currentProperty;
			mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
			EXPECT_EQ(currentProperty.m_ResourceID, properties[i].m_ResourceID);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(currentProperty.m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}
		}
	}

	class Properties_BaseMaterial : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());

			auto baseMaterialGroup = model->AddBaseMaterialGroup();
			auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
			auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

			std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
			for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
				properties[i].m_ResourceID = baseMaterialGroup->GetResourceID();
				auto material = someMaterial;
				if (i % 2 == 0) {
					material = anotherMaterial;
				}
				properties[i].m_PropertyIDs[0] = material;
				properties[i].m_PropertyIDs[1] = material;
				properties[i].m_PropertyIDs[2] = material;
			}
			mesh->SetAllTriangleProperties(properties);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Properties_BaseMaterial::wrapper;

	TEST_F(Properties_BaseMaterial, WriteRead)
	{
		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		// writer->WriteToFile("BaseMaterial_Out.3mf");

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		// reader->ReadFromFile("BaseMaterial_Out.3mf");
		reader->ReadFromBuffer(buffer);

		auto iterator = readModel->GetBaseMaterialGroups();
		while (iterator->MoveNext())
		{
			auto basematerialgroup = iterator->GetCurrentBaseMaterialGroup();
			ASSERT_EQ(basematerialgroup->GetCount(), 2);
		}
	}



	class Properties_Color : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());
			
			auto colorGroup = model->AddColorGroup();
			cSomeColor = wrapper->RGBAToColor(100, 200, 150, 255);
			cAnotherColor = wrapper->RGBAToColor(250, 100, 50, 200);
			auto someColor = colorGroup->AddColor(cSomeColor);
			auto anotherColor = colorGroup->AddColor(cAnotherColor);

			std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
			for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
				properties[i].m_ResourceID = colorGroup->GetResourceID();
				auto color = someColor;
				if (i % 2 == 0) {
					color = anotherColor;
				}
				properties[i].m_PropertyIDs[0] = color;
				properties[i].m_PropertyIDs[1] = anotherColor;
				properties[i].m_PropertyIDs[2] = someColor;
			}
			mesh->SetAllTriangleProperties(properties);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;
		sColor cSomeColor, cAnotherColor;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Properties_Color::wrapper;

	void CompareColors(sColor c1, sColor c2)
	{
		EXPECT_EQ(c1.m_Alpha, c2.m_Alpha);
		EXPECT_EQ(c1.m_Red, c2.m_Red);
		EXPECT_EQ(c1.m_Green, c2.m_Green);
		EXPECT_EQ(c1.m_Blue, c2.m_Blue);
	}

	TEST_F(Properties_Color, WriteRead)
	{
		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		//writer->WriteToFile("Color_Out.3mf");

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);
		// reader->ReadFromFile("Color_Out.3mf");

		auto iterator = readModel->GetColorGroups();
		while (iterator->MoveNext())
		{
			auto colorgroup = iterator->GetCurrentColorGroup();
			ASSERT_EQ(colorgroup->GetCount(), 2);
		}

		auto mesh = readModel->GetMeshObjectByID(2);
		std::vector<sTriangleProperties> properties;
		mesh->GetAllTriangleProperties(properties);

		for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
			auto colorGroup = readModel->GetColorGroupByID(properties[i].m_ResourceID);
			auto color = cSomeColor;
			if (i % 2 == 0) {
				color = cAnotherColor;
			}
			CompareColors(color, colorGroup->GetColor(properties[i].m_PropertyIDs[0]));
			CompareColors(cAnotherColor, colorGroup->GetColor(properties[i].m_PropertyIDs[1]));
			CompareColors(cSomeColor, colorGroup->GetColor(properties[i].m_PropertyIDs[2]));
		}
	}


}
