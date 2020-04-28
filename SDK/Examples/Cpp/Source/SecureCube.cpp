/*++

Copyright (C) 2020 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT AND/OR NETFABB BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

SecureCube.cpp : 3MF encrypted Cube creation example

--*/

#include <iostream>
#include <string>
#include <algorithm>
#include <map>

#include "lib3mf_implicit.hpp"

using namespace Lib3MF;


class SecureContentCallbacks {
public:
	// Sample random number generation callback. Do not use this beyond the scope of this example as it is not really a random generation function.
	static void NotRandomBytesAtAll(Lib3MF_uint64 byteData, Lib3MF_uint64 size, Lib3MF_pvoid userData, Lib3MF_uint64 * bytesWritten) {
		static Lib3MF_uint8 random = 0;
		Lib3MF_uint8 * buffer = (Lib3MF_uint8 *)byteData;
		*bytesWritten = size;
		while (size > 0)
			*(buffer + (--size)) = ++random;
	}

	// Structure to hold encryption context for keys
	struct KeyWrappingCbData {
		CWrapper * wrapper;
	};


	// Structure to hold encryption context for resources
	struct ContentEncryptionCbData {
		CWrapper * wrapper;
		std::map<Lib3MF_uint64, Lib3MF_uint64> context;
	};

	// Sample callback to wrap the key of an encryption process
	static void KeyWrappingCbSample(
		Lib3MF_AccessRight access,
		Lib3MF_uint64 inSize,
		const Lib3MF_uint8 * inBuffer,
		const Lib3MF_uint64 outSize,
		Lib3MF_uint64 * outNeeded,
		Lib3MF_uint8 * outBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status) {

		KeyWrappingCbData * cp = (KeyWrappingCbData *)userData;

		// Since we're using CAccessRight constructure, we have to account for 
		// the use of 'access' by calling Acquire on the CWrapper
		CAccessRight accessRight(cp->wrapper, access);
		cp->wrapper->Acquire(&accessRight);

		PConsumer consumer = accessRight.GetConsumer();

		std::string consumerId = consumer->GetConsumerID();
		std::cout << "ConsumerID " << consumer->GetConsumerID() << std::endl;
		// You can also check for keyid and keyvalue

		// A call could be made to first identify what the output buffer size should be.
		// In that case, outSize will be 0 or outBuffer will be null, and the proper size must be placed in outNeeded.
		if (nullptr == outBuffer || outSize == 0) {
			*outNeeded = inSize;
			*status = inSize;
			return;
		}

		// Query the data about the encryption process to be done
		eWrappingAlgorithm algorithm = accessRight.GetWrappingAlgorithm();
		eMgfAlgorithm mask = accessRight.GetMgfAlgorithm();
		eDigestMethod diges = accessRight.GetDigestMethod();

		// You should deal with the encryption/decryption process of the key.
		// Use the encryption process to wrap inBuffer into outBuffer using details above.
		// Use KeyWrappingCbData to hold any information you'll be needing at this point.
		throw std::runtime_error("TODO: Add your encryption wrapping process here");

		// Finally, this function should use status to return the number of bytes needed, 
		// encrypted/decrypted - or zero to indicate a failure.
		*status = outSize;
	}

	// Sample callback to encrypt contents of a resource
	static void ContentEncryptionCbSample(
		Lib3MF_ContentEncryptionParams params,
		Lib3MF_uint64 inSize,
		const Lib3MF_uint8 * inBuffer,
		const Lib3MF_uint64 outSize,
		Lib3MF_uint64 * outNeededSize,
		Lib3MF_uint8 * outBuffer,
		Lib3MF_pvoid userData,
		Lib3MF_uint64 * status) {

		ContentEncryptionCbData * cb = (ContentEncryptionCbData *)userData;

		// Since we're using CAccessRight constructure, we have to account for 
		// the use of 'access' by calling Acquire on the CWrapper
		CContentEncryptionParams cd(cb->wrapper, params);
		cb->wrapper->Acquire(&cd);

		// A descriptor uniquely identifies the encryption process for a resource
		Lib3MF_uint64 descriptor = cd.GetDescriptor();

		// You can map the descriptor in use as you'll probably keep several
		// contexts initialized at same time
		auto localDescriptor = cb->context.find(cd.GetDescriptor());
		if (localDescriptor != cb->context.end())
			// Use an existing context
			localDescriptor->second++;
		else {
			// Initialize a new context

			// Retrieve the encryption key, if there is one
			std::vector<Lib3MF_uint8> key;
			cd.GetKey(key);

			// Retrieve the additional authenticaton data, if it has been used to encrypt
			std::vector<Lib3MF_uint8> aad;
			cd.GetAdditionalAuthenticationData(aad);
			cb->context[cd.GetDescriptor()] = 0;
		}

		// The encryption process must be initialized if needed, processed and finalized.
		if (0 == inSize || nullptr == inBuffer) {
			// When input buffer is null or input size is 0, this is a request
			// to finalize this encryption process by verifying the tag
			std::vector<Lib3MF_uint8> tag;
			cd.GetAuthenticationTag(tag);
			// Proceed to verify this tag with the encryption context

			//add tag verification status here
			*status = 1;
		} else if (0 == outSize || nullptr == outBuffer) {
			// If outSize is zero or outBuffer is null, this is a call to figure out the output buffer size.
			// Put the respective value in outNeededSize.
			*outNeededSize = inSize;
			*status = inSize;
		} else {
			// Else, perform the encryption/descryption process
			throw std::runtime_error("TODO: Add your encryption process here");
			*status = outSize;
		}
		//This function should use status to return the number of bytes needed, encrypted/decrypted
		// or verified - or zero to indicate a failure.
	}
};

void printVersion(PWrapper wrapper) {
	Lib3MF_uint32 nMajor, nMinor, nMicro;
	wrapper->GetLibraryVersion(nMajor, nMinor, nMicro);
	std::cout << "lib3mf version = " << nMajor << "." << nMinor << "." << nMicro;
	std::string sReleaseInfo, sBuildInfo;
	if (wrapper->GetPrereleaseInformation(sReleaseInfo)) {
		std::cout << "-" << sReleaseInfo;
	}
	if (wrapper->GetBuildInformation(sBuildInfo)) {
		std::cout << "+" << sBuildInfo;
	}
	std::cout << std::endl;
}

// Utility functions to create vertices and triangles
sLib3MFPosition fnCreateVertex(float x, float y, float z) {
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFTriangle fnCreateTriangle(int v0, int v1, int v2) {
	sLib3MFTriangle result;
	result.m_Indices[0] = v0;
	result.m_Indices[1] = v1;
	result.m_Indices[2] = v2;
	return result;
}

void SecureContentExample() {
	PWrapper wrapper = CWrapper::loadLibrary();

	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "3MF SecureContent example" << std::endl;
	printVersion(wrapper);
	std::cout << "------------------------------------------------------------------" << std::endl;

	PModel model = wrapper->CreateModel();

	// After initializing the model, set the random number generation callback
	// A default one will be used if you don't, current implementation uses std::mt19937
	model->SetRandomNumberCallback(SecureContentCallbacks::NotRandomBytesAtAll, nullptr);

	PMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Box");

	// Create mesh structure of a cube
	std::vector<sLib3MFPosition> vertices(8);
	std::vector<sLib3MFTriangle> triangles(12);

	float fSizeX = 100.0f;
	float fSizeY = 200.0f;
	float fSizeZ = 300.0f;

	// Manually create vertices
	vertices[0] = fnCreateVertex(0.0f, 0.0f, 0.0f);
	vertices[1] = fnCreateVertex(fSizeX, 0.0f, 0.0f);
	vertices[2] = fnCreateVertex(fSizeX, fSizeY, 0.0f);
	vertices[3] = fnCreateVertex(0.0f, fSizeY, 0.0f);
	vertices[4] = fnCreateVertex(0.0f, 0.0f, fSizeZ);
	vertices[5] = fnCreateVertex(fSizeX, 0.0f, fSizeZ);
	vertices[6] = fnCreateVertex(fSizeX, fSizeY, fSizeZ);
	vertices[7] = fnCreateVertex(0.0f, fSizeY, fSizeZ);

	// Manually create triangles
	triangles[0] = fnCreateTriangle(2, 1, 0);
	triangles[1] = fnCreateTriangle(0, 3, 2);
	triangles[2] = fnCreateTriangle(4, 5, 6);
	triangles[3] = fnCreateTriangle(6, 7, 4);
	triangles[4] = fnCreateTriangle(0, 1, 5);
	triangles[5] = fnCreateTriangle(5, 4, 0);
	triangles[6] = fnCreateTriangle(2, 3, 7);
	triangles[7] = fnCreateTriangle(7, 6, 2);
	triangles[8] = fnCreateTriangle(1, 2, 6);
	triangles[9] = fnCreateTriangle(6, 5, 1);
	triangles[10] = fnCreateTriangle(3, 0, 4);
	triangles[11] = fnCreateTriangle(4, 7, 3);

	meshObject->SetGeometry(vertices, triangles);

	// Add build item
	model->AddBuildItem(meshObject.get(), wrapper->GetIdentityTransform());

	// Move this mesh out of the root model file into another model
	PPackagePart nonRootModel = model->FindOrCreatePackagePart("/3D/securecube.model");
	meshObject->SetPackagePart(nonRootModel.get());

	// Locate the keystore and setup the resource as a secure content
	PKeyStore keystore = model->GetKeyStore();

	// Add you (client) as consumer of the resource. You'll need to do this 
	// to be able to have a chance to wrap the content key
	// You can set optional public key and an optional key id
	PConsumer consumer = keystore->AddConsumer("MyConsumerID", std::string(), std::string());

	// Add a container for your secured resources. Resources within the same container will have a shared key.
	PResourceDataGroup dataGroup = keystore->AddResourceDataGroup();

	// Add access rights for your consumer into the datagroup
	PAccessRight accessRight = dataGroup->AddAccessRight(consumer.get(), eWrappingAlgorithm::RSA_OAEP, eMgfAlgorithm::MGF1_SHA1, eDigestMethod::SHA1);

	// Specify additional authentication data that you could use to further validate your encryption process
	std::vector<Lib3MF_uint8> aad = { '3','M','F','C','o','n','s','o','r','t','i','u','m',' ','S','a','m','p','l','e' };

	// This will effectively add your nonRootModel as a secure content
	PResourceData resourceData = keystore->AddResourceData(dataGroup.get(), nonRootModel.get(), eEncryptionAlgorithm::AES256_GCM, eCompression::Deflate, aad);

	// Query the writer and setup the encryption before saving results
	PWriter writer = model->QueryWriter("3mf");

	// Setup Key Wrapping process
	SecureContentCallbacks::KeyWrappingCbData keyData;
	keyData.wrapper = wrapper.get();
	writer->AddKeyWrappingCallback(consumer->GetConsumerID(), SecureContentCallbacks::KeyWrappingCbSample, &keyData);

	// Content Encryption process
	SecureContentCallbacks::ContentEncryptionCbData contentData;
	contentData.wrapper = wrapper.get();
	writer->SetContentEncryptionCallback(SecureContentCallbacks::ContentEncryptionCbSample, &contentData);

	// You'll need to complete the callback code for this call to work properly.
	writer->WriteToFile("secureCube.3mf");

	std::cout << "done." << std::endl;
}

int main() {
	try {
		SecureContentExample();
	} catch (ELib3MFException &e) {
		std::cout << e.what() << std::endl;
		return e.getErrorCode();
	}
	return 0;
}
