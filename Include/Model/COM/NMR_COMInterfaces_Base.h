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

NMR_COMInterfaces_Base.h defines the Loading and Saving COM Interfaces, which are
shown to the outside world.

--*/

#ifndef __NMR_COMINTERFACES_BASE
#define __NMR_COMINTERFACES_BASE

#ifndef NMR_COM_NATIVE
#include "Common/Platform/NMR_COM_Emulation.h"
#else
#include "Common/Platform/NMR_COM_Native.h"
#endif

#include "Common/Platform/NMR_SAL.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Model/Classes/NMR_ModelTypes.h"


#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_IStream.h"
#endif




// Define Class IDs
#define CLSID_Lib3MF_Base                   "8F1D86BF-CD19-4710-BC19-3C70D095642F"

#ifdef NMR_COM_NATIVE
static const IID IID_Lib3MF_Base = { 0x8f1d86bf, 0xcd19, 0x4710, { 0xbc, 0x19, 0x3c, 0x70, 0xd0, 0x95, 0x64, 0x2f } };
#endif //NMR_COM_NATIVE

namespace NMR {

	/**********************************************************************************************************
	*  ILib3MFBase is a base interface, which serves as parent for all interfaces
	*  related to the 3MF Library
	***********************************************************************************************************/

	LIB3MFINTERFACE(ILib3MFBase, ILib3MFUnknown, CLSID_Lib3MF_Base) {
		LIB3MFPUBLIC(ILib3MFBase)

		/**
		* Returns detailed information of the last known error an object method.
		* The error information is available for every method returning a LIB3MF_FAILED
		* constant.
		*
		* @param[out] pErrorCode Error Code
		* @param[out] pErrorMessage Returns pointer to the error message string, NULL if no error.
		* @return error code or 0 (success)
		*/
		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR *	pErrorMessage) LIB3MFABSTRACT;
	};


}

#endif // __NMR_COMINTERFACES_BASE
