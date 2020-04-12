#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const & consumer, eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, CIPHERVALUE const & cipherValue) 
		:m_pConsumer(consumer), m_EncryptionAlgorithm(encryptionAlgorithm), m_sCipherValue(cipherValue), m_bNew(false)
	{
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	CKeyStoreDecryptRight::CKeyStoreDecryptRight(PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptionAlgorithm) 
		:m_pConsumer(consumer), m_EncryptionAlgorithm(encryptionAlgorithm), m_bNew(true)
	{
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
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
	void CKeyStoreDecryptRight::open(CIPHERVALUE const & cipherValue)
	{
		m_sCipherValue = cipherValue;
	}
	bool CKeyStoreDecryptRight::isNew() {
		return m_bNew;
	}
}