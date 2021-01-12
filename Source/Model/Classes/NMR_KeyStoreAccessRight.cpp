#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreAccessRight::CKeyStoreAccessRight(
		PKeyStoreConsumer const & consumer, 
		eKeyStoreWrapAlgorithm const algorithm, 
		eKeyStoreMaskGenerationFunction const mgf, 
		eKeyStoreMessageDigest const digest, 
		std::vector<nfByte> const & cipherValue)
		:m_pConsumer(consumer), m_eAlgorithm(algorithm), m_eMgf(mgf), m_eDigest(digest), m_rgCipherValue(cipherValue)
	{
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (algorithm != eKeyStoreWrapAlgorithm::RSA_OAEP) {
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
		}

		if (mgf != eKeyStoreMaskGenerationFunction::MGF1_SHA1
			&& mgf != eKeyStoreMaskGenerationFunction::MGF1_SHA256) {
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
		}

		if (digest != eKeyStoreMessageDigest::SHA1
			&& digest != eKeyStoreMessageDigest::SHA256)
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
	}
	PKeyStoreConsumer CKeyStoreAccessRight::getConsumer() const {
		return m_pConsumer;
	}
	eKeyStoreWrapAlgorithm CKeyStoreAccessRight::getAlgorithm() const {
		return m_eAlgorithm;
	}
	eKeyStoreMaskGenerationFunction CKeyStoreAccessRight::getMgf() const {
		return m_eMgf;
	}
	eKeyStoreMessageDigest CKeyStoreAccessRight::getDigest() const {
		return m_eDigest;
	}
	std::vector<nfByte> const & CKeyStoreAccessRight::getCipherValue() const {
		return m_rgCipherValue;
	}
	void CKeyStoreAccessRight::setCipherValue(std::vector<nfByte> const & cv) {
		m_rgCipherValue = cv;
	}
	nfBool CKeyStoreAccessRight::isNew() const {
		return m_rgCipherValue.empty();
	}
}