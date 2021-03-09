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

UnitTest_Model.cpp: Defines Unittests for the Model classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class Model : public ::testing::Test {
	protected:
		virtual void SetUp() {
			m_pModel = wrapper->CreateModel();
		}
		virtual void TearDown() {
			m_pModel.reset();
		}

		PModel m_pModel;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper Model::wrapper;

	TEST_F(Model, QueryReader)
	{
		auto reader3MF = m_pModel->QueryReader("3mf");
		auto readerSTL = m_pModel->QueryReader("stl");
	}

	TEST_F(Model, QueryWriter)
	{
		auto writer3MF = m_pModel->QueryWriter("3mf");
		auto writerSTL = m_pModel->QueryWriter("stl");
	}


	TEST_F(Model, Set_GetUnit)
	{
		m_pModel->SetUnit(eModelUnit::Inch);
		ASSERT_EQ(m_pModel->GetUnit(), eModelUnit::Inch);
		m_pModel->SetUnit(eModelUnit::MilliMeter);
		ASSERT_EQ(m_pModel->GetUnit(), eModelUnit::MilliMeter);
	}

	TEST_F(Model, Set_GetLanguage)
	{
		std::string otherLanguage = "other";
		m_pModel->SetLanguage(otherLanguage);
		ASSERT_FALSE(m_pModel->GetLanguage().compare(otherLanguage));
	}


	TEST_F(Model, GetBuildItems)
	{
		auto iterator = m_pModel->GetBuildItems();
	}

	TEST_F(Model, GetBuildUUID)
	{
		bool bHasUUID = true;
		// by default everything has an UUID
		std::string uuid = m_pModel->GetBuildUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_FALSE(uuid.empty());

		try {
			m_pModel->SetBuildUUID("NOUUID");
			ASSERT_TRUE(false);
		}
		catch (...) {
			ASSERT_TRUE(true);
		}
		std::string inUUID("2b0f37c2-812c-46e7-a6e5-91460c6dbc09");
		m_pModel->SetBuildUUID(inUUID);
		uuid = m_pModel->GetBuildUUID(bHasUUID);
		ASSERT_TRUE(bHasUUID);
		ASSERT_TRUE(uuid.compare(inUUID) == 0);
	}

	TEST_F(Model, AddRemoveBuildItems)
	{
		auto meshObject = m_pModel->AddMeshObject();

		sTransform transform = getIdentityTransform();
		auto buildItem = m_pModel->AddBuildItem(meshObject.get(), transform);

		m_pModel->RemoveBuildItem(buildItem.get());
	}

	TEST_F(Model, GetBaseMaterials)
	{
		auto iterator = m_pModel->GetBaseMaterialGroups();
	}

	TEST_F(Model, GetColorGroups)
	{
		auto iterator = m_pModel->GetColorGroups();
	}

	TEST_F(Model, GetResources)
	{
		auto iterator = m_pModel->GetResources();
	}

	TEST_F(Model, GetTexture2DGroups)
	{
		auto iterator = m_pModel->GetTexture2DGroups();
	}

	TEST_F(Model, GetCompositeMaterials)
	{
		auto iterator = m_pModel->GetCompositeMaterials();
	}

	TEST_F(Model, GetBaseMaterialByID)
	{
		try {
			auto baseMaterial = m_pModel->GetBaseMaterialGroupByID(0);
			ASSERT_FALSE(true);
		}
		catch (ELib3MFException &e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_RESOURCENOTFOUND);
		}
	}

	TEST_F(Model, AddBaseMaterialGroup)
	{
		auto baseMaterial = m_pModel->AddBaseMaterialGroup();
		baseMaterial->GetResourceID();
		auto foundBaseMaterial = m_pModel->GetBaseMaterialGroupByID(baseMaterial->GetResourceID());
	}

	TEST_F(Model, AddSliceStack)
	{
		auto sliceStack = m_pModel->AddSliceStack(0.);
	}

	TEST_F(Model, GetSliceStack)
	{
		auto sliceStack = m_pModel->AddSliceStack(0.);
		Lib3MF_uint32 oldID = sliceStack->GetResourceID();
		auto newSliceStack = m_pModel->GetSliceStackByID(oldID);
		Lib3MF_uint32 newId = newSliceStack->GetResourceID();
		ASSERT_EQ(oldID, newId);
	}

}
