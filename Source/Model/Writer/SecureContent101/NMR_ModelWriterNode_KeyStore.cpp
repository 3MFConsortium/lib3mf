/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_ModelWriterNode_KeyStore.cpp implements the Model Writer KeyStore Node Class.
This is the class for exporting the 3mf keystore stream root node.

--*/

#include "Model/Writer/SecureContent101/NMR_ModelWriterNode_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_KeyStoreCEKParams.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_UUID.h"
#include "Libraries/cpp-base64/base64.h"
#include <vector>
#include <array>


void NMR::CModelWriterNode_KeyStore::writeConsumers() {
	nfUint64 count = m_pKeyStore->getConsumerCount();
	for (nfUint64 nIndex = 0; nIndex < count; ++nIndex) {
		PKeyStoreConsumer consumer = m_pKeyStore->getConsumer(nIndex);
		m_consumerIndexes[consumer] = nIndex;
		// <consumer>
		writeStartElement(XML_3MF_ELEMENT_CONSUMER);
		//@consumerid
		std::string consumerId = consumer->getConsumerID();
		if (consumerId.empty()) {
			throw CNMRException(NMR_ERROR_KEYSTOREMISSINGCONSUMERID);
		}
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_CONSUMER_ID, consumerId.c_str());
		//@keyid
		std::string keyId = consumer->getKeyID();
		if (!keyId.empty()) {
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_KEY_ID, keyId.c_str());
		}

		std::string kvStr = consumer->getKeyValue();
		if (!kvStr.empty()) {
			// <keyvalue>
			writeStartElement(XML_3MF_ELEMENT_KEYVALUE);
			writeText(kvStr.c_str(), (nfUint32)kvStr.length());
			// </keyvalue>
			writeFullEndElement();
		}

		// </consumer>
	    writeFullEndElement();
	 }
}

void NMR::CModelWriterNode_KeyStore::writeEncryptionAlgorithmAttribute(eKeyStoreEncryptAlgorithm ea) {
	if (ea == eKeyStoreEncryptAlgorithm::AES256_GCM) {
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_ENCRYPTION_ALGORITHM, XML_3MF_SECURE_CONTENT_ENCRYPTION_AES256);
	}
}

void NMR::CModelWriterNode_KeyStore::writeWrapAlgorithmAttribute(eKeyStoreWrapAlgorithm ea) {
	if (ea == eKeyStoreWrapAlgorithm::RSA_OAEP) {
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_WRAPPINGALGORITHM, XML_3MF_SECURE_CONTENT_KEYWRAPPING_RSASHORT);
	}
}

void NMR::CModelWriterNode_KeyStore::writeMgf(eKeyStoreMaskGenerationFunction mgf) {
	std::string value;
	switch (mgf) {
		default:
		case MGF1_SHA1:
			value = XML_3MF_SECURE_CONTENT_MGF1_SHA1;
			break;
		case MGF1_SHA224:
			value = XML_3MF_SECURE_CONTENT_MGF1_SHA224;
			break;
		case MGF1_SHA256:
			value = XML_3MF_SECURE_CONTENT_MGF1_SHA256;
			break;
		case MGF1_SHA384:
			value = XML_3MF_SECURE_CONTENT_MGF1_SHA384;
			break;
		case MGF1_SHA512:
			value = XML_3MF_SECURE_CONTENT_MGF1_SHA512;
			break;
	}
	writeConstStringAttribute(XML_3MF_SECURE_CONTENT_MGFALGORITHM, value.c_str());
}

void NMR::CModelWriterNode_KeyStore::writeDigest(eKeyStoreMessageDigest md) {
	std::string value;
	switch (md) {
		default:
		case SHA1:
			value = XML_3MF_SECURE_CONTENT_MD_SHA1;
			break;
		case SHA256:
			value = XML_3MF_SECURE_CONTENT_MD_SHA256;
			break;
		case SHA384:
			value = XML_3MF_SECURE_CONTENT_MD_SHA384;
			break;
		case SHA512:
			value = XML_3MF_SECURE_CONTENT_MD_SHA512;
			break;
	}
	writeConstStringAttribute(XML_3MF_SECURE_CONTENT_DIGESTMETHOD, value.c_str());
}

void NMR::CModelWriterNode_KeyStore::writeResourceDatagroup() {
	nfUint64 const count = m_pKeyStore->getResourceDataGroupCount();
	for (nfUint32 resourceIndex = 0; resourceIndex < count; ++resourceIndex) {
		PKeyStoreResourceDataGroup resourcedatagroup = m_pKeyStore->getResourceDataGroup(resourceIndex);
		// <resourcedatagroup>
		writeStartElement(XML_3MF_ELEMENT_RESOURCEDATAGROUP);
		// keyuuid - attribute
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_KEY_UUID, resourcedatagroup->getKeyUUID()->toString().c_str());

		nfUint64 const accessCount = resourcedatagroup->getAccessRightCount();
		for (nfUint64 accessIndex = 0; accessIndex < accessCount; ++accessIndex) {
			PKeyStoreAccessRight ar = resourcedatagroup->getAccessRight(accessIndex);
			writeAccessRight(ar);
		}

		nfUint64 const resourceDataCount = m_pKeyStore->getResourceDataCount();
		for (nfUint32 rdIndex = 0; rdIndex < resourceDataCount; ++rdIndex)
		{
			PKeyStoreResourceData rd = m_pKeyStore->getResourceData(rdIndex);
			if (rd->getGroup() == resourcedatagroup) {
				writeResourceData(rd);
			}
		}
		//</resourcedatagroup>
		writeFullEndElement();
	}
}

void NMR::CModelWriterNode_KeyStore::writeAccessRight(PKeyStoreAccessRight const & ar) {
	// <accessright>
	writeStartElement(XML_3MF_ELEMENT_ACCESSRIGHT);
	nfUint64 nIndex = m_consumerIndexes[ar->getConsumer()];
	// consumerindex - attribute
	writeConstStringAttribute(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, fnUint32ToString((NMR::nfUint32)nIndex).c_str());

	// <kekparams>
	writeStartElement(XML_3MF_ELEMENT_KEKPARAMS);
	// wrappingalgorithm - attribute
	writeWrapAlgorithmAttribute(ar->getAlgorithm());
	// mgfalgorithm - attribute
	writeMgf(ar->getMgf());
	// digestmethod - attribute
	writeDigest(ar->getDigest());
	//</kekparams>
	writeEndElement();

	// <cipherdata>
	writeStartElement(XML_3MF_ELEMENT_CIPHERDATA);
	// <xenc:CipherValue>
	writeStartElementWithPrefix(XML_3MF_ELEMENT_CIPHERVALUE, XML_3MF_NAMESPACEPREFIX_XENC);
	std::string encodedCv = base64_encode(ar->getCipherValue());
	writeText(encodedCv.c_str(), (nfUint32)encodedCv.length());
	// </xenc:CipherValue>
	writeFullEndElement();
	// </cipherdata>
	writeFullEndElement();
	// </accessright>
	writeFullEndElement();
}

void NMR::CModelWriterNode_KeyStore::writeResourceData(PKeyStoreResourceData const & rd) {
	// <resourcedata>
	writeStartElement(XML_3MF_ELEMENT_RESOURCEDATA);
	// path - attribute
	writeConstStringAttribute(XML_3MF_SECURE_CONTENT_PATH, rd->packagePath()->getPath().c_str());
	writeStartElement(XML_3MF_ELEMENT_CEKPARAMS);
	// encryptionalgorithm - attribute
	writeEncryptionAlgorithmAttribute(rd->getEncryptionAlgorithm());
	// compression - attribute
	if (rd->isCompressed())
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_COMPRESSION, XML_3MF_SECURE_CONTENT_COMPRESSION_DEFLATE);
	else
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_COMPRESSION, XML_3MF_SECURE_CONTENT_COMPRESSION_NONE);

	std::vector<nfByte> const & iv = rd->getInitVector();
	if (!iv.empty()) {
		writeStartElement(XML_3MF_SECURE_CONTENT_IV);
		std::string encodedIv = base64_encode(iv);
		writeText(encodedIv.c_str(), (nfUint32)encodedIv.length());
		writeFullEndElement();
	}

	std::vector<nfByte> const & tag = rd->getAuthTag();
	if (!tag.empty()) {
		writeStartElement(XML_3MF_SECURE_CONTENT_TAG);
		std::string encodedTag = base64_encode(tag);
		writeText(encodedTag.c_str(), (nfUint32)encodedTag.length());
		writeFullEndElement();
	}

	std::vector<nfByte> const & aad = rd->getAddAuthData();
	if (!aad.empty()) {
		writeStartElement(XML_3MF_SECURE_CONTENT_AAD);
		std::string encodedAad = base64_encode(aad);
		writeText(encodedAad.c_str(), (nfUint32)encodedAad.length());
		writeFullEndElement();
	}
	//</cekparams>
	writeFullEndElement();
	//</resourcedata>
	writeFullEndElement();
}

void NMR::CModelWriterNode_KeyStore::writeToXML() {
	writeStartElementWithNamespace(XML_3MF_ELEMENT_KEYSTORE, XML_3MF_NAMESPACE_SECURECONTENTSPEC);
	writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_XENC, XML_3MF_NAMESPACE_CIPHERVALUESPEC);
	writeConstPrefixedStringAttribute(XML_3MF_ATTRIBUTE_XMLNS, XML_3MF_NAMESPACEPREFIX_DS, XML_3MF_NAMESPACE_DIGITALSIGNATURESPEC);
	writeConstStringAttribute(XML_3MF_SECURE_CONTENT_UUID, m_pKeyStore->getUUID()->toString().c_str());
	writeConsumers();
	writeResourceDatagroup();
	writeFullEndElement();
}