/*++

Copyright (C) 2017 Autodesk Inc.
Copyright (C) 2015 Microsoft Corporation (Original Author)
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

UnitTest_Model.cpp: Defines Unittests for the model class

--*/

#include "CppUnitTest.h"
#include "Model\COM\NMR_COMInterface_ModelFactory.h"
#include "Model\COM\NMR_COMVersion.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace NMR
{

	TEST_CLASS(ModelFactoryTest)
	{
	private:

	public:
		TEST_METHOD(ModelFactory_Version)
		{
			HRESULT hResult;
			DWORD nMajorVersion;
			DWORD nMinorVersion;

			CCOMObject<CCOMModelFactory> * pNewModelFactory = new CCOMObject<CCOMModelFactory>;
			hResult = pNewModelFactory->GetSpecVersion(&nMajorVersion, &nMinorVersion);
			pNewModelFactory->Release();

			if (hResult != S_OK)
				Assert::Fail(L"Could not obtain Spec Version");
			if (nMajorVersion != NMR_APIVERSION_MAJOR || nMinorVersion != NMR_APIVERSION_MINOR) {
				std::wstring errString = std::wstring(L"invalid 3MF core spec version: (" + std::to_wstring(NMR_APIVERSION_MAJOR) + L", " + std::to_wstring(NMR_APIVERSION_MINOR));
				Assert::Fail(errString.c_str());
			}

		}

		TEST_METHOD(ModelFactory_InterfaceVersion)
		{
			HRESULT hResult;
			DWORD nInterfaceVersion;
			CCOMObject<CCOMModelFactory> * pNewModelFactory = new CCOMObject<CCOMModelFactory>;
			hResult = pNewModelFactory->GetInterfaceVersion(&nInterfaceVersion);
			pNewModelFactory->Release();

			if (hResult != S_OK)
				Assert::Fail(L"Could not obtain API interface version");
			if (nInterfaceVersion != NMR_APIVERSION_INTERFACE) {
				std::wstring errString = std::wstring(L"invalid 3MF interface version: " + std::to_wstring(NMR_APIVERSION_INTERFACE));
				Assert::Fail(errString.c_str());
			}
		}

		TEST_METHOD(ModelFactory_Extensions)
		{
			DWORD nAPIInterfaceVersion;
			BOOL bIsSupported;

			CCOMObject<CCOMModelFactory> * pNewModelFactory = new CCOMObject<CCOMModelFactory>;

			if (S_OK != pNewModelFactory->QueryExtension(L"http://schemas.microsoft.com/3dmanufacturing/mustfailspec/2000/00", &bIsSupported, &nAPIInterfaceVersion))
				Assert::Fail(L"Failed to check MustFail-Spec");
			if (bIsSupported)
				Assert::Fail(L"MustFail-Spec IS supported");

			if (S_OK != pNewModelFactory->QueryExtension(L"http://schemas.microsoft.com/3dmanufacturing/material/2015/02", &bIsSupported, &nAPIInterfaceVersion))
				Assert::Fail(L"Failed to check Materials-Spec");
			if (!bIsSupported)
				Assert::Fail(L"Materials-Spec not supported");
			if (nAPIInterfaceVersion != NMR_APIVERSION_INTERFACE_MATERIALSPEC) {
				std::wstring errString = std::wstring(L"invalid 3MF API interface version for Materials-Spec: " + std::to_wstring(NMR_APIVERSION_INTERFACE_MATERIALSPEC));
				Assert::Fail(errString.c_str());
			}

			pNewModelFactory->Release();
		}

	};

}
