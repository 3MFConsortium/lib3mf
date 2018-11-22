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

UnitTest_Model.cpp: Defines Unittests for the Model classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf.hpp"

namespace Lib3MF
{
	class Model : public ::testing::Test {
	protected:

		static void SetUpTestCase() {
		}

		static void TearDownTestCase() {
		}

		virtual void SetUp() {
			m_model = CLib3MFWrapper::CreateModel();
		}
		virtual void TearDown() {
			m_model.reset();
		}
	
		static PLib3MFModel m_model;
	};

	PLib3MFModel Model::m_model;

	TEST_F(Model, QueryReader)
	{
		auto reader3MF = Model::m_model->QueryReader("3mf");
		auto readerSTL = Model::m_model->QueryReader("stl");
	}

	TEST_F(Model, QueryWriter)
	{
		auto writer3MF = Model::m_model->QueryWriter("3mf");
		auto writerSTL = Model::m_model->QueryWriter("stl");
	}


	TEST_F(Model, Set_GetUnit)
	{
		Model::m_model->SetUnit(eModelUnitInch);
		ASSERT_EQ(Model::m_model->GetUnit(), eModelUnitInch);
		Model::m_model->SetUnit(eModelUnitMilliMeter);
		ASSERT_EQ(Model::m_model->GetUnit(), eModelUnitMilliMeter);
	}

	TEST_F(Model, Set_GetLanguage)
	{
		std::string otherLanguage = "other";
		Model::m_model->SetLanguage(otherLanguage);
		ASSERT_FALSE(Model::m_model->GetLanguage().compare(otherLanguage));
	}


	TEST_F(Model, GetBuildItems)
	{
		auto iterator = Model::m_model->GetBuildItems();
	}


}
