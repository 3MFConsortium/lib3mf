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

NMR_KeyStoreOpcPackageReader.cpp defines an OPC Package reader in a portable way.

--*/

#include "Model/Reader/NMR_KeyStoreOpcPackageReader.h"

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/NMR_SecureContext.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_ImportStream_Compressed.h"
#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/OPC/NMR_OpcPackageReader.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Reader/NMR_ModelReader_InstructionElement.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStore.h"


namespace NMR {
	CKeyStoreOpcPackageReader::CKeyStoreOpcPackageReader(PImportStream pImportStream, PKeyStore pKeyStore, PSecureContext pSecureContext, PModelReaderWarnings pWarnings, PProgressMonitor pProgressMonitor)
		:m_pSecureContext(pSecureContext), m_pKeyStore(pKeyStore)
	{
		if (nullptr == pSecureContext)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);

		m_nfHandler = 0;
		m_pPackageReader = std::make_shared<COpcPackageReader>(pImportStream, pWarnings, pProgressMonitor);
		m_pWarnings = pWarnings;

		PImportStream keyStoreStream = findKeyStoreStream();
		if (nullptr != keyStoreStream) {
			parseKeyStore(keyStoreStream, pProgressMonitor);

			if (!m_pKeyStore->empty()) {
				openAllResourceData();
			}
		}
	}

	COpcPackageRelationship * CKeyStoreOpcPackageReader::findRootRelation(std::string sRelationType, nfBool bMustBeUnique) {
		return m_pPackageReader->findRootRelation(sRelationType, bMustBeUnique);
	}

	POpcPackagePart CKeyStoreOpcPackageReader::createPart(std::string sPath) {
		auto pPart = m_pPackageReader->createPart(sPath);
		if (m_pSecureContext->hasDekCtx()) {
			NMR::PKeyStoreResourceData rd = m_pKeyStore->findResourceDataByPath(sPath);
			if (nullptr != rd) {
				ContentEncryptionDescriptor p = m_pSecureContext->getDekCtx();
				p.m_sDekDecryptData.m_sCipherValue = rd->getCipherValue();
				p.m_sDekDecryptData.m_nfHandler = rd->getHandle();
				p.m_sDekDecryptData.m_bCompression = rd->getCompression();
				PImportStream stream;
				PImportStream decryptStream = std::make_shared<CImportStream_Encrypted>(pPart->getImportStream(), p);
				if (rd->getCompression()) {
					PImportStream decompressStream = std::make_shared<CImportStream_Compressed>(decryptStream);
					stream = decompressStream;
				}
				else {
					stream = decryptStream;
				}
				pPart->setImportStream(stream);
			}
		}
		return pPart;
	}

	nfUint64 CKeyStoreOpcPackageReader::getPartSize(std::string sPath) {
		return m_pPackageReader->getPartSize(sPath);
	}

	PKeyStore CKeyStoreOpcPackageReader::getKeyStore() const {
		return m_pKeyStore;
	}

	void CKeyStoreOpcPackageReader::close() {
		checkAuthenticatedTags();
	}

	NMR::PImportStream CKeyStoreOpcPackageReader::findKeyStoreStream() {
		COpcPackageRelationship * pKeyStoreRelation = m_pPackageReader->findRootRelation(PACKAGE_KEYSTORE_RELATIONSHIP_TYPE, true);
		if (pKeyStoreRelation != nullptr) {
			std::string sTargetPartURI = pKeyStoreRelation->getTargetPartURI();
			POpcPackagePart pKeystorePart = m_pPackageReader->createPart(sTargetPartURI);
			if (pKeystorePart == nullptr)
				throw CNMRException(NMR_ERROR_KEYSTOREOPCCOULDNOTGETSTREAM);
			return pKeystorePart->getImportStream();
		}
		return nullptr;
	}

	void CKeyStoreOpcPackageReader::parseKeyStore(NMR::PImportStream keyStoreStream, NMR::PProgressMonitor pProgressMonitor) {

		PXmlReader pXMLReader = fnCreateXMLReaderInstance(keyStoreStream, pProgressMonitor);
		nfBool bHasModel = false;
		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCSTR pszLocalName = nullptr;
			pXMLReader->GetLocalName(&pszLocalName, nullptr);
			if (!pszLocalName)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

			if (strcmp(pszLocalName, XML_3MF_ATTRIBUTE_PREFIX_XML) == 0) {
				PModelReader_InstructionElement pXMLNode = std::make_shared<CModelReader_InstructionElement>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
			}

			// Compare with Keystore Node Name
			if (strcmp(pszLocalName, XML_3MF_ELEMENT_KEYSTORE) == 0) {
				if (bHasModel)
					throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
				bHasModel = true;

				PModelReaderNode_KeyStore pXMLNode = std::make_shared<CModelReaderNode_KeyStore>(m_pKeyStore.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
			}
		}
	}

	void CKeyStoreOpcPackageReader::openAllResourceData() {
		for (nfUint32 i = 0; i < m_pKeyStore->getResourceDataCount() && !m_pSecureContext->emptyKekCtx(); ++i) {
			PKeyStoreResourceData rd = m_pKeyStore->getResourceDataByIndex(i);
			for (auto it = m_pSecureContext->kekCtxBegin(); it != m_pSecureContext->kekCtxEnd() && !rd->empty(); ++it) {
				PKeyStoreConsumer consumer = m_pKeyStore->findConsumerById((*it).first);
				if (consumer) {
					PKeyStoreDecryptRight decryptRight = rd->findDecryptRightByConsumer(consumer);
					if (decryptRight) {
						KeyWrappingDescriptor ctx = (*it).second;
						ctx.m_sKekDecryptData.m_sConsumerId = consumer->getConsumerID();
						ctx.m_sKekDecryptData.m_sResourcePath = rd->getPath()->getPath();
						CIPHERVALUE closed = decryptRight->getCipherValue();
						rd->setCipherValue(closed);
						size_t decrypted = ctx.m_fnWrap(closed.m_key, ctx.m_sKekDecryptData);
						if (decrypted) {
							ctx.m_sKekDecryptData.m_KeyBuffer.resize(decrypted, 0);
							rd->open(ctx.m_sKekDecryptData.m_KeyBuffer);
							break;
						}
					}
				}
			}
		}
	}
	void CKeyStoreOpcPackageReader::checkAuthenticatedTags() {
		if (m_pSecureContext->hasDekCtx()) {
			int count = m_pKeyStore->getResourceDataCount();
			for (int i = 0; i < count; ++i) {
				NMR::PKeyStoreResourceData rd = m_pKeyStore->getResourceDataByIndex(i);
				ContentEncryptionDescriptor descriptor = m_pSecureContext->getDekCtx();
				descriptor.m_sDekDecryptData.m_sCipherValue = rd->getCipherValue();
				descriptor.m_sDekDecryptData.m_nfHandler = rd->getHandle();
				descriptor.m_fnCrypt(0, nullptr, nullptr, descriptor.m_sDekDecryptData);
			}
		}
	}
}
