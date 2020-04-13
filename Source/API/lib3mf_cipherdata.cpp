#include "lib3mf_cipherdata.hpp"
#include "lib3mf_interfaceexception.hpp"

using namespace Lib3MF::Impl;


Lib3MF_uint64 Lib3MF::Impl::CContentEncryptionParams::GetDescriptor() {
	return 0;
}

Lib3MF::eEncryptionAlgorithm Lib3MF::Impl::CContentEncryptionParams::GetEncryptionAlgorithm() {
	return Lib3MF::eEncryptionAlgorithm();
}

void Lib3MF::Impl::CContentEncryptionParams::GetKey(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {}

void Lib3MF::Impl::CContentEncryptionParams::GetInitializationVector(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {}

void Lib3MF::Impl::CContentEncryptionParams::GetAuthenticationTag(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {}

void Lib3MF::Impl::CContentEncryptionParams::SetAuthenticationTag(Lib3MF_uint64 nByteDataBufferSize, const Lib3MF_uint8 *pByteDataBuffer) {}

void Lib3MF::Impl::CContentEncryptionParams::GetAdditionalAuthenticationData(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {}

void Lib3MF::Impl::CContentEncryptionParams::SetAdditionalAuthenticationData(Lib3MF_uint64 nByteDataBufferSize, const Lib3MF_uint8 *pByteDataBuffer) {}

std::string Lib3MF::Impl::CContentEncryptionParams::GetKeyUUID() {
	return std::string();
}
