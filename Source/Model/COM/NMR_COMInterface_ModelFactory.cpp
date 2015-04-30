/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

NMR_COMFactory.h implements a custom COM creation export to link the COM interface without
bothering about global registration of a COM Server.

--*/

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/COM/NMR_COMInterface_ModelFactory.h" 
#include "Model/COM/NMR_COMInterface_Model.h" 
#include "Model/COM/NMR_COMVersion.h" 
#include "Common/Platform/NMR_COM_Native.h" 

namespace NMR {

	CCOMModelFactory::CCOMModelFactory()
	{

	}

	LIB3MFMETHODIMP CCOMModelFactory::CreateModel(_Outptr_ ILib3MFModel ** ppModel)
	{
		if (ppModel == nullptr)
			return LIB3MF_POINTER;

		try
		{
			*ppModel = new CCOMObject<CCOMModel>();
			return LIB3MF_OK;
		}
		catch (...) {
			return LIB3MF_FAIL;
		}
	}


	LIB3MFMETHODIMP CCOMModelFactory::GetSpecVersion(_Out_ DWORD * pMajorVersion, _Out_ DWORD * pMinorVersion)
	{
		if (pMajorVersion == nullptr)
			return LIB3MF_POINTER;
		if (pMinorVersion == nullptr)
			return LIB3MF_POINTER;

		*pMajorVersion = NMR_APIVERSION_MAJOR;
		*pMinorVersion = NMR_APIVERSION_MINOR;

		return LIB3MF_OK;
	}


	LIB3MFMETHODIMP CCOMModelFactory::GetInterfaceVersion(_Out_ DWORD * pInterfaceVersion)
	{
		if (pInterfaceVersion == nullptr)
			return LIB3MF_POINTER;

		*pInterfaceVersion = NMR_APIVERSION_INTERFACE;

		return LIB3MF_OK;
	}


}


