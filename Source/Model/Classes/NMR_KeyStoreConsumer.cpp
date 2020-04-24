#include "Model/Classes/NMR_KeyStoreConsumer.h"
namespace NMR {

	CKeyStoreConsumer::CKeyStoreConsumer(std::string const & consumerID, std::string const & keyID, std::string keyValue)
	{
		m_sConsumerID = consumerID;
		m_sKeyID = keyID;
		m_sKeyValue = keyValue;
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

	nfBool CKeyStoreConsumer::hasKeyID() const {
		return !m_sKeyID.empty();
	}

	std::string CKeyStoreConsumer::getKeyValue() const
	{
		return m_sKeyValue;
	}

	void CKeyStoreConsumer::setKeyValue(std::string const & keyValue)
	{
		m_sKeyValue = keyValue;
	}

	nfBool CKeyStoreConsumer::hasKeyValue() const {
		return !m_sKeyValue.empty();
	}
}
