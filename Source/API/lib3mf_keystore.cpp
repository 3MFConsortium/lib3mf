#include "lib3mf_keystore.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_keyvalue.hpp"
#include "lib3mf_resourcedata.hpp"
#include "lib3mf_packagepath.hpp"

using namespace Lib3MF::Impl;

IKeyValue * Lib3MF::Impl::CKeyStore::CreateKeyValue() {
	return new CKeyValue();
}

IConsumer * Lib3MF::Impl::CKeyStore::AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, IKeyValue * pKeyValue)
{
	NMR::RSAKEYVALUE keyValue;
	Lib3MF::sRSAKeyValue sKeyValue = pKeyValue->GetRSA();
	std::copy(sKeyValue.m_Modulus, sKeyValue.m_Modulus + sizeof(sKeyValue.m_Modulus), keyValue.m_modulus);
	
	NMR::PKeyStoreConsumer consumer = m_KeyStore->addConsumer(sConsumerID, sKeyID, keyValue);
	return new CConsumer(consumer);
}

Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetConsumerCount()
{
	return m_KeyStore->getConsumerCount();
}

IConsumer * Lib3MF::Impl::CKeyStore::GetConsumer(const Lib3MF_uint64 nConsumerIndex)
{
	NMR::PKeyStoreConsumer consumer = m_KeyStore->getConsumerByIndex(nConsumerIndex);
	return new CConsumer(consumer);
}

IConsumer * Lib3MF::Impl::CKeyStore::FindConsumer(const std::string & sConsumerID)
{
	NMR::PKeyStoreConsumer consumer = m_KeyStore->findConsumerById(sConsumerID);
	return new CConsumer(consumer);
}

void Lib3MF::Impl::CKeyStore::RemoveConsumer(IConsumer * pConsumerInstance) 
{
	CConsumer * consumer = reinterpret_cast<CConsumer *>(pConsumerInstance);
	m_KeyStore->removeConsumer(consumer->consumer());
}


Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetResourceDataCount()
{
	return m_KeyStore->resourceDataCount();;
}

IResourceData * Lib3MF::Impl::CKeyStore::GetResourceData(const Lib3MF_uint64 nResourceDataIndex)
{
	NMR::PKeyStoreResourceData rd = m_KeyStore->getResourceDataByIndex(nResourceDataIndex);
	return new CResourceData(rd);
}

IResourceData * Lib3MF::Impl::CKeyStore::AddResourceData(IPackagePath * pPackagePath, const Lib3MF::eEncryptionAlgorithm eEncryptionAlgorithm, const Lib3MF::eCompression eCompression)
{
	if (eEncryptionAlgorithm == Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
	}

	bool compression = (eCompression != Lib3MF::eCompression::None);

	CPackagePath * packagePath = reinterpret_cast<CPackagePath *>(pPackagePath);
	NMR::PKeyStoreResourceData rd = m_KeyStore->addResourceData(packagePath->Get(), NMR::eKeyStoreEncryptAlgorithm::Aes256Gcm, compression);
	return new CResourceData(rd);
}

IResourceData * Lib3MF::Impl::CKeyStore::FindResourceData(const std::string & sPath)
{
	NMR::PKeyStoreResourceData rd = m_KeyStore->findResourceDataByPath(sPath);
	return new CResourceData(rd);
}