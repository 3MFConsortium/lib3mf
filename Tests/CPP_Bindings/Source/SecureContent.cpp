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
		const std::string m_sFolderName;
		const std::string m_sFilenameReadWrite;
		const std::string m_sRelationShipPath;
		const std::string m_sRelationShipPath_file;
		const std::string m_sAttachmetType;
		const std::string m_sAttachmetPayload;
		const std::string m_sAttachmentPath;

		sPosition pVertices[8];
		sTriangle pTriangles[12];

	protected:
		SecureContentT() :
			m_sFolderName("TestOutput"), m_sFilenameReadWrite("output_attachment.3mf"),
			m_sRelationShipPath("/Attachments/secureTest"),
			m_sRelationShipPath_file("/Attachments/secureTestfromfile.xml"),
			m_sAttachmetType("http://schemas.autodesk.com/dmg/testsecureattachment/2020/01"),
			m_sAttachmetPayload("PAYLOAD"),
			m_sAttachmentPath(std::string(TESTFILESPATH) + "/Resources/secureattachment.xml")
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
			PReader reader3MF = model->QueryReader("3mf");
			reader3MF->ReadFromFile(sTestFilesPath + "/SecureContent/keystore.3mf");

		}
		virtual void TearDown() {
			model.reset();
		}

		PModel model;
		static void SetUpTestCase() {
			wrapper = CWrapper::loadLibrary();
		}
		static PWrapper wrapper;
	};
	PWrapper SecureContentT::wrapper;

	using Lib3MF_buffer = std::vector<Lib3MF_uint8>;

	struct EncryptionData {
		/**
		* KeyEncryptionCallback - A callback to provide lib3mf with the encrypted/decrypted data encryption key
		*
		* @param[in] pConsumer - Informatino about the consumer and key used in the encryption process
		* @param[in] eEncryptionAlgorithm - The encryption algorithm to be used. Cipher data should not be encoded.
		* @param[in] nPlainKeyBufferSize - Number of elements in buffer
		* @param[in] pPlainKeyBuffer - uint8 buffer of A description
		* @param[in] nCipherKeyBufferSize - Number of elements in buffer
		* @param[out] pCipherKeyNeededCount - will be filled with the count of the written elements, or needed buffer size.
		* @param[out] pCipherKeyBuffer - uint8  buffer of A description
		* @param[in] pUserData - Userdata that is passed to the callback function
		*/
		static void keyEncryptionCallback(
			Lib3MF_Consumer consumer,
			Lib3MF::eEncryptionAlgorithm algorithm,
			Lib3MF_uint64 plainKeySize,
			const Lib3MF_uint8 * plainKeyBuffer,
			const Lib3MF_uint64 cipherKeySize,
			Lib3MF_uint64* cipherKeyNeededCount,
			Lib3MF_uint8 * cipherKeyBuffer,
			Lib3MF_pvoid userData) {
		}

		/**
		* KeyDecryptionCallback - A callback to provide lib3mf with the encrypted/decrypted data encryption key
		*
		* @param[in] pConsumer - Informatino about the consumer and key used in the encryption process
		* @param[in] eEncryptionAlgorithm - The encryption algorithm to be used. Cipher data should not be encoded.
		* @param[in] nPlainKeyBufferSize - Number of elements in buffer
		* @param[out] pPlainKeyNeededCount - will be filled with the count of the written elements, or needed buffer size.
		* @param[out] pPlainKeyBuffer - uint8  buffer of A description
		* @param[in] nCipherKeyBufferSize - Number of elements in buffer
		* @param[in] pCipherKeyBuffer - uint8 buffer of A description
		* @param[in] pUserData - Userdata that is passed to the callback function
		*/
		static void keyDecryptionCallback(
			Lib3MF_Consumer consumer,
			Lib3MF::eEncryptionAlgorithm algorithm,
			const Lib3MF_uint64 plainKeyBufferSize,
			Lib3MF_uint64* plainKeyNeededCount,
			Lib3MF_uint8 * plainKeyBuffer,
			Lib3MF_uint64 cipherKeyBufferSize,
			const Lib3MF_uint8 * cipherKeyBuffer,
			Lib3MF_pvoid userdata) {
		}

		/**
		* DataEncryptionCallback - A description
		*
		* @param[in] pResourceData - Information about the ResourceData which is being encrypted
		* @param[in] nKeyBufferSize - Number of elements in buffer
		* @param[in] pKeyBuffer - uint8 buffer of The plain data encryption key properly decrypted. Buffer will have same size as CipherValue in decryptright
		* @param[in] nPlainDataBufferSize - Number of elements in buffer
		* @param[in] pPlainDataBuffer - uint8 buffer of Pointer to the data to be written
		* @param[in] nCipherDataBufferSize - Number of elements in buffer
		* @param[out] pCipherDataNeededCount - will be filled with the count of the written elements, or needed buffer size.
		* @param[out] pCipherDataBuffer - uint8  buffer of Pointer to the data to be written
		* @param[in] pUserData - Userdata that is passed to the callback function
		*/
		static void dataEncryptionCallback(
			Lib3MF::eEncryptionAlgorithm encryptionAlgorithm,
			Lib3MF_uint64 keyBufferSize,
			const Lib3MF_uint8 * keyBuffer,
			Lib3MF_uint64 plainDataBufferSize,
			const Lib3MF_uint8 * plainDataBuffer,
			const Lib3MF_uint64 cipherDataBufferSize,
			Lib3MF_uint64 * cipherDataNeededCount,
			Lib3MF_uint8 * cipherDataBuffer,
			Lib3MF_pvoid userData) {}

		/**
		* DataDecryptionCallback - A description
		*
		* @param[in] pResourceData - Information about the ResourceData which is being encrypted
		* @param[in] nKeyBufferSize - Number of elements in buffer
		* @param[in] pKeyBuffer - uint8 buffer of The plain data encryption key properly decrypted. Buffer will have same size as CipherValue in decryptright
		* @param[in] nPlainDataBufferSize - Number of elements in buffer
		* @param[out] pPlainDataNeededCount - will be filled with the count of the written elements, or needed buffer size.
		* @param[out] pPlainDataBuffer - uint8  buffer of Pointer to the data to be written
		* @param[in] nCipherDataBufferSize - Number of elements in buffer
		* @param[in] pCipherDataBuffer - uint8 buffer of Pointer to the data to be written
		* @param[in] pUserData - Userdata that is passed to the callback function
		*/
		static void dataDecryptionCallback(
			Lib3MF_ResourceData resourceData,
			Lib3MF_uint64 keyBufferSize,
			const Lib3MF_uint8 * keyBuffer,
			const Lib3MF_uint64 plainDataBufferSize,
			Lib3MF_uint64 * plainDataNeededCount,
			Lib3MF_uint8 * plainDataBuffer,
			Lib3MF_uint64 cipherDataBufferSize,
			const Lib3MF_uint8 * cipherDataBuffer,
			Lib3MF_pvoid userData) {

		}
	};

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

	TEST_F(SecureContentT, CheckKeyStoreConsumers) {
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const int consumerCount = keyStore->GetConsumerCount();
		for (int i = 0; i > consumerCount; ++i) {
			PConsumer consumer = keyStore->GetConsumer(i);
			/*
			<consumer consumerid="LIB3MF#TEST" keyid="contentKey">
				<keyvalue>
					<ds:RSAKeyValue>
						<ds:Modulus>w53...</ds:Modulus>
						<ds:Exponent>AQAB</ds:Exponent>
					</ds:RSAKeyValue>
				</keyvalue>
			</consumer>
			*/
			ASSERT_EQ("LIB3MF#TEST", consumer->GetConsumerID());
			ASSERT_EQ("contentKey", consumer->GetKeyID());

			PConsumer consumerNotFound = keyStore->FindConsumer("does not exist");
			ASSERT_EQ(nullptr, consumerNotFound);
			PConsumer consumerFound = keyStore->FindConsumer(consumer->GetConsumerID());
			ASSERT_EQ(consumer->GetConsumerID(), consumerFound->GetConsumerID());

			PKeyValue keyValue = consumer->GetKeyValue();

			// Values available here https://github.azc.ext.hp.com/3DSoftware/cencryptionsample/blob/master/testdata/sample.txt#L6
			ASSERT_EQ(1, keyValue->GetRSA().m_Exponent[2]);
			ASSERT_EQ(0, keyValue->GetRSA().m_Exponent[3]);
			ASSERT_EQ(1, keyValue->GetRSA().m_Exponent[4]);

			const Lib3MF_uint8 expectedModulus[257] = { 0xc3, 0x9d, 0xea, 0xe3, 0x2d, 0x8a, 0x7, 0x65, 0x9c, 0xa0, 0xe0, 0x54, 0x13, 0xd3, 0x84, 0x5c, 0x8d, 0xe, 0xb, 0x35, 0x1f, 0xe1, 0x22, 0x35, 0x27, 0xa7, 0xc3, 0xc7, 0xa5, 0xde, 0xc, 0x9f, 0xf, 0xce, 0xac, 0x4d, 0xe2, 0x93, 0xd1, 0xb5, 0x78, 0x2d, 0x29, 0xfa, 0x7f, 0x46, 0x24, 0x8b, 0xd2, 0x29, 0xef, 0x7f, 0xb0, 0x12, 0x7e, 0x40, 0x4c, 0x71, 0x7b, 0xa1, 0xf, 0xc5, 0xcf, 0xd2, 0xe9, 0x22, 0xdc, 0x92, 0x33, 0xbe, 0xcc, 0x53, 0x1f, 0xe2, 0x67, 0x9f, 0x3f, 0xcf, 0xad, 0x89, 0xd2, 0xf4, 0x31, 0x5a, 0xe9, 0xcf, 0x4c, 0x61, 0xc8, 0x93, 0x21, 0x5f, 0x2f, 0xb, 0x8b, 0xb4, 0xde, 0xf9, 0x19, 0x1f, 0xa3, 0xae, 0x1a, 0xae, 0x2b, 0x59, 0xe4, 0x99, 0xb8, 0xd0, 0x93, 0xa, 0x4, 0xf9, 0xf6, 0xae, 0xbb, 0x2f, 0x2e, 0x8e, 0xca, 0xd3, 0x6c, 0x42, 0xb, 0x3, 0x1c, 0xbd, 0x90, 0x3b, 0xcc, 0x61, 0xa5, 0x87, 0x50, 0xe1, 0xbc, 0x7b, 0x17, 0x4b, 0x8d, 0x9f, 0x92, 0xc0, 0xc4, 0x5c, 0x80, 0xd3, 0x29, 0xac, 0x16, 0xc8, 0x12, 0x48, 0xb3, 0x8d, 0x25, 0x6c, 0xda, 0x56, 0xdb, 0x2a, 0xf6, 0x3b, 0xe4, 0x20, 0xe1, 0x1d, 0x4f, 0x36, 0x2a, 0xe7, 0x63, 0xc0, 0x5c, 0xc5, 0x8, 0xa5, 0xe8, 0x1b, 0xca, 0xa4, 0xa1, 0x78, 0xd9, 0x66, 0x79, 0xd7, 0x2c, 0xb3, 0xcb, 0x5e, 0xaa, 0x26, 0x46, 0x9c, 0x23, 0xb1, 0xa4, 0x1c, 0x4b, 0xdf, 0x3f, 0xa8, 0x81, 0xc1, 0x5e, 0x9d, 0x7d, 0x52, 0xf2, 0x4a, 0xb6, 0x52, 0x1c, 0xce, 0x8d, 0x72, 0xc4, 0x8c, 0xfb, 0x3d, 0x7, 0x44, 0x9c, 0x87, 0x5, 0xd2, 0x69, 0xd7, 0xa9, 0x84, 0xbf, 0x3d, 0xb2, 0x12, 0x70, 0xad, 0xaf, 0xb7, 0xd0, 0xd4, 0xb7, 0x47, 0x77, 0xeb, 0xc1, 0xf0, 0xb3, 0xf9, 0xad, 0xc7, 0xeb, 0x4, 0xd5, 0x7 };
			std::vector<Lib3MF_uint8> expected(expectedModulus, expectedModulus + strlen((const char *)expectedModulus));
			for (std::string::size_type i = 0; i <= 257; ++i) {
				ASSERT_EQ(expectedModulus[i], keyValue->GetRSA().m_Modulus[i]);
			}
		}
	}
	TEST_F(SecureContentT, CheckKeyStoreResourceData) {
		auto keyStore = model->GetKeyStore();
		ASSERT_TRUE(keyStore != nullptr);
		const int resourceDataCount = keyStore->GetResourceDataCount();
		for (int i = 0; i < resourceDataCount; ++i) {
			PResourceData resourceData = keyStore->GetResourceData(i);

			PResourceData resourceDataNotFound = keyStore->FindResourceData("does not exist");
			ASSERT_EQ(nullptr, resourceDataNotFound);
			PResourceData resourceDataFound = keyStore->FindResourceData(resourceData->GetPath()->Get());
			ASSERT_EQ(resourceData->GetPath(), resourceDataFound->GetPath());
			/*
			<resourcedata path="/3D/3dexternal.model" encryptionalgorithm="http://www.w3.org/2009/xmlenc11#aes256-gcm">
				<decryptright consumerindex="0" encryptionalgorithm="http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p">
					<cipherdata>
						<xenc:CipherValue>
							pPwK6AQJbymz5ksxVajFzd2uw4p+YCF1uUFo8SsTfa/ZTSHJHTxv2NQTMJyrJ/f35aPkinWfJBa1F9In
							OdMlW5I2oIRu6KLvq8nh3qF9VsnU+F1goNSTJPNVIHhes8/X43qj3wJkk9uDquqfoaqF8sLj8aiTEXWf
							MtTfO5cISoJJ6b4APfLCRjR9N9AFnZ6tqpHgSM6ssOfEIZ3rlE4Uy44fNbVXfpHwb9Il6YtoX4EvV1b6
							2juoWrOrcsHyFvUAA/v5qEDNywD/9K2mfP0GmBqX78uhQgxTuPtPn7Z3gmk+8n7czjMM/mSnkwYkV83K
							FniGdHg5uMXALddzP0HWV2Ns0ClWNoWtybVyJaoumiAwBTDj9clyuNCYA3Q=
						</xenc:CipherValue>
					</cipherdata>
				</decryptright>
			</resourcedata>
			*/
			ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::Aes256Gcm, resourceData->GetEncryptionAlgorithm());
			ASSERT_EQ(Lib3MF::eCompression::None, resourceData->GetCompression());
			ASSERT_EQ("/3D/3dexternal.model", resourceData->GetPath()->Get());

			int inexistantDecrypt = 9999;
			ASSERT_EQ(nullptr, resourceData->GetDecryptRight(inexistantDecrypt));
			int drCount = resourceData->GetDecryptRightCount();
			for (int i = 0; i < drCount; ++i) {
				PDecryptRight decryptRight = resourceData->GetDecryptRight(i);
				//ASSERT_EQ(0, decryptRight->GetConsumerIndex());
				//ASSERT_EQ(Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p, decryptRight->GetEncryptionAlgorithm());
				
				//PBasicCipherData cipherData = decryptRight->GetCipherData();

				//std::vector<Lib3MF_uint8> cipherValueBuffer;
				//const Lib3MF_uint8 expectedCipher[381] = "Ao6tg4qOlIfRscGdYkAI/48xT3S6In5TQatVslcAPcpcn5oC5wxKNgghplIxjuw64SICHLfOuUZjLT3/LlP1E6MqhOhyxjBjAYsLhHBxcqlAynHyDJoKk27WYCQV+jCs4z6h78YXzVNto3uOlCghN2m5/XG0yqxaqhERtSfbrWJAIANUD1Rwkhmlg1Bemx2Ai2lzIajZwaWERYt3srNFORAVbR1CXONybXE6BXHnclzTbOV7AtTAOWcBrw1q38mDrnHkWwSu6qoD0yc4FCvEStDH1BvIMN28n7jaz7LAlRhwZTYvv95NdYLgJ0izXdHxApKl8T8u6z1ZjMUAGdn8SGLZajJhyTgqH3GhLYqtnnGw0JYYwEj7Dphdxqg=";
				//std::vector<Lib3MF_uint8> expected(expectedCipher, expectedCipher + strlen((const char *)expectedCipher));
				//cipherData->GetCipherValue(cipherValueBuffer);
				//ASSERT_EQ(cipherValueBuffer, expected);
				//for (std::string::size_type i = 0; i < expectedCipher.size(); ++i) {
				//	ASSERT_EQ(expectedCipher[i], cipherValueBuffer.at(i));
				//}
			}
		}
	}

	TEST_F(SecureContentT, End2EndSecureContent) {
		//create the attachment to be secured
		std::string path = "/3D/securemesh.xml";
		//add a mesh
		auto meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		//set the mesh apart of the main root model
		auto modelPath = meshObject->PackagePath();
		modelPath->Set(path);

		sTransform transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				transformation.m_Fields[i][j] = Lib3MF_single(i - j);
		}

		model->AddBuildItem(meshObject.get(), transformation);

		auto keyStore = model->GetKeyStore();
		//create a consumer (optional)
		auto consumer = keyStore->AddConsumer("LIB3MF#TEST", "", NULL);
		//create a resource data
		auto resourceData = keyStore->AddResourceData(modelPath.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);
		//add decryptright for the consumer (optional)
		auto decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);

		//Query writer
		auto writer = model->QueryWriter("3mf");
		//register the consumer key encryption callback (optional)
		writer->RegisterConsumer("LIB3MF#TEST", EncryptionData::keyEncryptionCallback);
		//register the data encryption callback
		//writer->RegisterEncryption(EncryptionData::dataEncryptionCallback);

		//write content
		Lib3MF_buffer buffer;
		writer->WriteToBuffer(buffer);
	}

	TEST_F(SecureContentT, 3MFCreateMultipleConsumersKeyStore) {
		Lib3MF::PKeyStore keyStore = model->GetKeyStore();
		
		std::string firstId = "firstId";
		std::string secondId = "secondId";

		std::string firstKeyId = "firstKeyId";
		std::string secondKeyId = "secondKeyId";

		Lib3MF::CKeyValue * keyValue = NULL;


		Lib3MF::PConsumer consumer1 = keyStore->AddConsumer(firstId, firstKeyId, keyValue);
		Lib3MF::PConsumer consumer2 = keyStore->AddConsumer(secondId, secondKeyId, keyValue);

		ASSERT_EQ(2, keyStore->GetConsumerCount());
		ASSERT_EQ(firstId, keyStore->GetConsumer(0)->GetConsumerID());
		ASSERT_EQ(secondId, keyStore->GetConsumer(1)->GetConsumerID());
	}

	TEST_F(SecureContentT, 3MFCreateMultipleResourceDataKeyStore) {
		auto lModel = wrapper->CreateModel();

		Lib3MF::PKeyStore keyStore = lModel->GetKeyStore();
		ASSERT_TRUE(nullptr != keyStore);

		std::string path1 = "/3D/nonrootmodel1.model";
		auto meshObject = lModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path1);
		
		keyStore->AddResourceData(meshObject->PackagePath().get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);

		std::string path2 = "/3D/nonrootmodel2.model";
		meshObject = lModel->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path2);

		keyStore->AddResourceData(meshObject->PackagePath().get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		ASSERT_EQ(2, keyStore->GetResourceDataCount());
		ASSERT_EQ(path1, keyStore->GetResourceData(0)->GetPath()->Get());
		ASSERT_EQ(path2, keyStore->GetResourceData(1)->GetPath()->Get());
	}
}
