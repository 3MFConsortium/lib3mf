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

UnitTest_Writer.cpp: Defines Unittests for the Writer classes

--*/

#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF
{
	class ProgressCallbackTest : public ::testing::Test {
	protected:
		virtual void SetUp() {
			model = wrapper->CreateModel();
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(InFolder + "Pyramid.3mf");
			writer3MF = model->QueryWriter("3mf");
		}
		virtual void TearDown() {
			model.reset();
			writer3MF.reset();
		}
	
		static PModel model;
		PWriter writer3MF;

		static std::string InFolder;
		static std::string OutFolder;
	public:
		static void* m_spUserData;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper ProgressCallbackTest::wrapper;

	PModel ProgressCallbackTest::model;
	std::string ProgressCallbackTest::InFolder(sTestFilesPath + "/Writer/");
	std::string ProgressCallbackTest::OutFolder(sOutFilesPath + "/Writer/");

	void* ProgressCallbackTest::m_spUserData = reinterpret_cast<void*>(&ProgressCallbackTest::model);

	void Callback_Positive(bool* pAbort, Lib3MF_double dProgress, eProgressIdentifier identifier, Lib3MF_pvoid pUserData)
	{
		std::cout << "Progress " << dProgress*100 << "/100";
		if (identifier != eProgressIdentifier::QUERYCANCELED) {
			std::string progressMessage;
			ProgressCallbackTest::wrapper->RetrieveProgressMessage(identifier, progressMessage);
			std::cout << ": info = \"" << progressMessage << "\"";
		}
		std::cout << "\n";
		EXPECT_TRUE(ProgressCallbackTest::m_spUserData == pUserData) << "Userdata does not match.";
		*pAbort = false;
	}

	void Callback_Negative(bool* pAbort, Lib3MF_double dProgress, eProgressIdentifier identifier, Lib3MF_pvoid pUserData)
	{
		Callback_Positive(pAbort, dProgress, identifier, pUserData);
		*pAbort = dProgress > 0.5;
	}

	TEST_F(ProgressCallbackTest, Write)
	{
		ProgressCallbackTest::writer3MF->SetProgressCallback(Callback_Positive, reinterpret_cast<Lib3MF_pvoid>(ProgressCallbackTest::m_spUserData));
		ProgressCallbackTest::writer3MF->WriteToFile(ProgressCallbackTest::OutFolder + "Pyramid.3mf");
	}

	TEST_F(ProgressCallbackTest, Read)
	{
		auto localModel = wrapper->CreateModel();
		auto reader = localModel->QueryReader("3mf");
		reader->SetProgressCallback(Callback_Positive, ProgressCallbackTest::m_spUserData);
		reader->ReadFromFile(InFolder + "Pyramid.3mf");
	}

	TEST_F(ProgressCallbackTest, WriteNegative)
	{
		try {
			ProgressCallbackTest::writer3MF->SetProgressCallback(Callback_Negative, ProgressCallbackTest::m_spUserData);
			ProgressCallbackTest::writer3MF->WriteToFile(ProgressCallbackTest::OutFolder + "Pyramid.3mf");
			EXPECT_TRUE(false);
		}
		catch(ELib3MFException& e) {
			EXPECT_EQ(e.getErrorCode(), LIB3MF_ERROR_CALCULATIONABORTED) << e.what() << "\n";
			return;
		}
	}


}
