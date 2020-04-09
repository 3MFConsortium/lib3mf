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

#include "Model/Writer/SecureContent085/NMR_ModelWriterNode_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreCEKParams.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_UUID.h"
#include "Libraries/cpp-base64/base64.h"
#include <vector>
#include <array>


void NMR::CModelWriterNode_KeyStore::writeConsumers() {
	size_t count = m_pKeyStore->getConsumerCount();
	for (uint32_t nIndex = 0; nIndex < count; ++nIndex) {
		PKeyStoreConsumer consumer = m_pKeyStore->getConsumerByIndex(nIndex);
		consumer->setIndex(nIndex);
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
	if (ea == eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_WRAPPINGALGORITHM, XML_3MF_SECURE_CONTENT_ENCRYPTION_RSA);
	}
	else if (ea == eKeyStoreEncryptAlgorithm::Aes256Gcm) {
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_WRAPPINGALGORITHM, XML_3MF_SECURE_CONTENT_ENCRYPTION_AES256);
	}
}

void NMR::CModelWriterNode_KeyStore::writeResourceDatagroup() {
	size_t count = m_pKeyStore->getResourceDataGroupCount();
	for (uint32_t resourceIndex = 0; resourceIndex < count; ++resourceIndex) {
		PKeyStoreResourceDataGroup resourcedatagroup = m_pKeyStore->getResourceDataGroupByIndex(resourceIndex);
		// <resourcedatagroup>
		writeStartElement(XML_3MF_ELEMENT_RESOURCEDATAGROUP);
		// keyuuid - attribute
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_KEY_UUID, resourcedatagroup->getKeyUuid().c_str());

		size_t accessCount = resourcedatagroup->getAccessRightCount();
		for (uint32_t accessIndex = 0; accessIndex < accessCount; ++accessIndex) {
			// <accessright>
			writeStartElement(XML_3MF_ELEMENT_ACCESSRIGHT);
			uint32_t nIndex = resourcedatagroup->getAccessRight(accessIndex)->getConsumer()->getIndex();
			// consumerIndex - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, std::to_string(nIndex).c_str());
			// TODO: accessright does not have wrappingalgorithm attribute, the sample from secure content spec 0.93 is wrong

			// <kekparams>
			KEKPARAMS kp = resourcedatagroup->getAccessRight(accessIndex)->getKEKParams();
			writeStartElement(XML_3MF_ELEMENT_KEKPARAMS);
			// wrappingalgorithm - attribute
			writeEncryptionAlgorithmAttribute(kp.wrappingalgorithm);
			// mgfalgorithm - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_MGFALGORITHM, kp.mgfalgorithm.c_str());
			// digestmethod - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_DIGESTMETHOD, kp.digestmethod.c_str());
			writeFullEndElement();

			// <CipherData>
			writeStartElement(XML_3MF_ELEMENT_CIPHERDATA);
			// <xenc:CipherValue>
			writeStartElementWithPrefix(XML_3MF_ELEMENT_CIPHERVALUE, XML_3MF_NAMESPACEPREFIX_XENC);
			
			CIPHERVALUE cv = resourcedatagroup->getAccessRight(accessIndex)->getCipherValue();
			//TODO throw when missing cipher value, pending because of callbacks implementation
			/*if (cv.m_iv.size() < 1 && cv.m_key.size() < 1 && cv.m_tag.size() < 1) {
				throw CNMRException(NMR_ERROR_KEYSTOREMISSINGCIPHERVALUE);
			}*/
			std::vector<unsigned char> cvVector;
			std::copy(cv.begin(), cv.end(), std::back_inserter(cvVector));

			std::string encodedCv = base64_encode(cvVector);
			writeText(encodedCv.c_str(), (nfUint32)encodedCv.length());
			writeFullEndElement();
			writeFullEndElement();
			
			writeFullEndElement();
		}

		size_t resourceDataCount = resourcedatagroup->getResourceDataCount();
		for (uint32_t resourceIndex = 0; resourceIndex < resourceDataCount; ++resourceIndex) {
			// <resourcedata>
			writeStartElement(XML_3MF_ELEMENT_RESOURCEDATA);
			// path - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_PATH, resourcedata->getPath().c_str());

			PKeyStoreCEKParams cekParams = resourcedatagroup->getResourceData(resourceIndex)->getCEKParams();
			eKeyStoreEncryptAlgorithm ea = cekParams->getEncryptionAlgorithm();
			// encryptionalgorithm - attribute
			writeEncryptionAlgorithmAttribute(ea);
			// compression - attribute
			if (cekParams->isCompressed()) {
				writeConstStringAttribute(XML_3MF_SECURE_CONTENT_COMPRESSION, XML_3MF_SECURE_CONTENT_COMPRESSION_DEFLATE);
			}
			
			std::vector<nfByte> iv = cekParams->getIv();
			if (!iv.empty()) {
				writeStartElement(XML_3MF_SECURE_CONTENT_IV);
				std::string encodedIv = base64_encode(iv);
				writeText(encodedIv.c_str(), (nfUint32) encodedIv.length());
				writeFullEndElement();
			}

			std::vector<nfByte> tag = cekParams->getTag();
			if (!tag.empty()) {
				writeStartElement(XML_3MF_SECURE_CONTENT_TAG);
				std::string encodedTag = base64_encode(tag);
				writeText(encodedTag.c_str(), (nfUint32)encodedTag.length());
				writeFullEndElement();
			}

			std::vector<nfByte> aad = cekParams->getAad();
			if (!aad.empty()) {
				writeStartElement(XML_3MF_SECURE_CONTENT_AAD);
				std::string encodedAad = base64_encode(aad);
				writeText(encodedAad.c_str(), (nfUint32)encodedAad.length());
				writeFullEndElement();
			}

			writeFullEndElement();
		}

		writeFullEndElement();
	}
}

NMR::CModelWriterNode_KeyStore::CModelWriterNode_KeyStore(CKeyStore * pKeyStore, CXmlWriter * pXMLWriter, PProgressMonitor pProgressMonitor)
: CModelWriterNode_KeyStoreBase(pKeyStore, pXMLWriter, pProgressMonitor) 
{
		
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