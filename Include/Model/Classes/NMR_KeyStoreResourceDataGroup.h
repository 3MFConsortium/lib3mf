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

NMR_KeyStoreResourceData.h defines the KeyStoreResourceData Class. A ResourceData is an in memory representation of the 3MF keystore resourcedata element.

--*/

#ifndef __NMR_KEYSTORERESOURCEDATAGROUP
#define __NMR_KEYSTORERESOURCEDATAGROUP

#include <map>
#include <memory>
#include <vector>
#include <mutex>
#include <set>

#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_PackageResourceID.h"
#include "Common/NMR_UUID.h"

namespace NMR {

	class CKeyStoreResourceDataCustomInformation {
	private:
		std::map <std::pair <std::string, std::string>, std::string> m_CustomInformationMap;
	public:
		void add(const std::string& sNameSpace, const std::string& sName, const std::string& sValue);
		bool has(const std::string& sNameSpace, const std::string& sName);
		bool remove(const std::string& sNameSpace, const std::string& sName);
		std::string get(const std::string& sNameSpace, const std::string& sName);
		std::set<std::string> nameSpaces();
		std::set<std::pair<std::string, std::string>> informationNames();
	};

	typedef std::shared_ptr<CKeyStoreResourceDataCustomInformation> PKeyStoreResourceDataCustomInformation;

	class CKeyStoreResourceDataGroup {
		PUUID m_sKeyUUID;
		std::vector<PKeyStoreAccessRight> m_AccessRights;
		std::map<std::string, PKeyStoreAccessRight> m_ConsumerAccesstRight;
		std::vector<nfByte> m_rgKey;
		std::mutex mtx;

		PKeyStoreResourceDataCustomInformation m_CustomInformation;

	public:
		CKeyStoreResourceDataGroup(PUUID const& keyUUID, std::vector<nfByte> const & key);

		PUUID getKeyUUID() const;
		nfUint64 addAccessRight(PKeyStoreAccessRight const & ar);
		void removeAccessRight(std::string const & consumerId);
		nfUint64 getAccessRightCount();
		PKeyStoreAccessRight getAccessRight(nfUint64 index) const;
		PKeyStoreAccessRight findAccessRightByConsumerID(std::string const & consumerId) const;

		PKeyStoreResourceDataCustomInformation customInformation() const;

		std::vector<nfByte> const & getKey() const;
		void setKey(std::vector<nfByte> const & key);

		inline nfBool isOpen() const {
			return !m_rgKey.empty();
		}
	};
	typedef std::shared_ptr<CKeyStoreResourceDataGroup> PKeyStoreResourceDataGroup;
}

#endif
