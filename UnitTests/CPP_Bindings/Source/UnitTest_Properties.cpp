/*++

Copyright (C) 2018 3MF Consortium

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

UnitTest_Properties.cpp: Defines Unittests for the Properties

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Properties : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();

			std::vector<sLib3MFPosition> vctVertices;
			std::vector<sLib3MFTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFMeshObject mesh;
	};


	TEST_F(Properties, GetProperties)
	{
		std::vector<sLib3MFTriangleProperties> properties;
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
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));
		sLib3MFTriangleProperties prop;
		prop.m_ResourceID = baseMaterialGroup->GetResourceID();
		prop.m_PropertyIDs[0] = someMaterial;
		prop.m_PropertyIDs[1] = someMaterial;
		prop.m_PropertyIDs[2] = anotherMaterial;

		ASSERT_SPECIFIC_THROW(mesh->SetTriangleProperties(0, prop), ELib3MFException);
	}

	TEST_F(Properties, SetGet_BaseMaterial)
	{
		auto baseMaterialGroup = model->AddBaseMaterialGroup();
		auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));
		auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));

		std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
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

		std::vector<sLib3MFTriangleProperties> gottenProperties;
		mesh->GetAllTriangleProperties(gottenProperties);
		for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(gottenProperties[i].m_ResourceID, properties[i].m_ResourceID);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(gottenProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}

			sLib3MFTriangleProperties currentProperty;
			mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
			EXPECT_EQ(currentProperty.m_ResourceID, properties[i].m_ResourceID);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(currentProperty.m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}
		}
	}

	class Properties_Writer : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();

			std::vector<sLib3MFPosition> vctVertices;
			std::vector<sLib3MFTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());

			auto baseMaterialGroup = model->AddBaseMaterialGroup();
			auto someMaterial = baseMaterialGroup->AddMaterial("SomeMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));
			auto anotherMaterial = baseMaterialGroup->AddMaterial("AnotherMaterial", CLib3MFWrapper::RGBAToColor(100, 200, 150, 255));

			std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
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

		PLib3MFModel model;
		PLib3MFMeshObject mesh;
	};

	TEST_F(Properties_Writer, BaseMaterial)
	{
		auto writer = model->QueryWriter("3mf");
		writer->WriteToFile("BaseMaterial_Out.3mf");

		auto readModel = CLib3MFWrapper::CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromFile("BaseMaterial_Out.3mf");

		auto iterator = readModel->GetBaseMaterialGroups();
		while (iterator->MoveNext())
		{
			auto basematerialgroup = iterator->GetCurrentBaseMaterialGroup();
			ASSERT_EQ(basematerialgroup->GetCount(), 2);
		}
	}

}
