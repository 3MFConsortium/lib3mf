/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

UnitTest_EncryptionUtils.h: Encryption Utilities for UnitTests

--*/

#ifndef __NMR_UNITTEST_ENCRYPTIONUTILS
#define __NMR_UNITTEST_ENCRYPTIONUTILS
#include "lib3mf_implicit.hpp"

#include <map>
#include <openssl/evp.h>
#include <openssl/pem.h>



using Byte = Lib3MF_uint8;
using ByteVector = std::vector<Byte>;

using PEVP_ENCODE_CTX = std::unique_ptr<EVP_ENCODE_CTX, decltype(&::EVP_ENCODE_CTX_free)>;
using PBIO = std::unique_ptr<BIO, decltype(&::BIO_free)>;
using PEVP_CIPHER_CTX = std::shared_ptr<EVP_CIPHER_CTX>;
using PEVP_PKEY = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;

struct KekContext {
	EVP_PKEY * key;
	size_t size;
};

struct DekContext {
	std::map<Lib3MF_uint64, PEVP_CIPHER_CTX> m_Context;
	Lib3MF::CWrapper * wrapper;
};

inline std::shared_ptr<EVP_CIPHER_CTX> make_shared(EVP_CIPHER_CTX * ctx) {
	return std::shared_ptr<EVP_CIPHER_CTX>(ctx, ::EVP_CIPHER_CTX_free);
}

namespace AesMethods {

	namespace Decrypt {
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv);

		size_t decrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * cipher, Lib3MF_uint8 * plain);

		bool finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * plain, Lib3MF_uint8 * tag);
	}
	namespace Encrypt {
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv);

		size_t encrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * plain, Lib3MF_uint8 * cipher);

		bool finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * cipher, Lib3MF_uint32 tagSize, Lib3MF_uint8 * tag);
	}
}

namespace RsaMethods {
	PEVP_PKEY loadPrivateKey(ByteVector const & privKey);

	PEVP_PKEY loadPublicKey(ByteVector const & pubKey);

	size_t getSize(PEVP_PKEY const & evpKey);

	size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain);

	size_t encrypt(EVP_PKEY * evpKey, size_t plainSize, uint8_t const * plain, uint8_t * cipher);
}

struct EncryptionCallbacks {

	static void dataEncryptClientCallback(
		Lib3MF::eEncryptionAlgorithm algorithm,
		Lib3MF_CipherData cipherData,
		Lib3MF_uint64 plainSize,
		const Lib3MF_uint8 * plainBuffer,
		const Lib3MF_uint64 cipherSize,
		Lib3MF_uint64 * cipherNeeded,
		Lib3MF_uint8 * cipherBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * result);

	static void keyEncryptClientCallback(
		Lib3MF_Consumer consumer,
		Lib3MF::eEncryptionAlgorithm algorithm,
		Lib3MF_uint64 plainSize,
		const Lib3MF_uint8 * plainBuffer,
		const Lib3MF_uint64 cipherSize,
		Lib3MF_uint64* cipherNeeded,
		Lib3MF_uint8 * cipherBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * result);

	static void dataDecryptClientCallback(
		Lib3MF::eEncryptionAlgorithm algorithm,
		Lib3MF_CipherData cipherData,
		Lib3MF_uint64 cipherSize,
		const Lib3MF_uint8 * cipherBuffer,
		const Lib3MF_uint64 plainSize,
		Lib3MF_uint64 * plainNeeded,
		Lib3MF_uint8 * plainBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * result);

	static void keyDecryptClientCallback(
		Lib3MF_Consumer consumer,
		Lib3MF::eEncryptionAlgorithm algorithm,
		Lib3MF_uint64 cipherSize,
		const Lib3MF_uint8 * cipherBuffer,
		const Lib3MF_uint64 plainSize,
		Lib3MF_uint64* plainNeeded,
		Lib3MF_uint8 * plainBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * result);
};
#endif