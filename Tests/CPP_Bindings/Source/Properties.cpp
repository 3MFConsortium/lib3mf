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

		for (size_t i=0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(properties[i].m_ResourceID, 0);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(properties[i].m_PropertyIDs[j], 0);
			}
		}
	}

	TEST_F(Properties, ClearProperties)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = baseMaterialGroup->GetResourceID();
			for (int j = 0; j < 3; j++) {
				properties[i].m_PropertyIDs[j] = (i % (2 + j)) ? anotherMaterial : someMaterial;
			}
		}
		mesh->SetAllTriangleProperties(properties);

		mesh->ClearAllProperties();
		mesh->GetAllTriangleProperties(properties);
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(properties[i].m_ResourceID, 0);
			for (size_t j = 0; j < 3; j++) {
				EXPECT_EQ(properties[i].m_PropertyIDs[j], 0);
			}
		}
	}

	TEST_F(Properties, ObjectLevelPropertiesSetGet)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		Lib3MF_uint32 nObjectResourceID = 0;
		Lib3MF_uint32 nObjectPropertyID = 0;

		EXPECT_FALSE(mesh->GetObjectLevelProperty(nObjectResourceID, nObjectPropertyID));

		mesh->SetObjectLevelProperty(baseMaterialGroup->GetResourceID(), someMaterial);

		EXPECT_TRUE(mesh->GetObjectLevelProperty(nObjectResourceID, nObjectPropertyID));
		EXPECT_EQ(nObjectResourceID, baseMaterialGroup->GetResourceID());
		EXPECT_EQ(nObjectPropertyID, someMaterial);
	}

	TEST_F(Properties, ObjectLevelPropertiesWriteRead)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = baseMaterialGroup->GetResourceID();
			for (int j = 0; j < 3; j++) {
				properties[i].m_PropertyIDs[j] = ( i % (2+j) ) ? someMaterial : anotherMaterial;
			}
		}
		mesh->SetAllTriangleProperties(properties);
		mesh->SetObjectLevelProperty(baseMaterialGroup->GetResourceID(), someMaterial);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader3MF = readModel->QueryReader("3mf");
		reader3MF->ReadFromBuffer(buffer);

		auto readMesh = readModel->GetMeshObjectByID(2);
		std::vector<sTriangleProperties> readProperties;
		readMesh->GetAllTriangleProperties(readProperties);

		ASSERT_EQ(readMesh->GetTriangleCount(), mesh->GetTriangleCount());
		for (size_t i = 0; i < readMesh->GetTriangleCount(); i++) {
			auto readBaseMaterialGroup = readModel->GetBaseMaterialGroupByID(readProperties[i].m_ResourceID);
			for (int j = 0; j < 3; j++) {
				CompareColors(baseMaterialGroup->GetDisplayColor(properties[i].m_PropertyIDs[j]), baseMaterialGroup->GetDisplayColor(readProperties[i].m_PropertyIDs[j]));
			}
		}

		Lib3MF_uint32 nObjectResourceID = 0;
		Lib3MF_uint32 nObjectPropertyID = 0;
		EXPECT_TRUE(readMesh->GetObjectLevelProperty(nObjectResourceID, nObjectPropertyID));
		EXPECT_EQ(nObjectResourceID, 1);
		EXPECT_EQ(nObjectPropertyID, someMaterial);

		auto readBaseMaterialGroup = readModel->GetBaseMaterialGroupByID(nObjectResourceID);
		CompareColors(baseMaterialGroup->GetDisplayColor(someMaterial), readBaseMaterialGroup->GetDisplayColor(nObjectPropertyID));
	}


	// Copy-pasted from the previous test ("ObjectLevelPropertiesWriteRead"), but adds metadata to the mesh
	TEST_F(Properties, ObjectLevelPropertiesWriteRead_WithMetaData)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = baseMaterialGroup->GetResourceID();
			for (int j = 0; j < 3; j++) {
				properties[i].m_PropertyIDs[j] = (i % (2 + j)) ? someMaterial : anotherMaterial;
			}
		}
		mesh->SetAllTriangleProperties(properties);
		mesh->SetObjectLevelProperty(baseMaterialGroup->GetResourceID(), someMaterial);

		mesh->GetMetaDataGroup()->AddMetaData("", "Designer", "SomeDesigner", "xs:string", true);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader3MF = readModel->QueryReader("3mf");
		reader3MF->ReadFromBuffer(buffer);

		auto readMesh = readModel->GetMeshObjectByID(2);
		std::vector<sTriangleProperties> readProperties;
		readMesh->GetAllTriangleProperties(readProperties);

		ASSERT_EQ(readMesh->GetTriangleCount(), mesh->GetTriangleCount());
		for (size_t i = 0; i < readMesh->GetTriangleCount(); i++) {
			auto readBaseMaterialGroup = readModel->GetBaseMaterialGroupByID(readProperties[i].m_ResourceID);
			for (int j = 0; j < 3; j++) {
				CompareColors(baseMaterialGroup->GetDisplayColor(properties[i].m_PropertyIDs[j]), baseMaterialGroup->GetDisplayColor(readProperties[i].m_PropertyIDs[j]));
			}
		}

		Lib3MF_uint32 nObjectResourceID = 0;
		Lib3MF_uint32 nObjectPropertyID = 0;
		EXPECT_TRUE(readMesh->GetObjectLevelProperty(nObjectResourceID, nObjectPropertyID));
		EXPECT_EQ(nObjectResourceID, 1);
		EXPECT_EQ(nObjectPropertyID, someMaterial);

		auto readBaseMaterialGroup = readModel->GetBaseMaterialGroupByID(nObjectResourceID);
		CompareColors(baseMaterialGroup->GetDisplayColor(someMaterial), readBaseMaterialGroup->GetDisplayColor(nObjectPropertyID));
	}

	TEST_F(Properties, ObjectLevelPropertiesReadOnly1)
	{
		auto readModel = wrapper->CreateModel();
		auto reader3MF = readModel->QueryReader("3mf");
		reader3MF->ReadFromFile(sTestFilesPath + "/" + "Properties" + "/PyramidWithoutProperties.3mf");
		CheckReaderWarnings(reader3MF, 0);

		auto readMesh = readModel->GetMeshObjectByID(2);
		Lib3MF_uint32 nResourceID = 0;
		Lib3MF_uint32 nPropertyID = 0;
		EXPECT_FALSE(readMesh->GetObjectLevelProperty(nResourceID, nPropertyID));
	}

	TEST_F(Properties, ObjectLevelPropertiesReadOnly2)
	{
		auto readModel = wrapper->CreateModel();
		auto reader3MF = readModel->QueryReader("3mf");
		reader3MF->ReadFromFile(sTestFilesPath + "/" + "Properties" + "/PyramidWithProperties.3mf");
		CheckReaderWarnings(reader3MF, 0);

		auto readMesh = readModel->GetMeshObjectByID(2);
		Lib3MF_uint32 nResourceID = 0;
		Lib3MF_uint32 nPropertyID = 0;
		EXPECT_TRUE(readMesh->GetObjectLevelProperty(nResourceID, nPropertyID));
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
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
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
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(gottenProperties[i].m_ResourceID, properties[i].m_ResourceID);
			for (size_t j = 0; j < 3; j++) {
				EXPECT_EQ(gottenProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}

			sTriangleProperties currentProperty;
			mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
			EXPECT_EQ(currentProperty.m_ResourceID, properties[i].m_ResourceID);
			for (size_t j = 0; j < 3; j++) {
				EXPECT_EQ(currentProperty.m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}
		}
	}

	TEST_F(Properties, WriteMeshWithoutProperties)
	{
		auto writer = model->QueryWriter("3mf");

		std::vector<sTriangleProperties> properties;
		mesh->GetAllTriangleProperties(properties);

		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
	}

	TEST_F(Properties, WriteMeshWithMissingProperties)
	{
		auto writer = model->QueryWriter("3mf");

		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", wrapper->RGBAToColor(100, 200, 150, 255));

		sTriangleProperties singleProperties;
		singleProperties.m_ResourceID = baseMaterialGroup->GetResourceID();
		singleProperties.m_PropertyIDs[0] = someMaterial;
		singleProperties.m_PropertyIDs[1] = someMaterial;
		singleProperties.m_PropertyIDs[2] = someMaterial;

		mesh->SetTriangleProperties(0, singleProperties);

		std::vector<Lib3MF_uint8> buffer;
		ASSERT_SPECIFIC_THROW(writer->WriteToBuffer(buffer), ELib3MFException);

		mesh->SetObjectLevelProperty(singleProperties.m_ResourceID, singleProperties.m_PropertyIDs[0]);
		writer->WriteToBuffer(buffer);
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
			for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
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

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
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
			for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
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

	TEST_F(Properties_Color, WriteRead)
	{
		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);

		auto iterator = readModel->GetColorGroups();
		while (iterator->MoveNext())
		{
			auto colorgroup = iterator->GetCurrentColorGroup();
			ASSERT_EQ(colorgroup->GetCount(), 2);
		}

		auto mesh = readModel->GetMeshObjectByID(2);
		std::vector<sTriangleProperties> properties;
		mesh->GetAllTriangleProperties(properties);

		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
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
