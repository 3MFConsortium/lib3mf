#include "lib3mf_decryptright.hpp"
#include "lib3mf_interfaceexception.hpp"

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
	return nullptr;
}

Lib3MF::eEncryptionAlgorithm Lib3MF::Impl::CDecryptRight::GetEncryptionAlgorithm() {
	return Lib3MF::eEncryptionAlgorithm();
}
