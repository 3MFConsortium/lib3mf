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

NMR_UUID.cpp implements a datatype and functions to handle UUIDs

--*/

#include "Common/NMR_UUID.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"

#include <algorithm>

#if defined(_WIN32) && !defined(__MINGW32__)
#include <Objbase.h>
#include <iomanip>
#else
#include <uuid/uuid.h>
#endif

namespace NMR
{
	CUUID::CUUID()
	{
#if defined(_WIN32) && !defined(__MINGW32__)
		GUID guid;
		if (CoCreateGuid(&guid) != S_OK)
			throw CNMRException(NMR_ERROR_UUIDGENERATIONFAILED);
		LPOLESTR str; 
		if (StringFromCLSID(guid, &str) != S_OK)
			throw CNMRException(NMR_ERROR_UUIDGENERATIONFAILED);
		set(fnUTF16toUTF8(str).c_str());
#else
		uuid_t uuid;
		uuid_generate_random(uuid);
		char s[37];
		uuid_unparse(uuid, s);
		set(std::string(s).c_str());
#endif
	}

	CUUID::CUUID(const nfChar* pString)
	{
		set(pString);
	}
	
	std::string CUUID::toString()
	{
		return m_sUUID;
	}

	bool InValid(char c)
	{
		return !(((c >= '0') && (c <= '9'))
			|| ((c >= 'a') && (c <= 'f'))
		 // || (c == '-')
			);
	}

	bool CUUID::set(const nfChar* pString)
	{
		// to lowercase, remove all non hex characters, insert dashes again
		std::string str = pString;
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		str.erase(std::remove_if(str.begin(), str.end(), &InValid), str.end());
		if (str.length() != 32) {
			throw CNMRException(NMR_ERROR_ILLFORMATUUID);
		}
		m_sUUID = str.substr(0, 8) + '-' + str.substr(8, 4) + '-' + str.substr(12, 4) + '-' + str.substr(16, 4) + '-' + str.substr(20, 12);
		return true;
	}

	CUUID& CUUID::operator=(const CUUID& uuid)
	{
		if (&uuid == this)
			return *this;
		m_sUUID = uuid.m_sUUID;
		return *this;
	}

	bool CUUID::operator==(const CUUID& uuid)
	{
		if (&uuid == this)
			return true;
		return m_sUUID == uuid.m_sUUID;
	}

}
