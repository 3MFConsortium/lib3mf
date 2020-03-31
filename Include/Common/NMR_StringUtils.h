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

NMR_StringUtils.h defines a few string helper functions that handle strings correctly
and Exception-safe

--*/

#ifndef __NMR_STRINGUTILS
#define __NMR_STRINGUTILS

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include <string>
#include <string.h>
#include <vector>

#define NMR_MAXSTRINGBUFFERSIZE 1073741823 // (2^30-1)

namespace NMR {

	template <typename T>
	T fnStringToType(_In_z_ const nfChar * pszValue);

	
	template<> float fnStringToType(_In_z_ const nfChar * pszValue);
	template<> double fnStringToType(_In_z_ const nfChar * pszValue);
	template<> nfInt32 fnStringToType(_In_z_ const nfChar * pszValue);
	template<> nfUint32 fnStringToType(_In_z_ const nfChar * pszValue);
	template<> unsigned long fnStringToType(_In_z_ const nfChar * pszValue);
	template<> std::string fnStringToType(_In_z_ const nfChar * pszValue);

	nfInt32 fnStringToInt32(_In_z_ const nfChar * pszValue);
	nfUint32 fnStringToUint32(_In_z_ const nfChar * pszValue);
	nfFloat fnStringToFloat(_In_z_ const nfChar * pszValue);
	nfDouble fnStringToDouble(_In_z_ const nfChar * pszValue);
	nfBool fnStringToSRGBColor(_In_z_ const nfChar * pszValue, _Out_ nfColor & cResult);
	nfUint32 fnHexStringToUint32(_In_z_ const nfChar * pszValue);

	std::string fnInt32ToString(_In_ nfInt32 nValue);
	std::string fnUint32ToString(_In_ nfUint32 nValue);
	std::string fnFloatToString(_In_ nfFloat fValue, _In_ nfUint32 precision);
	std::string fnDoubleToString(_In_ nfFloat dValue, _In_ nfUint32 precision);
	std::string fnColorToString(_In_ nfColor cColor);

	void fnStringToCommaSeparatedIntegerTriplet(_In_z_ const nfChar * pszValue, _Out_ nfInt32 & nValue1, _Out_ nfInt32 & nValue2, _Out_ nfInt32 & nValue3);
	
	template<typename T>
	std::string fnVectorToSpaceDelimitedString(_In_ const std::vector<T> v) {
		std::string result = "";
		for (auto i = v.begin(); i != v.end(); i++) {
			if (i != v.begin())
				result += " ";
			result += std::to_string(*i);
		}
		return result;
	};
	template<> std::string fnVectorToSpaceDelimitedString(_In_ const std::vector<std::string> v);

	void fnStringToBufferSafe(_In_ const std::string sString, _Out_opt_ nfChar * pszBuffer, nfUint32 cbBufferSize, _Out_opt_ nfUint32 * pcbNeededChars);
	void fnWStringToBufferSafe(_In_ const std::wstring sString, _Out_opt_ nfWChar * pwszBuffer, nfUint32 cbBufferSize, _Out_opt_ nfUint32 * pcbNeededChars);

	// UTF8 Functions
	nfBool fnUTF16CharIsSurrogate(_In_ nfWChar cChar);
	nfBool fnUTF16CharIsHighSurrogate(_In_ nfWChar cChar);
	nfBool fnUTF16CharIsLowSurrogate(_In_ nfWChar cChar);
	nfUint32 fnUTF16toCharacterID(_In_ nfUint16 nHighSurrogate, _In_ nfUint16 nLowSurrogate);
	void fnCharacterIDToUTF16(_In_ nfUint32 nCharacterID, _Out_ nfUint16 & nHighSurrogate, _Out_ nfUint16 & nLowSurrogate);

	std::string fnUTF16toUTF8(_In_ const std::wstring sString);
	std::wstring fnUTF8toUTF16(_In_ const std::string sString);

	// Returns number of converted WChars
	// If the input data did contain not enough chars for the last multi-byte characters, pcbNeededCharacters is not equal 0.
	// pszLastChar returns the last converted Multibyte character.
	nfUint32 fnBufferedUTF8toUTF16(_In_ const nfChar * pszInBuffer, _Out_ nfWChar * pszwOutBuffer, _In_ nfUint32 cbBufferSize, _Out_ nfUint32 * pnLastChar, _Out_ nfUint32 * pcbNeededCharacters);

	// Path and File name functions
	nfBool fnStartsWithPathDelimiter(_In_ const std::string sPath);
	std::string fnRemoveLeadingPathDelimiter(_In_ const std::string sPath);
	std::string fnIncludeLeadingPathDelimiter(_In_ const std::string sPath);
	std::string fnExtractFileName(_In_ const std::string sFullPath);
	std::string fnExtractFileDir(_In_ const std::string sFullPath);

	template<typename T>
	std::vector<T> fnVctType_fromString(_In_ const std::string sInString)
	{
		std::string sString = sInString;
		std::vector<T> vctValues;

		nfChar * pszString = &(sString[0]);
		nfChar * pCurrent = pszString;

		nfBool bFinished = false;
		while (!bFinished) {
			// Find next space
			const nfChar * pBegin = pCurrent;
			while ((*pCurrent != ' ') && (*pCurrent))
				pCurrent++;

			if (*pCurrent == ' ') {
				*pCurrent = 0;
			}
			else {
				bFinished = true;
			}

			// If we have not found a space, convert value to T
			if (pBegin != pCurrent) {
				vctValues.push_back(fnStringToType<T>(pBegin));
			}

			// If we are finished, break, otherwise skip space!
			if (!bFinished) {
				pCurrent++;
			}
		}
		return vctValues;
	}

	void decomposeKeyIntoNamespaceAndName(const std::string &sKey, std::string &sNameSpace, std::string &sName);
	std::string composeNamespaceAndNameIntoKey(const std::string &sNameSpace, const std::string &sName);
}

#endif // __NMR_STRINGUTILS
