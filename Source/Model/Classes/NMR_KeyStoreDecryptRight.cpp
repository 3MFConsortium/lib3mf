#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const & consumer, eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, CIPHERVALUE const & cipherValue) {
		m_pConsumer= consumer;
		//TODO: this is different from the sample file, what should we do?
		//if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::Aes256Gcm) {
		//	throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		//}
		m_EncryptionAlgorithm = encryptionAlgorithm;
		m_sCipherValue = cipherValue;
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
	eKeyStoreEncryptAlgorithm CKeyStoreDecryptRight::getEncryptionAlgorithm()
	{
		return m_EncryptionAlgorithm;
	}
	CIPHERVALUE CKeyStoreDecryptRight::getCipherValue() const {
		return m_sCipherValue;
	}
	void CKeyStoreDecryptRight::setCipherValue(CIPHERVALUE const & cipherValue)
	{
		m_sCipherValue = cipherValue;
	}
}