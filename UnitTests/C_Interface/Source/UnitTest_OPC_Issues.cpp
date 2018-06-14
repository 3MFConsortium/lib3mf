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

UnitTest_OPC_Issues.cpp: Defines Unittests for OPC related requirements of a 
3MF-reader/writer.
--*/

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"

#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"


namespace NMR {


	// A new one of these is create for each test
	class OPC_IssuesTest : public testing::TestWithParam<const char*>
	{
	public:
		virtual void SetUp() {}
		virtual void TearDown() {}
	};

	INSTANTIATE_TEST_CASE_P(Read_OPC_IssuesTest, OPC_IssuesTest,
		::testing::Values(
			"Box_ExtensionCustom.3mf",
			"Box_OverrideCustom.3mf",
			"Box_OverridePositive.3mf"
		));

	TEST_P(OPC_IssuesTest, ReadPositive)
	{
		std::string fileName = GetParam();
		std::string path = sTestFilesPath + separator() + "OPC_Issues" + separator() + fileName;

		CustomLib3MFBase p3MFReader;
		CustomLib3MFBase pModel;
		ASSERT_EQ(lib3mf_createmodel(&(pModel.get())), S_OK) << L"Could not create model";
		ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could not create Model Reader";

		HRESULT hResult;
		DWORD nErrorCode;
		LPCSTR sErrorMessage;
		hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), path.c_str());
		lib3mf_getlasterror(pModel.get(), &nErrorCode, &sErrorMessage);
		ASSERT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << fileName;
	}

}

