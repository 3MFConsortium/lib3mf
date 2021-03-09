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
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_PackageResourceID.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h" 
#include <memory>
#include <algorithm>

namespace NMR {
	CKeyStore::CKeyStore() {
		m_UUID = std::make_shared<CUUID>();
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

	void CKeyStore::addConsumer(PKeyStoreConsumer const & consumer)
	{
		std::lock_guard<std::mutex> guard(mtx);
		if (m_Consumers.size() >= XML_3MF_SECURE_CONTENT_MAXELEMENTCOUNT)
			throw CNMRException(NMR_ERROR_KEYSTORETOOMANYELEMENTS);

		std::string const id = consumer->getConsumerID();
		if (m_ConsumerRefs.find(id) != m_ConsumerRefs.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATECONSUMER);
		}
		m_Consumers.push_back(consumer);
		m_ConsumerRefs[id] = consumer;
	}

	nfUint64 CKeyStore::getConsumerCount() const
	{
		return m_Consumers.size();
	}

	PKeyStoreConsumer CKeyStore::getConsumer(nfUint64 index) const
	{	
		if (index >= m_Consumers.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Consumers[(size_t)index];
	}

	PKeyStoreConsumer CKeyStore::findConsumerById(std::string id)
	{	
		auto ref = m_ConsumerRefs.find(id);
		if ( ref != m_ConsumerRefs.end())
			return (*ref).second;
		return nullptr;
	}

	void CKeyStore::removeConsumer(NMR::PKeyStoreConsumer consumer)
	{
		std::lock_guard<std::mutex> guard(mtx);
		size_t n = m_ConsumerRefs.erase(consumer->getConsumerID());
		if (n > 0) {
			for (auto it : m_ResourceDataGroups) {
				it->removeAccessRight(consumer->getConsumerID());
			}
			auto found = std::find(m_Consumers.begin(), m_Consumers.end(), consumer);
			m_Consumers.erase(found);
		}
	}

	nfUint64 CKeyStore::getResourceDataGroupCount() const
	{
		return m_ResourceDataGroups.size();
	}

	PKeyStoreResourceDataGroup CKeyStore::getResourceDataGroup(nfUint64 index) const {
		if (index < m_ResourceDataGroups.size())
			return m_ResourceDataGroups[(size_t)index];
		throw CNMRException(NMR_ERROR_INVALIDINDEX);
	}

	void CKeyStore::addResourceDataGroup(PKeyStoreResourceDataGroup const & dataGroup)
	{
		std::lock_guard<std::mutex> guard(mtx);

		if (m_ResourceDataGroups.size() >= XML_3MF_SECURE_CONTENT_MAXELEMENTCOUNT)
			throw CNMRException(NMR_ERROR_KEYSTORETOOMANYELEMENTS);

		PUUID const keyUUID = dataGroup->getKeyUUID();
		if (m_ResourceDataGroupsRefs.find(keyUUID) != m_ResourceDataGroupsRefs.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATERESOURCEDATAGROUP);
		}
		m_ResourceDataGroups.push_back(dataGroup);
		m_ResourceDataGroupsRefs[keyUUID] = dataGroup;
	}

	PKeyStoreResourceDataGroup CKeyStore::findResourceDataGroupByResourceDataPath(PPackageModelPath const & rdPath) {
		auto found = findResourceData(rdPath);
		if (nullptr != found)
			return found->getGroup();
		return nullptr;
	}

	void CKeyStore::removeResourceDataGroup(PKeyStoreResourceDataGroup rdg) {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfUint64 CKeyStore::addResourceData(PKeyStoreResourceData const & rd) {
		std::lock_guard<std::mutex> guard(mtx);

		if (m_ResourceDatas.size() >= XML_3MF_SECURE_CONTENT_MAXELEMENTCOUNT)
			throw CNMRException(NMR_ERROR_KEYSTORETOOMANYELEMENTS);

		if (nullptr == rd->getGroup())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		auto found = m_ResourceDataRefs.find(rd->packagePath());
		if (found != m_ResourceDataRefs.end())
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATERESOURCEDATA);

		m_ResourceDatas.push_back(rd);
		m_ResourceDataRefs[rd->packagePath()] = rd;
		return m_ResourceDatas.size() - 1;
	}

	void CKeyStore::removeResourceData(NMR::PKeyStoreResourceData const & rd) {
		std::lock_guard<std::mutex> guard(mtx);
		size_t n = m_ResourceDataRefs.erase(rd->packagePath());
		if (n > 0) {
			auto found = std::find(m_ResourceDatas.begin(), m_ResourceDatas.end(), rd);
			m_ResourceDatas.erase(found);
		}
	}

	nfUint64 CKeyStore::getResourceDataCount() {
		return m_ResourceDatas.size();
	}

	PKeyStoreResourceData CKeyStore::getResourceData(nfUint64 index) const {
		if (index < m_ResourceDatas.size())
			return m_ResourceDatas[(size_t)index];
		throw CNMRException(NMR_ERROR_INVALIDINDEX);
	}

	PKeyStoreResourceData CKeyStore::findResourceData(PPackageModelPath const & path) {
		auto found = m_ResourceDataRefs.find(path);
		if (found != m_ResourceDataRefs.end())
			return (*found).second;
		return nullptr;
	}

	std::vector<PKeyStoreResourceData> CKeyStore::getResourceDataByGroup(PKeyStoreResourceDataGroup const & rdg) const {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	PKeyStoreResourceDataGroup CKeyStore::findResourceDataGroupByResourceDataPath(std::string const & rdPath) {
		PKeyStoreResourceData rd = findResourceData(rdPath);
		if (nullptr != rd)
			return rd->getGroup();
		return nullptr;
	}

	PKeyStoreResourceData CKeyStore::findResourceData(std::string const & path) {
		auto found = std::find_if(m_ResourceDatas.begin(), m_ResourceDatas.end(), [&path](PKeyStoreResourceData const & rd) {
			return rd->packagePath()->getPath() == path;
		});
		if (found != m_ResourceDatas.end())
			return *found;
		return nullptr;
	}

	bool CKeyStore::empty() const {
		return m_Consumers.empty() && m_ResourceDataGroups.empty();
	}

	void CKeyStore::clearAll() {
		m_ConsumerRefs.clear();
		m_Consumers.clear();
		m_ResourceDataGroupsRefs.clear();
		m_ResourceDataGroups.clear();
	}

}