#include "lib3mf_keyvalue.hpp"
#include "lib3mf_interfaceexception.hpp"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CKeyValue::CKeyValue() {
}

Lib3MF::sRSAKeyValue Lib3MF::Impl::CKeyValue::ReadRSAFromBuffer(const Lib3MF_uint64 nPubKeyBufferSize, const Lib3MF_uint8 *pPubKeyBuffer) {
	return Lib3MF::sRSAKeyValue();
}

Lib3MF::sRSAKeyValue Lib3MF::Impl::CKeyValue::GetRSA() {
	return m_KeyValue;
}

void Lib3MF::Impl::CKeyValue::SetRSA(const Lib3MF::sRSAKeyValue TheKeyValue) {
	m_KeyValue = TheKeyValue;
}
