#include "UnitTest_EncryptionUtils.h"

#include "gtest/gtest.h"

#include <openssl/rand.h>

#define AES256GCM_KEYSIZE 32
#define AES256GCM_IVSIZE 12
#define AES256GCM_TAGSIZE 16
#define RSAOEAP_KEYSIZE 256


namespace AesMethods {

	namespace Decrypt {
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv, Lib3MF_uint64 aadSize, Lib3MF_uint8 const * aad) {
			PEVP_CIPHER_CTX ctx = make_shared(EVP_CIPHER_CTX_new());
			if (!ctx)
				throw std::runtime_error("unable to initialize context");

			if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr))
				throw std::runtime_error("unable to initialize cipher");

			if (!EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, key, iv))
				throw std::runtime_error("unable to initialize key and iv");

			if (aadSize > 0) {
				int len;
				if (!EVP_DecryptUpdate(ctx.get(), nullptr, &len, aad, (int)aadSize))
					throw std::runtime_error("aad failed");
			}
			return ctx;
		}

		size_t decrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * cipher, Lib3MF_uint8 * plain) {
			int len = 0;
			if (!EVP_DecryptUpdate(ctx.get(), plain, &len, cipher, size))
				throw std::runtime_error("unable to decrypt");
			return len;
		}

		bool finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * plain, Lib3MF_uint8 * tag) {
			if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, 16, tag))
				throw std::runtime_error("unable to set tag");
			int len = 0;
			int ret = EVP_DecryptFinal_ex(ctx.get(), plain, &len);
			return ret > 0;
		}
	}
	namespace Encrypt {
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv, Lib3MF_uint64 aadSize, Lib3MF_uint8 const * aad) {
			PEVP_CIPHER_CTX ctx = make_shared(EVP_CIPHER_CTX_new());
			if (!ctx)
				throw std::runtime_error("unable to intialize cipher context");

			if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr))
				throw std::runtime_error("unable to initialize encryption mechanism");

			if (1 != EVP_EncryptInit_ex(ctx.get(), nullptr, nullptr, key, iv))
				throw std::runtime_error("unable to init key and iv");
			if (aadSize > 0) {
				int len;
				if (1 != EVP_EncryptUpdate(ctx.get(), nullptr, &len, aad, (int)aadSize))
					throw std::runtime_error("aad failed");
			}
			return ctx;
		}

		size_t encrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * plain, Lib3MF_uint8 * cipher) {
			int len = 0;
			if (1 != EVP_EncryptUpdate(ctx.get(), cipher, &len, plain, size))
				throw std::runtime_error("unable to encrypt message");
			return len;
		}

		bool finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * cipher, Lib3MF_uint32 tagSize, Lib3MF_uint8 * tag) {
			int len = 0;
			if (1 != EVP_EncryptFinal_ex(ctx.get(), cipher, &len))
				throw std::runtime_error("unable to finalize encryption");

			if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, tagSize, tag))
				throw std::runtime_error("unable to get tag");
			return true;
		}
	}
}

namespace RsaMethods {
	PEVP_PKEY loadPrivateKey(ByteVector const & privKey) {
		PBIO keybio(BIO_new(BIO_s_mem()), ::BIO_free);
		BIO_write(keybio.get(), privKey.data(), (int)privKey.size());
		EVP_PKEY * evpKey = 0;
		PEM_read_bio_PrivateKey(keybio.get(), &evpKey, NULL, NULL);
		PEVP_PKEY pevpKey(evpKey, ::EVP_PKEY_free);
		return pevpKey;
	}

	PEVP_PKEY loadPublicKey(ByteVector const & pubKey) {
		PBIO keybio(BIO_new(BIO_s_mem()), ::BIO_free);
		BIO_write(keybio.get(), pubKey.data(), (int)pubKey.size());
		EVP_PKEY * evpKey = 0;
		PEM_read_bio_PUBKEY(keybio.get(), &evpKey, NULL, NULL);
		PEVP_PKEY pevpKey(evpKey, ::EVP_PKEY_free);
		return pevpKey;
	}

	size_t getSize(EVP_PKEY * evpKey) {
		PRSA rsa(EVP_PKEY_get1_RSA(evpKey), ::RSA_free);
		size_t rsaSize = RSA_size(rsa.get());
		return rsaSize;
	}

	size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain) {
		PRSA rsa(EVP_PKEY_get1_RSA(evpKey), ::RSA_free);
		int result = RSA_private_decrypt((int)cipherSize, cipher, plain, rsa.get(), RSA_PKCS1_OAEP_PADDING);
		if (result < 0)
			throw std::runtime_error("unable to decrypt");
		return result;
	}

	size_t encrypt(EVP_PKEY * evpKey, size_t plainSize, uint8_t const * plain, uint8_t * cipher) {
		PRSA rsa(EVP_PKEY_get1_RSA(evpKey), ::RSA_free);
		int result = RSA_public_encrypt((int)plainSize, plain, cipher, rsa.get(), RSA_PKCS1_OAEP_PADDING);
		if (result < 0)
			throw std::runtime_error("unable do encrypt");
		return result;
	}
}

void EncryptionCallbacks::cleanup() {
	RAND_cleanup();
	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
}

void EncryptionCallbacks::dataEncryptClientCallback(
	Lib3MF_ContentEncryptionParams params,
	Lib3MF_uint64 plainSize,
	const Lib3MF_uint8 * plainBuffer,
	const Lib3MF_uint64 cipherSize,
	Lib3MF_uint64 * cipherNeeded,
	Lib3MF_uint8 * cipherBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * status) {

	DekContext * dek = (DekContext *)userData;

	Lib3MF::CContentEncryptionParams p(dek->wrapper, params);
	dek->wrapper->Acquire(&p);


	if (Lib3MF::eEncryptionAlgorithm::AES256_GCM != p.GetEncryptionAlgorithm())
		*status = 0;
	else {
		PEVP_CIPHER_CTX ctx;

		auto it = dek->ciphers.find(p.GetDescriptor());

		if (it != dek->ciphers.end()) {
			ctx = it->second;
		} else {
			ByteVector key, iv, aad;
			p.GetKey(key);
			p.GetInitializationVector(iv);
			p.GetAdditionalAuthenticationData(aad);
			ctx = AesMethods::Encrypt::init(key.data(), iv.data(), aad.size(), aad.data());
			dek->ciphers[p.GetDescriptor()] = ctx;
		}

		if (0 == plainSize || nullptr == plainBuffer) {
			ByteVector tag(16, 0);

			if (!AesMethods::Encrypt::finish(ctx, cipherBuffer, (Lib3MF_uint32)tag.size(), tag.data())) {
				*status = 0;
			} else {
				*status = tag.size();
				p.SetAuthenticationTag(tag);
			}
			dek->ciphers.erase(it);
			ctx.reset();
		} else if (0 == cipherSize || nullptr == cipherBuffer) {
			*cipherNeeded = plainSize;
			*status = plainSize;
		} else {
			size_t encrypted = AesMethods::Encrypt::encrypt(ctx, (Lib3MF_uint32)plainSize, plainBuffer, cipherBuffer);
			*status = encrypted;
		}
	}
}

void EncryptionCallbacks::keyEncryptClientCallback(
	Lib3MF_AccessRight accessRight,
	Lib3MF_uint64 plainSize,
	const Lib3MF_uint8 * plainBuffer,
	const Lib3MF_uint64 cipherSize,
	Lib3MF_uint64* cipherNeeded,
	Lib3MF_uint8 * cipherBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * status) {

	KekContext const * context = (KekContext const *)userData;
	Lib3MF::CAccessRight ar(context->wrapper, accessRight);
	context->wrapper->Acquire(&ar);

	if (Lib3MF::eWrappingAlgorithm::RSA_OAEP != ar.GetWrappingAlgorithm()
		|| Lib3MF::eMgfAlgorithm::MGF1_SHA1 != ar.GetMgfAlgorithm()
		|| Lib3MF::eDigestMethod::SHA1 != ar.GetDigestMethod())
		*status = 0;
	else if (nullptr == cipherBuffer || cipherSize == 0) {
		*cipherNeeded = RsaMethods::getSize(context->key);
		*status = *cipherNeeded;
	} else {
		KekContext const * context = (KekContext const *)userData;
		*status = RsaMethods::encrypt(context->key, (size_t) plainSize, plainBuffer, cipherBuffer);
	}
}

void EncryptionCallbacks::dataDecryptClientCallback(
	Lib3MF_ContentEncryptionParams params,
	Lib3MF_uint64 cipherSize,
	const Lib3MF_uint8 * cipherBuffer,
	const Lib3MF_uint64 plainSize,
	Lib3MF_uint64 * plainNeeded,
	Lib3MF_uint8 * plainBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * status) {

	DekContext * dek = (DekContext *)userData;

	Lib3MF::CContentEncryptionParams p(dek->wrapper, params);
	dek->wrapper->Acquire(&p);


	if (Lib3MF::eEncryptionAlgorithm::AES256_GCM != p.GetEncryptionAlgorithm())
		*status = 0;
	else {
		PEVP_CIPHER_CTX ctx;

		auto it = dek->ciphers.find(p.GetDescriptor());

		if (it != dek->ciphers.end()) {
			ctx = it->second;
		} else {
			ByteVector key(32, 0), iv(12, 0), aad;
			p.GetKey(key);
			p.GetInitializationVector(iv);
			p.GetAdditionalAuthenticationData(aad);
			ctx = AesMethods::Decrypt::init(key.data(), iv.data(), aad.size(), aad.data());
			dek->ciphers[p.GetDescriptor()] = ctx;
		}

		if (0 == cipherSize || nullptr == cipherBuffer) {
			ByteVector tag(16, 0);
			p.GetAuthenticationTag(tag);
			if (!AesMethods::Decrypt::finish(ctx, plainBuffer, tag.data())) {
				*status = 0;
			} else {
				*status = tag.size();
			}
			dek->ciphers.erase(it);
			ctx.reset();
		} else if (0 == plainSize || nullptr == plainBuffer) {
			*plainNeeded = cipherSize;
			*status = cipherSize;
		} else {
			size_t decrypted = AesMethods::Decrypt::decrypt(ctx, (Lib3MF_uint32)plainSize, cipherBuffer, plainBuffer);
			*status = decrypted;
		}
	}
}

void EncryptionCallbacks::keyDecryptClientCallback(
	Lib3MF_AccessRight accessRight,
	Lib3MF_uint64 cipherSize,
	const Lib3MF_uint8 * cipherBuffer,
	const Lib3MF_uint64 plainSize,
	Lib3MF_uint64* plainNeeded,
	Lib3MF_uint8 * plainBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * status) {

	KekContext const * context = (KekContext const *)userData;
	Lib3MF::CAccessRight ar(context->wrapper, accessRight);
	context->wrapper->Acquire(&ar);

	if (Lib3MF::eWrappingAlgorithm::RSA_OAEP != ar.GetWrappingAlgorithm()
		|| Lib3MF::eMgfAlgorithm::MGF1_SHA1 != ar.GetMgfAlgorithm()
		|| Lib3MF::eDigestMethod::SHA1 != ar.GetDigestMethod())
		*status = 0;
	else if (nullptr == plainBuffer || 0 == plainSize) {
		// plainNeeded should be bigger than the plain text size for padding cases to avoid mem leak.
		*plainNeeded = RsaMethods::getSize(context->key) - 42;
		*status = 32;
	}
	else {
		ASSERT_EQ(cipherSize, 256);
		ASSERT_GE(plainSize, 32);

		*status = RsaMethods::decrypt(context->key, (size_t) cipherSize, cipherBuffer, plainBuffer);
	}
}

void EncryptionCallbacks::randomNumberCallback(
	Lib3MF_uint64 nByteData,
	Lib3MF_uint64 nNumBytes,
	Lib3MF_pvoid pUserData,
	Lib3MF_uint64 * pBytesWritten) {

	int rc = RAND_bytes((unsigned char *)nByteData,  (int)nNumBytes);
	if (rc != 1)
		*pBytesWritten = 0;
	else 
		*pBytesWritten = nNumBytes;
}

