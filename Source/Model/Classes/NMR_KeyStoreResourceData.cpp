#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"
#include <memory>

#define IV_SIZE 12
#define TAG_SIZE 16
#define KEY_SIZE 32

namespace NMR {


	nfUint64 CKeyStoreResourceData::s_nfHandleCount = 0;

	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path) {
		m_sPath = path;
		m_EncryptionAlgorithm = eKeyStoreEncryptAlgorithm::Aes256Gcm;
		m_nfHandle = ++s_nfHandleCount;
		m_eState = eResourceDataState::NEW;
	}

	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path, eKeyStoreEncryptAlgorithm const & ea, nfBool const & compression)
	{
		m_sPath = path;
		m_EncryptionAlgorithm = ea;
		m_bCompression = compression;
		m_nfHandle = ++s_nfHandleCount;
		m_eState = eResourceDataState::CLOSED;
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(NMR::PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptAlgorithm) {
		PKeyStoreDecryptRight dr = std::make_shared<CKeyStoreDecryptRight>(consumer, encryptAlgorithm);
		return addDecryptRight(dr);
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(PKeyStoreDecryptRight const &dr)
	{	
		if (m_ConsumerDecryptRight.find(dr->getConsumer()->getConsumerID()) != m_ConsumerDecryptRight.end()) {
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATEDECRYPTRIGHT);
		}

		m_DecryptRights.push_back(dr);
		m_ConsumerDecryptRight[dr->getConsumer()->getConsumerID()] = dr;
		return dr;
	}

	nfUint32 CKeyStoreResourceData::getDecryptRightCount()
	{	
		return (uint32_t)m_DecryptRights.size();
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::getDecryptRight(nfUint32 index) const 
	{	
		if (index >= m_DecryptRights.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		return m_DecryptRights[index];
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::findDecryptRightByConsumer(NMR::PKeyStoreConsumer const& consumer) 
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (m_ConsumerDecryptRight.find(consumer->getConsumerID()) != m_ConsumerDecryptRight.end())
			return m_ConsumerDecryptRight[consumer->getConsumerID()];
		return nullptr;
	}

	void CKeyStoreResourceData::removeDecryptRight(NMR::PKeyStoreConsumer const& consumer)
	{
		if (!consumer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		size_t n = m_ConsumerDecryptRight.erase(consumer->getConsumerID());
		if (n > 0) {
			auto it = m_DecryptRights.begin();
			while (it != m_DecryptRights.end()) {
				if ((*it)->getConsumer()->getConsumerID() == consumer->getConsumerID()) {
					it = m_DecryptRights.erase(it);
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

	nfUint64 CKeyStoreResourceData::getHandle() const {
		return m_nfHandle;
	}

	nfBool CKeyStoreResourceData::empty() const {
		return m_DecryptRights.empty();
	}

	CIPHERVALUE CKeyStoreResourceData::getCipherValue() const {
		return m_sCipherValue;
	}

	void CKeyStoreResourceData::open(CIPHERVALUE const & cv) {
		m_sCipherValue = cv;
		m_eState = eResourceDataState::OPEN;
	}

	void CKeyStoreResourceData::close() {
		m_eState = eResourceDataState::CLOSED;
	}

	void CKeyStoreResourceData::refreshIV(std::vector<nfByte> const & newIV) {
		if (newIV.size() != IV_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_sCipherValue.m_iv = newIV;
	}

	void CKeyStoreResourceData::refreshKey(std::vector<nfByte> const & newKey) {
		if (newKey.size() != KEY_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_sCipherValue.m_key = newKey;
	}

	bool CKeyStoreResourceData::isClosed() const {
		return m_eState == eResourceDataState::CLOSED;
	}

	bool CKeyStoreResourceData::isNew() const {
		return m_eState == eResourceDataState::NEW;
	}

}
