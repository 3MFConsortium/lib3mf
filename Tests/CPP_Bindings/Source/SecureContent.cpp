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
			Lib3MF_ResourceData resourceData,
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




	TEST_F(SecureContentT, 3MFWriteSecureMesh) {
		//create the attachment to be secured
		std::string path = "3D/securemesh.xml";
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
		writer->RegisterEncryption(EncryptionData::dataEncryptionCallback);

		//write content
		Lib3MF_buffer buffer;
		writer->WriteToBuffer(buffer);
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