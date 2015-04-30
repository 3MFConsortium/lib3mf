/*++

Copyright (C) 2015 Microsoft Corporation
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

NMR_StringUtils.cpp implements a few string helper functions that handle strings
correctly and Exception-safe

--*/

#include "Common/NMR_StringUtils.h" 
#include "Common/NMR_Exception.h" 
#include <climits>
#include <sstream>
#include <math.h>

namespace NMR {

	nfInt32 fnWStringToInt32(_In_z_ const nfWChar * pwszValue)
	{
		__NMRASSERT(pwszValue);
		nfInt32 nResult = 0;

		// Convert to integer and make a input and range check!
		nfWChar * pEndPtr;

		nResult = wcstol(pwszValue, &pEndPtr, 10);

		// Check if any conversion happened
		if ((pEndPtr == pwszValue) || (!pEndPtr))
			throw CNMRException(NMR_ERROR_EMPTYSTRINGTOINTCONVERSION);

		if ((*pEndPtr != L'\0') && (*pEndPtr != L' '))
			throw CNMRException(NMR_ERROR_INVALIDSTRINGTOINTCONVERSION);

		if ((nResult == LONG_MAX) || (nResult == LONG_MIN))
			throw CNMRException(NMR_ERROR_STRINGTOINTCONVERSIONOUTOFRANGE);

		return nResult;
	}

	nfUint32 fnWStringToUint32(_In_z_ const nfWChar * pwszValue)
	{
		__NMRASSERT(pwszValue);
		nfUint32 nResult = 0;

		// Convert to integer and make a input and range check!
		nfWChar * pEndPtr;

		nResult = wcstoul(pwszValue, &pEndPtr, 10);

		// Check if any conversion happened
		if ((pEndPtr == pwszValue) || (!pEndPtr))
			throw CNMRException(NMR_ERROR_EMPTYSTRINGTOINTCONVERSION);

		if ((*pEndPtr != L'\0') && (*pEndPtr != L' '))
			throw CNMRException(NMR_ERROR_INVALIDSTRINGTOINTCONVERSION);

		if (nResult == ULONG_MAX)
			throw CNMRException(NMR_ERROR_STRINGTOINTCONVERSIONOUTOFRANGE);

		return nResult;
	}

	nfFloat fnWStringToFloat(_In_z_ const nfWChar * pwszValue)
	{
		return (nfFloat)fnWStringToDouble(pwszValue);
	}

	nfDouble fnWStringToDouble(_In_z_ const nfWChar * pwszValue)
	{
		__NMRASSERT(pwszValue);
		nfDouble dResult = 0.0;

		// Convert to double and make a input and range check!
		nfWChar * pEndPtr;

		dResult = wcstod(pwszValue, &pEndPtr);

		// Check if any conversion happened
		if ((pEndPtr == pwszValue) || (!pEndPtr))
			throw CNMRException(NMR_ERROR_EMPTYSTRINGTODOUBLECONVERSION);

		if ((*pEndPtr != L'\0') && (*pEndPtr != L' '))
			throw CNMRException(NMR_ERROR_INVALIDSTRINGTODOUBLECONVERSION);

		if ((dResult == HUGE_VAL) || (dResult == -HUGE_VAL))
			throw CNMRException(NMR_ERROR_STRINGTODOUBLECONVERSIONOUTOFRANGE);

		return dResult;
	}

	std::wstring fnInt32ToWString(_In_ nfInt32 nValue)
	{
		std::wstringstream sStream;
		sStream << nValue;
		return sStream.str();
	}

	std::wstring fnUint32ToWString(_In_ nfUint32 nValue)
	{
		std::wstringstream sStream;
		sStream << nValue;
		return sStream.str();
	}

	std::wstring fnFloatToWString(_In_ nfFloat fValue, _In_ nfUint32 precision)
	{
		std::wstringstream sStream;
		sStream << fValue;
		return sStream.str();
	}

	std::wstring fnDoubleToWString(_In_ nfFloat dValue, _In_ nfUint32 precision)
	{
		std::wstringstream sStream;
		sStream << dValue;
		return sStream.str();
	}

	void fnWStringToBufferSafe(_In_ const std::wstring sString, _Out_opt_ nfWChar * pwszBuffer, nfUint32 cbBufferSize, _Out_opt_ nfUint32 * pcbNeededChars)
	{
		__NMRASSERT(pwszBuffer);

		// Check for possible integer overflows
		size_t cbLength = sString.length();
		if (cbLength > NMR_MAXSTRINGBUFFERSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// return used buffer size
		nfUint32 cbNeededChars = ((nfUint32)cbLength);
		if (pcbNeededChars)
			*pcbNeededChars = cbNeededChars;

		// copy string
		if (pwszBuffer) {
			if (cbNeededChars >= cbBufferSize)
				throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

#ifndef __GCC
			wcscpy_s(pwszBuffer, cbBufferSize, sString.c_str());
#else
			wcscpy(pwszBuffer, sString.c_str());
#endif
		}
	}

}
