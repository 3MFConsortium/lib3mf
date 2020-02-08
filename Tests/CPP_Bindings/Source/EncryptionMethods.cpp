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

	struct RsaMethods {
		static PEVP_PKEY loadKey(ByteVector const & privKey);
		static size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain);
		static size_t getSize(PEVP_PKEY const & evpKey);
	};

	struct AesMethods {
		static PEVP_CIPHER_CTX init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv);
		static size_t decrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * cipher, Lib3MF_uint8 * plain);
		static bool finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * plain, Lib3MF_uint8 * tag);
	};

	class EncryptionMethods : public ::testing::Test {
	protected:
		PModel model;
		PEVP_PKEY privateKey;
	public:
		EncryptionMethods(): privateKey(nullptr, ::EVP_PKEY_free) {
			ByteVector key = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/sample.pem");
			privateKey = RsaMethods::loadKey(key);
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


	PEVP_PKEY RsaMethods::loadKey(ByteVector const & privKey) {
		PBIO keybio(BIO_new(BIO_s_mem()), ::BIO_free);
		BIO_write(keybio.get(), privKey.data(), (int)privKey.size());
		EVP_PKEY * evpKey = 0;
		PEM_read_bio_PrivateKey(keybio.get(), &evpKey, NULL, NULL);
		PEVP_PKEY pevpKey(evpKey, ::EVP_PKEY_free);
		return pevpKey;
	}

	size_t RsaMethods::getSize(PEVP_PKEY const & evpKey) {
		RSA * rsa = EVP_PKEY_get1_RSA(evpKey.get());
		size_t rsaSize = RSA_size(rsa);
		return rsaSize;
	}

	size_t RsaMethods::decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain) {
		RSA * rsa = EVP_PKEY_get1_RSA(evpKey);
		size_t result = RSA_private_decrypt((int)cipherSize, cipher, plain, rsa, RSA_PKCS1_OAEP_PADDING);
		if (-1 == result)
			throw std::runtime_error("unable to decrypt");
		return result;
	}


	PEVP_CIPHER_CTX AesMethods::init(Lib3MF_uint8 const * key, Lib3MF_uint8 const * iv) {
		PEVP_CIPHER_CTX ctx = make_shared(EVP_CIPHER_CTX_new());
		if (!ctx)
			throw std::runtime_error("unable to initialize context");

		if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), NULL, NULL, NULL))
			throw std::runtime_error("unable to initialize cipher");

		if (!EVP_DecryptInit_ex(ctx.get(), NULL, NULL, key, iv))
			throw std::runtime_error("unable to initialize key and iv");
		return ctx;
	}

	size_t AesMethods::decrypt(PEVP_CIPHER_CTX ctx, Lib3MF_uint32 size, Lib3MF_uint8 const * cipher, Lib3MF_uint8 * plain) {
		int len = 0;
		if (!EVP_DecryptUpdate(ctx.get(), plain, &len, cipher, size))
			throw std::runtime_error("unable to decrypt");
		return len;
	}

	bool AesMethods::finish(PEVP_CIPHER_CTX ctx, Lib3MF_uint8 * plain, Lib3MF_uint8 * tag) {
		if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, 16, tag))
			throw std::runtime_error("unable to set tag");
		int len = 0;
		int ret = EVP_DecryptFinal_ex(ctx.get(), plain, &len);
		return ret > 0;
	}

	struct KekContext {
		EVP_PKEY * key;
		size_t size;
	};

	struct DekContext {
		std::map<Lib3MF_uint64, PEVP_CIPHER_CTX> m_Context;
	};

	struct ClientCallbacks {
		static void dekClientCallback(
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
					ctx = AesMethods::init(cv.m_Key, cv.m_IV);
					dek->m_Context[cd.GetDescriptor()] = ctx;
				}

				if (0 != cipherSize) {
					size_t decrypted = AesMethods::decrypt(ctx, (Lib3MF_uint32)cipherSize, cipherBuffer, plainBuffer);
					*result = decrypted;
				} else {
					if (!AesMethods::finish(ctx, plainBuffer, cv.m_Tag))
						*result = -2;
				}
			}
		}

		static void kekClientCallback(
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
		reader->RegisterDEKClient(ClientCallbacks::dekClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", ClientCallbacks::kekClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
	}
}