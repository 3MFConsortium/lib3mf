#include "lib3mf_cipherdata.hpp"
#include "lib3mf_interfaceexception.hpp"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CCipherData::CCipherData(Lib3MF::sAes256CipherData const & CipherValue) {
	m_CipherValue = CipherValue;
}

Lib3MF::sAes256CipherData Lib3MF::Impl::CCipherData::GetAes256Gcm() {
	return m_CipherValue;
}

void Lib3MF::Impl::CCipherData::SetAes256Gcm(const Lib3MF::sAes256CipherData CipherValue) {
	m_CipherValue = CipherValue;
}
