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
		static PWrapper wrapper;

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

			PConsumer consumerNotFound = keyStore->FindConsumer("does not exist");
			ASSERT_EQ(nullptr, consumerNotFound);
			PConsumer consumerFound = keyStore->FindConsumer(consumer->GetConsumerID());
			ASSERT_EQ(consumer->GetConsumerID(), consumerFound->GetConsumerID());

			std::string expected = "";
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

		PResourceData resourceDataNotFound = keyStore->FindResourceData("does not exist");
		ASSERT_EQ(nullptr, resourceDataNotFound);

		for (int i = 0; i < resourceDataCount; ++i) {
			PResourceData resourceData = keyStore->GetResourceData(i);
			ASSERT_TRUE(resourceData != nullptr);

			PResourceData resourceDataFound = keyStore->FindResourceData(resourceData->GetPath()->Get());
			ASSERT_TRUE(resourceDataFound != nullptr);

			ASSERT_EQ(resourceData->GetPath(), resourceDataFound->GetPath());

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

	struct CallbackData {
		int value;
		std::vector<Lib3MF_uint64> context;
		/**
		* DataDecryptionCallback - A description
		*
		* @param[in] eEncryptionAlgorithm - The encryption algorithm to be used
		* @param[in] pCipherData - The plain data encryption key properly decrypted. Buffer will have same size as CipherValue in decryptright
		* @param[in] nCipherBufferSize - Number of elements in buffer
		* @param[in] pCipherBuffer - uint8 buffer of Pointer to the cipher data buffer
		* @param[in] nPlainBufferSize - Number of elements in buffer
		* @param[in] pPlainBuffer - uint8 buffer of Pointer to the plain data buffer
		* @param[in] pUserData - Userdata that is passed to the callback function
		*/
		static void testDEKCallback(
			Lib3MF::eEncryptionAlgorithm algorithm, 
			Lib3MF_CipherData cipherData, 
			Lib3MF_uint64 cipherSize, 
			const Lib3MF_uint8 * cipherBuffer, 
			Lib3MF_uint64 plainSize, 
			const Lib3MF_uint8 * plainBuffer, 
			Lib3MF_pvoid userData) {

			CallbackData * cb = reinterpret_cast<CallbackData *>(userData);
			ASSERT_EQ(cb->value, 1);
			
			CCipherData * c = reinterpret_cast<CCipherData *>(cipherData);
			Lib3MF_uint64 descriptor = c->GetDescriptor();
			ASSERT_EQ(descriptor, 1);
			
			sAes256CipherData cipher = c->GetAes256Gcm();
			//TODO assert cipher somehow
			
			cb->context.push_back(descriptor);
		}
	};

	TEST_F(SecureContentT, DEKCallbackTest) {
		auto reader = model->QueryReader("3mf");
		CallbackData data;
		data.value = 1;
		reader->RegisterDEKClient(CallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
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
