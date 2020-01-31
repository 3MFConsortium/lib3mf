#include "lib3mf_cipherdata.hpp"
#include "lib3mf_interfaceexception.hpp"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CCipherData::CCipherData(Lib3MF::sAes256CipherData const & CipherValue, Lib3MF_uint64 handle) {
	m_CipherValue = CipherValue;
	m_handle = handle;
}

Lib3MF::sAes256CipherData Lib3MF::Impl::CCipherData::GetAes256Gcm() {
	return m_CipherValue;
}

void Lib3MF::Impl::CCipherData::SetAes256Gcm(const Lib3MF::sAes256CipherData CipherValue) {
	m_CipherValue = CipherValue;
}

Lib3MF_uint64 Lib3MF::Impl::CCipherData::GetDescriptor() {
	return m_handle;
}
