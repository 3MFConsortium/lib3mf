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

NMR_KeyStore.cpp implements the KeyStore Class.
A KeyStore is an in memory representation of the 3MF file.

--*/

#include "Model/Classes/NMR_KeyStore.h"
#include "Common/NMR_StringUtils.h" 
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include <memory>
namespace NMR {
	CKeyStore::CKeyStore() {
	}

	CKeyStore::~CKeyStore() {
		clearAll();
	}

	PUUID CKeyStore::getUUID()
	{
		return m_UUID;
	}

	void CKeyStore::setUUID(PUUID uuid)
	{
		m_UUID = uuid;
	}

	PKeyStoreConsumer CKeyStore::addConsumer(std::string id, std::string keyId, RSAKEYVALUE keyValue)
	{
		PKeyStoreConsumer consumer = std::make_shared<CKeyStoreConsumer>(id, keyId, keyValue);
		m_Consumers.push_back(consumer);
		if (m_ConsumerRefs.find(id) != m_ConsumerRefs.end()) {
			m_ConsumerRefs[id] = consumer;
		}
		return consumer;
	}

	nfUint32 CKeyStore::getConsumerCount() const
	{
		return (uint32_t)m_Consumers.size();
	}

	PKeyStoreConsumer CKeyStore::getConsumerByIndex(nfUint64 index) const
	{	
		return m_Consumers[index];
	}

	PKeyStoreConsumer CKeyStore::findConsumerById(std::string id)
	{	
		return m_ConsumerRefs[id];
	}

	void CKeyStore::removeConsumer(NMR::PKeyStoreConsumer consumer)
	{
		size_t n = m_ConsumerRefs.erase(consumer->getConsumerID());
		if (n > 0) {
			for (auto it = m_Consumers.begin(); it != m_Consumers.end(); it++) {
				if ((*it) == consumer) {
					m_Consumers.erase(it);
				}
			}
		}
	}

	nfUint32 CKeyStore::resourceDataCount() const
	{
		return (uint32_t)m_ResourceDatas.size();
	}

	PKeyStoreResourceData CKeyStore::getResourceDataByIndex(nfUint64 index) const
	{	
		return m_ResourceDatas[index];
	}

	PKeyStoreResourceData CKeyStore::addResourceData(std::string path, eKeyStoreEncryptAlgorithm ea, nfBool compression)
	{
		PKeyStoreResourceData resourceData = std::make_shared<CKeyStoreResourceData>(path, ea, compression);
		m_ResourceDatas.push_back(resourceData);
		if (m_ResourceDataRefs.find(path) != m_ResourceDataRefs.end()) {
			m_ResourceDataRefs[path] = resourceData;
		}
		return resourceData;
	}

	PKeyStoreResourceData CKeyStore::findResourceDataByPath(std::string path)
	{
		return m_ResourceDataRefs[path];
	}

	bool CKeyStore::empty() const {
		return m_Consumers.empty() && m_ResourceDatas.empty();
	}

	void CKeyStore::clearAll() {
		m_ConsumerRefs.clear();
		m_Consumers.clear();
		m_ResourceDataRefs.clear();
		m_ResourceDatas.clear();
	}

}