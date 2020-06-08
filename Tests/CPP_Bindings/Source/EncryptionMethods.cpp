#include "UnitTest_Utilities.h"
#include "UnitTest_EncryptionUtils.h"
#include "lib3mf_implicit.hpp"

#include <vector>

namespace Lib3MF {

	class EncryptionMethods : public ::testing::Test {
	protected:
		PModel model;
		PEVP_PKEY privateKey;
		sPosition pVertices[8];
		sTriangle pTriangles[12];
		std::string publicKey;
		
	public:
		EncryptionMethods(): privateKey(nullptr, ::EVP_PKEY_free) {
			ByteVector key = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/sample.pem");
			privateKey = RsaMethods::loadPrivateKey(key);
			ByteVector	pubKey = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/sample.pub.pem");
			publicKey = std::string(pubKey.begin(), pubKey.end());
			model = wrapper->CreateModel();
		}

		virtual ~EncryptionMethods() {
			//attempt to solve leaks
			EncryptionCallbacks::cleanup();
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

			model->SetRandomNumberCallback(EncryptionCallbacks::randomNumberCallback, nullptr);

		}

		virtual void TearDown() {
			model.reset();
		}

		void reset(DekContext & context) {
			context.ciphers.clear();
			context.wrapper = wrapper.get();
		}

		void reset(KekContext & context) {
			context.key = privateKey.get();
			context.wrapper = wrapper.get();
		}


		void generateTestFile(bool compressed, std::string const & fileName) {
			ByteVector buffer;
			{
				PModel modelToCrpt = wrapper->CreateModel();
				modelToCrpt->SetRandomNumberCallback(EncryptionCallbacks::randomNumberCallback, nullptr);

				PReader reader = modelToCrpt->QueryReader("3mf");
				reader->ReadFromFile(sTestFilesPath + "/Production/detachedmodel.3mf");

				auto meshObjIt = modelToCrpt->GetMeshObjects();
				ASSERT_EQ(meshObjIt->Count(), 1);
				ASSERT_TRUE(meshObjIt->MoveNext());
				auto meshObj = meshObjIt->GetCurrentMeshObject();
				ASSERT_NE(nullptr, meshObj);
				auto part = meshObj->PackagePart();
				ASSERT_NE(nullptr, meshObj);
				auto keyStore = modelToCrpt->GetKeyStore();
				auto consumer = keyStore->AddConsumer("LIB3MF#TEST", "contentKey", publicKey);
				auto rdGroup = keyStore->AddResourceDataGroup();
				rdGroup->AddAccessRight(consumer.get(),
					eWrappingAlgorithm::RSA_OAEP,
					eMgfAlgorithm::MGF1_SHA1,
					eDigestMethod::SHA1);

				ByteVector aad = { 'l', 'i', 'b', '3', 'm', 'f', 's', 'a', 'm', 'p', 'l', 'e' };
				auto rd = keyStore->AddResourceData(rdGroup.get(), part.get(),
					eEncryptionAlgorithm::AES256_GCM, (compressed? eCompression::Deflate : eCompression::NoCompression), aad);

				PWriter writer = modelToCrpt->QueryWriter("3mf");
				DekContext dekUserData;
				reset(dekUserData);
				writer->SetContentEncryptionCallback(EncryptionCallbacks::dataEncryptClientCallback, (Lib3MF_pvoid)(&dekUserData));

				KekContext kekUserData;
				reset(kekUserData);
				writer->AddKeyWrappingCallback("LIB3MF#TEST", EncryptionCallbacks::keyEncryptClientCallback, &kekUserData);

				writer->WriteToBuffer(buffer);

				auto count = writer->GetWarningCount();
				ASSERT_EQ(count, 0);
				//WriteBufferToFile(buffer, sOutFilesPath + "/SecureContent/" + fileName);
			}
			{
				PModel encryptedModel = wrapper->CreateModel();
				PReader reader = encryptedModel->QueryReader("3mf");
				DekContext dekUserData;
				reset(dekUserData);
				reader->SetContentEncryptionCallback(EncryptionCallbacks::dataDecryptClientCallback, (Lib3MF_pvoid)(&dekUserData));

				KekContext kekUserData;
				reset(kekUserData);
				reader->AddKeyWrappingCallback("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, &kekUserData);

				reader->ReadFromBuffer(buffer);

				auto meshObjIt = encryptedModel->GetMeshObjects();
				ASSERT_EQ(meshObjIt->Count(), 1);
				auto objCount = encryptedModel->GetObjects();
				ASSERT_EQ(objCount->Count(), 28);
			}
		}
	};

	PWrapper EncryptionMethods::wrapper;
	
	TEST_F(EncryptionMethods, RemoveEncryptionFromResource) {

		auto reader = model->QueryReader("3mf");
		//set descryption callbacks in the reader
		DekContext dekUserData;
		reset(dekUserData);
		reader->SetContentEncryptionCallback(EncryptionCallbacks::dataDecryptClientCallback, (Lib3MF_pvoid)(&dekUserData));

		KekContext kekUserData;
		reset(kekUserData);
		reader->AddKeyWrappingCallback("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, &kekUserData);

		//do the actual reading
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted_compressed.3mf");

		//assert model assumptions
		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
		
		auto keyStore = model->GetKeyStore();
		ASSERT_EQ(1, keyStore->GetResourceDataGroupCount());
		ASSERT_EQ(1, keyStore->GetResourceDataCount());

		//find the target resource data and remove it from keystore
		Lib3MF::PPackagePart part = model->FindOrCreatePackagePart("/3D/3dexternal.model");
		auto resourceData = keyStore->FindResourceData(part.get());
		ASSERT_NE(nullptr, resourceData);
		keyStore->RemoveResourceData(resourceData.get());

		//write model again
		PWriter writer = model->QueryWriter("3mf");
		ByteVector buffer;
		writer->WriteToBuffer(buffer);
	}
	
	TEST_F(EncryptionMethods, WriteEncryptedProductionModel) {
		Lib3MF::PModel secureModel = wrapper->CreateModel();
		secureModel->SetRandomNumberCallback(EncryptionCallbacks::randomNumberCallback, nullptr);

		//create a mesh and add to the model
		Lib3MF::PMeshObject meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}
		secureModel->AddBuildItem(meshObject.get(), transformation);

		//set the mesh in a non root model
		std::string path = "/3D/securemesh.model";
		auto secureMeshPart = secureModel->FindOrCreatePackagePart(path);
		meshObject->SetPackagePart(secureMeshPart.get());

		Lib3MF::PKeyStore keyStore = secureModel->GetKeyStore();
		keyStore->SetUUID("b7aa9c75-5fbd-48c1-a893-40289e45ab8f");

		//add a consumer
		std::string keyValue = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId = "KEK_xxx";
		std::string consumerId = "HP#MOP44B#SG5693454";
		Lib3MF::PConsumer consumer = keyStore->AddConsumer(consumerId, keyId, keyValue);

		//add a resource data group
		Lib3MF::PResourceDataGroup dataGroup = keyStore->AddResourceDataGroup();

		//establish consumer access to the datagroup
		Lib3MF::PAccessRight accessRight = dataGroup->AddAccessRight(
			consumer.get(),
			Lib3MF::eWrappingAlgorithm::RSA_OAEP,
			Lib3MF::eMgfAlgorithm::MGF1_SHA1,
			Lib3MF::eDigestMethod::SHA1); 
		
		
		ByteVector aad = { 1, 2 , 3, 4, 5, 6, 7, 8, 9, 10 };
		//add resource data as part of the datagroup
		Lib3MF::PResourceData resourceData = keyStore->AddResourceData(
			dataGroup.get(),
			secureMeshPart.get(), 
			Lib3MF::eEncryptionAlgorithm::AES256_GCM, 
			Lib3MF::eCompression::Deflate, 
			aad);


		PWriter writer = secureModel->QueryWriter("3mf");
		//set encryption callbacks
		DekContext writeDekUserData;
		reset(writeDekUserData);
		writer->SetContentEncryptionCallback(EncryptionCallbacks::dataEncryptClientCallback, (Lib3MF_pvoid)(&writeDekUserData));
		
		KekContext writeKekUserData;
		reset(writeKekUserData);
		writer->AddKeyWrappingCallback(consumerId, EncryptionCallbacks::keyEncryptClientCallback, &writeKekUserData);

		//do the actual writing
		ByteVector buffer;
		writer->WriteToBuffer(buffer);

		//re-read the model
		auto reader = model->QueryReader("3mf");
		//sets decryption callbacks
		DekContext readDekUserData;
		reset(readDekUserData);
		reader->SetContentEncryptionCallback(EncryptionCallbacks::dataDecryptClientCallback, (Lib3MF_pvoid)(&readDekUserData));

		KekContext readKekUserData;
		reset(readKekUserData);
		reader->AddKeyWrappingCallback(consumerId, EncryptionCallbacks::keyDecryptClientCallback, &readKekUserData);

		//do the actual reading
		reader->ReadFromBuffer(buffer);

		//assert assumptions
		auto meshObj = model->GetMeshObjects();
		ASSERT_EQ(1, meshObj->Count());
	}



	TEST_F(EncryptionMethods, WriteAdditionalConsumerToEncryptedModel) {
		ByteVector buffer;
		{
			//read existing model
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			reset(dekUserData);
			reader->SetContentEncryptionCallback(EncryptionCallbacks::dataDecryptClientCallback, (Lib3MF_pvoid)(&dekUserData));

			KekContext kekUserData;
			reset(kekUserData);
			reader->AddKeyWrappingCallback("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, &kekUserData);

			//do the actual reading
			reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

			//assert model assumptions
			auto meshObjIt = model->GetMeshObjects();
			ASSERT_EQ(meshObjIt->Count(), 1);

			auto keyStore = model->GetKeyStore();
			ASSERT_EQ(keyStore->GetConsumerCount(), 1);
			ASSERT_EQ(keyStore->GetResourceDataCount(), 1);
			ASSERT_EQ(keyStore->GetResourceDataGroupCount(), 1);

			auto dataGroup = keyStore->GetResourceDataGroup(0);
			auto existingConsumer = keyStore->GetConsumer(0);
			auto existingAccess = dataGroup->FindAccessRightByConsumer(existingConsumer.get());

			ASSERT_NE(existingAccess, nullptr);

			//add new consumer
			auto consumer = keyStore->AddConsumer("LIB3MF#TEST2", "", "");


			//add rights for the consumer to the existing datagroup
			Lib3MF::PAccessRight accessRight = dataGroup->AddAccessRight(
				consumer.get(),
				Lib3MF::eWrappingAlgorithm::RSA_OAEP,
				Lib3MF::eMgfAlgorithm::MGF1_SHA1,
				Lib3MF::eDigestMethod::SHA1);
			//write new model
			auto writer = model->QueryWriter("3mf");
			reset(dekUserData);
			writer->SetContentEncryptionCallback(EncryptionCallbacks::dataEncryptClientCallback, &dekUserData);
			reset(kekUserData);
			writer->AddKeyWrappingCallback("LIB3MF#TEST2", EncryptionCallbacks::keyEncryptClientCallback, &kekUserData);
			//do the actual writing
			writer->WriteToBuffer(buffer);
		}

		//reset model for assertion of assumption
		model.reset();

		model = wrapper->CreateModel();
		model->SetRandomNumberCallback(EncryptionCallbacks::randomNumberCallback, nullptr);
		{
			//read generated model 
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			reset(dekUserData);
			reader->SetContentEncryptionCallback(EncryptionCallbacks::dataDecryptClientCallback, (Lib3MF_pvoid)(&dekUserData));

			KekContext kekUserData;
			reset(kekUserData);
			reader->AddKeyWrappingCallback("LIB3MF#TEST2", EncryptionCallbacks::keyDecryptClientCallback, &kekUserData);
			reader->ReadFromBuffer(buffer);

			//assert assumptions
			auto meshObjIt = model->GetMeshObjects();
			ASSERT_EQ(meshObjIt->Count(), 1);

			auto keyStore = model->GetKeyStore();
			ASSERT_EQ(keyStore->GetConsumerCount(), 2);
			ASSERT_EQ(keyStore->GetResourceDataGroupCount(), 1);
			auto dataGroup = keyStore->GetResourceDataGroup(0);
			for (Lib3MF_uint32 i = 0; i < keyStore->GetConsumerCount(); ++i) {
				auto c = keyStore->GetConsumer(i);
				auto a = dataGroup->FindAccessRightByConsumer(c.get());
				ASSERT_NE(a, nullptr);
			}
		}
	}

	TEST_F(EncryptionMethods, MakeExistingModelEncrypted) {
		generateTestFile(false, "keystore_encrypted.3mf");
		generateTestFile(true, "keystore_encrypted_compressed.3mf");
	}


}
