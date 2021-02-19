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

UnitTest_MultiProperties.cpp: Defines Unittests for MultiProperties

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class MultiProperties : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());

			baseMaterialGroup = model->AddBaseMaterialGroup();
			sColor tIn;
			tIn.m_Red = 20;
			tIn.m_Blue = 10;
			tIn.m_Green = 100;
			tIn.m_Alpha = 255;
			Lib3MF_uint32 bmPID1 = baseMaterialGroup->AddMaterial("Material1", tIn);
			tIn.m_Green = 200;
			Lib3MF_uint32 bmPID2 = baseMaterialGroup->AddMaterial("Material2", tIn);

			compositeMaterialGroup = model->AddCompositeMaterials(baseMaterialGroup.get());
			std::vector<sCompositeConstituent> constituents(2);
			constituents[0].m_MixingRatio = 0.5;
			constituents[0].m_PropertyID = bmPID1;
			constituents[1].m_MixingRatio = 0.5;
			constituents[1].m_PropertyID = bmPID2;
			compositeMaterialGroup->AddComposite(constituents);


			colorGroup = model->AddColorGroup();
			colorGroup->AddColor(tIn);
			tIn.m_Alpha = 100;
			colorGroup->AddColor(tIn);

			std::string sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			std::string sPath_Texture = "/3D/Textures/MyTexture.png";

			auto textureAttachment = model->AddAttachment(sPath_Texture, sRelationshipType_Texture);
			textureAttachment->ReadFromFile(sTestFilesPath + "Textures/tex1.png");
			texture2D = model->AddTexture2DFromAttachment(textureAttachment.get());

			std::vector<sTex2Coord> coords(0);
			texture2DGroup = model->AddTexture2DGroup(texture2D.get());
			std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
			for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
				properties[i].m_ResourceID = texture2DGroup->GetResourceID();

				coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
				properties[i].m_PropertyIDs[0] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
				coords.push_back({ 1.0*(i + 1) / mesh->GetTriangleCount(), 1.0 - 1.0*i / mesh->GetTriangleCount() });
				properties[i].m_PropertyIDs[1] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
				coords.push_back({ 1.0*i / mesh->GetTriangleCount(), 1.0 - 1.0*(i + 1) / mesh->GetTriangleCount() });
				properties[i].m_PropertyIDs[2] = texture2DGroup->AddTex2Coord(coords.rbegin()[0]);
			}
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;
		PBaseMaterialGroup baseMaterialGroup;
		PCompositeMaterials compositeMaterialGroup;
		PColorGroup colorGroup;
		PTexture2D texture2D;
		PTexture2DGroup texture2DGroup;

		void setupMultiPropertyGroup(PMultiPropertyGroup multiPropertyGroup);
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper MultiProperties::wrapper;

	TEST_F(MultiProperties, Create)
	{
		auto multiPropertyGroup = model->AddMultiPropertyGroup();
		ASSERT_EQ(model->GetPropertyTypeByID(multiPropertyGroup->GetResourceID()), ePropertyType::Multi);
		auto multiPropertyGroup_ReOut = model->GetMultiPropertyGroupByID(multiPropertyGroup->GetResourceID());
	}

	TEST_F(MultiProperties, AddGet)
	{
		auto multiPropertyGroup = model->AddMultiPropertyGroup();

		ASSERT_EQ(multiPropertyGroup->GetCount(), 0);
		ASSERT_EQ(multiPropertyGroup->GetLayerCount(), 0);

		setupMultiPropertyGroup(multiPropertyGroup);
	}

	void MultiProperties::setupMultiPropertyGroup(PMultiPropertyGroup multiPropertyGroup)
	{
		multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ baseMaterialGroup->GetResourceID(), eBlendMethod::Mix });
		ASSERT_EQ(multiPropertyGroup->GetLayerCount(), 1);

		// cannot add two materials
		ASSERT_SPECIFIC_THROW(multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ compositeMaterialGroup->GetResourceID(), eBlendMethod::Mix }
		), ELib3MFException);

		multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ colorGroup->GetResourceID(), eBlendMethod::Multiply });

		// cannot add two colorgroups
		ASSERT_SPECIFIC_THROW(multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ colorGroup->GetResourceID(), eBlendMethod::Mix }
		), ELib3MFException);

		// cannot add resource of incorrect type
		ASSERT_SPECIFIC_THROW(multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ texture2D->GetResourceID(), eBlendMethod::Mix }
		), ELib3MFException);

		multiPropertyGroup->AddLayer(
			sMultiPropertyLayer{ texture2DGroup->GetResourceID(), eBlendMethod::Mix });

		ASSERT_EQ(multiPropertyGroup->GetLayerCount(), 3);


		std::vector<Lib3MF_uint32> material_PropertyIDs;
		baseMaterialGroup->GetAllPropertyIDs(material_PropertyIDs);
		std::vector<Lib3MF_uint32> color_PropertyIDs;
		colorGroup->GetAllPropertyIDs(color_PropertyIDs);
		std::vector<Lib3MF_uint32> texture_PropertyIDs;
		texture2DGroup->GetAllPropertyIDs(texture_PropertyIDs);

		std::vector<Lib3MF_uint32> propertyIDs({ material_PropertyIDs[0], color_PropertyIDs[0], texture_PropertyIDs[0]});
		multiPropertyGroup->AddMultiProperty(propertyIDs);
		propertyIDs[0] = material_PropertyIDs[1];
		multiPropertyGroup->AddMultiProperty(propertyIDs);
		propertyIDs[2] = texture_PropertyIDs[3];
		Lib3MF_uint32 nPropertyID = multiPropertyGroup->AddMultiProperty(propertyIDs);

		propertyIDs[2] = texture_PropertyIDs[4];
		multiPropertyGroup->SetMultiProperty(nPropertyID, propertyIDs);

		std::vector<Lib3MF_uint32> onbtainedPropertyIDs;
		multiPropertyGroup->GetMultiProperty(nPropertyID, onbtainedPropertyIDs);
		ASSERT_TRUE(propertyIDs == onbtainedPropertyIDs);

		multiPropertyGroup->RemoveMultiProperty(nPropertyID);
		nPropertyID = multiPropertyGroup->AddMultiProperty(propertyIDs);
		ASSERT_EQ(multiPropertyGroup->GetCount(), 3);

		// TODO: invalid property ids must not be used
		//propertyIDs[0] = 20;
		//ASSERT_SPECIFIC_THROW(multiPropertyGroup->SetMultiProperty(nPropertyID, propertyIDs), ELib3MFException);
		//ASSERT_SPECIFIC_THROW(multiPropertyGroup->AddMultiProperty(propertyIDs), ELib3MFException);
	}

	TEST_F(MultiProperties, IncorrectNumberOfBlendMethods)
	{
		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->SetStrictModeActive(true);
		ASSERT_SPECIFIC_THROW(reader->ReadFromFile(sTestFilesPath + "Properties/MultiProperty_Out_incorrect.3mf"), ELib3MFException);
	}

	TEST_F(MultiProperties, WriteRead)
	{
		auto multiPropertyGroup = model->AddMultiPropertyGroup();

		setupMultiPropertyGroup(multiPropertyGroup);

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		
		{
			std::vector<Lib3MF_uint32> vctPropertyIDs;
			multiPropertyGroup->GetAllPropertyIDs(vctPropertyIDs);
			for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
				properties[i].m_ResourceID = multiPropertyGroup->GetResourceID();
				for (Lib3MF_uint64 k = 0; k < 3; k++) {
					properties[i].m_PropertyIDs[k] = vctPropertyIDs[(i + k) % vctPropertyIDs.size()];
				}
			}
			mesh->SetAllTriangleProperties(properties);
		}
	
		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		reader->ReadFromBuffer(buffer);

		int multiPropertyCount = 0;
		auto iterator = readModel->GetMultiPropertyGroups();
		while (iterator->MoveNext())
		{
			auto readMultiPropertyGroup = iterator->GetCurrentMultiPropertyGroup();
			ASSERT_EQ(readMultiPropertyGroup->GetCount(), multiPropertyGroup->GetCount());
			ASSERT_EQ(readMultiPropertyGroup->GetLayerCount(), multiPropertyGroup->GetLayerCount());
			multiPropertyCount++;
		}
		ASSERT_EQ(multiPropertyCount, 1);
	}

}
