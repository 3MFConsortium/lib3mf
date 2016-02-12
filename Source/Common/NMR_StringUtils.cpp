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
#include <string.h>
#include <vector>

namespace NMR {

	// Lookup table to convert UTF8 bytes to sequence length
	const nfByte UTF8DecodeTable[256] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
		4,4,4,4,4,4,4,4,5,5,5,5,6,6,0,0
	};

	// Masks to decode highest UTF8 sequence byte
	const nfByte UTF8DecodeMask[7] = {0, 0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x01};

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

	nfWChar fnColorDigitToHex(_In_ nfByte digit)
	{
		if (digit < 10)
			return (nfWChar)(digit + 48);
		if (digit < 16)
			return (nfWChar)(digit + 55);

		return L' ';
	}

	std::wstring fnColorToWString(_In_ nfColor cColor)
	{
		nfWChar pBuffer[16];
		pBuffer[0] = L'#';
		pBuffer[2] = fnColorDigitToHex(cColor & 0xf); // R
		pBuffer[1] = fnColorDigitToHex((cColor >> 4) & 0xf); // R
		pBuffer[4] = fnColorDigitToHex((cColor >> 8) & 0xf); // G
		pBuffer[3] = fnColorDigitToHex((cColor >> 12) & 0xf); // G
		pBuffer[6] = fnColorDigitToHex((cColor >> 16) & 0xf); // B
		pBuffer[5] = fnColorDigitToHex((cColor >> 20) & 0xf); // B
		pBuffer[8] = fnColorDigitToHex((cColor >> 24) & 0xf); // A
		pBuffer[7] = fnColorDigitToHex((cColor >> 28) & 0xf); // A
		pBuffer[9] = 0;

		return std::wstring(pBuffer);
	}

	std::wstring fnDoubleToWString(_In_ nfFloat dValue, _In_ nfUint32 precision)
	{
		std::wstringstream sStream;
		sStream << dValue;
		return sStream.str();
	}

	nfBool fnWStringToSRGBColor(_In_z_ const nfWChar * pwszValue, _Out_ nfColor & cResult)
	{
		cResult = 0;

		if (!pwszValue)
			return false;

		nfUint32 nRed = 255;
		nfUint32 nGreen = 255;
		nfUint32 nBlue = 255;
		nfUint32 nAlpha = 255;

		std::wstring sString(pwszValue);
		if (sString.length() == 7) {
			if (sString[0] != L'#')
				return false;

			nRed = fnWHexStringToUint32(sString.substr(1, 2).c_str());
			nGreen = fnWHexStringToUint32(sString.substr(3, 2).c_str());
			nBlue = fnWHexStringToUint32(sString.substr(5, 2).c_str());

			cResult = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);

			return true;
		}

		if (sString.length() == 9) {
			if (sString[0] != L'#')
				return false;

			nRed = fnWHexStringToUint32(sString.substr(1, 2).c_str());
			nGreen = fnWHexStringToUint32(sString.substr(3, 2).c_str());
			nBlue = fnWHexStringToUint32(sString.substr(5, 2).c_str());
			nAlpha = fnWHexStringToUint32(sString.substr(7, 2).c_str());

			cResult = nRed | (nGreen << 8) | (nBlue << 16) | (nAlpha << 24);

			return true;
		}

		return false;
	}

	nfUint32 fnWHexStringToUint32(_In_z_ const nfWChar * pwszValue)
	{
		if (!pwszValue)
			return 0;
		nfWChar * p;
		nfUint32 nResult = wcstoul(pwszValue, &p, 16);
		if (*p != 0)
			throw CNMRException(NMR_ERROR_INVALIDHEXVALUE);
		if (nResult == ULONG_MAX)
			throw CNMRException(NMR_ERROR_RANGEERROR);

		return nResult;
	}

	nfInt32 fnWStringToInt32Comma(_In_z_ const nfWChar * pwszValue)
	{
		__NMRASSERT(pwszValue);
		nfInt32 nResult = 0;

		// Convert to integer and make a input and range check!
		nfWChar * pEndPtr;

		nResult = wcstol(pwszValue, &pEndPtr, 10);

		// Check if any conversion happened
		if ((pEndPtr == pwszValue) || (!pEndPtr))
			throw CNMRException(NMR_ERROR_EMPTYSTRINGTOINTCONVERSION);

		if ((*pEndPtr != L'\0') && (*pEndPtr != L' ') && (*pEndPtr != L','))
			throw CNMRException(NMR_ERROR_INVALIDSTRINGTOINTCONVERSION);

		if ((nResult == LONG_MAX) || (nResult == LONG_MIN))
			throw CNMRException(NMR_ERROR_STRINGTOINTCONVERSIONOUTOFRANGE);

		return nResult;
	}


	void fnStringToCommaSeparatedIntegerTriplet(_In_z_ const nfWChar * pwszValue, _Out_ nfInt32 & nValue1, _Out_ nfInt32 & nValue2, _Out_ nfInt32 & nValue3)
	{
		const wchar_t * pwszCommaValue1 = wcschr(pwszValue, L',');
		if (pwszCommaValue1 != nullptr) {
			if (*pwszCommaValue1 == 0)
				throw CNMRException(NMR_ERROR_INVALIDINTEGERTRIPLET);
			pwszCommaValue1++;

			const wchar_t * pwszCommaValue2 = wcschr(pwszCommaValue1, L',');
			if (pwszCommaValue2 != nullptr) {
				if (*pwszCommaValue2 == 0)
					throw CNMRException(NMR_ERROR_INVALIDINTEGERTRIPLET);
				pwszCommaValue2++;

				nValue1 = fnWStringToInt32Comma(pwszValue);
				nValue2 = fnWStringToInt32Comma(pwszCommaValue1);
				nValue3 = fnWStringToInt32Comma(pwszCommaValue2);
			}
			else
				throw CNMRException(NMR_ERROR_INVALIDINTEGERTRIPLET);

		}
		else
			throw CNMRException(NMR_ERROR_INVALIDINTEGERTRIPLET);

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

#ifndef __GNUC__
			wcscpy_s(pwszBuffer, cbBufferSize, sString.c_str());
#else
			wcscpy(pwszBuffer, sString.c_str());
#endif
		}
	}


	void fnStringToBufferSafe(_In_ const std::string sString, _Out_opt_ nfChar * pszBuffer, nfUint32 cbBufferSize, _Out_opt_ nfUint32 * pcbNeededChars)
	{
		__NMRASSERT(pszBuffer);

		// Check for possible integer overflows
		size_t cbLength = sString.length();
		if (cbLength > NMR_MAXSTRINGBUFFERSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// return used buffer size
		nfUint32 cbNeededChars = ((nfUint32)cbLength);
		if (pcbNeededChars)
			*pcbNeededChars = cbNeededChars;

		// copy string
		if (pszBuffer) {
			if (cbNeededChars >= cbBufferSize)
				throw CNMRException(NMR_ERROR_INSUFFICIENTBUFFERSIZE);

#ifndef __GNUC__
			strcpy_s(pszBuffer, cbBufferSize, sString.c_str());
#else
			strcpy(pszBuffer, sString.c_str());
#endif
		}
	}

	// UTF conversion functions
	nfBool fnUTF16CharIsSurrogate(_In_ nfWChar cChar)
	{
		nfUint32 nSignature = (cChar & 0xfc00);
		return (nSignature == 0xd800) || (nSignature == 0xdc00);
	}

	nfBool fnUTF16CharIsHighSurrogate(_In_ nfWChar cChar)
	{
		nfUint32 nSignature = (cChar & 0xfc00);
		return (nSignature == 0xd800);
	}

	nfBool fnUTF16CharIsLowSurrogate(_In_ nfWChar cChar)
	{
		nfUint32 nSignature = (cChar & 0xfc00);
		return (nSignature == 0xdc00);
	}

	nfUint32 fnUTF16toCharacterID(_In_ nfUint16 nHighSurrogate, _In_ nfUint16 nLowSurrogate)
	{
		if ((fnUTF16CharIsHighSurrogate(nLowSurrogate)) && (fnUTF16CharIsLowSurrogate(nHighSurrogate))) {
			std::swap(nLowSurrogate, nHighSurrogate); // UTF16LE
		}

		if ((!fnUTF16CharIsHighSurrogate(nHighSurrogate)) || (!fnUTF16CharIsLowSurrogate(nLowSurrogate)))
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF8);

		nfUint32 nCode = (((nfUint32)(nHighSurrogate & 0x3ff)) << 10 | ((nfUint32)(nLowSurrogate & 0x3ff)));
		return nCode + 0x10000;
	}

	void fnCharacterIDToUTF16(_In_ nfUint32 nCharacterID, _Out_ nfUint16 & nHighSurrogate, _Out_ nfUint16 & nLowSurrogate)
	{
		if ((nCharacterID < 0x10000) || (nCharacterID > 0x10FFFF))
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);
		nCharacterID -= 0x10000;
		nHighSurrogate = (nCharacterID >> 10) | 0xd800;
		nLowSurrogate = (nCharacterID & 0x3ff) | 0xdc00;
	}


	std::string fnUTF16toUTF8(_In_ const std::wstring sString)
	{

		// Check Input Sanity
		size_t nLength = sString.length();
		if (nLength == 0)
			return "";
		if (nLength > NMR_MAXSTRINGBUFFERSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// Reserve UTF8 Buffer
		nfUint32 nBufferSize = (nfUint32)nLength * 4 + 1;
		std::vector<nfChar> Buffer;
		Buffer.resize(nBufferSize);

		// nfInt32 nResult;
		// Alternative: Convert via Win API
		// nResult = WideCharToMultiByte(CP_UTF8, 0, sString.c_str(), (nfUint32)nLength, &Buffer[0], nBufferSize, nullptr, nullptr);
		// if (nResult == 0)
		//	throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF8);

		const nfWChar * pChar = sString.c_str();
		nfChar * pOutput = &Buffer[0];

		while (*pChar) {
			nfWChar cChar = *pChar;
			nfUint32 nCharacter;
			pChar++;

			if (fnUTF16CharIsSurrogate(cChar)) {
				nfWChar cLowChar = *pChar;
				if (cLowChar == 0)
					throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF8);
				pChar++;

				nCharacter = fnUTF16toCharacterID(cChar, cLowChar);
			}
			else {
				nCharacter = cChar;
			}

			if (nCharacter < 0x80) {
				// One Byte Encoding
				*pOutput = nCharacter;
				pOutput++;
			}
			else if (nCharacter < 0x800) {
				// Two Byte Encoding
				*pOutput = (nCharacter >> 6) | 0xC0;
				pOutput++;
				*pOutput = (nCharacter & 0x3f) | 0x80;
				pOutput++;
			}
			else if (nCharacter < 0x10000) {
				// Three Byte Encoding
				*pOutput = (nCharacter >> 12) | 0xE0;
				pOutput++;
				*pOutput = ((nCharacter >> 6) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = (nCharacter & 0x3f) | 0x80;
				pOutput++;
			}
			else if (nCharacter < 0x200000) {
				// Four Byte Encoding
				*pOutput = (nCharacter >> 18) | 0xF0;
				pOutput++;
				*pOutput = ((nCharacter >> 12) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 6) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = (nCharacter & 0x3f) | 0x80;
				pOutput++;
			}
			else if (nCharacter < 0x4000000) {
				// Five Byte Encoding
				*pOutput = (nCharacter >> 24) | 0xF8;
				pOutput++;
				*pOutput = ((nCharacter >> 18) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 12) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 6) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = (nCharacter & 0x3f) | 0x80;
				pOutput++;
			}
			else {
				// Six Byte Encoding
				*pOutput = (nCharacter >> 30) | 0xFC;
				pOutput++;
				*pOutput = ((nCharacter >> 24) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 18) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 12) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = ((nCharacter >> 6) & 0x3f) | 0x80;
				pOutput++;
				*pOutput = (nCharacter & 0x3f) | 0x80;
				pOutput++;
			}

		}

		// write end byte
		*pOutput = 0;

		return std::string(&Buffer[0]);

	}




	std::wstring fnUTF8toUTF16(_In_ const std::string sString)
	{

		// Check Input Sanity
		size_t nLength = sString.length();
		if (nLength == 0)
			return L"";
		if (nLength > NMR_MAXSTRINGBUFFERSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// Reserve UTF8 Buffer
		nfUint32 nBufferSize = (nfUint32)nLength;
		std::vector<nfWChar> Buffer;
		Buffer.resize(nBufferSize * 2 + 2);

		// Alternative: Convert via Win API
		// nfInt32 nResult;
		//nResult = MultiByteToWideChar(CP_UTF8, 0, sString.c_str(), (nfUint32)nLength, &Buffer[0], nBufferSize);
		//if (nResult == 0)
			//throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);

		const nfChar * pChar = sString.c_str();
		nfWChar * pOutput = &Buffer[0];

		while (*pChar) {
			nfByte cChar = (nfByte) *pChar;
			nfUint32 nLength = UTF8DecodeTable[(nfUint32)cChar];
			pChar++;

			if (nLength == 0)
				throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);
			__NMRASSERT(nLength <= 6);

			// Check for BOM (0xEF,0xBB,0xBF), this also checks for #0 characters at the end,
			// so it does not read over the string end!
			nfBool bIsBOM = false;
			if (cChar == 0xef) {
				if (*((const nfByte*) pChar) == 0xbb) {
					if (*((const nfByte*) (pChar + 1)) == 0xbf) {
						bIsBOM = true;
					}
				}
			};


			if (!bIsBOM) {
				nfUint32 nCode = cChar & UTF8DecodeMask[nLength];

				while (nLength > 1) {
					cChar = *pChar;
					if ((cChar & 0xc0) != 0x80)
						throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);
					pChar++;

					// Map UTF8 sequence to code
					nCode = (nCode << 6) | (cChar & 0x3f);
					nLength--;
				}

				// Map Code to UTF16
				if (nCode < 0xd800) {
					*pOutput = nCode;
					pOutput++;
				}
				else {
					nfUint16 nHighSurrogate, nLowSurrogate;
					fnCharacterIDToUTF16(nCode, nHighSurrogate, nLowSurrogate);
					*pOutput = nHighSurrogate;
					pOutput++;
					*pOutput = nLowSurrogate;
					pOutput++;
				}
			}
			else {
				// If we find a UTF8 bom, we just ignore it.
				__NMRASSERT(nLength == 3);
				pChar += 2;
			}
		}

		// write end byte
		*pOutput = 0;

		return std::wstring(&Buffer[0]);

	}


	nfUint32 fnBufferedUTF8toUTF16(_In_ const nfChar * pszInBuffer, _Out_ nfWChar * pszwOutBuffer, _In_ nfUint32 cbBufferSize, _Out_ nfUint32 * pnLastChar, _Out_ nfUint32 * pcbNeededCharacters)
	{
		if (pszInBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pszwOutBuffer == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if ((pnLastChar == nullptr) || (pcbNeededCharacters == nullptr))
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (cbBufferSize > NMR_MAXSTRINGBUFFERSIZE)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		// Set default values
		nfUint32 cbOutCount = 0;
		*pnLastChar = 0;
		*pcbNeededCharacters = 0;

		// Set iterating pointers
		const nfChar * pInChar = pszInBuffer;
		nfWChar * pOutChar = pszwOutBuffer;

		// Iterate through input
		nfInt32 cbCount = (nfInt32) cbBufferSize;
		while (cbCount > 0) {
			nfByte cChar = *pInChar;

			// Check Multibyte Length Character
			nfUint32 nLength = UTF8DecodeTable[(nfUint32)cChar];
			if (nLength == 0)
				throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);
			__NMRASSERT(nLength <= 6);

			// If we do not have enough Bytes left for the multibyte character, return needed count.
			if (((nfInt32) nLength) > cbCount) {
				*pcbNeededCharacters = nLength - cbCount;
				return cbOutCount;
			}

			// Set multibyte character to next char
			*pnLastChar += nLength;

			// Read multibyte character byte by byte.
			pInChar++;
			cbCount--;

			// Check for BOM (0xEF,0xBB,0xBF), this also checks for #0 characters at the end,
			// so it does not read over the string end!
			nfBool bIsBOM = false;
			if (cChar == 0xef) {
				if (*((const nfByte *)pInChar) == 0xbb) {
					if (*((const nfByte *) (pInChar + 1)) == 0xbf) {
						bIsBOM = true;
					}
				}
			};

			if (!bIsBOM) {
				// create utf16 code
				nfUint32 nCode = cChar & UTF8DecodeMask[nLength];

				while (nLength > 1) {
					cChar = *pInChar;
					if ((cChar & 0xc0) != 0x80)
						throw CNMRException(NMR_ERROR_COULDNOTCONVERTTOUTF16);
					pInChar++;
					cbCount--;

					// Map UTF8 sequence to code
					nCode = (nCode << 6) | (cChar & 0x3f);
					nLength--;
				}

				// Map Code to UTF16
				if (nCode < 0xd800) {
					*pOutChar = nCode;
					pOutChar++;
					cbOutCount++;
				}
				else {
					nfUint16 nHighSurrogate, nLowSurrogate;
					fnCharacterIDToUTF16(nCode, nHighSurrogate, nLowSurrogate);
					*pOutChar = nHighSurrogate;
					pOutChar++;
					*pOutChar = nLowSurrogate;
					pOutChar++;

					cbOutCount += 2;
				}
			}
			else {
				__NMRASSERT(nLength == 3);
				pInChar += 2;
				cbCount -= 2;

			}

		}

		// everything has been processed.
		return cbOutCount;

	}



	std::wstring fnRemoveLeadingPathDelimiter(_In_ const std::wstring sPath)
	{
		const nfWChar * pChar = sPath.c_str();

		while ((*pChar == L'/') || (*pChar == L'\\'))
			pChar++;

		return std::wstring(pChar);
	}

	std::wstring fnIncludeLeadingPathDelimiter(_In_ const std::wstring sPath)
	{
		if (sPath.length() == 0) {
			return L"/";
		}

		const nfWChar * pChar = sPath.c_str();
		if ((*pChar == L'/') || (*pChar == L'\\'))
			return sPath;
		std::wstring sPrefix = L"/";

		return sPrefix + sPath;
	}

	std::wstring fnExtractFileName(_In_ const std::wstring sFullPath)
	{
		const nfWChar * pChar = sFullPath.c_str();
		const nfWChar * pLastDelimiter = nullptr;

		while (*pChar != 0) {
			if ((*pChar == L'/') || (*pChar == L'\\'))
				pLastDelimiter = pChar;

			pChar++;
		}

		if (pLastDelimiter != nullptr) {
			// Leave away delimiter
			pLastDelimiter++;
			return std::wstring(pLastDelimiter);
		}
		else {
			// We have no directory given
			return sFullPath;
		}

	}
}
