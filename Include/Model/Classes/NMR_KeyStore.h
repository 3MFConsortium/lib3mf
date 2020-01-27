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

#include <list>
#include <map>
#include <memory>
#include <vector>
#include "Common/NMR_Types.h"
#include "Common/NMR_KeyStoreTypes.h"

namespace NMR {
	class CKeyStoreConsumer;
	typedef std::shared_ptr<CKeyStoreConsumer> PKeyStoreConsumer;
	class CKeyStoreResourceData;
	typedef std::shared_ptr<CKeyStoreResourceData> PKeyStoreResourceData;



	class CKeyStore {
	private:
		std::vector<PKeyStoreConsumer> m_Consumers;
		std::map<std::string, PKeyStoreConsumer> m_ConsumerRefs;

		std::vector<PKeyStoreResourceData> m_ResourceDatas;
		std::map<std::string, PKeyStoreResourceData> m_ResourceDataRefs;
	public:
		CKeyStore();
		~CKeyStore();
		PKeyStoreConsumer addConsumer(std::string id, std::string keyId, RSAKEYVALUE keyValue);
		nfUint32 getConsumerCount() const;
		PKeyStoreConsumer getConsumerByIndex(nfUint64 index) const;
		PKeyStoreConsumer findConsumerById(std::string id);
		void removeConsumer(NMR::PKeyStoreConsumer consumer);
		nfUint32 resourceDataCount() const;
		PKeyStoreResourceData getResourceDataByIndex(nfUint64 index) const;
		PKeyStoreResourceData addResourceData(std::string path, eKeyStoreEncryptAlgorithm ea, nfBool compression);
		PKeyStoreResourceData findResourceDataByPath(std::string path);
	protected:
		void clearAll();
	};

	typedef std::shared_ptr<CKeyStore> PKeyStore;
}

#endif