#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {


	void CKeyStoreResourceDataCustomInformation::add(const std::string& sNameSpace, const std::string& sName, const std::string& sValue)
	{
		m_CustomInformationMap.insert(std::make_pair(std::make_pair(sNameSpace, sName), sValue));
	}

	bool CKeyStoreResourceDataCustomInformation::has(const std::string& sNameSpace, const std::string& sName)
	{
		auto iIter = m_CustomInformationMap.find(std::make_pair(sNameSpace, sName));
		return iIter != m_CustomInformationMap.end();
	}

	bool CKeyStoreResourceDataCustomInformation::remove(const std::string& sNameSpace, const std::string& sName)
	{
		auto iIter = m_CustomInformationMap.find(std::make_pair(sNameSpace, sName));
		bool bFoundInformation = (iIter != m_CustomInformationMap.end());
		m_CustomInformationMap.erase(iIter);
		return bFoundInformation;
	}

	std::string CKeyStoreResourceDataCustomInformation::get(const std::string& sNameSpace, const std::string& sName)
	{
		auto iIter = m_CustomInformationMap.find(std::make_pair(sNameSpace, sName));
		if (iIter != m_CustomInformationMap.end())
			return iIter->second;

		return "";
	}



	std::set<std::string> CKeyStoreResourceDataCustomInformation::nameSpaces()
	{
		std::set<std::string> sNameSpaces;
		for (auto iIter : m_CustomInformationMap) {
			sNameSpaces.insert(iIter.first.first);
		}

		return sNameSpaces;
	}

	std::set<std::pair<std::string, std::string>> CKeyStoreResourceDataCustomInformation::informationNames()
	{
		std::set<std::pair<std::string, std::string>> sNamesSet;
		for (auto iIter : m_CustomInformationMap) {
			sNamesSet.insert(iIter.first);
		}

		return sNamesSet;

	}



	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const& keyUUID, std::vector<nfByte> const & key)
	{
		m_sKeyUUID = keyUUID;
		m_rgKey = key;
		m_CustomInformation = std::make_shared<CKeyStoreResourceDataCustomInformation>();
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

	PKeyStoreResourceDataCustomInformation CKeyStoreResourceDataGroup::customInformation() const
	{
		return m_CustomInformation;
	}

	std::vector<nfByte> const & CKeyStoreResourceDataGroup::getKey() const {
		return m_rgKey;
	}

	void CKeyStoreResourceDataGroup::setKey(std::vector<nfByte> const & key) {
		m_rgKey = key;
	}




}
