#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {
	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const & keyUUID)
	{
		m_sKeyUUID = keyUUID;
	}
	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const & keyUUID, std::vector<PKeyStoreAccessRight> const & ar, std::vector<PKeyStoreResourceData> const & rd)
	{
		m_sKeyUUID = keyUUID;
		m_AccessRights = ar;
		m_ResourcesData = rd;
	}

	void CKeyStoreResourceDataGroup::removeAccessRight(NMR::PKeyStoreConsumer const & consumer)
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		size_t n = m_ConsumerAccesstRight.erase(consumer->getConsumerID());
		if (n > 0) {
			auto it = m_AccessRights.begin();
			while (it != m_AccessRights.end()) {
				if ((*it)->getConsumer()->getConsumerID() == consumer->getConsumerID()) {
					it = m_AccessRights.erase(it);
				}
			}
		}
	}

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::addAccessRight(NMR::PKeyStoreConsumer const & consumer, eKeyStoreEncryptAlgorithm const & encryptAlgorithm)
	{
		PKeyStoreAccessRight ar = std::make_shared<CKeyStoreAccessRight>(consumer, encryptAlgorithm);
		return addAccessRight(ar);
	}

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::addAccessRight(PKeyStoreAccessRight const & ar)
	{
		if (m_ConsumerAccesstRight.find(ar->getConsumer()->getConsumerID()) != m_ConsumerAccesstRight.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATEACCESSRIGHT);
		}

		m_AccessRights.push_back(ar);
		m_ConsumerAccesstRight[ar->getConsumer()->getConsumerID()] = ar;
		return ar;
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

	PKeyStoreAccessRight CKeyStoreResourceDataGroup::findAccessRightByConsumer(NMR::PKeyStoreConsumer const & consumer)
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_ConsumerAccesstRight.find(consumer->getConsumerID()) != m_ConsumerAccesstRight.end())
			return m_ConsumerAccesstRight[consumer->getConsumerID()];
		return nullptr;
	}

}
