#include "Model/Classes/NMR_KeyStoreConsumer.h"
namespace NMR {

	CKeyStoreConsumer::CKeyStoreConsumer(std::string const & consumerID) {
		m_sConsumerID = consumerID;
	}

	CKeyStoreConsumer::CKeyStoreConsumer(std::string const & consumerID, std::string const & keyID, RSAKEYVALUE keyValue)
	{
		m_sConsumerID = consumerID;
		m_sKeyID = keyID;
		m_keyValue = keyValue;
	}

	std::string CKeyStoreConsumer::getConsumerID() const {
		return m_sConsumerID;
	}

	std::string CKeyStoreConsumer::getKeyID() const {
		return m_sKeyID;
	}

	void CKeyStoreConsumer::setKeyID(std::string const & keyID) {
		m_sKeyID = keyID;
	}

	RSAKEYVALUE CKeyStoreConsumer::getKeyValue() const {
		return m_keyValue;
	}

	void CKeyStoreConsumer::setKeyValue(RSAKEYVALUE const & keyValue) {
		m_keyValue = keyValue;
		m_bHasKeyValue = true;
	}

	nfBool CKeyStoreConsumer::hasKeyValue() const {
		return m_bHasKeyValue;
	}

}
