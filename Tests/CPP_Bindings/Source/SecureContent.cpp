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


	//read (model with) existing resource and make some existing resource encrypted
	//read (model with and further write) existing encrypted resource and add another consumer to it
	//write (model with) encrypted resource from scratch.
	//read (model with) existing encrypted resource, add another resource to the same consumers
	//nothing to prevent we read encrypted meshes and move them around to plain text resources
	//add remove consumer/resourcedata

	TEST_F(SecureContentT, 3MFWriteSecureAttachment) {
		//We want to add a secure attachment to the model
		//auto attachment = model->AddAttachment(m_sRelationShipPath + ".xml", m_sAttachmetType);
		//attachment->ReadFromFile(m_sAttachmentPath);
		////Before writing, we need to establish the consumer
		//std::vector<Lib3MF_uint8> empty;
		//PConsumer consumer = model->AddConsumer("LIB3MF#TEST", "", empty, empty);

		////Then, we need to establish that the attachment will be encrypted, by adding it as resource data;
		//PResourceData resourceData = model->AddResourceData(attachment.get() , Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::Deflate);

		////Further, we need to define that the consumer we created has decryptrights on this resource;
		//resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);

		////Then we need to register for this consumer data encryption key callback
		//PWriter writer = model->QueryWriter("3mf");
		//writer->RegisterConsumer("LIB3MF#TEST", EncryptionData::encryptionCallback);

		////Then we can write to whatever. Callback will be called on that callstack
		//std::vector<Lib3MF_uint8> buffer;
		//writer->WriteToBuffer(buffer);
	}

	TEST_F(SecureContentT, 3MFReadKeyStore) {
		PReader reader3MF = model->QueryReader("3mf");
		reader3MF->ReadFromFile(sTestFilesPath + "/SecureContent/keystore.3mf");
		auto keyStore = model->GetKeyStore();
		PConsumerIterator consumerIterator = keyStore->GetConsumers();
		while (consumerIterator->MoveNext()) {
			PConsumer consumer = consumerIterator->GetCurrentConsumer();
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

		PResourceDataIterator resourceDataIterator = keyStore->GetResourcesData();
		while (resourceDataIterator->MoveNext()) {
			PResourceData resourceData = resourceDataIterator->GetCurrentResourceData();

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

	TEST_F(SecureContentT, 3MFCreateKeyStore) {
		/*PKeyStore keyStore;

		PRSAKeyValue keyValue;
		const Lib3MF_uint8 expectedModulus[345] = "w53q4y2KB2WcoOBUE9OEXI0OCzUf4SI1J6fDx6XeDJ8PzqxN4pPRtXgtKfp/RiSL0invf7ASfkBMcXuhD8XP0uki3JIzvsxTH+Jnnz/PrYnS9DFa6c9MYciTIV8vC4u03vkZH6OuGq4rWeSZuNCTCgT59q67Ly6OytNsQgsDHL2QO8xhpYdQ4bx7F0uNn5LAxFyA0ymsFsgSSLONJWzaVtsq9jvkIOEdTzYq52PAXMUIpegbyqSheNlmedcss8teqiZGnCOxpBxL3z+ogcFenX1S8kq2UhzOjXLEjPs9B0SchwXSadephL89shJwra+30NS3R3frwfCz+a3H6wTVBw==";
		std::vector<Lib3MF_uint8> exponentBuffer(expectedModulus, expectedModulus + strlen((const char *)expectedModulus));
		keyValue->SetExponent(exponentBuffer);
		std::vector<Lib3MF_uint8> modulusBuffer = { 'A', 'Q', 'A', 'B' };
		keyValue->SetModulus(modulusBuffer);

		PConsumer consumer = keyStore->AddConsumer("LIB3MF#TEST", "contentKey", keyValue.get());

		auto meshObject = model->AddMeshObject();
		auto path = meshObject->PackagePath();
		PResourceData resourceData = keyStore->AddResourceData(path.get(), Lib3MF::eEncryptionAlgorithm::Aes256Gcm, Lib3MF::eCompression::None);

		PDecryptRight decryptRight = resourceData->AddDecryptRight(consumer.get(), Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p);
		 
		const Lib3MF_uint8 expectedCipher[381] = "Ao6tg4qOlIfRscGdYkAI/48xT3S6In5TQatVslcAPcpcn5oC5wxKNgghplIxjuw64SICHLfOuUZjLT3/LlP1E6MqhOhyxjBjAYsLhHBxcqlAynHyDJoKk27WYCQV+jCs4z6h78YXzVNto3uOlCghN2m5/XG0yqxaqhERtSfbrWJAIANUD1Rwkhmlg1Bemx2Ai2lzIajZwaWERYt3srNFORAVbR1CXONybXE6BXHnclzTbOV7AtTAOWcBrw1q38mDrnHkWwSu6qoD0yc4FCvEStDH1BvIMN28n7jaz7LAlRhwZTYvv95NdYLgJ0izXdHxApKl8T8u6z1ZjMUAGdn8SGLZajJhyTgqH3GhLYqtnnGw0JYYwEj7Dphdxqg=";
		std::vector<Lib3MF_uint8> cipherData(expectedCipher, expectedCipher + strlen((const char *)expectedCipher));
		CBasicCipherData d = CBasicCipherData();
		decryptRight->SetCipherData(cipherData);*/
	}

	TEST_F(SecureContentT, 3MFWriteSecureMesh) {
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

	TEST_F(SecureContentT, 3MFReadExternalModel) {
		auto reader = model->QueryReader("3mf");
		try {
			reader->ReadFromFile(sTestFilesPath + "SecureContent/detachedmodel.3mf");
		} catch (const std::exception & e) {
			std::cout << e.what();
		}
		auto iter = model->GetBuildItems();
		while (iter->MoveNext()) {
			auto bi = iter->GetCurrent();
			bool hasUUID;
			std::cout << "BuildItem: " << bi->GetUUID(hasUUID) << ", hasTransform: " << bi->HasObjectTransform() << std::endl;
		}
	}

	TEST_F(SecureContentT, 3MFWriteExternalModel) {
		//create the attachment to be secured
		std::string path = "/3D/nonrootmodel1.model";

		auto meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		sTransform transformation = wrapper->GetIdentityTransform();
		meshObject->PackagePath()->Set(path);
		model->AddBuildItem(meshObject.get(), transformation);

		meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path);
		transformation = wrapper->GetTranslationTransform(0.0, 250.0, 0.0);
		model->AddBuildItem(meshObject.get(), transformation);

		std::string path2 = "/3D/nonrootmodel2.model";
		meshObject = model->AddMeshObject();
		meshObject->SetGeometry(CLib3MFInputVector<sPosition>(pVertices, 8), CLib3MFInputVector<sTriangle>(pTriangles, 12));
		meshObject->PackagePath()->Set(path2);
		transformation = wrapper->GetTranslationTransform(0.0, 250.0, 0.0);
		model->AddBuildItem(meshObject.get(), transformation);

		auto writer = model->QueryWriter("3mf");
		writer->WriteToFile(sTestFilesPath + "SecureContent/nonrootmodels.3mf");

		auto modelAssert = wrapper->CreateModel();
		auto reader = modelAssert->QueryReader("3mf");
		reader->ReadFromFile(sTestFilesPath + "SecureContent/nonrootmodels.3mf");
		ASSERT_EQ(3, modelAssert->GetObjects()->Count());
	}


	TEST_F(SecureContentT, 3MFReadSecureAttachment) {
		//auto buffer = ReadFileIntoBuffer(sTestFilesPath + "/SecureContent/" + m_sFilenameReadWrite);
		////We want to read a 3mf that could be secured
		//PReader reader = model->QueryReader("3mf");
		////We register for our known ConsumerID
		//reader->RegisterConsumer("LIB3MF#TEST", EncryptionData::decryptionCallback);//if cannot read, add warning and let it fails
		////We read the content. data key encryption callback will be called on the read callstack
		//reader->ReadFromBuffer(buffer);

		////find a mesh
		////get path

		////find consumer
		//PConsumer consumer = model->FindConsumer("LIB3MF#2");
		//if (nullptr == consumer) {
		//	consumer = model->AddConsumer("LIB3MF#2", "", std::vector<unsigned char>(), std::vector<unsigned char>());
		//}
		////find resource data
		//PResourceData resourceData = model->FindResourceData("/SecureContent/model2.xml");
		////add consumer to resource data
		//resourceData->AddDecryptRight(consumer.get(), eEncryptionAlgorithm::RsaOaepMgf1p);
		////query writer
		////register consumer
		////save
	}

}
