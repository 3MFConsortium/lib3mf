#include "lib3mf_consumer.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "Model/Classes/NMR_KeyStoreConsumer.h"


using namespace Lib3MF::Impl;

Lib3MF::Impl::CConsumer::CConsumer(NMR::PKeyStoreConsumer const & consumer) {
	m_Consumer = consumer;
}

std::string Lib3MF::Impl::CConsumer::GetConsumerID() {
	return m_Consumer->getConsumerID();
}

std::string Lib3MF::Impl::CConsumer::GetKeyID() {
	return m_Consumer->getKeyID();
}

std::string Lib3MF::Impl::CConsumer::GetKeyValue() {
	return m_Consumer->getKeyValue();
}
