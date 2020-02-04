#include "lib3mf_decryptright.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"

using namespace Lib3MF::Impl;

NMR::PKeyStoreDecryptRight Lib3MF::Impl::CDecryptRight::decryptRight() const
{
	return m_DecryptRight;
}

Lib3MF::Impl::CDecryptRight::CDecryptRight(NMR::PKeyStoreDecryptRight dR)
{
	m_DecryptRight = dR;
}

IConsumer * Lib3MF::Impl::CDecryptRight::GetConsumer() {
	return new CConsumer(m_DecryptRight->getConsumer());
}

Lib3MF::eEncryptionAlgorithm Lib3MF::Impl::CDecryptRight::GetEncryptionAlgorithm() {
	NMR::eKeyStoreEncryptAlgorithm ea = m_DecryptRight->getEncryptionAlgorithm();
	if (ea == NMR::eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
		return Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p;
	}
	return Lib3MF::eEncryptionAlgorithm::Aes256Gcm;
}
