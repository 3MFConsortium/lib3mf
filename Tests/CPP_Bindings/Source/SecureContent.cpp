#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

namespace Lib3MF {

	class SecureContentT : public ::testing::Test {
	protected:
		sPosition pVertices[8];
		sTriangle pTriangles[12];

		PModel model;

		const std::string UNENCRYPTEDKEYSTORE = "/SecureContent/keystore.3mf";
		const std::string UNENCRYPTEDCOMPRESSEDKEYSTORE = "/SecureContent/keystore_compressed.3mf";
		const std::string NEGATIVEKEYSTOREENCRYPTEDEMPTY = "/SecureContent/negative_keystore_encrypted_empty.3mf";
		const std::string NEGATIVEKEYSTOREENCRYPTEDMISSINGATTRIBUTES = "/SecureContent/negative_keystore_encrypted_missing_attributes.3mf";
		const std::string NEGATIVEKEYSTOREENCRYPTEDINVALIDATTRIBUTES = "/SecureContent/negative_keystore_encrypted_invalid_attributes.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREEMPTY = "/SecureContent/negative_keystore_empty.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREMISSINGATTRIBUTES = "/SecureContent/negative_keystore_missing_attributes.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREINVALIDATTRIBUTES = "/SecureContent/negative_keystore_invalid_attributes.3mf";
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

		PReader readKeyStore(std::string filename) {
			auto reader = model->QueryReader("3mf");
			reader->ReadFromFile(sTestFilesPath + filename);
			return reader;
		}

		PReader readUnencryptedKeyStore() {
			return readKeyStore(UNENCRYPTEDKEYSTORE);
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

		keyStore->RemoveConsumer(consumer1.get());
		ASSERT_EQ(1, keyStore->GetConsumerCount());
		ASSERT_EQ(secondId, keyStore->GetConsumer(0)->GetConsumerID());
	}

	TEST_F(SecureContentT, CreateMultipleResourceDataKeyStore) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);

		std::string path1 = "/3D/nonrootmodel1.model";
		auto part1 = model->FindOrCreatePackagePart(path1);

		keyStore->AddResourceData(part1.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);

		std::string path2 = "/3D/nonrootmodel2.model";
		auto part2 = model->FindOrCreatePackagePart(path2);

		keyStore->AddResourceData(part2.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		ASSERT_EQ(2, keyStore->GetResourceDataCount());
		ASSERT_EQ(path1, keyStore->GetResourceData(0)->GetPath()->Get());
		ASSERT_EQ(path2, keyStore->GetResourceData(1)->GetPath()->Get());
	}


	TEST_F(SecureContentT, ManageDecryptRight) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);

		std::string path1 = "/3D/nonrootmodel1.model";
		auto part1 = model->FindOrCreatePackagePart(path1);

		keyStore->AddResourceData(part1.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);

		std::string path2 = "/3D/nonrootmodel2.model";
		auto part2 = model->FindOrCreatePackagePart(path2);

		keyStore->AddResourceData(part2.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("consumerId1", "consumerKeyId1", "consumerKeyValue1");
		Lib3MF::PConsumer consumer2 = keyStore->AddConsumer("consumerId2", "consumerKeyId2", "consumerKeyValue2");

		ASSERT_EQ(2, keyStore->GetConsumerCount());
		ASSERT_EQ("consumerId1", keyStore->GetConsumer(0)->GetConsumerID());
		ASSERT_EQ("consumerId2", keyStore->GetConsumer(1)->GetConsumerID());

		//Test add decrypt right to different resource data

		keyStore->GetResourceData(0)->AddDecryptRight(consumer1.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
		keyStore->GetResourceData(1)->AddDecryptRight(consumer2.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
				
		ASSERT_EQ(1, keyStore->GetResourceData(0)->GetDecryptRightCount());
		ASSERT_EQ(consumer1->GetConsumerID(), keyStore->GetResourceData(0)->GetDecryptRight(0)->GetConsumer()->GetConsumerID());
		ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p, keyStore->GetResourceData(0)->GetDecryptRight(0)->GetEncryptionAlgorithm());
		
		ASSERT_EQ(1, keyStore->GetResourceData(1)->GetDecryptRightCount());
		ASSERT_EQ(consumer2->GetConsumerID(), keyStore->GetResourceData(1)->GetDecryptRight(0)->GetConsumer()->GetConsumerID());
		ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p, keyStore->GetResourceData(1)->GetDecryptRight(0)->GetEncryptionAlgorithm());

		//Test remove decrypt right from resource data using a consumer
		keyStore->GetResourceData(0)->RemoveDecrypt(consumer1.get());

		ASSERT_EQ(0, keyStore->GetResourceData(0)->GetDecryptRightCount());
		
		//Test remove consumer that should also delete every decrypt right associated 
		keyStore->RemoveConsumer(consumer2.get());
		ASSERT_EQ(1, keyStore->GetConsumerCount());
		ASSERT_EQ(0, keyStore->GetResourceData(1)->GetDecryptRightCount());
	}





	//
	//Keystore test which require preexisting model
	//

	TEST_F(SecureContentT, ModelReaderKeyStoreNoWarnings) {
		PReader reader3MF = readUnencryptedKeyStore();
		CheckReaderWarnings(reader3MF, 0);
	}

	TEST_F(SecureContentT, ModelReaderEmptyKeyStoreWarnings) {
		PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREEMPTY);
		CheckReaderWarnings(reader3MF, 1);
		Lib3MF_uint32 iWarning = 0;
		
		// NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX
		// consumer index is higher than consumer count
		reader3MF->GetWarning(0, iWarning);
		ASSERT_EQ(0x80F6, iWarning);
	}

	TEST_F(SecureContentT, ModelReaderKeyStoreNoAttributesWarnings) {
		PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREMISSINGATTRIBUTES);
		CheckReaderWarnings(reader3MF, 4);
		Lib3MF_uint32 iWarning = 0;

		// NMR_ERROR_KEYSTOREMISSINGCONSUMERID
		// consumer id is not defined
		reader3MF->GetWarning(0, iWarning);
		ASSERT_EQ(0x80F3, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDCIPHERVALUE 
		// cipher value is not there
		reader3MF->GetWarning(1, iWarning);
		ASSERT_EQ(0x80F9, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX
		// consumer index is not defined
		reader3MF->GetWarning(2, iWarning);
		ASSERT_EQ(0x80F6, iWarning);

		// NMR_ERROR_MISSINGUUID
		// no UUID attribute in <keystore>
		reader3MF->GetWarning(3, iWarning);
		ASSERT_EQ(0x80B0, iWarning);
	}

	TEST_F(SecureContentT, ModelReaderKeyStoreInvalidAttributesWarnings) {
		PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREINVALIDATTRIBUTES);
		CheckReaderWarnings(reader3MF, 5);
		Lib3MF_uint32 iWarning = 0;

		// NMR_ERROR_KEYSTOREDUPLICATECONSUMERID
		// consumer has multiple consumerid attributes
		reader3MF->GetWarning(0, iWarning);
		ASSERT_EQ(0x80F1, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCRYPTIONALGORITHM 
		// invalid encryptionalgorithm attribute in ResourceData
		reader3MF->GetWarning(1, iWarning);
		ASSERT_EQ(0x80F7, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDCOMPRESSION 
		// invalid compression attribute in ResourceData
		reader3MF->GetWarning(2, iWarning);
		ASSERT_EQ(0x80F8, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCRYPTIONALGORITHM
		// invalid encryptionalgorithm attribute in DecryptRight
		reader3MF->GetWarning(3, iWarning);
		ASSERT_EQ(0x80F7, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDCIPHERVALUE 
		// cipher value is not there
		reader3MF->GetWarning(4, iWarning);
		ASSERT_EQ(0x80F9, iWarning);
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

			std::string expected = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----";
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
			try {
				PDecryptRight invalidDecryptRight = resourceData->GetDecryptRight(inexistantDecrypt);
				ASSERT_FALSE(true);
			}
			catch (ELib3MFException const & e) {
				ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_INVALIDPARAM);
			}
			

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
		std::map<Lib3MF_uint64, Lib3MF_uint64> context;

		static void testDEKCallback(
			Lib3MF::eEncryptionAlgorithm algorithm,
			Lib3MF_CipherData cipherData,
			Lib3MF_uint64 inSize,
			const Lib3MF_uint8 * inBuffer,
			const Lib3MF_uint64 outSize,
			Lib3MF_uint64 * outNeededSize,
			Lib3MF_uint8 * outBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 * result) {

			CCipherData cd(SecureContentT::wrapper.get(), cipherData);
			SecureContentT::wrapper->Acquire(&cd);

			ASSERT_GE(cd.GetDescriptor(), 1);
			if (nullptr != userData) {
				DEKCallbackData * cb = reinterpret_cast<DEKCallbackData *>(userData);
				auto localDescriptor = cb->context.find(cd.GetDescriptor());
				if (localDescriptor != cb->context.end())
					localDescriptor->second++;
				else
					cb->context[cd.GetDescriptor()] = 0;
			}

			sAes256CipherValue cipher = cd.GetAes256Gcm();
			if (0 != inSize) {
				std::copy(inBuffer, inBuffer + outSize, outBuffer);
			} else {
				Lib3MF_uint8 fakeTag[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
				std::copy(fakeTag, fakeTag + 16, cipher.m_Tag);
				cd.SetAes256Gcm(cipher);
			}

			*result = outSize;
		}
	};

	TEST_F(SecureContentT, DEKReadTest) {
		auto reader = model->QueryReader("3mf");
		DEKCallbackData data;
		reader->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}
	

	TEST_F(SecureContentT, ReadCompressedTest) {
		auto reader = model->QueryReader("3mf");
		DEKCallbackData data;
		reader->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDCOMPRESSEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}

	TEST_F(SecureContentT, DEKWriteTest) {
		readUnencryptedKeyStore();
		auto writer = model->QueryWriter("3mf");
		DEKCallbackData data;
		writer->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&data));
		writer->WriteToFile(sOutFilesPath + UNENCRYPTEDCOMPRESSEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}


	struct KEKCallbackData {
		int value;
		std::string consumerId;
		std::string keyId;

		static void testKEKCallback(
			Lib3MF_Consumer pConsumer,
			Lib3MF::eEncryptionAlgorithm algorithm,
			Lib3MF_uint64 inSize,
			const Lib3MF_uint8 * inBuffer,
			const Lib3MF_uint64 outSize,
			Lib3MF_uint64 * outNeeded,
			Lib3MF_uint8 * outBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 *result) {


			CConsumer c(SecureContentT::wrapper.get(), pConsumer);
			SecureContentT::wrapper->Acquire(&c);

			if (nullptr != userData) {
				KEKCallbackData * cb = reinterpret_cast<KEKCallbackData *>(userData);
				ASSERT_EQ(cb->value, 1);
				cb->value = 2;

				ASSERT_EQ(c.GetConsumerID(), cb->consumerId);

				ASSERT_EQ(c.GetKeyID(), cb->keyId);
			}

			ASSERT_FALSE(c.GetKeyValue().empty());

			Lib3MF_uint32 needed = 0;
			std::vector<char> buffer;

			std::copy(inBuffer, inBuffer + outSize, outBuffer);
			*result = outSize;
		}
	};

	TEST_F(SecureContentT, KEKReadTest) {
		auto reader = model->QueryReader("3mf");
		KEKCallbackData data;
		data.value = 1;
		data.consumerId = "LIB3MF#TEST";
		data.keyId = "contentKey";
		reader->RegisterKEKClient(data.consumerId, KEKCallbackData::testKEKCallback, 256, reinterpret_cast<Lib3MF_pvoid>(&data));
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(2, data.value);
	}


	TEST_F(SecureContentT, KEKWriteTest) {
		readUnencryptedKeyStore();
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);
		ASSERT_EQ(keyStore->GetConsumerCount(), 1);
		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("consumerId", "contentKey", "consumerKeyValue");
		Lib3MF::PResourceData rd = keyStore->GetResourceData(0);
		Lib3MF::PDecryptRight dr = rd->AddDecryptRight(consumer1.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
		
		auto writer = model->QueryWriter("3mf");
		
		DEKCallbackData contentData;
		writer->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&contentData));

		KEKCallbackData wrappingData;
		wrappingData.value = 1;
		wrappingData.consumerId = "consumerId";
		wrappingData.keyId = "contentKey";
		writer->RegisterKEKClient(wrappingData.consumerId, KEKCallbackData::testKEKCallback, 256, reinterpret_cast<Lib3MF_pvoid>(&wrappingData));
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		ASSERT_EQ(2, wrappingData.value);
	}

	TEST_F(SecureContentT, KEKWriteTestUnregisteredConsumer) {
		readUnencryptedKeyStore();
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);
		
		ASSERT_TRUE(keyStore->GetResourceDataCount() >= 1);

		Lib3MF::PResourceData rd = keyStore->GetResourceData(0);
		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("UnregisteredConsumer", "UnregisteredKey", "");
		Lib3MF::PDecryptRight dr = rd->AddDecryptRight(consumer1.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);

		auto writer = model->QueryWriter("3mf");
		DEKCallbackData contentData;
		writer->RegisterDEKClient(DEKCallbackData::testDEKCallback, reinterpret_cast<Lib3MF_pvoid>(&contentData));

		std::vector<Lib3MF_uint8> buffer;
		try {
			writer->WriteToBuffer(buffer);
			ASSERT_FALSE(true);
		} catch (Lib3MF::ELib3MFException const &e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_SECURECONTEXTNOTREGISTERED);
		}
	}


	//
	// End to end tests
	//

	TEST_F(SecureContentT, WriteKeyStore) {
		Lib3MF::PModel secureModel = wrapper->CreateModel();
		//create the attachment to be secured
		//add a mesh
		Lib3MF::PMeshObject meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}

		secureModel->AddBuildItem(meshObject.get(), transformation);

		//set the mesh apart of the main root model
		std::string path = "/3D/securemesh.model";
		auto secureMeshPart = secureModel->FindOrCreatePackagePart(path);
		meshObject->SetPackagePart(secureMeshPart.get());

		Lib3MF::PKeyStore keyStore = secureModel->GetKeyStore();
		
		keyStore->SetUUID("b7aa9c75-5fbd-48c1-a893-40289e45ab8f");
		//create a consumer (optional)
		std::string keyValue = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId = "KEK_xxx";
		std::string consumerId = "HP#MOP44B#SG5693454";
		Lib3MF::PConsumer consumer = keyStore->AddConsumer(consumerId, keyId, keyValue);
		//create a resource data
		Lib3MF::PResourceData resourceData = keyStore->AddResourceData(secureMeshPart.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);
		//add decryptright for the consumer (optional)
		Lib3MF::PDecryptRight decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
		
		//Query writer
		PWriter writer = secureModel->QueryWriter("3mf");

		writer->RegisterDEKClient(DEKCallbackData::testDEKCallback, nullptr);
		writer->RegisterKEKClient("HP#MOP44B#SG5693454", KEKCallbackData::testKEKCallback, 256, nullptr);

		std::vector<Lib3MF_uint8> buffer;
		//Write content
		writer->WriteToBuffer(buffer);
	}

	TEST_F(SecureContentT, WriteMultipleConsumersSecureContent) {
		Lib3MF::PModel secureModel = wrapper->CreateModel();
		//create the attachment to be secured
		//add a mesh
		Lib3MF::PMeshObject meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));

		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}

		secureModel->AddBuildItem(meshObject.get(), transformation);

		//set the mesh apart of the main root model
		std::string path = "/3D/securemesh.model";
		auto secureMeshPart = secureModel->FindOrCreatePackagePart(path);
		meshObject->SetPackagePart(secureMeshPart.get());

		Lib3MF::PKeyStore keyStore = secureModel->GetKeyStore();

		keyStore->SetUUID("b7aa9c75-5fbd-48c1-a893-40289e45ab8f");
		//create the first consumer
		std::string keyValue = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId = "KEK_xxx";
		std::string consumerId = "HP#MOP44B#SG5693454";
		Lib3MF::PConsumer consumer = keyStore->AddConsumer(consumerId, keyId, keyValue);

		//create the second consumer
		std::string keyValue2 = "-----BEGIN PUBLIC KEY-----\r\nPIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId2 = "KEK_xxx";
		std::string consumerId2 = "HP#MOP44B#SG5693455";
		Lib3MF::PConsumer consumer2 = keyStore->AddConsumer(consumerId2, keyId2, keyValue2);

		//create a resource data
		Lib3MF::PResourceData resourceData = keyStore->AddResourceData(secureMeshPart.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		//add decryptright for the consumer (optional)
		Lib3MF::PDecryptRight decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
		Lib3MF::PDecryptRight decryptRight2 = resourceData->AddDecryptRight(consumer2.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);

		//Query writer
		PWriter writer = secureModel->QueryWriter("3mf");

		writer->RegisterDEKClient(DEKCallbackData::testDEKCallback, nullptr);
		writer->RegisterKEKClient("HP#MOP44B#SG5693454", KEKCallbackData::testKEKCallback, 256, nullptr);
		writer->RegisterKEKClient("HP#MOP44B#SG5693455", KEKCallbackData::testKEKCallback, 256, nullptr);
		
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
	}

}
