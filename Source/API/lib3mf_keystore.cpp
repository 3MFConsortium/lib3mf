#include "lib3mf_types.hpp"
#include "lib3mf_keystore.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_resourcedatagroup.hpp"
#include "lib3mf_resourcedata.hpp"
#include "lib3mf_packagepart.hpp"
#include "lib3mf_utils.hpp"

#include "Model/Classes/NMR_KeyStoreFactory.h"

using namespace Lib3MF::Impl;

Lib3MF::Impl::CKeyStore::CKeyStore(NMR::PKeyStore pKeyStore) 
	:m_pKeyStore(pKeyStore)
{
	if (nullptr == pKeyStore)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDKEYSTORE);
}

IConsumer * Lib3MF::Impl::CKeyStore::AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, const std::string & sKeyValue)
{
	NMR::PKeyStoreConsumer consumer = NMR::CKeyStoreFactory::makeConsumer(sConsumerID);
	consumer->setKeyID(sKeyID);
	consumer->setKeyValue(sKeyValue);
	m_pKeyStore->addConsumer(consumer);
	return new CConsumer(consumer);
}

Lib3MF_uint64 Lib3MF::Impl::CKeyStore::GetConsumerCount()
{
	return m_pKeyStore->getConsumerCount();
}

IConsumer * Lib3MF::Impl::CKeyStore::GetConsumer(const Lib3MF_uint64 nConsumerIndex)
{
	NMR::PKeyStoreConsumer consumer = m_pKeyStore->getConsumer(nConsumerIndex);
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

Lib3MF_uint64 Lib3MF::Impl::CKeyStore::GetResourceDataGroupCount()
{
	return (Lib3MF_uint32)m_pKeyStore->getResourceDataGroupCount();
}

IResourceDataGroup * Lib3MF::Impl::CKeyStore::GetResourceDataGroup(const Lib3MF_uint64 nResourceDataIndex)
{
	NMR::PKeyStoreResourceDataGroup dg = m_pKeyStore->getResourceDataGroup(nResourceDataIndex);
	return new CResourceDataGroup(dg);
}

IResourceDataGroup * Lib3MF::Impl::CKeyStore::AddResourceDataGroup() {
	NMR::PKeyStoreResourceDataGroup dg = NMR::CKeyStoreFactory::makeResourceDataGroup(std::make_shared<NMR::CUUID>());
	m_pKeyStore->addResourceDataGroup(dg);
	return new CResourceDataGroup(dg);
}

void Lib3MF::Impl::CKeyStore::RemoveResourceDataGroup(IResourceDataGroup * pTheResourceDataGroup) {}

Lib3MF::Impl::IResourceDataGroup * Lib3MF::Impl::CKeyStore::FindResourceDataGroup(Lib3MF::Impl::IPackagePart *pPartPath) {
	NMR::PKeyStoreResourceDataGroup dg = m_pKeyStore->findResourceDataGroupByResourceDataPath(pPartPath->Get());
	if (!dg)
		return nullptr;
	return new CResourceDataGroup(dg);
}

Lib3MF::Impl::IResourceData * Lib3MF::Impl::CKeyStore::AddResourceData(Lib3MF::Impl::IResourceDataGroup *pResourceDataGroup, Lib3MF::Impl::IPackagePart *pPartPath, Lib3MF::eEncryptionAlgorithm eAlgorithm, Lib3MF::eCompression eCompression, Lib3MF_uint64 nAdditionalAuthenticationDataBufferSize, const Lib3MF_uint8 *pAdditionalAuthenticationDataBuffer) {
	bool compression = translateCompression(eCompression);
	NMR::eKeyStoreEncryptAlgorithm algorithm = translateEncryptionAlgorithm(eAlgorithm);
	CResourceDataGroup * dg = dynamic_cast<CResourceDataGroup *>(pResourceDataGroup);
	if (!dg)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	std::vector<NMR::nfByte> aad(pAdditionalAuthenticationDataBuffer, pAdditionalAuthenticationDataBuffer + nAdditionalAuthenticationDataBufferSize);
	NMR::PKeyStoreResourceData rd = NMR::CKeyStoreFactory::makeResourceData(pPartPath->Get(), algorithm, compression, aad);
	m_pKeyStore->addResourceData(dg->resourceDataGroup(), rd);
	return new CResourceData(rd);
}

void Lib3MF::Impl::CKeyStore::RemoveResourceData(Lib3MF::Impl::IResourceData *pResourceData) {
	CResourceData * rd = dynamic_cast<CResourceData *>(pResourceData);
	if (!rd)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	m_pKeyStore->removeResourceData(rd->resourceData());
}

Lib3MF_uint64 Lib3MF::Impl::CKeyStore::GetResourceDataCount() {
	return m_pKeyStore->getResourceDataCount();
}

Lib3MF::Impl::IResourceData * Lib3MF::Impl::CKeyStore::GetResourceData(Lib3MF_uint64 nResourceDataIndex) {
	NMR::PKeyStoreResourceData rd = m_pKeyStore->getResourceData(nResourceDataIndex);
	return new CResourceData(rd);
}

IResourceData * Lib3MF::Impl::CKeyStore::FindResourceData(IPackagePart * pResourcePath) {
	NMR::PKeyStoreResourceData rd = m_pKeyStore->findResourceData(pResourcePath->Get());
	if (!rd)
		return nullptr;
	return new CResourceData(rd);
}
