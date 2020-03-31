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

void NMR::CModelWriterNode_KeyStore::writeResourceDatas() {
	size_t count = m_pKeyStore->getResourceDataCount();
	for (uint32_t resourceIndex = 0; resourceIndex < count; ++resourceIndex) {
		PKeyStoreResourceData resourcedata = m_pKeyStore->getResourceDataByIndex(resourceIndex);
		// <resourcedata>
		writeStartElement(XML_3MF_ELEMENT_RESOURCEDATA);
		// path - attribute
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_PATH, resourcedata->getPath()->getPath().c_str());
		// encryptionalgorithm AES256 - attribute
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_ENCRYPTION_ALGORITHM, XML_3MF_SECURE_CONTENT_ENCRYPTION_AES256);
		// compression - attribute
		if (resourcedata->getCompression()) {
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_COMPRESSION, XML_3MF_SECURE_CONTENT_COMPRESSION_DEFLATE);
		}
		size_t accessCount = resourcedata->getDecryptRightCount();
		for (uint32_t accessIndex = 0; accessIndex < accessCount; ++accessIndex) {
			// <decryptright>
			writeStartElement(XML_3MF_ELEMENT_DECRYPTRIGHT);
			uint32_t nIndex = resourcedata->getDecryptRight(accessIndex)->getConsumer()->getIndex();
			// consumerIndex - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_CONSUMER_INDEX, std::to_string(nIndex).c_str());
			// encryptionalgorithm RSA - attribute
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_ENCRYPTION_ALGORITHM, XML_3MF_SECURE_CONTENT_ENCRYPTION_RSA);
			// <CipherData>
			writeStartElement(XML_3MF_ELEMENT_CIPHERDATA);
			// <xenc:CipherValue>
			writeStartElementWithPrefix(XML_3MF_ELEMENT_CIPHERVALUE, XML_3MF_NAMESPACEPREFIX_XENC);
			
			CIPHERVALUE cv = resourcedata->getDecryptRight(accessIndex)->getCipherValue();
			//TODO throw when missing cipher value, pending because of callbacks implementation
			/*if (cv.m_iv.size() < 1 && cv.m_key.size() < 1 && cv.m_tag.size() < 1) {
				throw CNMRException(NMR_ERROR_KEYSTOREMISSINGCIPHERVALUE);
			}*/
			std::vector<unsigned char> cvVector;
			std::copy(cv.m_iv.begin(), cv.m_iv.end(), std::back_inserter(cvVector));
			std::copy(cv.m_key.begin(), cv.m_key.end(), std::back_inserter(cvVector));
			std::copy(cv.m_tag.begin(), cv.m_tag.end(), std::back_inserter(cvVector));

			std::string encodedCv = base64_encode(cvVector);
			writeText(encodedCv.c_str(), (nfUint32)encodedCv.length());

			writeFullEndElement();
			writeFullEndElement();
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
	writeResourceDatas();
	writeFullEndElement();
}