#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreAccessRight::CKeyStoreAccessRight(PKeyStoreConsumer const & consumer, eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, CIPHERVALUE const & cipherValue)
		:m_pConsumer(consumer), m_EncryptionAlgorithm(encryptionAlgorithm), m_sCipherValue(cipherValue), m_bNew(false)
	{
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	CKeyStoreAccessRight::CKeyStoreAccessRight(PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptionAlgorithm)
		:m_pConsumer(consumer), m_EncryptionAlgorithm(encryptionAlgorithm), m_bNew(true)
	{
		if (encryptionAlgorithm != eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
		}
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	PKeyStoreConsumer CKeyStoreAccessRight::getConsumer()
	{
		return m_pConsumer;
	}
	eKeyStoreEncryptAlgorithm CKeyStoreAccessRight::getEncryptionAlgorithm()
	{
		return m_EncryptionAlgorithm;
	}
	CIPHERVALUE CKeyStoreAccessRight::getCipherValue() const {
		return m_sCipherValue;
	}
	void CKeyStoreAccessRight::setCipherValue(CIPHERVALUE const & cipherValue)
	{
		m_sCipherValue = cipherValue;
	}
	bool CKeyStoreAccessRight::isNew() {
		return m_bNew;
	}
}