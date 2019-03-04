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

UnitTest_MultiProperties.cpp: Defines Unittests for MultiProperties

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class MultiProperties : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();

			std::vector<sLib3MFPosition> vctVertices;
			std::vector<sLib3MFTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());

			baseMaterialGroup = model->AddBaseMaterialGroup();
			sLib3MFColor tIn;
			tIn.m_Red = 20;
			tIn.m_Blue = 10;
			tIn.m_Green = 100;
			tIn.m_Alpha = 255;
			baseMaterialGroup->AddMaterial("Material1", tIn);
			tIn.m_Green = 200;
			baseMaterialGroup->AddMaterial("Material2", tIn);

			compositeMaterialGroup = model->AddCompositeMaterials(baseMaterialGroup.get());

			colorGroup = model->AddColorGroup();
			colorGroup->AddColor(tIn);
			tIn.m_Alpha = 100;
			colorGroup->AddColor(tIn);

			std::string sRelationshipType_Texture = "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture";
			std::string sPath_Texture = "/3D/Textures/MyTexture.png";

			auto textureAttachment = model->AddAttachment(sPath_Texture, sRelationshipType_Texture);
			textureAttachment->ReadFromFile(sTestFilesPath + "Textures/tex1.png");
			texture2D = model->AddTexture2DFromAttachment(textureAttachment.get());

			std::vector<sLib3MFTex2Coord> coords(0);
			auto texture2DGroup = model->AddTexture2DGroup(texture2D.get());
			std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
			for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
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

		PLib3MFModel model;
		PLib3MFMeshObject mesh;
		PLib3MFBaseMaterialGroup baseMaterialGroup;
		PLib3MFCompositeMaterials compositeMaterialGroup;
		PLib3MFColorGroup colorGroup;
		PLib3MFTexture2D texture2D;
		PLib3MFTexture2DGroup texture2DGroup;
	};

	TEST_F(MultiProperties, Create)
	{
		auto multiMaterialGroup = model->AddMultiPropertyGroup();
		ASSERT_EQ(model->GetPropertyTypeByID(multiMaterialGroup->GetResourceID()), eLib3MFPropertyType::ePropertyTypeMulti);
		auto multiMaterialGroup_ReOut = model->GetMultiPropertyGroupByID(multiMaterialGroup->GetResourceID());
	}

	TEST_F(MultiProperties, AddGet)
	{
		auto multiMaterialGroup = model->AddMultiPropertyGroup();

		ASSERT_EQ(multiMaterialGroup->GetCount(), 0);
		ASSERT_EQ(multiMaterialGroup->GetLayerCount(), 0);

		multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ baseMaterialGroup->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMix });
		ASSERT_EQ(multiMaterialGroup->GetCount(), 1);

		// cannot add two materials
		ASSERT_SPECIFIC_THROW( multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ compositeMaterialGroup->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMix }
		), ELib3MFException );

		multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ colorGroup->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMultiply });

		// cannot add resource of incorrect type
		ASSERT_SPECIFIC_THROW(multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ texture2D->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMix }
		), ELib3MFException);

		multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ texture2DGroup->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMix });

		// cannot add resource twice
		ASSERT_SPECIFIC_THROW(multiMaterialGroup->AddLayer(
			sLib3MFMultiPropertyLayer{ texture2DGroup->GetResourceID(), eLib3MFBlendMethod::eBlendMethodMix }
		), ELib3MFException);

		ASSERT_EQ(multiMaterialGroup->GetCount(), 3);

	}

	//	std::vector<sLib3MFCompositeConstituent> constituents(1);
	//	constituents[0].m_MixingRatio = 0.5;
	//	constituents[0].m_PropertyID = 100;

	//	ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
	//	constituents[0].m_PropertyID = 2;
	//	constituents[0].m_MixingRatio = -0.5;
	//	ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
	//	constituents[0].m_MixingRatio = 1.5;
	//	ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
	//	constituents[0].m_MixingRatio = 0.5;

	//	Lib3MF_uint32 propertyID1 = compositeMaterial->AddComposite(constituents);

	//	constituents.push_back(sLib3MFCompositeConstituent({ 1, 0.2 }));
	//	Lib3MF_uint32 propertyID2 = compositeMaterial->AddComposite(constituents);

	//	ASSERT_EQ(compositeMaterial->GetCount(), 2);

	//	std::vector<sLib3MFCompositeConstituent> outConstituents;
	//	compositeMaterial->GetComposite(propertyID2, outConstituents);

	//	std::vector<Lib3MF_uint32> outPropertyIDs;
	//	compositeMaterial->GetAllPropertyIDs(outPropertyIDs);
	//	ASSERT_EQ(2, outPropertyIDs.size());
	//	ASSERT_EQ(propertyID1, outPropertyIDs[0]);
	//	ASSERT_EQ(propertyID2, outPropertyIDs[1]);

	//	compositeMaterial->RemoveComposite(propertyID1);
	//	ASSERT_EQ(compositeMaterial->GetCount(), 1);
	//}

	//TEST_F(MultiProperties, WriteRead)
	//{
	//	auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());
	//	ASSERT_EQ(compositeMaterial->GetCount(), 0);

	//	std::vector<sLib3MFCompositeConstituent> constituents1(0);
	//	constituents1.push_back(sLib3MFCompositeConstituent({ 1, 0.5 }));
	//	Lib3MF_uint32 propertyID1 = compositeMaterial->AddComposite(constituents1);
	//	std::vector<sLib3MFCompositeConstituent> constituents2(0);
	//	constituents2.push_back(sLib3MFCompositeConstituent({ 1, 0.8 }));
	//	constituents2.push_back(sLib3MFCompositeConstituent({ 2, 0.2 }));
	//	Lib3MF_uint32 propertyID2 = compositeMaterial->AddComposite(constituents2);

	//	std::vector<sLib3MFTriangleProperties> properties(mesh->GetTriangleCount());
	//	for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
	//		properties[i].m_ResourceID = compositeMaterial->GetResourceID();
	//		for (int k = 0; k < 3; k++) {
	//			properties[i].m_PropertyIDs[k] = ((i + k) % 2) ? propertyID1 : propertyID2;
	//		}
	//	}
	//	mesh->SetAllTriangleProperties(properties);

	//	auto writer = model->QueryWriter("3mf");
	//	std::vector<Lib3MF_uint8> buffer;
	//	writer->WriteToBuffer(buffer);
	//	// writer->WriteToFile("Composites_Out.3mf");

	//	auto readModel = CLib3MFWrapper::CreateModel();
	//	auto reader = readModel->QueryReader("3mf");
	//	// reader->ReadFromFile("Composites_Out.3mf");
	//	reader->ReadFromBuffer(buffer);


	//	int compositeMaterialsCount = 0;
	//	auto iterator = readModel->GetCompositeMaterials();
	//	while (iterator->MoveNext())
	//	{
	//		auto readCompositeMaterial = iterator->GetCurrentCompositeMaterials();
	//		ASSERT_EQ(readCompositeMaterial->GetCount(), compositeMaterial->GetCount());
	//		compositeMaterialsCount++;
	//		auto readBaseMaterialGroup = compositeMaterial->GetBaseMaterialGroup();
	//		readBaseMaterialGroup->GetCount();
	//		ASSERT_EQ(readBaseMaterialGroup->GetCount(), baseMaterialGroup1->GetCount());
	//		for (Lib3MF_uint32 i = 1; i < readBaseMaterialGroup->GetCount() + 1; i++) {
	//			ASSERT_TRUE(readBaseMaterialGroup->GetName(i) == baseMaterialGroup1->GetName(i));
	//			// ASSERT_TRUE(readBaseMaterialGroup->GetDisplayColor(i) == baseMaterialGroup1->GetDisplayColor(i));
	//		}
	//	}
	//	ASSERT_EQ(compositeMaterialsCount, 1);

	//	std::vector<sLib3MFTriangleProperties> obtainedProperties;
	//	mesh->GetAllTriangleProperties(obtainedProperties);
	//	int count = 0;
	//	for (Lib3MF_uint64 i = 0; i < mesh->GetTriangleCount(); i++) {
	//		EXPECT_EQ(obtainedProperties[i].m_ResourceID, properties[i].m_ResourceID);
	//		for (Lib3MF_uint64 j = 0; j < 3; j++) {
	//			EXPECT_EQ(obtainedProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
	//		}

	//		sLib3MFTriangleProperties currentProperty;
	//		mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
	//		
	//		ASSERT_EQ(model->GetPropertyTypeByID(currentProperty.m_ResourceID), eLib3MFPropertyType::ePropertyTypeComposite);
	//		auto currentCompositeMaterials = model->GetCompositeMaterialsByID(currentProperty.m_ResourceID);

	//		auto readBaseMaterialGroup = currentCompositeMaterials->GetBaseMaterialGroup();

	//		for (Lib3MF_uint64 k = 0; k < 3; k++) {
	//			EXPECT_EQ(currentProperty.m_PropertyIDs[k], properties[i].m_PropertyIDs[k]);

	//			std::vector<sLib3MFCompositeConstituent> obtainedConstituents;
	//			currentCompositeMaterials->GetComposite(currentProperty.m_PropertyIDs[k], obtainedConstituents);

	//			std::vector<sLib3MFCompositeConstituent>& testConstituents = ((i + k) % 2) ? constituents1 : constituents2;
	//			ASSERT_EQ(obtainedConstituents.size(), testConstituents.size());
	//			for (Lib3MF_uint64 cIndex = 0; cIndex < obtainedConstituents.size(); cIndex++) {
	//				EXPECT_EQ(readBaseMaterialGroup->GetName(obtainedConstituents[cIndex].m_PropertyID),
	//					baseMaterialGroup1->GetName(testConstituents[cIndex].m_PropertyID));
 //					EXPECT_DOUBLE_EQ(obtainedConstituents[cIndex].m_MixingRatio,
	//					testConstituents[cIndex].m_MixingRatio);
	//			}

	//		}
	//	}

	//}



}
