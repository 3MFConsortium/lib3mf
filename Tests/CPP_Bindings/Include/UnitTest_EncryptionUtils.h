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
using PRSA = std::unique_ptr<RSA, decltype(&::RSA_free)>;

struct KekContext {
	EVP_PKEY * key;
	size_t size;
	Lib3MF::CWrapper * wrapper;
};

struct DekContext {
	std::map<Lib3MF_uint64, PEVP_CIPHER_CTX> ciphers;
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

	size_t getSize(EVP_PKEY * evpKey);

	size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain);

	size_t encrypt(EVP_PKEY * evpKey, size_t plainSize, uint8_t const * plain, uint8_t * cipher);
}

struct EncryptionCallbacks {

	static void cleanup();

	/**
	* ContentEncryptionCallback - A callback to encrypt/decrypt content called on each resource encrypted. This might be called several times depending on content size. If cipher size is zero, clients must return the result of authenticated tag validation
	*
	* @param[in] pCEKParams - The params of the encryption process.
	* @param[in] nInputBufferSize - Number of elements in buffer
	* @param[in] pInputBuffer - uint8 buffer of Buffer to the original data. In encrypting, this will be the plain data. If derypting, this will be the cipher data
	* @param[in] nOutputBufferSize - Number of elements in buffer
	* @param[out] pOutputNeededCount - will be filled with the count of the written elements, or needed buffer size.
	* @param[out] pOutputBuffer - uint8  buffer of Buffer to hold the transformed data. When encrypting, this will be the cipher data. When decrypting, this shall be the plain data. If buffer is null, neededBytes return the necessary amount of bytes. Otherwise, amount of bytes decrypted if succeed or zero on error
	* @param[in] pUserData - Userdata that is passed to the callback function
	*/

	static void dataEncryptClientCallback(
		Lib3MF_ContentEncryptionParams params,
		Lib3MF_uint64 plainSize,
		const Lib3MF_uint8 * plainBuffer,
		const Lib3MF_uint64 cipherSize,
		Lib3MF_uint64 * cipherNeeded,
		Lib3MF_uint8 * cipherBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status);


	/**
	* KeyWrappingCallback - A callback used to wrap (encrypt) the content key available in keystore resource group
	*
	* @param[in] accessRight - The information about the parameters used used to wrap the key to the contents
	* @param[in] plainSize - Number of elements in buffer
	* @param[in] plainBuffer - uint8 buffer of Buffer to the input value. When encrypting, this should be the plain key. When decrypting, this should be the key cipher.
	* @param[in] cipherSize - Number of elements in buffer
	* @param[out] cipherNeeded - will be filled with the count of the written elements, or needed buffer size.
	* @param[out] cipherBuffer - uint8  buffer of Buffer where the data will be placed. When encrypting, this will be the key cipher. When decrypting, this will be the plain key. When buffer is null, neededBytes contains the required bytes to run. When not, Amount it reflects the encrypted/decrypted bytes when succeed or zero when error
	* @param[in] userData - Userdata that is passed to the callback function
	*/
	static void keyEncryptClientCallback(
		Lib3MF_AccessRight accessRight,
		Lib3MF_uint64 plainSize,
		const Lib3MF_uint8 * plainBuffer,
		const Lib3MF_uint64 cipherSize,
		Lib3MF_uint64* cipherNeeded,
		Lib3MF_uint8 * cipherBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status);

	static void dataDecryptClientCallback(
		Lib3MF_ContentEncryptionParams params,
		Lib3MF_uint64 cipherSize,
		const Lib3MF_uint8 * cipherBuffer,
		const Lib3MF_uint64 plainSize,
		Lib3MF_uint64 * plainNeeded,
		Lib3MF_uint8 * plainBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status);

	static void keyDecryptClientCallback(
		Lib3MF_AccessRight accessRight,
		Lib3MF_uint64 cipherSize,
		const Lib3MF_uint8 * cipherBuffer,
		const Lib3MF_uint64 plainSize,
		Lib3MF_uint64* plainNeeded,
		Lib3MF_uint8 * plainBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status);

	/**
	* RandomNumberCallback - Callback to generate random numbers
	*
	* @param[in] nByteData - Point to a buffer where to store the data
	* @param[in] nNumBytes - Size of available bytes in the buffer
	* @param[in] pUserData - Userdata that is passed to the callback function
	* @param[out] pBytesWritten - Number of bytes generated when succeed. 0 or less if failed.
	*/
	static void randomNumberCallback(
		Lib3MF_uint64 nByteData, 
		Lib3MF_uint64 nNumBytes, 
		Lib3MF_pvoid pUserData, 
		Lib3MF_uint64 * pBytesWritten);
};
#endif