#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const & consumer, eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, AES256GCMCIPHERVALUE const & cipherValue) {
		m_pConsumer= consumer;
		//TODO: this is different from the sample file, what should we do?
		//if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::Aes256Gcm) {
		//	throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		//}
		m_EncryptionAlgorithm = encryptionAlgorithm;
		m_cipherValue = cipherValue;
	}
	PKeyStoreConsumer CKeyStoreDecryptRight::getConsumer()
	{
		return m_pConsumer;
	}
}