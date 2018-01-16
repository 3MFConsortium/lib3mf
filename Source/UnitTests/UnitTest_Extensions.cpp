/*++

Copyright (C) 2017 Autodesk, Inc.
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

UnitTest_Extensions.cpp: Defines Unittests for general extension handling

--*/

#include "UnitTests/UnitTest_Utilities.h"
#include "Model/COM/NMR_DLLInterfaces.h"
#include <memory>
#include <vector>
#include <string>
#include <cmath>

#include <iostream>
#include "gtest/gtest.h"

namespace NMR {
	
	TEST(Extensions, VersionNumber)
	{
		DWORD nMajorVersion;
		DWORD nMinorVersion;
		EXPECT_EQ(lib3mf_getspecversion(&nMajorVersion, &nMinorVersion), S_OK);
		EXPECT_EQ(nMajorVersion, (DWORD)(1));
	}

	TEST(Extensions, InterfaceVersion)
	{
		DWORD nInterfaceVersionMajor, nInterfaceVersionMinor, nInterfaceVersionMicro;
		EXPECT_EQ(NMR::lib3mf_getinterfaceversion(&nInterfaceVersionMajor, &nInterfaceVersionMinor, &nInterfaceVersionMicro), S_OK) << L"Failed to obtain interface version.";
		EXPECT_EQ(nInterfaceVersionMajor, (DWORD)(NMR_APIVERSION_INTERFACE_MAJOR)) << std::wstring(L"invalid 3MF major interface version: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MAJOR)).c_str();
		EXPECT_EQ(nInterfaceVersionMinor, (DWORD)(NMR_APIVERSION_INTERFACE_MINOR)) << std::wstring(L"invalid 3MF minor interface version: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MINOR)).c_str();
		EXPECT_EQ(nInterfaceVersionMicro, (DWORD)(NMR_APIVERSION_INTERFACE_MICRO)) << std::wstring(L"invalid 3MF patch interface version: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MICRO)).c_str();
		
		CustomLib3MFBase pModel;
		// Create Model Instance
		EXPECT_EQ( NMR::lib3mf_createmodel(&pModel.get()), S_OK) << L"Failed to create model.";
	}
	
	TEST(Extensions, Extensions)
	{
		DWORD nAPIInterfaceVersion;
		BOOL bIsSupported;
		std::wstring errString;
		
		// We do not support the mustfail spec
		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/thisspecmustfail/2000/00", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check MustFail-Spec";
		EXPECT_FALSE(bIsSupported) << L"MustFail-Spec IS supported";
		
		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/material/2015/02", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check Materials-Spec";
		EXPECT_TRUE(bIsSupported) << L"Materials-Spec not supported";
		errString = std::wstring(L"invalid 3MF API interface version for Materials-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MATERIALSPEC));
		EXPECT_EQ(nAPIInterfaceVersion, (DWORD)(NMR_APIVERSION_INTERFACE_MATERIALSPEC)) << errString;

		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/production/2015/06", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check Production-Spec";
		EXPECT_TRUE(bIsSupported) << L"Production-Spec not supported";
		errString = std::wstring(L"invalid 3MF API interface version for Production-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_PRODUCTIONSEPC));
		EXPECT_EQ(nAPIInterfaceVersion, (DWORD)(NMR_APIVERSION_INTERFACE_PRODUCTIONSEPC)) << errString;

		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/slice/2015/07", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check Slice-Spec";
		EXPECT_TRUE(bIsSupported) << L"Slice-Spec not supported";
		errString = std::wstring(L"invalid 3MF API interface version for Production-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_SLICESPEC));
		EXPECT_EQ(nAPIInterfaceVersion, (DWORD)(NMR_APIVERSION_INTERFACE_SLICESPEC)) << errString;
		
		EXPECT_EQ(NMR::lib3mf_queryextension(L"http://schemas.microsoft.com/3dmanufacturing/beamlattice/2017/02", &bIsSupported, &nAPIInterfaceVersion),
			S_OK) << L"Failed to check Beam-Spec";
		EXPECT_TRUE(bIsSupported) << L"Beam-Spec not supported";
		errString = std::wstring(L"invalid 3MF API interface version for BeamLattice-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_BEAMLATTICESPEC));
		EXPECT_EQ(nAPIInterfaceVersion,(DWORD)(NMR_APIVERSION_INTERFACE_BEAMLATTICESPEC)) << errString;
	}

	TEST(Extensions, RequiredExtensions_Pass)
	{
		CustomLib3MFBase pModel;
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		{
			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
			ASSERT_EQ(lib3mf_reader_readfromfileutf8(p3MFReader.get(),
				(sTestFilesPath + separator() + "RequiredExtensions" + separator() + "Pass.3mf").c_str()), S_OK) << L"Error reading file with required extensions.";
		}
	}
	TEST(Extensions, RequiredExtensions_Unsupported)
	{
		CustomLib3MFBase pModel;
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		{
			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
			ASSERT_EQ(lib3mf_reader_readfromfileutf8(p3MFReader.get(),
				(sTestFilesPath + separator() + "RequiredExtensions" + separator() + "Unsupported.3mf").c_str()), S_OK) << L"Error reading file with unsupported required extensions.";

			DWORD nWarningCount, nErrorCode, nNeededChars;
			wchar_t warningString[1024];
			ASSERT_EQ(lib3mf_reader_getwarningcount(p3MFReader.get(), &nWarningCount), S_OK) << L"Could not get warning count";
			ASSERT_EQ(nWarningCount, (DWORD)(1)) << L"Expected 1 warning; Received: " << nWarningCount;
			for (DWORD iWarning = 0; iWarning < nWarningCount; iWarning++) {
				ASSERT_EQ(lib3mf_reader_getwarning(p3MFReader.get(), iWarning, &nErrorCode, warningString, 1024, &nNeededChars), S_OK) << L"Could not get warning.";
				// Insert custom warning handling here
			}
		}
	}
	TEST(Extensions, RequiredExtensions_NotListed)
	{
		HRESULT hResult;
		CustomLib3MFBase pModel;
		ASSERT_EQ(lib3mf_createmodel(&pModel.get()), S_OK) << L"Could not create 3MF model";
		{
			CustomLib3MFBase p3MFReader;
			ASSERT_EQ(lib3mf_model_queryreader(pModel.get(), "3mf", &p3MFReader.get()), S_OK) << L"Could create Model Reader";
			hResult = lib3mf_reader_readfromfileutf8(p3MFReader.get(),
				(sTestFilesPath + separator() + "RequiredExtensions" + separator() + "NotListed.3mf").c_str());
			ASSERT_NE(hResult, S_OK) << L"False Positive: Requiering an extension that is not listed must fail.";
		}
	}
}

