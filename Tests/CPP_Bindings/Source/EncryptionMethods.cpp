#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

#include <vector>

#include <openssl/evp.h>
#include <openssl/pem.h>

namespace Lib3MF {

	using Byte = uint8_t;
	using ByteVector = std::vector<uint8_t>;

	using PEVP_ENCODE_CTX = std::unique_ptr<EVP_ENCODE_CTX, decltype(&::EVP_ENCODE_CTX_free)>;
	using PBIO = std::unique_ptr<BIO, decltype(&::BIO_free)>;
	using PEVP_CIPHER_CTX = std::shared_ptr<EVP_CIPHER_CTX>;
	using PEVP_PKEY = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;

	std::shared_ptr<EVP_CIPHER_CTX> make_shared(EVP_CIPHER_CTX * ctx) {
		return std::shared_ptr<EVP_CIPHER_CTX>(ctx, ::EVP_CIPHER_CTX_free);
	}

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

			Lib3MF_uint32 finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * plain, Lib3MF_uint32 tagSize, Lib3MF_uint8 * tag) {
				int len = 0;
				if (1 != EVP_EncryptFinal_ex(ctx.get(), plain, &len))
					throw std::runtime_error("unable to finalize encryption");

				if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, tagSize, tag))
					throw std::runtime_error("unable to get tag");
				return len;
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

	class EncryptionMethods : public ::testing::Test {
	protected:
		PModel model;
		PEVP_PKEY privateKey;
	public:
		EncryptionMethods(): privateKey(nullptr, ::EVP_PKEY_free) {
			ByteVector key = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/sample.pem");
			privateKey = RsaMethods::loadPrivateKey(key);
			model = wrapper->CreateModel();
		}
		static PWrapper wrapper;

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}

		virtual void SetUp() {
			model = wrapper->CreateModel();
		}

		virtual void TearDown() {
			model.reset();
		}

	};

	PWrapper EncryptionMethods::wrapper;

	struct KekContext {
		EVP_PKEY * key;
		size_t size;
	};

	struct DekContext {
		std::map<Lib3MF_uint64, PEVP_CIPHER_CTX> m_Context;
	};

	struct ClientCallbacks {
		static void dataDecryptClientCallback(
			Lib3MF::eEncryptionAlgorithm algorithm, 
			Lib3MF_CipherData cipherData, 
			Lib3MF_uint64 cipherSize, 
			const Lib3MF_uint8 * cipherBuffer, 
			const Lib3MF_uint64 plainSize, 
			Lib3MF_uint64 * plainNeeded, 
			Lib3MF_uint8 * plainBuffer, 
			Lib3MF_pvoid userData,
			Lib3MF_uint64 * result) {

			if (algorithm != eEncryptionAlgorithm::Aes256Gcm)
				*result = -1;
			else {
				CCipherData cd(EncryptionMethods::wrapper.get(), cipherData);
				EncryptionMethods::wrapper->Acquire(&cd);

				sAes256CipherValue cv = cd.GetAes256Gcm();

				DekContext * dek = (DekContext *)userData;
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
					if (!AesMethods::Decrypt::finish(ctx, plainBuffer, cv.m_Tag))
						*result = -2;
				}
			}
		}

		static void keyDecryptClientCallback(
			Lib3MF_Consumer consumer, 
			Lib3MF::eEncryptionAlgorithm algorithm, 
			Lib3MF_uint64 cipherSize, 
			const Lib3MF_uint8 * cipherBuffer, 
			const Lib3MF_uint64 plainSize, 
			Lib3MF_uint64* plainNeeded, 
			Lib3MF_uint8 * plainBuffer, 
			Lib3MF_pvoid userData, 
			Lib3MF_uint64 * result) {


			if (algorithm != eEncryptionAlgorithm::RsaOaepMgf1p)
				*result = -1;
			else {

				KekContext const * context = (KekContext const *)userData;

				ASSERT_EQ(cipherSize, context->size);
				ASSERT_GE(plainSize, context->size);

				*result = RsaMethods::decrypt(context->key, cipherSize, cipherBuffer, plainBuffer);
			}
		}
	};

	TEST_F(EncryptionMethods, ReadEncrypted3MF) {
		auto reader = model->QueryReader("3mf");

		DekContext dekUserData;
		reader->RegisterDEKClient(ClientCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", ClientCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
	}
	
	TEST_F(EncryptionMethods, ReadEncryptedCompressed3MF) {
		auto reader = model->QueryReader("3mf");

		DekContext dekUserData;
		reader->RegisterDEKClient(ClientCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", ClientCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted_compressed.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
	}

}