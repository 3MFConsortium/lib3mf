#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {

	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const & keyUUID, std::vector<PKeyStoreAccessRight> const & ar)
	{
		m_sKeyUUID = keyUUID;
		m_AccessRights = ar;
	}

	void CKeyStoreResourceDataGroup::removeAccessRight(std::string const & consumerId)
	{
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

	nfUint32 CKeyStoreResourceDataGroup::addAccessRight(PKeyStoreAccessRight const & ar)
	{
		std::string consumerId = ar->getConsumer()->getConsumerID();
		if (m_ConsumerAccesstRight.find(consumerId) != m_ConsumerAccesstRight.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATEACCESSRIGHT);
		}
		mtx.lock();
		m_AccessRights.push_back(ar);
		nfUint32 elIdx = (nfUint32)m_AccessRights.size() - 1;
		m_ConsumerAccesstRight[consumerId] = ar;
		mtx.unlock();
		return elIdx;
	}

	nfUint32 CKeyStoreResourceDataGroup::getAccessRightCount()
	{
		return (uint32_t)m_AccessRights.size();
	}

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::getAccessRight(nfUint32 index) const
	{
		if (index >= m_AccessRights.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		return m_AccessRights[index];
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
