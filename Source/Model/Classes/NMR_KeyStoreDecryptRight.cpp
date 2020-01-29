#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptionAlgorithm, AES256GCMCIPHERVALUE const& cipherValue) {
		m_pConsumer = consumer;
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::Aes256Gcm) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		m_EncryptionAlgorithm = encryptionAlgorithm;
		m_cipherValue = cipherValue;
	}

	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptionAlgorithm) {
		m_pConsumer = consumer;
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::Aes256Gcm) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		m_EncryptionAlgorithm = encryptionAlgorithm;
	}

	PKeyStoreConsumer CKeyStoreDecryptRight::getConsumer()
	{
		return m_pConsumer;
	}
}