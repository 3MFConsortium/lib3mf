#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {
	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path) {
		m_sPath = path;
		m_EncryptionAlgorithm = eKeyStoreEncryptAlgorithm::Aes256Gcm;
	}

	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path, eKeyStoreEncryptAlgorithm const & ea, nfBool const & compression)
	{
		m_sPath = path;
		m_EncryptionAlgorithm = ea;
		m_bCompression = compression;
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(PKeyStoreDecryptRight decryptRight)
	{
		if (!decryptRight->getConsumer().get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_ConsumerDecryptRight.find(decryptRight->getConsumer()->getConsumerID()) != m_ConsumerDecryptRight.end()) {
			throw CNMRException(NMR_ERROR_DUPLICATE_KEYSTORECONSUMER);
		}
		m_DecryptRights.push_back(decryptRight);
		m_ConsumerDecryptRight[decryptRight->getConsumer()->getConsumerID()] = decryptRight;
		return decryptRight;
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(NMR::PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptAlgorithm) {
		NMR::CIPHERVALUE value = { 0, 0, 0 };
		return this->addDecryptRight(consumer, encryptAlgorithm, value);
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(NMR::PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptAlgorithm, NMR::CIPHERVALUE const& cipherValue)
	{	
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_ConsumerDecryptRight.find(consumer->getConsumerID()) != m_ConsumerDecryptRight.end()) {
			throw CNMRException(NMR_ERROR_DUPLICATE_KEYSTORECONSUMER);
		}

		PKeyStoreDecryptRight decryptRight = std::make_shared<CKeyStoreDecryptRight>(consumer, encryptAlgorithm, cipherValue);
		m_DecryptRights.push_back(decryptRight);
		m_ConsumerDecryptRight[consumer->getConsumerID()] = decryptRight;
		return decryptRight;
	}

	nfUint32 CKeyStoreResourceData::getDecryptRightCount()
	{	
		return (uint32_t)m_DecryptRights.size();
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::getDecryptRight(nfUint32 index) const 
	{	
		return m_DecryptRights[index];
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::findDecryptRightByConsumer(NMR::PKeyStoreConsumer const& consumer) 
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		return m_ConsumerDecryptRight[consumer->getConsumerID()];
	}

	void CKeyStoreResourceData::removeDecryptRight(NMR::PKeyStoreConsumer const& consumer)
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		size_t n = m_ConsumerDecryptRight.erase(consumer->getConsumerID());
		if (n > 0) {
			for (auto it = m_DecryptRights.begin(); it != m_DecryptRights.end(); it++) {
				if ((*it)->getConsumer()->getConsumerID() == consumer->getConsumerID()) {
					m_DecryptRights.erase(it);
				}
			}
		}
	}

	eKeyStoreEncryptAlgorithm CKeyStoreResourceData::getEncryptionAlgorithm() const
	{	
		return m_EncryptionAlgorithm;
	}

	nfBool CKeyStoreResourceData::getCompression() const
	{
		return m_bCompression;
	}
	NMR::PPackageModelPath CKeyStoreResourceData::getPath() const
	{
		NMR::CResourceHandler * pResourceHandler = new NMR::CResourceHandler();
		return pResourceHandler->makePackageModelPath(m_sPath);
	}

	nfBool CKeyStoreResourceData::empty() const {
		return m_DecryptRights.empty();
	}

	CIPHERVALUE CKeyStoreResourceData::getCipherValue() const {
		return m_sCipherValue;
	}

	void CKeyStoreResourceData::setCipherValue(CIPHERVALUE const & cv) {
		m_sCipherValue = cv;
		m_bOpen = true;
	}

	bool CKeyStoreResourceData::isOpen() const {
		return m_bOpen;
	}

}
