#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"
//#include <openssl/conf.h>
//#include <openssl/evp.h>
//#include <openssl/bio.h>
//#include <openssl/pem.h>
//#include <openssl/rsa.h>
//#include <openssl/err.h>

namespace Lib3MF {

	class SecureContentT : public ::testing::Test {
	protected:
		sPosition pVertices[8];
		sTriangle pTriangles[12];

		PModel model;

		const std::string UNENCRYPTEDKEYSTORE = "/SecureContent/keystore.3mf";
	protected:
		SecureContentT()
		{}

		virtual void SetUp() {
			float fSizeX = 100.0f;
			float fSizeY = 200.0f;
			float fSizeZ = 300.0f;

			// Manually create vertices
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

		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}

		PReader readUnencryptedKeyStore() {
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
			return reader;
		}
	public:
		static PWrapper wrapper;
	};
	PWrapper SecureContentT::wrapper;

	using Lib3MF_buffer = std::vector<Lib3MF_uint8>;

	/***********************************
		The 'model' is empty on the tests.
		You need to do the reading/writing.
		Read existing models by defining 
		functions for it, so we better track
		the files used for testing.
	***********************************/

	//
	//Keystore basic, empty model tests
	//

	TEST_F(SecureContentT, ModelKeyStoreUUID) {
		std::string uuid = "df81fc77-cfd1-4266-a432-9759a0d26c2a";
		auto ks = model->GetKeyStore();
		ASSERT_TRUE(ks != nullptr);
		ks->SetUUID(uuid);
		bool hasUUID;
		std::string uuid2 = ks->GetUUID(hasUUID);
		ASSERT_TRUE(hasUUID);
		ASSERT_EQ(uuid2, uuid);
	}

	TEST_F(SecureContentT, CreateMultipleConsumersKeyStore) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();

		std::string firstId = "firstId";
		std::string secondId = "secondId";

		std::string firstKeyId = "firstKeyId";
		std::string secondKeyId = "secondKeyId";
		std::string firstKeyValue = "firstKeyValue";
		std::string secondKeyValue = "secondKeyValue";

		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer(firstId, firstKeyId, firstKeyValue);
		Lib3MF::PConsumer consumer2 = keyStore->AddConsumer(secondId, secondKeyId, secondKeyValue);

		ASSERT_EQ(2, keyStore->GetConsumerCount());
		ASSERT_EQ(firstId, keyStore->GetConsumer(0)->GetConsumerID());
		ASSERT_EQ(secondId, keyStore->GetConsumer(1)->GetConsumerID());
	}

	TEST_F(SecureContentT, CreateMultipleResourceDataKeyStore) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);

		std::string path1 = "/3D/nonrootmodel1.model";
		auto meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path1);

		keyStore->AddResourceData(meshObject->PackagePath().get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);

		std::string path2 = "/3D/nonrootmodel2.model";
		meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path2);

		keyStore->AddResourceData(meshObject->PackagePath().get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		ASSERT_EQ(2, keyStore->GetResourceDataCount());
		ASSERT_EQ(path1, keyStore->GetResourceData(0)->GetPath()->Get());
		ASSERT_EQ(path2, keyStore->GetResourceData(1)->GetPath()->Get());
	}

	//TODO add multiple consumers
	// add multiple resource datas
	// add one decryptright for each consumer on each resource data
	// remove one consumer
	// assert the remaining consumer is correct
	// assert that decryptrights for the removed consumer are also gone
	TEST_F(SecureContentT, ManageDecryptRight) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
	}





	//
	//Keystore test which require preexisting model
	//

	TEST_F(SecureContentT, ModelReaderKeyStoreNoWarnings) {
		PReader reader3MF = readUnencryptedKeyStore();
		CheckReaderWarnings(reader3MF, 0);
	}

	TEST_F(SecureContentT, CheckKeyStoreConsumers) {
		readUnencryptedKeyStore();
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const int consumerCount = keyStore->GetConsumerCount();
		for (int i = 0; i < consumerCount; ++i) {
			PConsumer consumer = keyStore->GetConsumer(i);

			ASSERT_EQ("LIB3MF#TEST", consumer->GetConsumerID());
			ASSERT_EQ("contentKey", consumer->GetKeyID());

			try {
				PConsumer consumerNotFound = keyStore->FindConsumer("does not exist");
				ASSERT_FALSE(true);
			} catch (ELib3MFException const & e) {
				ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_KEYSTORECONSUMERNOTFOUND);
			}
			PConsumer consumerFound = keyStore->FindConsumer(consumer->GetConsumerID());
			ASSERT_EQ(consumer->GetConsumerID(), consumerFound->GetConsumerID());

			std::string expected = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
			std::string keyValue = consumer->GetKeyValue();
			ASSERT_EQ(expected, keyValue);

		}
	}
	TEST_F(SecureContentT, CheckKeyStoreResourceData) {
		readUnencryptedKeyStore();
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const int resourceDataCount = keyStore->GetResourceDataCount();

		ASSERT_TRUE(resourceDataCount > 0);

		try {
			PResourceData resourceDataNotFound = keyStore->FindResourceData("does not exist");
			ASSERT_FALSE(true);
		} catch (ELib3MFException const & e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_KEYSTORERESOURCEDATANOTFOUND);
		}

		for (int i = 0; i < resourceDataCount; ++i) {
			PResourceData resourceData = keyStore->GetResourceData(i);
			ASSERT_TRUE(resourceData != nullptr);

			PResourceData resourceDataFound = keyStore->FindResourceData(resourceData->GetPath()->Get());
			ASSERT_TRUE(resourceDataFound != nullptr);

			ASSERT_EQ(resourceData->GetPath()->Get(), resourceDataFound->GetPath()->Get());

			ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::Aes256Gcm, resourceData->GetEncryptionAlgorithm());
			ASSERT_EQ(Lib3MF::eCompression::None, resourceData->GetCompression());
			ASSERT_EQ("/3D/3dexternal.model", resourceData->GetPath()->Get());

			int inexistantDecrypt = 9999;
			ASSERT_EQ(nullptr, resourceData->GetDecryptRight(inexistantDecrypt));

			int decryptRightCount = resourceData->GetDecryptRightCount();
			ASSERT_TRUE(decryptRightCount > 0);
			for (int i = 0; i < decryptRightCount; ++i) {
				PDecryptRight decryptRight = resourceData->GetDecryptRight(i);
				ASSERT_TRUE(decryptRight != nullptr);
				PConsumer consumer = decryptRight->GetConsumer();
				ASSERT_TRUE(consumer != nullptr);
				
				ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p, decryptRight->GetEncryptionAlgorithm());
			}
		}
	}

	struct DEKCallbackData {
		int value;
		std::vector<Lib3MF_uint64> context;
		static void testDEKCallback(
			Lib3MF::eEncryptionAlgorithm algorithm, 
			Lib3MF_CipherData cipherData, 
			Lib3MF_uint64 cipherSize, 
			const Lib3MF_uint8 * cipherBuffer, 
			const Lib3MF_uint64 plainSize, 
			Lib3MF_uint64 * plainNeededSize,
			Lib3MF_uint8 * plainBuffer, 
			Lib3MF_pvoid userData) {

			DEKCallbackData * cb = reinterpret_cast<DEKCallbackData *>(userData);
			ASSERT_EQ(cb->value, 1);
			
			Lib3MF_uint64 descriptor = 0;
			lib3mf_cipherdata_getdescriptor(cipherData, &descriptor);
			ASSERT_EQ(descriptor, 1);
			
			sAes256CipherValue cipher;
			lib3mf_cipherdata_getaes256gcm(cipherData, &cipher);
			
			cb->context.push_back(descriptor);

			std::copy(cipherBuffer, cipherBuffer + plainSize, plainBuffer);
		}
	};

	TEST_F(SecureContentT, DEKCallbackTest) {
		auto reader = model->QueryReader("3mf");
		DEKCallbackData data;
		data.value = 1;
		reader->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
	}

	struct KEKCallbackData {
		int value;
		static void testKEKCallback(
			Lib3MF_Consumer pConsumer,
			Lib3MF::eEncryptionAlgorithm algorithm,
			Lib3MF_uint64 cipherSize,
			const Lib3MF_uint8 * cipherBuffer,
			const Lib3MF_uint64 plainSize,
			Lib3MF_uint64 * plainNeeded,
			Lib3MF_uint8 * plainBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 *result) {

			KEKCallbackData * cb = reinterpret_cast<KEKCallbackData *>(userData);
			ASSERT_EQ(cb->value, 1);


			Lib3MF_uint32 needed = 0;
			std::vector<char> buffer;

			lib3mf_consumer_getconsumerid(pConsumer, 0, &needed, nullptr);
			buffer.resize(needed, 0);
			lib3mf_consumer_getconsumerid(pConsumer, (Lib3MF_uint32)buffer.size(), nullptr, buffer.data());
			std::string consumerId(buffer.begin(), buffer.end() - 1);

			ASSERT_EQ(consumerId, "LIB3MF#TEST");
		
			lib3mf_consumer_getkeyid(pConsumer, 0, &needed, nullptr);
			buffer.resize(needed, 0);
			lib3mf_consumer_getkeyid(pConsumer, (Lib3MF_uint32)buffer.size(), nullptr, buffer.data());
			std::string keyId(buffer.begin(), buffer.end() - 1);

			ASSERT_EQ(keyId, "contentKey");

			lib3mf_consumer_getkeyvalue(pConsumer, 0, &needed, nullptr);
			buffer.resize(needed, 0);
			lib3mf_consumer_getkeyvalue(pConsumer, (Lib3MF_uint32)buffer.size(), nullptr, buffer.data());
			std::string keyvalue(buffer.begin(), buffer.end() - 1);

			//ASSERT_EQ(keyvalue, "");

			if (plainSize == 0) {
				*plainNeeded = cipherSize;
				*result = 0;
			} else {
				std::copy(cipherBuffer, cipherBuffer + plainSize, plainBuffer);
				*result = plainSize;
			}
		}
	};

	TEST_F(SecureContentT, KEKCallbackTest) {
		auto reader = model->QueryReader("3mf");
		KEKCallbackData data;
		data.value = 1;
		reader->RegisterKEKClient("LIB3MF#TEST", KEKCallbackData::testKEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
	}







	//
	// End to end tests
	//

	TEST_F(SecureContentT, End2EndSecureContent) {
		auto secureModel = wrapper->CreateModel();
		//create the attachment to be secured
		std::string path = "/3D/securemesh.xml";
		//add a mesh
		auto meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		//set the mesh apart of the main root model
		auto modelPath = meshObject->PackagePath();
		modelPath->Set(path);

		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}

		secureModel->AddBuildItem(meshObject.get(), transformation);

		auto keyStore = secureModel->GetKeyStore();
		//create a consumer (optional)
		auto consumer = keyStore->AddConsumer("LIB3MF#TEST", "", NULL);
		//create a resource data
		auto resourceData = keyStore->AddResourceData(modelPath.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);
		//add decryptright for the consumer (optional)
		auto decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);

		//Query writer
		auto writer = secureModel->QueryWriter("3mf");
		//register the consumer key encryption callback (optional)
		//writer->RegisterKEKClient("LIB3MF#TEST", EncryptionData::keyEncryptionCallback, nullptr);
		//register the data encryption callback
		//writer->RegisterEncryption(EncryptionData::dataEncryptionCallback);

		//write content
		Lib3MF_buffer buffer;
		writer->WriteToBuffer(buffer);
	}


}
