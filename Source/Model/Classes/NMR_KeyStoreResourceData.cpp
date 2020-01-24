#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"

#include <memory>
namespace NMR {
	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path) {
		m_sPath = path;
		m_EncryptionAlgorithm = eKeyStoreEncryptAlgorithm::Aes256Gcm;
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::addDecryptRight(NMR::PKeyStoreConsumer consumer, eKeyStoreEncryptAlgorithm ea)
	{	
		NMR::AES256GCMCIPHERVALUE value;
		PKeyStoreDecryptRight dR = std::make_shared<CKeyStoreDecryptRight>(consumer, ea, value);
		m_DecryptRights.push_back(dR);
		if(m_ConsumerDecryptRight.find(consumer) != m_ConsumerDecryptRight.end())
			m_ConsumerDecryptRight[consumer] = dR;
		else throw CNMRException(NMR_ERROR_DUPLICATE_KEYSTORECONSUMER);
		return dR;
	}

	nfUint32 CKeyStoreResourceData::getDecryptRightCount()
	{	
		return (uint32_t)m_DecryptRights.size();
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::getDecryptRight(nfUint32 index) const 
	{	
		return m_DecryptRights[index];
	}

	PKeyStoreDecryptRight CKeyStoreResourceData::findDecryptRightByConsumer(NMR::PKeyStoreConsumer consumer) 
	{
		return m_ConsumerDecryptRight[consumer];
	}

	void CKeyStoreResourceData::removeDecryptRight(NMR::PKeyStoreConsumer consumer)
	{
		size_t n = m_ConsumerDecryptRight.erase(consumer);
		if (n > 0) {
			for (auto it = m_DecryptRights.begin(); it != m_DecryptRights.end(); it++) {
				if ((*it)->getConsumer() == consumer) {
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
}