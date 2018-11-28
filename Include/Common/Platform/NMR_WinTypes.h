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

NMR_WinTypes.h defines Standard Windows Types on Non-Windows Environments

--*/

#ifndef __NMR_WINTYPES
#define __NMR_WINTYPES

#ifdef __GNUC__
typedef int BOOL;
typedef float FLOAT;
typedef double DOUBLE;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef signed int INT;
typedef float FLOAT;
typedef unsigned int ULONG;
typedef const wchar_t * LPCWSTR;
typedef char CHAR;
typedef wchar_t * LPWSTR;
typedef unsigned int UINT;
typedef unsigned int HRESULT;
typedef const char * LPCSTR;
typedef char * LPSTR;
typedef unsigned long long ULONG64;
typedef unsigned long long UINT64;

#else

#include <Windows.h>

#endif

#endif // __NMR_WINTYPES
