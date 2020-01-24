#include "lib3mf_consumer.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_keyvalue.hpp"
#include "Model/Classes/NMR_KeyStoreConsumer.h"


using namespace Lib3MF::Impl;

NMR::PKeyStoreConsumer Lib3MF::Impl::CConsumer::consumer() const
{
	return m_Consumer;
}

Lib3MF::Impl::CConsumer::CConsumer(NMR::PKeyStoreConsumer const & consumer) {
	m_Consumer = consumer;
}

std::string Lib3MF::Impl::CConsumer::GetConsumerID() {
	return m_Consumer->getConsumerID();
}

std::string Lib3MF::Impl::CConsumer::GetKeyID() {
	return m_Consumer->getKeyID();
}

IKeyValue * Lib3MF::Impl::CConsumer::GetKeyValue() {
	Lib3MF::sRSAKeyValue theKeyValue;
	NMR::RSAKEYVALUE value = m_Consumer->getKeyValue();
	std::copy(value.m_modulus, value.m_modulus + sizeof(value.m_modulus), theKeyValue.m_Modulus);
	std::copy(value.m_exponent, value.m_exponent+ sizeof(value.m_exponent), theKeyValue.m_Exponent);
	CKeyValue * kv = new CKeyValue();
	kv->SetRSA(theKeyValue);
	return kv;
}
