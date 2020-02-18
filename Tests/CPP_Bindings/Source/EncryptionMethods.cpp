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

	struct KekContext {
		EVP_PKEY * key;
		size_t size;
	};

	struct DekContext {
		std::map<Lib3MF_uint64, PEVP_CIPHER_CTX> m_Context;
	};

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

	class EncryptionMethods : public ::testing::Test {
	protected:
		PModel model;
		PEVP_PKEY privateKey;
		sPosition pVertices[8];
		sTriangle pTriangles[12];
		
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
			float fSizeX = 100.0f;
			float fSizeY = 200.0f;
			float fSizeZ = 300.0f;
			pVertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
			pVertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
			pVertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
			pVertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
			pVertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
			pVertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
			pVertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
			pVertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

			// Manually create triangles
			pTriangles[0] = fnCreateTriangle(2, 1, 0);
			pTriangles[1] = fnCreateTriangle(0, 3, 2);
			pTriangles[2] = fnCreateTriangle(4, 5, 6);
			pTriangles[3] = fnCreateTriangle(6, 7, 4);
			pTriangles[4] = fnCreateTriangle(0, 1, 5);
			pTriangles[5] = fnCreateTriangle(5, 4, 0);
			pTriangles[6] = fnCreateTriangle(2, 3, 7);
			pTriangles[7] = fnCreateTriangle(7, 6, 2);
			pTriangles[8] = fnCreateTriangle(1, 2, 6);
			pTriangles[9] = fnCreateTriangle(6, 5, 1);
			pTriangles[10] = fnCreateTriangle(3, 0, 4);
			pTriangles[11] = fnCreateTriangle(4, 7, 3);
			model = wrapper->CreateModel();
		}

		virtual void TearDown() {
			model.reset();
		}

		void cleanup(DekContext & context) {
			context.m_Context.clear();
		}

		void cleanup(KekContext & context) {
			context.key = nullptr;
			context.size = 0;
		}

	};

	PWrapper EncryptionMethods::wrapper;



	struct ClientCallbacks {

		static void dataEncryptClientCallback(
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
				}
				else {
					ctx = AesMethods::Encrypt::init(cv.m_Key, cv.m_IV);
					dek->m_Context[cd.GetDescriptor()] = ctx;
				}

				if (0 != cipherSize) {
					size_t encrypted = AesMethods::Encrypt::encrypt(ctx, (Lib3MF_uint32)plainSize, plainBuffer, cipherBuffer);
					*result = encrypted;
				}
				else {
					if (!AesMethods::Encrypt::finish(ctx, cipherBuffer, sizeof(cv.m_Tag), cv.m_Tag)) {
						*result = -2;
					}
					else {
						cd.SetAes256Gcm(cv);
						dek->m_Context.erase(it);
					}
				}
			}
		}

		static void keyEncryptClientCallback(
			Lib3MF_Consumer consumer,
			Lib3MF::eEncryptionAlgorithm algorithm,
			Lib3MF_uint64 plainSize,
			const Lib3MF_uint8 * plainBuffer,
			const Lib3MF_uint64 cipherSize,
			Lib3MF_uint64* cipherNeeded,
			Lib3MF_uint8 * cipherBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 * result) {

			if (algorithm != eEncryptionAlgorithm::RsaOaepMgf1p)
				*result = -1;
			else {

				KekContext const * context = (KekContext const *)userData;
				*result = RsaMethods::encrypt(context->key, plainSize, plainBuffer, cipherBuffer);
			}
		}

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
					if (!AesMethods::Decrypt::finish(ctx, plainBuffer, cv.m_Tag)) {
						*result = -2;
					}
					else {
						dek->m_Context.erase(it);
					}
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
	
	//TODO: create new encrypted model with a mesh, save it. Read, and do asserts
	TEST_F(EncryptionMethods, WriteEncryptedProductionModel) {
		std::string filePath = sOutFilesPath + "/SecureContent/write_encrypted_keystore.3mf";

		Lib3MF::PModel secureModel = wrapper->CreateModel();
		std::string path = "/3D/securemesh.model";
		Lib3MF::PMeshObject meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		Lib3MF::PPackagePath modelPath = meshObject->PackagePath();
		modelPath->Set(path);
		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}
		secureModel->AddBuildItem(meshObject.get(), transformation);
		
		Lib3MF::PKeyStore keyStore = secureModel->GetKeyStore();
		keyStore->SetUUID("b7aa9c75-5fbd-48c1-a893-40289e45ab8f");
		std::string keyValue = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId = "KEK_xxx";
		std::string consumerId = "HP#MOP44B#SG5693454";
		Lib3MF::PConsumer consumer = keyStore->AddConsumer(consumerId, keyId, keyValue);
		Lib3MF::PResourceData resourceData = keyStore->AddResourceData(modelPath.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);
		Lib3MF::PDecryptRight decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);		

		PWriter writer = secureModel->QueryWriter("3mf");

		DekContext writeDekUserData;
		writer->RegisterDEKClient(ClientCallbacks::dataEncryptClientCallback, (Lib3MF_pvoid)(&writeDekUserData));
		
		KekContext writeKekUserData;
		writeKekUserData.key = privateKey.get();
		writeKekUserData.size = RsaMethods::getSize(privateKey);
		writer->RegisterKEKClient(consumerId, ClientCallbacks::keyEncryptClientCallback, (Lib3MF_uint32)writeKekUserData.size, &writeKekUserData);

		writer->WriteToFile(filePath);

		//READ PART
		auto reader = model->QueryReader("3mf");

		DekContext readDekUserData;
		reader->RegisterDEKClient(ClientCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&readDekUserData));

		KekContext readKekUserData;
		readKekUserData.key = privateKey.get();
		readKekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient(consumerId, ClientCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)readKekUserData.size, &readKekUserData);
		reader->ReadFromFile(filePath);
		auto meshObj = model->GetMeshObjects();
		ASSERT_EQ(1, meshObj->Count());
	}



	TEST_F(EncryptionMethods, WriteAdditionalConsumerToEncryptedModel) {
		ByteVector buffer;
		{
			//read existing model
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			reader->RegisterDEKClient(ClientCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

			KekContext kekUserData;
			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);
			reader->RegisterKEKClient("LIB3MF#TEST", ClientCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
			reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

			auto meshObjIt = model->GetMeshObjects();
			ASSERT_EQ(meshObjIt->Count(), 1);

			auto keyStore = model->GetKeyStore();
			ASSERT_EQ(keyStore->GetConsumerCount(), 1);
			ASSERT_EQ(keyStore->GetResourceDataCount(), 1);

			//add new consumer
			auto consumer = keyStore->AddConsumer("LIB3MF#TEST2", "", "");
			auto resourceData = keyStore->GetResourceData(0);
			resourceData->AddDecryptRight(consumer.get(), eEncryptionAlgorithm::RsaOaepMgf1p);

			cleanup(dekUserData);
			cleanup(kekUserData);

			//write new model
			auto writer = model->QueryWriter("3mf");

			writer->RegisterDEKClient(ClientCallbacks::dataEncryptClientCallback, &dekUserData);

			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);

			writer->RegisterKEKClient("LIB3MF#TEST2", ClientCallbacks::keyEncryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
			writer->WriteToBuffer(buffer);
		}
		//reset model
		model.reset();
		model = wrapper->CreateModel();
		{
			//read generated model
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			reader->RegisterDEKClient(ClientCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

			KekContext kekUserData;
			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);
			reader->RegisterKEKClient("LIB3MF#TEST2", ClientCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
			reader->ReadFromBuffer(buffer);

			auto meshObjIt = model->GetMeshObjects();
			ASSERT_EQ(meshObjIt->Count(), 1);

			auto keyStore = model->GetKeyStore();
			ASSERT_EQ(keyStore->GetConsumerCount(), 2);
			ASSERT_EQ(keyStore->GetResourceDataCount(), 1);
		}
	}
}
