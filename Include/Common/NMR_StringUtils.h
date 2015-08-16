/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_StringUtils.h defines a few string helper functions that handle strings correctly
and Exception-safe

--*/

#ifndef __NMR_STRINGUTILS
#define __NMR_STRINGUTILS

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include <string>

#define NMR_MAXSTRINGBUFFERSIZE 1073741823 // (2^30-1)

namespace NMR {

	nfInt32 fnWStringToInt32(_In_z_ const nfWChar * pwszValue);
	nfUint32 fnWStringToUint32(_In_z_ const nfWChar * pwszValue);
	nfFloat fnWStringToFloat(_In_z_ const nfWChar * pwszValue);
	nfDouble fnWStringToDouble(_In_z_ const nfWChar * pwszValue);
	nfBool	fnWStringToSRGBColor(_In_z_ const nfWChar * pwszValue, _Out_ nfColor & cResult);
	nfUint32 fnWHexStringToUint32(_In_z_ const nfWChar * pwszValue);

	std::wstring fnInt32ToWString(_In_ nfInt32 nValue);
	std::wstring fnUint32ToWString(_In_ nfUint32 nValue);
	std::wstring fnFloatToWString(_In_ nfFloat fValue, _In_ nfUint32 precision);
	std::wstring fnDoubleToWString(_In_ nfFloat dValue, _In_ nfUint32 precision);
	std::wstring fnColorToWString(_In_ nfColor cColor);

	void fnStringToCommaSeparatedIntegerTriplet(_In_z_ const nfWChar * pwszValue, _Out_ nfInt32 & nValue1, _Out_ nfInt32 & nValue2, _Out_ nfInt32 & nValue3);

	void fnWStringToBufferSafe(_In_ const std::wstring sString, _Out_opt_ nfWChar * pwszBuffer, nfUint32 cbBufferSize, _Out_opt_ nfUint32 * pcbNeededChars);

}

#endif // __NMR_STRINGUTILS
