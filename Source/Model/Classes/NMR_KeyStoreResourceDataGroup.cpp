#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {

	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const& keyUUID, std::vector<nfByte> const & key)
	{
		m_sKeyUUID = keyUUID;
		m_rgKey = key;
	}

	void CKeyStoreResourceDataGroup::removeAccessRight(std::string const & consumerId)
	{
		std::lock_guard<std::mutex> guard(mtx);

		if (consumerId.empty())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		size_t n = m_ConsumerAccesstRight.erase(consumerId);
		if (n > 0) {
			auto it = m_AccessRights.begin();
			while (it != m_AccessRights.end()) {
				if ((*it)->getConsumer()->getConsumerID() == consumerId) {
					it = m_AccessRights.erase(it);
				}
			}
		}
	}

	PUUID CKeyStoreResourceDataGroup::getKeyUUID() const {
		return m_sKeyUUID;
	}

	nfUint64 CKeyStoreResourceDataGroup::addAccessRight(PKeyStoreAccessRight const & ar)
	{
		std::lock_guard<std::mutex> guard(mtx);

		if (m_AccessRights.size() >= XML_3MF_SECURE_CONTENT_MAXELEMENTCOUNT)
			throw CNMRException(NMR_ERROR_KEYSTORETOOMANYELEMENTS);

		std::string consumerId = ar->getConsumer()->getConsumerID();
		if (m_ConsumerAccesstRight.find(consumerId) != m_ConsumerAccesstRight.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATEACCESSRIGHT);
		}

		m_AccessRights.push_back(ar);
		nfUint64 elIdx = m_AccessRights.size() - 1;
		m_ConsumerAccesstRight[consumerId] = ar;
		return elIdx;
	}

	nfUint64 CKeyStoreResourceDataGroup::getAccessRightCount()
	{
		return m_AccessRights.size();
	}

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::getAccessRight(nfUint64 index) const
	{
		if (index >= m_AccessRights.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		return m_AccessRights[(size_t)index];
	}

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::findAccessRightByConsumerID(std::string const & consumerId) const
	{
		if (consumerId.empty())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		auto found = m_ConsumerAccesstRight.find(consumerId);
		if (found != m_ConsumerAccesstRight.end())
			return (*found).second;
		return nullptr;
	}

	std::vector<nfByte> const & CKeyStoreResourceDataGroup::getKey() const {
		return m_rgKey;
	}

	void CKeyStoreResourceDataGroup::setKey(std::vector<nfByte> const & key) {
		m_rgKey = key;
	}

}
