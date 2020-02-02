#include "lib3mf_types.hpp"
#include "lib3mf_keystore.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_resourcedata.hpp"
#include "lib3mf_packagepath.hpp"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CKeyStore::CKeyStore(NMR::PKeyStore pKeyStore) 
	:m_pKeyStore(pKeyStore)
{
	if (nullptr == pKeyStore)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDKEYSTORE);
}

IConsumer * Lib3MF::Impl::CKeyStore::AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, const std::string & sKeyValue)
{
	NMR::PKeyStoreConsumer consumer = m_pKeyStore->addConsumer(sConsumerID, sKeyID, sKeyValue);
	return new CConsumer(consumer);
}

Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetConsumerCount()
{
	return m_pKeyStore->getConsumerCount();
}

IConsumer * Lib3MF::Impl::CKeyStore::GetConsumer(const Lib3MF_uint64 nConsumerIndex)
{
	NMR::PKeyStoreConsumer consumer = m_pKeyStore->getConsumerByIndex(nConsumerIndex);
	return new CConsumer(consumer);
}

IConsumer * Lib3MF::Impl::CKeyStore::FindConsumer(const std::string & sConsumerID)
{
	NMR::PKeyStoreConsumer consumer = m_pKeyStore->findConsumerById(sConsumerID);
	if (!consumer)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_KEYSTORECONSUMERNOTFOUND);
	return new CConsumer(consumer);
}

void Lib3MF::Impl::CKeyStore::RemoveConsumer(IConsumer * pConsumerInstance) 
{
	CConsumer * pConsumer = dynamic_cast<CConsumer *>(pConsumerInstance);
	if (!pConsumer) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDKEYSTORECONSUMER);
	}
	m_pKeyStore->removeConsumer(pConsumer->consumer());
}


Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetResourceDataCount()
{
	return m_pKeyStore->getResourceDataCount();;
}

IResourceData * Lib3MF::Impl::CKeyStore::GetResourceData(const Lib3MF_uint64 nResourceDataIndex)
{
	NMR::PKeyStoreResourceData rd = m_pKeyStore->getResourceDataByIndex(nResourceDataIndex);
	return new CResourceData(rd);
}

IResourceData * Lib3MF::Impl::CKeyStore::AddResourceData(IPackagePath * pPackagePath, const Lib3MF::eEncryptionAlgorithm eEncryptionAlgorithm, const Lib3MF::eCompression eCompression)
{
	if (eEncryptionAlgorithm == Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p) {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
	}

	bool compression = (eCompression != Lib3MF::eCompression::None);

	NMR::PKeyStoreResourceData rd = m_pKeyStore->addResourceData(pPackagePath->Get(), NMR::eKeyStoreEncryptAlgorithm::Aes256Gcm, compression);
	return new CResourceData(rd);
}

IResourceData * Lib3MF::Impl::CKeyStore::FindResourceData(const std::string & sPath)
{
	NMR::PKeyStoreResourceData rd = m_pKeyStore->findResourceDataByPath(sPath);
	if (!rd)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_KEYSTORERESOURCEDATANOTFOUND);
	return new CResourceData(rd);
}

std::string Lib3MF::Impl::CKeyStore::GetUUID(bool & bHasUUID) {
	bHasUUID = m_pKeyStore->getUUID() != nullptr;
	if (bHasUUID)
		return m_pKeyStore->getUUID()->toString();
	return "";
}

void Lib3MF::Impl::CKeyStore::SetUUID(const std::string & sUUID)
{
	NMR::PUUID pUUID = std::make_shared<NMR::CUUID>(sUUID);
	m_pKeyStore->setUUID(pUUID);
}