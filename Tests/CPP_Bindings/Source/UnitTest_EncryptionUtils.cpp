#include "UnitTest_EncryptionUtils.h"

#include "gtest/gtest.h"

namespace AesMethods {

	namespace Decrypt {
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv) {
			PEVP_CIPHER_CTX ctx = make_shared(EVP_CIPHER_CTX_new());
			if (!ctx)
				throw std::runtime_error("unable to initialize context");

			if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), NULL, NULL, NULL))
				throw std::runtime_error("unable to initialize cipher");

			if (!EVP_DecryptInit_ex(ctx.get(), NULL, NULL, key, iv))
				throw std::runtime_error("unable to initialize key and iv");
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
		PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv) {
			PEVP_CIPHER_CTX ctx = make_shared(EVP_CIPHER_CTX_new());
			if (!ctx)
				throw std::runtime_error("unable to intialize cipher context");

			if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), NULL, NULL, NULL))
				throw std::runtime_error("unable to initialize encryption mechanism");

			if (1 != EVP_EncryptInit_ex(ctx.get(), NULL, NULL, key, iv))
				throw std::runtime_error("unable to init key and iv");
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

	size_t getSize(PEVP_PKEY const & evpKey) {
		RSA * rsa = EVP_PKEY_get1_RSA(evpKey.get());
		size_t rsaSize = RSA_size(rsa);
		return rsaSize;
	}

	size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain) {
		RSA * rsa = EVP_PKEY_get1_RSA(evpKey);
		size_t result = RSA_private_decrypt((int)cipherSize, cipher, plain, rsa, RSA_PKCS1_OAEP_PADDING);
		if (-1 == result)
			throw std::runtime_error("unable to decrypt");
		return result;
	}

	size_t encrypt(EVP_PKEY * evpKey, size_t plainSize, uint8_t const * plain, uint8_t * cipher) {
		RSA * rsa = EVP_PKEY_get1_RSA(evpKey);
		size_t result = RSA_public_encrypt((int)plainSize, plain, cipher, rsa, RSA_PKCS1_OAEP_PADDING);
		if (-1 == result)
			throw std::runtime_error("unable do encrypt");
		return result;
	}
}

void EncryptionCallbacks::dataEncryptClientCallback(
	Lib3MF::eEncryptionAlgorithm algorithm,
	Lib3MF_CipherData cipherData,
	Lib3MF_uint64 plainSize,
	const Lib3MF_uint8 * plainBuffer,
	const Lib3MF_uint64 cipherSize,
	Lib3MF_uint64 * cipherNeeded,
	Lib3MF_uint8 * cipherBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * result
) {
	if (algorithm != Lib3MF::eEncryptionAlgorithm::Aes256Gcm)
		*result = -1;
	else {
		DekContext * dek = (DekContext *)userData;

		Lib3MF::CCipherData cd(dek->wrapper, cipherData);
		dek->wrapper->Acquire(&cd);

		Lib3MF::sAes256CipherValue cv = cd.GetAes256Gcm();

		PEVP_CIPHER_CTX ctx;

		auto it = dek->m_Context.find(cd.GetDescriptor());

		if (it != dek->m_Context.end()) {
			ctx = it->second;
		} else {
			ctx = AesMethods::Encrypt::init(cv.m_Key, cv.m_IV);
			dek->m_Context[cd.GetDescriptor()] = ctx;
		}

		if (0 != cipherSize) {
			size_t encrypted = AesMethods::Encrypt::encrypt(ctx, (Lib3MF_uint32)plainSize, plainBuffer, cipherBuffer);
			*result = encrypted;
		} else {
			if (!AesMethods::Encrypt::finish(ctx, cipherBuffer, sizeof(cv.m_Tag), cv.m_Tag)) {
				*result = -2;
			} else {
				cd.SetAes256Gcm(cv);
				dek->m_Context.erase(it);
			}
		}
	}
}

void EncryptionCallbacks::keyEncryptClientCallback(
	Lib3MF_Consumer consumer,
	Lib3MF::eEncryptionAlgorithm algorithm,
	Lib3MF_uint64 plainSize,
	const Lib3MF_uint8 * plainBuffer,
	const Lib3MF_uint64 cipherSize,
	Lib3MF_uint64* cipherNeeded,
	Lib3MF_uint8 * cipherBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * result) {

	if (algorithm != Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p)
		*result = -1;
	else {

		KekContext const * context = (KekContext const *)userData;
		*result = RsaMethods::encrypt(context->key, plainSize, plainBuffer, cipherBuffer);
	}
}

void EncryptionCallbacks::dataDecryptClientCallback(
	Lib3MF::eEncryptionAlgorithm algorithm,
	Lib3MF_CipherData cipherData,
	Lib3MF_uint64 cipherSize,
	const Lib3MF_uint8 * cipherBuffer,
	const Lib3MF_uint64 plainSize,
	Lib3MF_uint64 * plainNeeded,
	Lib3MF_uint8 * plainBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * result) {

	if (algorithm != Lib3MF::eEncryptionAlgorithm::Aes256Gcm)
		*result = -1;
	else {
		DekContext * dek = (DekContext *)userData;
		Lib3MF::CCipherData cd(dek->wrapper, cipherData);
		dek->wrapper->Acquire(&cd);

		Lib3MF::sAes256CipherValue cv = cd.GetAes256Gcm();

		PEVP_CIPHER_CTX ctx;

		auto it = dek->m_Context.find(cd.GetDescriptor());

		if (it != dek->m_Context.end()) {
			ctx = it->second;
		} else {
			ctx = AesMethods::Decrypt::init(cv.m_Key, cv.m_IV);
			dek->m_Context[cd.GetDescriptor()] = ctx;
		}

		if (0 != cipherSize) {
			size_t decrypted = AesMethods::Decrypt::decrypt(ctx, (Lib3MF_uint32)cipherSize, cipherBuffer, plainBuffer);
			*result = decrypted;
		} else {
			if (!AesMethods::Decrypt::finish(ctx, plainBuffer, cv.m_Tag)) {
				*result = -2;
			} else {
				dek->m_Context.erase(it);
			}
		}
	}
}

void EncryptionCallbacks::keyDecryptClientCallback(
	Lib3MF_Consumer consumer,
	Lib3MF::eEncryptionAlgorithm algorithm,
	Lib3MF_uint64 cipherSize,
	const Lib3MF_uint8 * cipherBuffer,
	const Lib3MF_uint64 plainSize,
	Lib3MF_uint64* plainNeeded,
	Lib3MF_uint8 * plainBuffer,
	Lib3MF_pvoid userData,
	Lib3MF_uint64 * result) {


	if (algorithm != Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p)
		*result = -1;
	else {

		KekContext const * context = (KekContext const *)userData;

		ASSERT_EQ(cipherSize, context->size);
		ASSERT_GE(plainSize, context->size);

		*result = RsaMethods::decrypt(context->key, cipherSize, cipherBuffer, plainBuffer);
	}
}

