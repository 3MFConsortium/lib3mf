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

	TEST_F(EncryptionMethods, ReadEncrypted3MF) {
		auto reader = model->QueryReader("3mf");

		DekContext dekUserData;
		dekUserData.wrapper = wrapper.get();
		reader->RegisterDEKClient(EncryptionCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
	}
	
	TEST_F(EncryptionMethods, ReadEncryptedCompressed3MF) {
		auto reader = model->QueryReader("3mf");

		DekContext dekUserData;
		dekUserData.wrapper = wrapper.get();
		reader->RegisterDEKClient(EncryptionCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

		KekContext kekUserData;
		kekUserData.key = privateKey.get();
		kekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
		reader->ReadFromFile(sTestFilesPath + "/SecureContent/keystore_encrypted_compressed.3mf");

		auto resources = model->GetResources();
		ASSERT_EQ(28, resources->Count());
	}
	
	//TODO: create new encrypted model with a mesh, save it. Read, and do asserts
	TEST_F(EncryptionMethods, WriteEncryptedProductionModel) {
		Lib3MF::PModel secureModel = wrapper->CreateModel();
		Lib3MF::PMeshObject meshObject = secureModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}
		secureModel->AddBuildItem(meshObject.get(), transformation);

		std::string path = "/3D/securemesh.model";
		auto secureMeshPart = secureModel->FindOrCreatePackagePart(path);
		meshObject->SetPackagePart(secureMeshPart.get());

		Lib3MF::PKeyStore keyStore = secureModel->GetKeyStore();
		keyStore->SetUUID("b7aa9c75-5fbd-48c1-a893-40289e45ab8f");
		std::string keyValue = "-----BEGIN PUBLIC KEY-----\r\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw53q4y2KB2WcoOBUE9OE\r\nXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP\r\n0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCT\r\nCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWza\r\nVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFe\r\nnX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTV\r\nBwIDAQAB\r\n-----END PUBLIC KEY-----\r\n\t\t";
		std::string keyId = "KEK_xxx";
		std::string consumerId = "HP#MOP44B#SG5693454";
		Lib3MF::PConsumer consumer = keyStore->AddConsumer(consumerId, keyId, keyValue);
		Lib3MF::PResourceData resourceData = keyStore->AddResourceData(secureMeshPart.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);
		Lib3MF::PDecryptRight decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);		

		PWriter writer = secureModel->QueryWriter("3mf");

		DekContext writeDekUserData;
		writeDekUserData.wrapper = wrapper.get();
		writer->RegisterDEKClient(EncryptionCallbacks::dataEncryptClientCallback, (Lib3MF_pvoid)(&writeDekUserData));
		
		KekContext writeKekUserData;
		writeKekUserData.key = privateKey.get();
		writeKekUserData.size = RsaMethods::getSize(privateKey);
		writer->RegisterKEKClient(consumerId, EncryptionCallbacks::keyEncryptClientCallback, (Lib3MF_uint32)writeKekUserData.size, &writeKekUserData);

		ByteVector buffer;
		writer->WriteToBuffer(buffer);

		//READ PART
		auto reader = model->QueryReader("3mf");

		DekContext readDekUserData;
		readDekUserData.wrapper = wrapper.get();
		reader->RegisterDEKClient(EncryptionCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&readDekUserData));

		KekContext readKekUserData;
		readKekUserData.key = privateKey.get();
		readKekUserData.size = RsaMethods::getSize(privateKey);
		reader->RegisterKEKClient(consumerId, EncryptionCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)readKekUserData.size, &readKekUserData);
		reader->ReadFromBuffer(buffer);
		auto meshObj = model->GetMeshObjects();
		ASSERT_EQ(1, meshObj->Count());
	}



	TEST_F(EncryptionMethods, WriteAdditionalConsumerToEncryptedModel) {
		ByteVector buffer;
		{
			//read existing model
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			dekUserData.wrapper = wrapper.get();
			reader->RegisterDEKClient(EncryptionCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

			KekContext kekUserData;
			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);
			reader->RegisterKEKClient("LIB3MF#TEST", EncryptionCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
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

			writer->RegisterDEKClient(EncryptionCallbacks::dataEncryptClientCallback, &dekUserData);

			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);

			writer->RegisterKEKClient("LIB3MF#TEST2", EncryptionCallbacks::keyEncryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
			writer->WriteToBuffer(buffer);
		}
		//reset model
		model.reset();
		model = wrapper->CreateModel();
		{
			//read generated model
			auto reader = model->QueryReader("3mf");
			DekContext dekUserData;
			dekUserData.wrapper = wrapper.get();
			reader->RegisterDEKClient(EncryptionCallbacks::dataDecryptClientCallback, reinterpret_cast<Lib3MF_pvoid>(&dekUserData));

			KekContext kekUserData;
			kekUserData.key = privateKey.get();
			kekUserData.size = RsaMethods::getSize(privateKey);
			reader->RegisterKEKClient("LIB3MF#TEST2", EncryptionCallbacks::keyDecryptClientCallback, (Lib3MF_uint32)kekUserData.size, &kekUserData);
			reader->ReadFromBuffer(buffer);

			auto meshObjIt = model->GetMeshObjects();
			ASSERT_EQ(meshObjIt->Count(), 1);

			auto keyStore = model->GetKeyStore();
			ASSERT_EQ(keyStore->GetConsumerCount(), 2);
			ASSERT_EQ(keyStore->GetResourceDataCount(), 1);
		}
	}
}
