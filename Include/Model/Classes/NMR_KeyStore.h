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

NMR_KeyStore.h defines the KeyStore Class. A keystore is an in memory representation of the 3MF file keystore.

--*/

#ifndef __NMR_KEYSTORE
#define __NMR_KEYSTORE

#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/NMR_UUID.h"

namespace NMR {
	class CKeyStoreConsumer;
	using PKeyStoreConsumer = std::shared_ptr<CKeyStoreConsumer>;
	class CKeyStoreResourceDataGroup;
	using PKeyStoreResourceDataGroup = std::shared_ptr<CKeyStoreResourceDataGroup>;
	class CKeyStoreResourceData;
	using PKeyStoreResourceData = std::shared_ptr<CKeyStoreResourceData>;
	class CKeyStoreAccessRight;
	using PKeyStoreAccessRight = std::shared_ptr<CKeyStoreAccessRight>;

	class CPackageModelPath;
	using PPackageModelPath = std::shared_ptr<CPackageModelPath>;

	class CKeyStore {
	private:
		PUUID m_UUID;

		std::vector<PKeyStoreConsumer> m_Consumers;
		std::map<std::string, PKeyStoreConsumer> m_ConsumerRefs;

		std::vector<PKeyStoreResourceDataGroup> m_ResourceDataGroups;
		std::map<PUUID, PKeyStoreResourceDataGroup> m_ResourceDataGroupsRefs;

		std::vector<PKeyStoreResourceData> m_ResourceDatas;
		std::map<PPackageModelPath, PKeyStoreResourceData> m_ResourceDataRefs;

		std::mutex mtx;
	public:
		CKeyStore();
		~CKeyStore();
		PUUID getUUID();
		void setUUID(PUUID uuid);
		void addConsumer(PKeyStoreConsumer const &consumer);
		nfUint64 getConsumerCount() const;
		PKeyStoreConsumer getConsumer(nfUint64 index) const;
		PKeyStoreConsumer findConsumerById(std::string id);
		void removeConsumer(NMR::PKeyStoreConsumer consumer);
		
		nfUint64 getResourceDataGroupCount() const;
		PKeyStoreResourceDataGroup getResourceDataGroup(nfUint64 index) const;
		void addResourceDataGroup(PKeyStoreResourceDataGroup const &dataGroup);
		PKeyStoreResourceDataGroup findResourceDataGroupByResourceDataPath(PPackageModelPath const & rdPath);
		void removeResourceDataGroup(PKeyStoreResourceDataGroup rdg);

		nfUint64 addResourceData(PKeyStoreResourceData const & rd);
		void removeResourceData(NMR::PKeyStoreResourceData const & rd);
		nfUint64 getResourceDataCount();
		PKeyStoreResourceData getResourceData(nfUint64 index) const;
		PKeyStoreResourceData findResourceData(PPackageModelPath const & path);

		std::vector<PKeyStoreResourceData> getResourceDataByGroup(PKeyStoreResourceDataGroup const & rdg) const;

		PKeyStoreResourceDataGroup findResourceDataGroupByResourceDataPath(std::string const & rdPath);
		PKeyStoreResourceData findResourceData(std::string const & path);

		bool empty() const;
	protected:
		void clearAll();
	};

	typedef std::shared_ptr<CKeyStore> PKeyStore;
}

#endif
