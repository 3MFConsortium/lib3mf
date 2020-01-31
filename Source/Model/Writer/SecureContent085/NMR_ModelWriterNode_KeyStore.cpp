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

void NMR::CModelWriterNode_KeyStore::writeConsumers() {
	const auto count = m_pKeyStore->getConsumerCount();
	for (uint32_t index = 0; index < count; ++index) {
		const PKeyStoreConsumer consumer = m_pKeyStore->getConsumerByIndex(index);

		// <consumer>
		writeStartElement(XML_3MF_ELEMENT_CONSUMER);
		//@consumerid
		//TODO throw if doesn't exists
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_CONSUMER_ID, consumer->getConsumerID().c_str());
		//TODO optional value, don't write if empty
		//@keyid
		writeConstStringAttribute(XML_3MF_SECURE_CONTENT_KEY_ID, consumer->getKeyID().c_str());

		std::string kvStr = consumer->getKeyValue();
		if (!kvStr.empty()) {
			// <keyvalue>
			writeStartElement(XML_3MF_ELEMENT_KEY_VALUE);
			writeText(kvStr.c_str(), (nfUint32)kvStr.length());
			// </keyvalue>
			writeFullEndElement();
		}

		// </consumer>
	    writeFullEndElement();
	 }
}

void NMR::CModelWriterNode_KeyStore::writeResourceDatas() {
	auto count = m_pKeyStore->getResourceDataCount();
	for (uint32_t index = 0; index < count; ++index) {
		PKeyStoreResourceData resourcedata = m_pKeyStore->getResourceDataByIndex(index);
		writeStartElement(XML_3MF_ELEMENT_RESOURCEDATA);
			writeConstStringAttribute(XML_3MF_SECURE_CONTENT_PATH, resourcedata->getPath()->getPath().c_str());
			//TODO navigate decryptrights
			writeStartElement(XML_3MF_ELEMENT_DECRYPTRIGHT);
				writeStartElement(XML_3MF_ELEMENT_CIPHERDATA);
					//TODO throw if no ciphervalue
					writeStartElement(XML_3MF_ELEMENT_CIPHERVALUE);
					writeFullEndElement();

				writeFullEndElement();

			writeFullEndElement();

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