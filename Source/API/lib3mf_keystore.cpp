#include "lib3mf_keystore.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_keyvalue.hpp"

using namespace Lib3MF::Impl;

IKeyValue * Lib3MF::Impl::CKeyStore::CreateKeyValue() {
	return new CKeyValue();
}

IConsumer * Lib3MF::Impl::CKeyStore::AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, IKeyValue * pKeyValue)
{
	//TODO SecureContent: add a consumer to the implementation array
	//	Create a consumer
	//	Add consumer to implemenation array
	//	Add consumer reference to map<string, consumer> consumerReference
	return nullptr;
}

Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetConsumerCount()
{
	//TODO Secure Content: return size of implementation array
	return Lib3MF_uint32();
}

IConsumer * Lib3MF::Impl::CKeyStore::GetConsumer(const Lib3MF_uint64 nConsumerIndex)
{
	//TODO Secure Content: return element from the implementation array
	return nullptr;
}

IConsumer * Lib3MF::Impl::CKeyStore::FindConsumer(const std::string & sConsumerID)
{
	//TODO Secure Content: find consumer on consumerReference using sConsumerID
	return nullptr;
}

void Lib3MF::Impl::CKeyStore::RemoveConsumer(IConsumer * pConsumerInstance) 
{
	//TODO Secure Content: find this consumer and remove it from array of consumers
}


Lib3MF_uint32 Lib3MF::Impl::CKeyStore::GetResourceDataCount()
{
	//TODO Secure Content: return size of elements in the resourcedata
	return Lib3MF_uint32();
}

IResourceData * Lib3MF::Impl::CKeyStore::GetResourceData(const Lib3MF_uint64 nResourceDataIndex)
{
	//TODO Secure Content: return a resource data from inside resource data array
	return nullptr;
}

IResourceData * Lib3MF::Impl::CKeyStore::AddResourceData(IPackagePath * pPackagePath, const Lib3MF::eEncryptionAlgorithm eEncryptionAlgorithm, const Lib3MF::eCompression eCompression)
{
	//TODO Secure Content: 
	//	create resource data, 
	//	add to resourcedata array
	//	add reference to resourcedataReference map<Path, resourceData>
	return nullptr;
}

IResourceData * Lib3MF::Impl::CKeyStore::FindResourceData(const std::string & sPath)
{
	//TODO Secure Content: find resourcedata for path sPath insid resourceDataReference
	return nullptr;
}