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

NMR_WinTypes.h defines Standard Windows Types on Non-Windows Environments

--*/

#ifndef __NMR_WINTYPES
#define __NMR_WINTYPES

#ifdef __GCC
#define BOOL int
#define FLOAT float
#define BYTE unsigned char
#define DWORD unsigned int
#define FLOAT float
#define ULONG unsigned int
#define LPCWSTR const wchar_t *
#define CHAR
#define LPWSTR wchar_t *
#define UINT unsigned int
#define HRESULT unsigned int
#define LPCSTR const char *
#define LPSTR char *
#define ULONG64 unsigned long long
#define UINT64 unsigned long long

#else

#include <Windows.h>

#endif

#endif // __NMR_WINTYPES
