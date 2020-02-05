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
	using PEVP_CIPHER_CTX = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
	using PEVP_PKEY = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;

	struct RsaMethods {
		static PEVP_PKEY loadKey(ByteVector const & privKey);
		static size_t decrypt(EVP_PKEY * evpKey, size_t cipherSize, uint8_t const * cipher, uint8_t * plain);
		static size_t getSize(PEVP_PKEY const & evpKey);
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

	struct KekContext {
		EVP_PKEY * key;
		size_t size;
	};


	struct DekContext {

	};

	struct Dek {
		std::map<Lib3MF_uint64, DekContext> m_Context;
		static void dekClientCallback(
			Lib3MF::eEncryptionAlgorithm algorithm, 
			Lib3MF_CipherData cipherData, 
			Lib3MF_uint64 cipherSize, 
			const Lib3MF_uint8 * cipherBuffer, 
			const Lib3MF_uint64 plainSize, 
			Lib3MF_uint64* plainNeeded, 
			Lib3MF_uint8 * plainBuffer, 
			Lib3MF_pvoid userData) {

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
				throw std::runtime_error("Not implemented");

			KekContext const * context = (KekContext const *)userData;
			*result = 0;
			if (nullptr == plainBuffer || 0 == plainSize || plainSize < context->size) {
				if (nullptr == plainNeeded)
					throw std::runtime_error("invalid size");
				*plainNeeded = context->size;
			} else {
				*result = RsaMethods::decrypt(context->key, context->size, cipherBuffer, plainBuffer);
			}
		}
	};

	TEST_F(EncryptionMethods, ReadEncrypted3MF) {
		auto reader = model->QueryReader("3mf");
		//Dek dekUserData;
		//reader->RegisterDEKClient(Dek::dekClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));
		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", Dek::kekClientCallback, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(1, resources->Count());

	}
}