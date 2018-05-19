/*++

Copyright (C) 2018 Autodesk, Inc.
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

UnitTest_ProgressCallback.cpp: Defines Unittests for the progress callback functionality

--*/

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"

#include "Common/NMR_ErrorConst.h"


namespace NMR {
	int nUserData = 42;

	bool Callback_Positive(int step, NMR::ProgressIdentifier identifier, void* userData) {
		//std::cout << "Step " << step;
		if (identifier != NMR::ProgressIdentifier::PROGRESS_QUERYCANCELED) {
			const char * progressMessage;
			EXPECT_EQ(NMR::lib3mf_retrieveprogressmessage(identifier, &progressMessage), S_OK) << L"Could not get progress message";
			//std::cout << ": info = \"" << progressMessage << "\"";
		}
		//std::cout << "\n";
		EXPECT_TRUE(nUserData == *(reinterpret_cast<int*>(userData))) << L"Userdata does not match.";
		return true;
	}

	bool Callback_Negative(int step, NMR::ProgressIdentifier identifier, void* userData) {
		Callback_Positive(step, identifier, userData);
		if (step > 50)
			return false;
		return true;
	}

	bool LoadFileWithProgress(const char* fileName) {
		using namespace NMR;

		HRESULT hResult;
		std::string sFullName = sTestFilesPath + separator() + "Production" + separator() + fileName;
		{
			CustomLib3MFBase pModel;
			EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
			CustomLib3MFBase p3MFReader;
			EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
			{
				EXPECT_EQ(lib3mf_reader_setprogresscallback(p3MFReader.get(), Callback_Negative, &nUserData), S_OK) << L"Could not set progress callback";
				hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), sFullName.c_str());

				EXPECT_NE(hResult, S_OK) << L"Expected Failure!";
				DWORD nErrorCode;
				LPCSTR sErrorMessage;
				lib3mf_getlasterror(p3MFReader.get(), &nErrorCode, &sErrorMessage);
				EXPECT_EQ(nErrorCode, NMR_USERABORTED) << L"Expected User Abort!";
			}
		}
		
		CustomLib3MFBase pModel;
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		EXPECT_EQ(lib3mf_reader_setprogresscallback(p3MFReader.get(), Callback_Positive, &nUserData), S_OK) << L"Could not set progress callback";
		hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), sFullName.c_str());

		DWORD nWarningCount;
		lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
		EXPECT_EQ(nWarningCount, 0) << L"Warnings in the 3MF file : " << nWarningCount;
		for (DWORD iWarning = 0; iWarning < nWarningCount; iWarning++) {
			DWORD nErrorCode;
			wchar_t sWarning[4096];
			EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
			EXPECT_TRUE(true) << iWarning << L": " << nErrorCode << ", " << sWarning;
		}

		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;

		return true;

		//// Re-Output file
		//CustomLib3MFBase p3MFWriter;
		//std::string sFileName = fileName;
		//EXPECT_EQ(NMR::lib3mf_model_querywriter(pModel.get(), "3mf", &p3MFWriter.get()), S_OK) << "Cannot create model writer";
		//EXPECT_EQ(lib3mf_writer_setprogresscallback(p3MFWriter.get(), Callback_Positive, &nUserData), S_OK) << L"Could not set progress callback";
		//EXPECT_EQ(NMR::lib3mf_writer_writetofileutf8(p3MFWriter.get(), (std::string("TestOutput") + separator() + +"ReOut_Progress" + fileName).c_str()), S_OK) << "Cannot write 3mf";
		//
		//return true;
	}

}
// A new one of these is create for each test
class ProgressCallback_ReadWriteFileTest : public testing::TestWithParam<const char*>
{
public:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(InstantiationName,
	ProgressCallback_ReadWriteFileTest,
	::testing::Values("Replaces_PP_701_04.3mf", "2ProductionBoxes_OneSliceFile.3mf"));


TEST_P(ProgressCallback_ReadWriteFileTest, LoadWrite)
{
	ASSERT_TRUE(NMR::LoadFileWithProgress(GetParam()));
}



