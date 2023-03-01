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

NMR_ModelToolpathProfile.h defines the Model Toolpath Profile.

--*/

#ifndef __NMR_MODELTOOLPATHPROFILE
#define __NMR_MODELTOOLPATHPROFILE

#include "Common/NMR_Types.h" 

#define MODELTOOLPATH_MAXCOUNT (1024 * 1024)

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <list>

namespace NMR {

	struct sModelToolpathProfileValue {
		std::string m_sNameSpace;
		std::string m_sValueName;
		std::string m_sValue;
	};

	class CModelToolpathProfile {
	private:
		std::string m_sUUID;
		std::string m_sName;

		std::vector<std::pair<std::string, std::string>> m_Parameters;
		std::map<std::pair<std::string, std::string>, std::string> m_Values;

	public:
		CModelToolpathProfile() = delete;
		CModelToolpathProfile(std::string sUUID, std::string sName);

		std::string getUUID();
		std::string getName();
		void setName (const std::string & sName);

		bool hasValue (const std::string & sNameSpace, const std::string & sValueName);
		std::string getValue(const std::string& sNameSpace, const std::string& sValueName);
		void addValue(const std::string& sNameSpace, const std::string& sValueName, const std::string & sValue);

		uint32_t getParameterCount();
		std::string getParameterName(const uint32_t nIndex);
		std::string getParameterNameSpace(const uint32_t nIndex);

		std::list<sModelToolpathProfileValue> listValues();

	};

	typedef std::shared_ptr <CModelToolpathProfile> PModelToolpathProfile;

}

#endif // __NMR_MODELTOOLPATHPROFILE

