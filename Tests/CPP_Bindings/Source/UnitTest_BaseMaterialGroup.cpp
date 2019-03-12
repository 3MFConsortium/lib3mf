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

UnitTest_BaseMaterialGroup.cpp: Defines Unittests for the BaseMaterialGroup class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class BaseMaterialGroup : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = CLib3MFWrapper::CreateModel();
			baseMaterialGroup = model->AddBaseMaterialGroup();
		}
		virtual void TearDown() {
			model.reset();
		}

		PLib3MFModel model;
		PLib3MFBaseMaterialGroup baseMaterialGroup;
	};

	TEST_F(BaseMaterialGroup, GetCount)
	{
		ASSERT_EQ(baseMaterialGroup->GetCount(), 0);
	}

	TEST_F(BaseMaterialGroup, AddGetSetMaterial)
	{
		sLib3MFColor tIn;
		tIn.m_Red = 20;
		tIn.m_Blue = 10;
		tIn.m_Green = 100;
		tIn.m_Alpha = 255;
		std::string inName = "MyMaterial";
		Lib3MF_uint32 propertyID = baseMaterialGroup->AddMaterial(inName, tIn);

		baseMaterialGroup->AddMaterial("OtherMaterial", CLib3MFWrapper::RGBAToColor(0, 10, 20, 30));
		ASSERT_EQ(baseMaterialGroup->GetCount(), 2);

		std::vector<Lib3MF_uint32> propertyIDs;
		baseMaterialGroup->GetAllPropertyIDs(propertyIDs);
		ASSERT_EQ(propertyIDs.size(), 2);
		
		sLib3MFColor tReOut = baseMaterialGroup->GetDisplayColor(propertyIDs[0]);
		ASSERT_EQ(tIn.m_Red, tReOut.m_Red);
		ASSERT_EQ(tIn.m_Green, tReOut.m_Green);
		ASSERT_EQ(tIn.m_Blue, tReOut.m_Blue);
		ASSERT_EQ(tIn.m_Alpha, tReOut.m_Alpha);

		baseMaterialGroup->SetDisplayColor(propertyIDs[0], CLib3MFWrapper::RGBAToColor(12, 123, 23, 234));

		std::string outName = baseMaterialGroup->GetName(propertyIDs[0]);
		ASSERT_TRUE(inName == outName);

		baseMaterialGroup->SetName(propertyIDs[1], "NewName");
		outName = baseMaterialGroup->GetName(propertyIDs[1]);
		ASSERT_TRUE(outName == "NewName");
	}

	TEST_F(BaseMaterialGroup, AddRemoveMaterial)
	{
		baseMaterialGroup->AddMaterial("1", CLib3MFWrapper::RGBAToColor(0, 10, 20, 30));
		baseMaterialGroup->AddMaterial("2", CLib3MFWrapper::RGBAToColor(0, 10, 20, 30));

		std::vector<Lib3MF_uint32> propertyIDs;
		baseMaterialGroup->GetAllPropertyIDs(propertyIDs);

		baseMaterialGroup->RemoveMaterial(propertyIDs[0]);
		ASSERT_SPECIFIC_THROW(baseMaterialGroup->GetName(propertyIDs[0]), ELib3MFException);
		ASSERT_EQ("2", baseMaterialGroup->GetName(propertyIDs[1]));
	}

}
