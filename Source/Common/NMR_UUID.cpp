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

NMR_UUID.cpp implements a datatype and functions to handle UUIDs

--*/

#include "Common/NMR_UUID.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"

#include <algorithm>

#ifdef GUID_WINDOWS
#include <objbase.h>
#include <iomanip>
#endif

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
#endif

#ifndef GUID_CFUUID
#define GUID_CFUUID
#endif

#ifdef GUID_CFUUID
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef GUID_CUSTOM
#include <ctime>
#endif

namespace NMR
{

#ifdef GUID_CUSTOM
	std::mutex CUUID::S_uuid_gen_mutex;
	std::unique_ptr<std::mt19937> CUUID::S_mtwister;
#endif

	CUUID::CUUID()
	{
#ifdef GUID_WINDOWS
		GUID guid;
		if (CoCreateGuid(&guid) != S_OK)
			throw CNMRException(NMR_ERROR_UUIDGENERATIONFAILED);
		LPOLESTR str; 
		if (StringFromCLSID(guid, &str) != S_OK)
			throw CNMRException(NMR_ERROR_UUIDGENERATIONFAILED);
		set(fnUTF16toUTF8(str).c_str());
#endif

#ifdef GUID_LIBUUID
		uuid_t uuid;
		uuid_generate_random(uuid);
		char s[37];
		uuid_unparse(uuid, s);
		set(std::string(s).c_str());
#endif

#ifdef GUID_CFUUID
		auto newId = CFUUIDCreate(NULL);
		auto bytes = CFUUIDGetUUIDBytes(newId);
		CFRelease(newId);

		char out[16*2+3+1+1];
		out[36] = 0;
		snprintf(out, 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			bytes.byte0, bytes.byte1, bytes.byte2, bytes.byte3,
			bytes.byte4, bytes.byte5,
			bytes.byte6, bytes.byte7,
			bytes.byte8, bytes.byte9,
			bytes.byte10, bytes.byte11, bytes.byte12, bytes.byte13, bytes.byte14, bytes.byte15);
		set(out);
#endif

#ifdef GUID_CUSTOM
		std::lock_guard<std::mutex> l_lock(S_uuid_gen_mutex);
		if (!S_mtwister)
		{
			S_mtwister.reset(new std::mt19937);
			std::random_device rand_dev;
			// Prepare seed data
			uint32_t l_seed_data[std::mt19937::state_size];
			uint32_t l_cur_time = static_cast<uint32_t>(time(nullptr));
			for (size_t i = 0; i < std::mt19937::state_size; ++i)
			{
				l_seed_data[i] = rand_dev() ^ l_cur_time;
			}

			// Init Mersenne Twister pseudo-random generator
			std::seed_seq l_seed_seq(std::begin(l_seed_data), std::end(l_seed_data));
			S_mtwister->seed(l_seed_seq);
		}

		// generation of a v4 UUID according to https://tools.ietf.org/html/rfc4122#section-4.4
		std::uniform_int_distribution<std::mt19937::result_type> distHexaDec(0, 15);
		std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);
		const nfChar* hexaDec = "0123456789abcdef";
		nfChar string[33];
		for (int i = 0; i < 32; i++)
			string[i] = hexaDec[distHexaDec(*S_mtwister)];
		string[12] = hexaDec[4]; // set version 4
		string[16] = hexaDec[8 + dist4(*S_mtwister)];
		string[32] = 0;

		set(string);
#endif
	}

	CUUID::CUUID(const nfChar* pString)
	{
		set(pString);
	}

	CUUID::CUUID(const std::string &string)
	{
		set(string.c_str());
	}
	
	std::string CUUID::toString() const
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
