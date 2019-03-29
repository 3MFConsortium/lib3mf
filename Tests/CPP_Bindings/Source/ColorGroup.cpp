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

UnitTest_ColorGroup.cpp: Defines Unittests for the ColorGroup class

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class ColorGroup : public ::testing::Test {
	protected:

		virtual void SetUp() {
			model = wrapper->CreateModel();
			colorGroup = model->AddColorGroup();
		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		PColorGroup colorGroup;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ColorGroup::wrapper;

	TEST_F(ColorGroup, GetCount)
	{
		ASSERT_EQ(colorGroup->GetCount(), 0);
	}

	TEST_F(ColorGroup, AddGetSetColor)
	{
		sColor tIn;
		tIn.m_Red = 20;
		tIn.m_Blue = 10;
		tIn.m_Green = 100;
		tIn.m_Alpha = 255;
		Lib3MF_uint32 propertyID = colorGroup->AddColor(tIn);

		colorGroup->AddColor(wrapper->RGBAToColor(0, 10, 20, 30));
		ASSERT_EQ(colorGroup->GetCount(), 2);

		std::vector<Lib3MF_uint32> propertyIDs;
		colorGroup->GetAllPropertyIDs(propertyIDs);
		ASSERT_EQ(propertyIDs.size(), 2);
		
		sColor tReOut = colorGroup->GetColor(propertyIDs[0]);
		ASSERT_EQ(tIn.m_Red, tReOut.m_Red);
		ASSERT_EQ(tIn.m_Green, tReOut.m_Green);
		ASSERT_EQ(tIn.m_Blue, tReOut.m_Blue);
		ASSERT_EQ(tIn.m_Alpha, tReOut.m_Alpha);

		colorGroup->SetColor(propertyIDs[0], wrapper->RGBAToColor(12, 123, 23, 234));
	}

	TEST_F(ColorGroup, AddRemoveColor)
	{
		colorGroup->AddColor(wrapper->RGBAToColor(0, 10, 20, 30));
		colorGroup->AddColor(wrapper->RGBAToColor(5, 15, 25, 35));

		std::vector<Lib3MF_uint32> propertyIDs;
		colorGroup->GetAllPropertyIDs(propertyIDs);

		colorGroup->RemoveColor(propertyIDs[0]);
		ASSERT_SPECIFIC_THROW(colorGroup->GetColor(propertyIDs[0]), ELib3MFException);
		ASSERT_EQ(wrapper->RGBAToColor(5, 15, 25, 35).m_Alpha, colorGroup->GetColor(propertyIDs[1]).m_Alpha);
		ASSERT_EQ(wrapper->RGBAToColor(5, 15, 25, 35).m_Blue, colorGroup->GetColor(propertyIDs[1]).m_Blue);
		ASSERT_EQ(wrapper->RGBAToColor(5, 15, 25, 35).m_Green, colorGroup->GetColor(propertyIDs[1]).m_Green);
		ASSERT_EQ(wrapper->RGBAToColor(5, 15, 25, 35).m_Red, colorGroup->GetColor(propertyIDs[1]).m_Red);
	}

}
