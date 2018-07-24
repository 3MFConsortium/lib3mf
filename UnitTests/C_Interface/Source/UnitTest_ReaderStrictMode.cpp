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

UnitTest_UnitTest_ReaderStrictMode.cpp: Defines Unittests for the StrictMode of the Reader.

--*/

#include "UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"


#include <iostream>
#include "gtest/gtest.h"


TEST(Reader, StrictMode)
{
	std::string fileName = "MustWarnOrFail.3mf";	// this file contains one issue that triggers a warning
	using namespace NMR;

	auto CheckFile = [](BOOL useStrictMode, const std::string& fileName, CustomLib3MFBase& pModel) {
		EXPECT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		CustomLib3MFBase p3MFReader;
		EXPECT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
		BOOL bStrictModeActive;
		EXPECT_EQ(lib3mf_reader_getstrictmodeactive(p3MFReader.get(), &bStrictModeActive), S_OK) << L"Could query strict mode of Model Reader";
		EXPECT_FALSE(bStrictModeActive) << L"Invalid default value for strict mode";

		EXPECT_EQ(lib3mf_reader_setstrictmodeactive(p3MFReader.get(), useStrictMode), S_OK) << "Could set strict mode of Model Reader";
		EXPECT_EQ(lib3mf_reader_getstrictmodeactive(p3MFReader.get(), &bStrictModeActive), S_OK) << "Could query strict mode of Model Reader";
		EXPECT_EQ(bStrictModeActive, useStrictMode);

		HRESULT hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(), fileName.c_str());
		if (useStrictMode) {
			EXPECT_NE(hResult, S_OK) << L"Could read 3MF file allthough it should have failed";
			DWORD nErrorCode;
			LPCSTR sErrorMessage;
			hResult = lib3mf_getlasterror(p3MFReader.get(), &nErrorCode, &sErrorMessage);
			EXPECT_EQ(hResult, S_OK) << L"Could not get last error";
			EXPECT_EQ(nErrorCode, 32934) << L"Wrong error occurred.";
		}
		else {
			DWORD nWarningCount;
			lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount);
			EXPECT_EQ(nWarningCount, 1) << L"Wrong number of warnings in file with 1 issue";
			for (DWORD iWarning = 0; iWarning<nWarningCount; iWarning++) {
				DWORD nErrorCode;
				wchar_t sWarning[4096];
				EXPECT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, sWarning, 4096, NULL), S_OK) << L"Could not get warning " << iWarning;
				EXPECT_EQ(nErrorCode, 32934) << L"Wrong error occurred.";
				if (nErrorCode != 32934)
					EXPECT_TRUE(false) << iWarning << L": " << nErrorCode << ", " << sWarning;
			}

			DWORD nErrorCode;
			LPCSTR sErrorMessage;
			lib3mf_getlasterror(p3MFReader.get(), &nErrorCode, &sErrorMessage);
			EXPECT_EQ(hResult, S_OK) << L"Could not read 3MF file. " << hResult << ", " << nErrorCode << ", " << sErrorMessage;
		}
	};

	{
		CustomLib3MFBase pModel;
		CheckFile(true, sTestFilesPath + separator() + "Mixed" + separator() + fileName, pModel);
	}
	
	{
		CustomLib3MFBase pModel;
		CheckFile(false, sTestFilesPath + separator() + "Mixed" + separator()+  fileName, pModel);
	}
	

}
