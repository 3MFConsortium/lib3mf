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

UnitTest_CompositeMaterials.cpp: Defines Unittests for Composite Materials

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class CompositeMaterials : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();

			std::vector<sPosition> vctVertices;
			std::vector<sTriangle> vctTriangles;
			fnCreateBox(vctVertices, vctTriangles);

			mesh = model->AddMeshObject();
			mesh->SetGeometry(vctVertices, vctTriangles);
			model->AddBuildItem(mesh.get(), getIdentityTransform());


			baseMaterialGroup1 = model->AddBaseMaterialGroup();
			baseMaterialGroup2 = model->AddBaseMaterialGroup();

			sColor tIn;
			tIn.m_Red = 20;
			tIn.m_Blue = 10;
			tIn.m_Green = 100;
			tIn.m_Alpha = 255;
			baseMaterialGroup1->AddMaterial("Material1", tIn);
			tIn.m_Green = 200;
			baseMaterialGroup1->AddMaterial("Material2", tIn);

			tIn.m_Red = 250;
			tIn.m_Blue = 10;
			tIn.m_Green = 0;
			tIn.m_Alpha = 255;
			baseMaterialGroup2->AddMaterial("MaterialB1", tIn);
			tIn.m_Blue = 200;
			baseMaterialGroup2->AddMaterial("MaterialB2", tIn);
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PMeshObject mesh;
		PBaseMaterialGroup baseMaterialGroup1, baseMaterialGroup2;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper CompositeMaterials::wrapper;

	TEST_F(CompositeMaterials, Create)
	{
		auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());

		auto baseMaterialGroup1_Out = compositeMaterial->GetBaseMaterialGroup();
		ASSERT_EQ(baseMaterialGroup1_Out->GetResourceID(), baseMaterialGroup1->GetResourceID());
	}

	void compareConstituents(std::vector<sCompositeConstituent> constituentsA,  std::vector<sCompositeConstituent> constituentsB)
	{
		ASSERT_EQ(constituentsA.size(), constituentsB.size());
		for (size_t i = 0; i < constituentsA.size(); i++) {
			ASSERT_EQ(constituentsA[i].m_PropertyID, constituentsB[i].m_PropertyID);
			ASSERT_DOUBLE_EQ(constituentsA[i].m_MixingRatio, constituentsB[i].m_MixingRatio);
		}
	}

	TEST_F(CompositeMaterials, AddGet)
	{
		auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());
		ASSERT_EQ(compositeMaterial->GetCount(), 0);

		std::vector<sCompositeConstituent> constituents(1);
		constituents[0].m_MixingRatio = 0.5;
		constituents[0].m_PropertyID = 100;

		ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
		constituents[0].m_PropertyID = 2;
		constituents[0].m_MixingRatio = -0.5;
		ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
		constituents[0].m_MixingRatio = 1.5;
		ASSERT_SPECIFIC_THROW(compositeMaterial->AddComposite(constituents), ELib3MFException);
		constituents[0].m_MixingRatio = 0.5;

		Lib3MF_uint32 propertyID1 = compositeMaterial->AddComposite(constituents);

		constituents.push_back(sCompositeConstituent({ 1, 0.2 }));
		Lib3MF_uint32 propertyID2 = compositeMaterial->AddComposite(constituents);

		ASSERT_EQ(compositeMaterial->GetCount(), 2);

		std::vector<sCompositeConstituent> outConstituents;
		compositeMaterial->GetComposite(propertyID2, outConstituents);

		std::vector<Lib3MF_uint32> outPropertyIDs;
		compositeMaterial->GetAllPropertyIDs(outPropertyIDs);
		ASSERT_EQ(2, outPropertyIDs.size());
		ASSERT_EQ(propertyID1, outPropertyIDs[0]);
		ASSERT_EQ(propertyID2, outPropertyIDs[1]);

		compositeMaterial->RemoveComposite(propertyID1);
		ASSERT_EQ(compositeMaterial->GetCount(), 1);
	}

	TEST_F(CompositeMaterials, WriteRead)
	{
		auto compositeMaterial = model->AddCompositeMaterials(baseMaterialGroup1.get());
		ASSERT_EQ(compositeMaterial->GetCount(), 0);

		std::vector<sCompositeConstituent> constituents1(0);
		constituents1.push_back(sCompositeConstituent({ 1, 0.5 }));
		Lib3MF_uint32 propertyID1 = compositeMaterial->AddComposite(constituents1);
		std::vector<sCompositeConstituent> constituents2(0);
		constituents2.push_back(sCompositeConstituent({ 1, 0.8 }));
		constituents2.push_back(sCompositeConstituent({ 2, 0.2 }));
		Lib3MF_uint32 propertyID2 = compositeMaterial->AddComposite(constituents2);

		std::vector<sTriangleProperties> properties(mesh->GetTriangleCount());
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			properties[i].m_ResourceID = compositeMaterial->GetResourceID();
			for (int k = 0; k < 3; k++) {
				properties[i].m_PropertyIDs[k] = ((i + k) % 2) ? propertyID1 : propertyID2;
			}
		}
		mesh->SetAllTriangleProperties(properties);

		auto writer = model->QueryWriter("3mf");
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		// writer->WriteToFile("Composites_Out.3mf");

		auto readModel = wrapper->CreateModel();
		auto reader = readModel->QueryReader("3mf");
		// reader->ReadFromFile("Composites_Out.3mf");
		reader->ReadFromBuffer(buffer);


		int compositeMaterialsCount = 0;
		auto iterator = readModel->GetCompositeMaterials();
		while (iterator->MoveNext())
		{
			auto readCompositeMaterial = iterator->GetCurrentCompositeMaterials();
			ASSERT_EQ(readCompositeMaterial->GetCount(), compositeMaterial->GetCount());
			compositeMaterialsCount++;
			auto readBaseMaterialGroup = compositeMaterial->GetBaseMaterialGroup();
			readBaseMaterialGroup->GetCount();
			ASSERT_EQ(readBaseMaterialGroup->GetCount(), baseMaterialGroup1->GetCount());
			for (Lib3MF_uint32 i = 1; i < readBaseMaterialGroup->GetCount() + 1; i++) {
				ASSERT_TRUE(readBaseMaterialGroup->GetName(i) == baseMaterialGroup1->GetName(i));
				// ASSERT_TRUE(readBaseMaterialGroup->GetDisplayColor(i) == baseMaterialGroup1->GetDisplayColor(i));
			}
		}
		ASSERT_EQ(compositeMaterialsCount, 1);

		std::vector<sTriangleProperties> obtainedProperties;
		mesh->GetAllTriangleProperties(obtainedProperties);
		for (size_t i = 0; i < mesh->GetTriangleCount(); i++) {
			EXPECT_EQ(obtainedProperties[i].m_ResourceID, properties[i].m_ResourceID);
			for (Lib3MF_uint64 j = 0; j < 3; j++) {
				EXPECT_EQ(obtainedProperties[i].m_PropertyIDs[j], properties[i].m_PropertyIDs[j]);
			}

			sTriangleProperties currentProperty;
			mesh->GetTriangleProperties(Lib3MF_uint32(i), currentProperty);
			
			ASSERT_EQ(model->GetPropertyTypeByID(currentProperty.m_ResourceID), ePropertyType::Composite);
			auto currentCompositeMaterials = model->GetCompositeMaterialsByID(currentProperty.m_ResourceID);

			auto readBaseMaterialGroup = currentCompositeMaterials->GetBaseMaterialGroup();

			for (Lib3MF_uint64 k = 0; k < 3; k++) {
				EXPECT_EQ(currentProperty.m_PropertyIDs[k], properties[i].m_PropertyIDs[k]);

				std::vector<sCompositeConstituent> obtainedConstituents;
				currentCompositeMaterials->GetComposite(currentProperty.m_PropertyIDs[k], obtainedConstituents);

				std::vector<sCompositeConstituent>& testConstituents = ((i + k) % 2) ? constituents1 : constituents2;
				ASSERT_EQ(obtainedConstituents.size(), testConstituents.size());
				for (size_t cIndex = 0; cIndex < obtainedConstituents.size(); cIndex++) {
					EXPECT_EQ(readBaseMaterialGroup->GetName(obtainedConstituents[cIndex].m_PropertyID),
						baseMaterialGroup1->GetName(testConstituents[cIndex].m_PropertyID));
 					EXPECT_DOUBLE_EQ(obtainedConstituents[cIndex].m_MixingRatio,
						testConstituents[cIndex].m_MixingRatio);
				}

			}
		}

	}



}
