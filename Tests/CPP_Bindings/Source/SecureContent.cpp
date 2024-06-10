#include "UnitTest_Utilities.h"
#include "lib3mf_implicit.hpp"

#include <algorithm>
#include <cctype>

namespace Lib3MF {

	class SecureContentT : public Lib3MFTest {
	protected:
		sPosition pVertices[8];
		sTriangle pTriangles[12];
		std::string publicKey;

		PModel model;

		const std::string UNENCRYPTEDKEYSTORE = "/SecureContent/keystore.3mf";
		const std::string UNENCRYPTEDCOMPRESSEDKEYSTORE = "/SecureContent/keystore_compressed.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREEMPTY = "/SecureContent/negative_keystore_empty.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREMISSINGATTRIBUTES = "/SecureContent/negative_keystore_missing_attributes.3mf";
		const std::string NEGATIVEUNENCRYPTEDKEYSTOREINVALIDATTRIBUTES = "/SecureContent/negative_keystore_invalid_attributes.3mf";
	protected:
		SecureContentT()
		{}

		virtual void SetUp() {
			std::vector<Lib3MF_uint8> pubKey = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/sample.pub.pem");
			publicKey = std::string(pubKey.begin(), pubKey.end());

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
			model->SetRandomNumberCallback(notRandomBytesAtAll, nullptr);
		}

		virtual void TearDown() {
			model.reset();
		}

		PReader readKeyStore(std::string filename) {
			auto reader = model->QueryReader("3mf");
			KEKCallbackData kekData;
			kekData.value = 1;
			kekData.consumerId = "LIB3MF#TEST";
			kekData.keyId = "contentKey";
			reader->AddKeyWrappingCallback(kekData.consumerId, testKEKCallback, (Lib3MF_pvoid)&kekData);

			DEKCallbackData dekData;
			reader->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&dekData);

			reader->ReadFromFile(sTestFilesPath + filename);
			return reader;
		}

		PReader readUnencryptedKeyStore() {
			return readKeyStore(UNENCRYPTEDKEYSTORE);
		}
	public:
		static void notRandomBytesAtAll(Lib3MF_uint64 byteData, Lib3MF_uint64 size, Lib3MF_pvoid userData, Lib3MF_uint64 * bytesWritten) {
			static Lib3MF_uint8 random = 0;
			Lib3MF_uint8 * buffer = (Lib3MF_uint8 *)byteData;
			*bytesWritten = size;
			while (size > 0)
				*(buffer + (--size)) = ++random;
		}

		struct KEKCallbackData {
			int value;
			std::string consumerId;
			std::string keyId;
		};

		struct DEKCallbackData {
			std::map<Lib3MF_uint64, Lib3MF_uint64> context;
		};

		static void testKEKCallback(
			Lib3MF_AccessRight access,
			Lib3MF_uint64 inSize,
			const Lib3MF_uint8 * inBuffer,
			const Lib3MF_uint64 outSize,
			Lib3MF_uint64 * outNeeded,
			Lib3MF_uint8 * outBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 * status) {


			CAccessRight a(SecureContentT::wrapper.get(), access);
			SecureContentT::wrapper->Acquire(&a);

			PConsumer c = a.GetConsumer();

			ASSERT_NE(userData, nullptr);

			KEKCallbackData * cb = (KEKCallbackData *)userData;
			ASSERT_GE(cb->value, 1);
			cb->value++;

			ASSERT_EQ(c->GetConsumerID(), cb->consumerId);
			ASSERT_EQ(c->GetKeyID(), cb->keyId);
			ASSERT_FALSE(c->GetKeyValue().empty());

			if (nullptr == outBuffer || outSize == 0) {
				*outNeeded = inSize;
				*status = inSize;
			} else {
				std::copy(inBuffer, inBuffer + outSize, outBuffer);
				*status = outSize;
			}
		}

		static void testDEKCallback(
			Lib3MF_ContentEncryptionParams params,
			Lib3MF_uint64 inSize,
			const Lib3MF_uint8 * inBuffer,
			const Lib3MF_uint64 outSize,
			Lib3MF_uint64 * outNeededSize,
			Lib3MF_uint8 * outBuffer,
			Lib3MF_pvoid userData,
			Lib3MF_uint64 * status) {

			CContentEncryptionParams cd(SecureContentT::wrapper.get(), params);
			SecureContentT::wrapper->Acquire(&cd);

			ASSERT_GE(cd.GetDescriptor(), 1);
			ASSERT_NE(userData, nullptr);

			DEKCallbackData * cb = (DEKCallbackData *)userData;
			auto localDescriptor = cb->context.find(cd.GetDescriptor());
			if (localDescriptor != cb->context.end())
				localDescriptor->second++;
			else
				cb->context[cd.GetDescriptor()] = 0;

			if (0 == inSize || nullptr == inBuffer) {
				//finalize
				*status = 1;
			} else if (0 == outSize || nullptr == outBuffer) {
				//return size needed
				*outNeededSize = inSize;
				*status = inSize;
			} else {
				//perform encryption/decription process
				std::copy(inBuffer, inBuffer + outSize, outBuffer);
				*status = outSize;
			}
		}

		void generateTestFiles(bool compressed, std::string const & fileName) {
			std::vector<Lib3MF_uint8> buffer;
			{
				PModel modelToCrpt = wrapper->CreateModel();

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

				std::vector<Lib3MF_uint8> aad = { 'l', 'i', 'b', '3', 'm', 'f', 's', 'a', 'm', 'p', 'l', 'e' };
				auto rd = keyStore->AddResourceData(rdGroup.get(), part.get(),
					eEncryptionAlgorithm::AES256_GCM, (compressed ? eCompression::Deflate : eCompression::NoCompression), aad);

				PWriter writer = modelToCrpt->QueryWriter("3mf");
				DEKCallbackData contentData;
				writer->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&contentData);

				KEKCallbackData wrappingData;
				wrappingData.value = 1;
				wrappingData.consumerId = "LIB3MF#TEST";
				wrappingData.keyId = "contentKey";
				writer->AddKeyWrappingCallback(wrappingData.consumerId, testKEKCallback, (Lib3MF_pvoid)&wrappingData);

				writer->WriteToBuffer(buffer);

				auto count = writer->GetWarningCount();
				ASSERT_GE(count, 1U);
				Lib3MF_uint32 warning;
				writer->GetWarning(0, warning);
				// Using cryptographically weak random number generator
				ASSERT_EQ(warning, 0xA00C);
				//WriteBufferToFile(buffer, sOutFilesPath + "/SecureContent/" + fileName);
			}
			{
				PModel encryptedModel = wrapper->CreateModel();
				PReader reader = encryptedModel->QueryReader("3mf");
				DEKCallbackData dekData;
				reader->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&dekData);

				KEKCallbackData kekData;
				kekData.value = 1;
				kekData.consumerId = "LIB3MF#TEST";
				kekData.keyId = "contentKey";
				reader->AddKeyWrappingCallback(kekData.consumerId, testKEKCallback, (Lib3MF_pvoid)&kekData);

				reader->ReadFromBuffer(buffer);

				auto meshObjIt = encryptedModel->GetMeshObjects();
				ASSERT_EQ(meshObjIt->Count(), 1);
				auto objCount = encryptedModel->GetObjects();
				ASSERT_EQ(objCount->Count(), 28);
			}
		}
	};
	

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

		auto dataGroup = keyStore->AddResourceDataGroup();

		std::string dguuid = dataGroup->GetKeyUUID();
		ASSERT_FALSE(dguuid.empty());

		keyStore->AddResourceData(dataGroup.get(), part1.get(), Lib3MF::eEncryptionAlgorithm::AES256_GCM, Lib3MF::eCompression::Deflate, std::vector<Lib3MF_uint8>());

		std::string path2 = "/3D/nonrootmodel2.model";
		auto part2 = model->FindOrCreatePackagePart(path2);

		keyStore->AddResourceData(dataGroup.get(), part2.get(), Lib3MF::eEncryptionAlgorithm::AES256_GCM, Lib3MF::eCompression::Deflate, std::vector<Lib3MF_uint8>());

		ASSERT_EQ(2, keyStore->GetResourceDataCount());
		ASSERT_EQ(path1, keyStore->GetResourceData(0)->GetPath()->GetPath());
		ASSERT_EQ(path2, keyStore->GetResourceData(1)->GetPath()->GetPath());
	}


	TEST_F(SecureContentT, ManageAccessRight) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);

		std::string path1 = "/3D/nonrootmodel1.model";
		auto part1 = model->FindOrCreatePackagePart(path1);

		auto dataGroup1 = keyStore->AddResourceDataGroup();

		keyStore->AddResourceData(dataGroup1.get(), part1.get(), Lib3MF::eEncryptionAlgorithm::AES256_GCM, Lib3MF::eCompression::Deflate, std::vector<Lib3MF_uint8>());

		std::string path2 = "/3D/nonrootmodel2.model";
		auto part2 = model->FindOrCreatePackagePart(path2);

		auto dataGroup2 = keyStore->AddResourceDataGroup();

		keyStore->AddResourceData(dataGroup2.get(), part2.get(), Lib3MF::eEncryptionAlgorithm::AES256_GCM, Lib3MF::eCompression::Deflate, std::vector<Lib3MF_uint8>());

		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("consumerId1", "consumerKeyId1", "consumerKeyValue1");
		Lib3MF::PConsumer consumer2 = keyStore->AddConsumer("consumerId2", "consumerKeyId2", "consumerKeyValue2");

		//Test add decrypt right to different resource data

		dataGroup1->AddAccessRight(consumer1.get(), eWrappingAlgorithm::RSA_OAEP, eMgfAlgorithm::MGF1_SHA1, eDigestMethod::SHA1);
		dataGroup2->AddAccessRight(consumer2.get(), eWrappingAlgorithm::RSA_OAEP, eMgfAlgorithm::MGF1_SHA256, eDigestMethod::SHA256);

		auto accessRight1 = keyStore->GetResourceDataGroup(0)->FindAccessRightByConsumer(consumer1.get());
		ASSERT_NE(accessRight1, nullptr);
		ASSERT_EQ(consumer1->GetConsumerID(), accessRight1->GetConsumer()->GetConsumerID());
		ASSERT_EQ(Lib3MF::eWrappingAlgorithm::RSA_OAEP, accessRight1->GetWrappingAlgorithm());
		ASSERT_EQ(Lib3MF::eMgfAlgorithm::MGF1_SHA1, accessRight1->GetMgfAlgorithm());
		ASSERT_EQ(Lib3MF::eDigestMethod::SHA1, accessRight1->GetDigestMethod());

		auto accessRight2 = keyStore->GetResourceDataGroup(1)->FindAccessRightByConsumer(consumer2.get());
		ASSERT_NE(accessRight2, nullptr);
		ASSERT_EQ(consumer2->GetConsumerID(), accessRight2->GetConsumer()->GetConsumerID());
		ASSERT_EQ(Lib3MF::eWrappingAlgorithm::RSA_OAEP, accessRight2->GetWrappingAlgorithm());
		ASSERT_EQ(Lib3MF::eMgfAlgorithm::MGF1_SHA256, accessRight2->GetMgfAlgorithm());
		ASSERT_EQ(Lib3MF::eDigestMethod::SHA256, accessRight2->GetDigestMethod());

		//we can't find access right for consumer 1 on the second resource data group
		auto notFound = keyStore->GetResourceDataGroup(1)->FindAccessRightByConsumer(consumer1.get());
		ASSERT_EQ(notFound, nullptr);

		//Test remove decrypt right from resource data using a consumer
		keyStore->GetResourceDataGroup(0)->RemoveAccessRight(consumer1.get());

		notFound = keyStore->GetResourceDataGroup(0)->FindAccessRightByConsumer(consumer1.get());
		ASSERT_EQ(notFound, nullptr);
	}





	//
	//Keystore test which require preexisting model
	//

	TEST_F(SecureContentT, ModelReaderKeyStoreNoWarnings) {
		PReader reader3MF = readUnencryptedKeyStore();
		CheckReaderWarnings(reader3MF, 0);
	}

	TEST_F(SecureContentT, ModelReaderEmptyKeyStoreWarnings) {
		try {
			PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREEMPTY);
			ASSERT_FALSE(true);
		} catch (ELib3MFException const & e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_GENERICEXCEPTION);
			//Lib3MF_uint32 iWarning = 0;

			//// NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX
			//// consumer index is higher than consumer count
			//reader3MF->GetWarning(0, iWarning);
			//ASSERT_EQ(0x80F6, iWarning);
		}
	}

	TEST_F(SecureContentT, ModelReaderKeyStoreNoAttributesWarnings) {
		PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREMISSINGATTRIBUTES);
		CheckReaderWarnings(reader3MF, 8);
		Lib3MF_uint32 iWarning = 0;

		// NMR_ERROR_KEYSTOREMISSINGCONSUMERID
		// consumerid is not defined on consumer
		reader3MF->GetWarning(0, iWarning);
		ASSERT_EQ(0x80F3, iWarning);

		// NMR_ERROR_KEYSTOREMISSINGKEYUUID
		// resourcedatagroup keyuuid is not defined
		reader3MF->GetWarning(1, iWarning);
		ASSERT_EQ(0x8107, iWarning);

		// NMR_ERROR_KEYSTOREMISSINGALGORTHM
		// missing wrappingalgorithm on kekparams
		reader3MF->GetWarning(2, iWarning);
		ASSERT_EQ(0x810A, iWarning);

		// NMR_ERROR_KEYSTOREMISSINGCONSUMERINDEX
		// consumerindex on accessright is not defined
		reader3MF->GetWarning(3, iWarning);
		ASSERT_EQ(0x8105, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCODING 
		// invalid key store element value in ResourceData
		reader3MF->GetWarning(4, iWarning);
		ASSERT_EQ(0x810E, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCODING 
		// invalid key store element value in ResourceData
		reader3MF->GetWarning(5, iWarning);
		ASSERT_EQ(0x810E, iWarning);


		// NMR_ERROR_KEYSTOREMISSINGALGORTHM
		// missing encryptionalgorithm on cekparams
		reader3MF->GetWarning(6, iWarning);
		ASSERT_EQ(0x810A, iWarning);

		// NMR_ERROR_MISSINGUUID
		//missing guid on keystore
		reader3MF->GetWarning(7, iWarning);
		ASSERT_EQ(0x80B0, iWarning);

	}

	TEST_F(SecureContentT, ModelReaderKeyStoreInvalidAttributesWarnings) {
		PReader reader3MF = readKeyStore(NEGATIVEUNENCRYPTEDKEYSTOREINVALIDATTRIBUTES);
		CheckReaderWarnings(reader3MF, 10);
		Lib3MF_uint32 iWarning = 0;

		// NMR_ERROR_KEYSTOREDUPLICATECONSUMERID
		// consumer has multiple consumerid attributes
		reader3MF->GetWarning(0, iWarning);
		ASSERT_EQ(0x80F1, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDKEYUUID
		// resourcedatagroup keyuuid is invalid
		reader3MF->GetWarning(1, iWarning);
		ASSERT_EQ(0x80FF, iWarning);

		// NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE
		// encryptionalgorithm on accessright
		reader3MF->GetWarning(2, iWarning);
		ASSERT_EQ(0x80A7, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDALGORITHM 
		// invalid wrappinglagorithm attribute in kekparams
		reader3MF->GetWarning(3, iWarning);
		ASSERT_EQ(0x80F7, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDMGF
		// invalid mgfalgorithm
		reader3MF->GetWarning(4, iWarning);
		ASSERT_EQ(0x8102, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDDIGEST
		// invlid digestmethod
		reader3MF->GetWarning(5, iWarning);
		ASSERT_EQ(0x8103, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDALGORITHM 
		// invalid encryptionalgorithm attribute in cekparams
		reader3MF->GetWarning(6, iWarning);
		ASSERT_EQ(0x80F7, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDCOMPRESSION 
		// invalid compression attribute in ResourceData
		reader3MF->GetWarning(7, iWarning);
		ASSERT_EQ(0x80F8, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCODING 
		// invalid key store element value in ResourceData
		reader3MF->GetWarning(8, iWarning);
		ASSERT_EQ(0x810E, iWarning);

		// NMR_ERROR_KEYSTOREINVALIDENCODING 
		// invalid key store element value in ResourceData
		reader3MF->GetWarning(9, iWarning);
		ASSERT_EQ(0x810E, iWarning);
	}
	
	TEST_F(SecureContentT, CheckKeyStoreConsumers) {
		readUnencryptedKeyStore();
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const size_t consumerCount = (size_t) keyStore->GetConsumerCount();
		for (size_t i = 0; i < consumerCount; ++i) {
			PConsumer consumer = keyStore->GetConsumer(i);

			ASSERT_EQ("LIB3MF#TEST", consumer->GetConsumerID());
			ASSERT_EQ("contentKey", consumer->GetKeyID());

			PConsumer consumerNotFound = keyStore->FindConsumer("does not exist");
			ASSERT_EQ(nullptr, consumerNotFound);
			PConsumer consumerFound = keyStore->FindConsumer(consumer->GetConsumerID());
			ASSERT_EQ(consumer->GetConsumerID(), consumerFound->GetConsumerID());

			std::string keyValue = consumer->GetKeyValue();
			keyValue.erase(std::remove_if(keyValue.begin(), keyValue.end(), ::isspace), keyValue.end());
			std::string expected = publicKey;
			expected.erase(std::remove_if(expected.begin(), expected.end(), ::isspace), expected.end());


			ASSERT_EQ(expected, keyValue);

		}
	}

	TEST_F(SecureContentT, CheckKeyStoreResourceData) {
		readUnencryptedKeyStore();
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const size_t resourceDataCount = (size_t) keyStore->GetResourceDataCount();

		ASSERT_TRUE(resourceDataCount > 0);

		PPackagePart newPart = model->FindOrCreatePackagePart("/3D/newpart.model");
		PResourceData resourceDataNotFound = keyStore->FindResourceData(newPart.get());
		ASSERT_TRUE(nullptr == resourceDataNotFound);

		for (size_t i = 0; i < resourceDataCount; ++i) {
			PResourceData resourceData = keyStore->GetResourceData(i);
			ASSERT_TRUE(resourceData != nullptr);

			PResourceData resourceDataFound = keyStore->FindResourceData(resourceData->GetPath().get());
			ASSERT_TRUE(resourceDataFound != nullptr);

			ASSERT_EQ(resourceData->GetPath()->GetPath(), resourceDataFound->GetPath()->GetPath());

			ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::AES256_GCM, resourceData->GetEncryptionAlgorithm());
			ASSERT_EQ(Lib3MF::eCompression::NoCompression, resourceData->GetCompression());
			ASSERT_EQ("/3D/3dexternal.model", resourceData->GetPath()->GetPath());
		}
	}

	TEST_F(SecureContentT, DEKReadTest) {
		auto reader = model->QueryReader("3mf");
		DEKCallbackData data;
		reader->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&data);
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}
	

	TEST_F(SecureContentT, ReadCompressedTest) {
		auto reader = model->QueryReader("3mf");
		DEKCallbackData data;
		reader->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&data);
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDCOMPRESSEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}

	TEST_F(SecureContentT, DEKWriteTest) {
		readUnencryptedKeyStore();
		auto writer = model->QueryWriter("3mf");
		DEKCallbackData data;
		writer->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&data);
		writer->WriteToFile(sOutFilesPath + UNENCRYPTEDCOMPRESSEDKEYSTORE);
		ASSERT_EQ(data.context.size(), 1);
		ASSERT_GE(data.context.begin()->second, 1);
	}

	TEST_F(SecureContentT, KEKReadTest) {
		auto reader = model->QueryReader("3mf");
		KEKCallbackData data;
		data.value = 1;
		data.consumerId = "LIB3MF#TEST";
		data.keyId = "contentKey";
		reader->AddKeyWrappingCallback(data.consumerId, testKEKCallback, (Lib3MF_pvoid)&data);
		reader->ReadFromFile(sTestFilesPath + UNENCRYPTEDKEYSTORE);
		ASSERT_EQ(3, data.value);
	}


	TEST_F(SecureContentT, KEKWriteTest) {
		readUnencryptedKeyStore();
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);
		ASSERT_EQ(keyStore->GetConsumerCount(), 1);
		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("consumerId", "contentKey", "consumerKeyValue");
		Lib3MF::PResourceData rd = keyStore->GetResourceData(0);
		Lib3MF::PResourceDataGroup dg = keyStore->FindResourceDataGroup(rd->GetPath().get());
		Lib3MF::PAccessRight ar = dg->AddAccessRight(consumer1.get(), eWrappingAlgorithm::RSA_OAEP, eMgfAlgorithm::MGF1_SHA1, eDigestMethod::SHA1);

		auto writer = model->QueryWriter("3mf");
		
		DEKCallbackData contentData;
		writer->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&contentData);

		KEKCallbackData wrappingData;
		wrappingData.value = 1;
		wrappingData.consumerId = "consumerId";
		wrappingData.keyId = "contentKey";
		writer->AddKeyWrappingCallback(wrappingData.consumerId, testKEKCallback, (Lib3MF_pvoid)&wrappingData);
		std::vector<Lib3MF_uint8> buffer;
		writer->WriteToBuffer(buffer);
		ASSERT_EQ(3, wrappingData.value);
	}

	TEST_F(SecureContentT, KEKWriteTestUnregisteredConsumer) {
		readUnencryptedKeyStore();
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);
		
		ASSERT_TRUE(keyStore->GetResourceDataCount() >= 1);

		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer("UnregisteredConsumer", "contentKey", "");
		Lib3MF::PResourceData rd = keyStore->GetResourceData(0);
		Lib3MF::PLib3MFResourceDataGroup dg = keyStore->FindResourceDataGroup(rd->GetPath().get());
		Lib3MF::PAccessRight ar = dg->AddAccessRight(consumer1.get(), eWrappingAlgorithm::RSA_OAEP, eMgfAlgorithm::MGF1_SHA1, eDigestMethod::SHA1);

		auto writer = model->QueryWriter("3mf");
		DEKCallbackData contentData;
		writer->SetContentEncryptionCallback(testDEKCallback, (Lib3MF_pvoid)&contentData);

		std::vector<Lib3MF_uint8> buffer;
		try {
			writer->WriteToBuffer(buffer);
			ASSERT_FALSE(true);
		} catch (Lib3MF::ELib3MFException const &e) {
			ASSERT_EQ(e.getErrorCode(), LIB3MF_ERROR_SECURECONTEXTNOTREGISTERED);
		}
	}

	TEST_F(SecureContentT, MakeExistingModelEncrypted) {
		generateTestFiles(false, "keystore.3mf");
		generateTestFiles(true, "keystore_compressed.3mf");
	}
}
