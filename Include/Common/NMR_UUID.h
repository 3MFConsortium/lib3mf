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

NMR_UUID.h defines a datatype and functions to handle UUIDs

--*/

#ifndef __NMR_UUID
#define __NMR_UUID

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include <string>

#ifdef GUID_CUSTOM
#include <mutex>
#include <random>
#include <memory>
#endif


namespace NMR
{
	class CUUID {
	private:
		std::string m_sUUID;
#ifdef GUID_CUSTOM
		static std::mutex S_uuid_gen_mutex;
		static std::unique_ptr<std::mt19937> S_mtwister;
#endif
	public:
		CUUID();
		CUUID(const nfChar* pString);
		CUUID(const std::string & string);
		std::string toString() const;

		bool set(const nfChar* pString);

		CUUID& operator=(const CUUID& uuid);
		bool operator==(const CUUID& uuid);
	};

	typedef std::shared_ptr<CUUID> PUUID;
}

#endif // __NMR_UUID
