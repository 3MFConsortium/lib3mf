#include "lib3mf_accessright.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CAccessRight::CAccessRight(NMR::PKeyStoreAccessRight ar) {
	m_pAccessRight = ar;
}

IConsumer * Lib3MF::Impl::CAccessRight::GetConsumer() {
	return new CConsumer(m_pAccessRight->getConsumer());
}

Lib3MF::eWrappingAlgorithm Lib3MF::Impl::CAccessRight::GetWrappingAlgorithm() {
	NMR::eKeyStoreWrapAlgorithm ea = m_pAccessRight->getKEKParams().m_eAlgorithm;
	return static_cast<Lib3MF::eWrappingAlgorithm>(ea);
}

Lib3MF::eMgfAlgorithm Lib3MF::Impl::CAccessRight::GetMgfAlgorithm() {
	NMR::eKeyStoreMaskGenerationFunction mgf = m_pAccessRight->getKEKParams().m_eMask;
	return static_cast<Lib3MF::eMgfAlgorithm>(mgf);
}

Lib3MF::eDigestMethod Lib3MF::Impl::CAccessRight::GetDigestMethod() {
	NMR::eKeyStoreMessageDigest digest = m_pAccessRight->getKEKParams().m_eDigest;
	return static_cast<Lib3MF::eDigestMethod>(digest);
}

NMR::PKeyStoreAccessRight Lib3MF::Impl::CAccessRight::accessRight() const {
	return m_pAccessRight;
}