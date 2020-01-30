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
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/OPC/NMR_OpcPackageReader.h"
#include "Common/OPC/NMR_OpcPackagePart.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Reader/NMR_ModelReader_InstructionElement.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStore.h"



namespace NMR {
	CKeyStoreOpcPackageReader::CKeyStoreOpcPackageReader(PImportStream pImportStream, PModelReaderWarnings pWarnings, PProgressMonitor pProgressMonitor)
	{
		m_pPackageReader = std::make_shared<COpcPackageReader>(pImportStream, pWarnings, pProgressMonitor);
		m_KeyStore = std::make_shared<CKeyStore>();

		PImportStream keyStoreStream = findKeyStoreStream();
		if (nullptr != keyStoreStream) {
			parseKeyStore(keyStoreStream, pProgressMonitor);
			for (int i = m_KeyStore->getResourceDataCount())
			//for each resource data
			//	if (there are no decrypt rights)
			//		do nothing
			//	if (there are no registered consumers)
			//		do nothing
			//	for each registered consumer
			//		find decryptright
			//			succeed = consumercallback(cipherkey, plainkey, consumer keyvalue, encryption algorithm)
			//			if succeed 
			//				set ciphervalue into resource data (resource data is open)
			//	
		}
	}

	COpcPackageRelationship * CKeyStoreOpcPackageReader::findRootRelation(std::string sRelationType, nfBool bMustBeUnique) {
		return m_pPackageReader->findRootRelation(sRelationType, bMustBeUnique);
	}

	POpcPackagePart CKeyStoreOpcPackageReader::createPart(std::string sPath) {
		auto pPart = m_pPackageReader->createPart(sPath);
		NMR::PKeyStoreResourceData rd = m_KeyStore->findResourceDataByPath(sPath);
		if (nullptr != rd) {
			//if resource data is open
			//	create encrypted stream with ciphervalue and decryptcontext (function and userdata)
			//else
			//	create encrypted stream with null ciphervalue and decryptcontext (function and userdata)
			PImportStream encryptedStream = std::make_shared<CImportStream_Encrypted>(pPart->getImportStream(), rd->getCipherValue(), m_sDecryptContext);
		}
		return pPart;
	}

	nfUint64 CKeyStoreOpcPackageReader::getPartSize(std::string sPath) {
		return m_pPackageReader->getPartSize(sPath);
	}

	PKeyStore CKeyStoreOpcPackageReader::getKeyStore() const {
		return m_KeyStore;
	}

	NMR::PImportStream CKeyStoreOpcPackageReader::findKeyStoreStream() {
		COpcPackageRelationship * pKeyStoreRelation = m_pPackageReader->findRootRelation(PACKAGE_KEYSTORE_RELATIONSHIP_TYPE, true);
		if (pKeyStoreRelation != nullptr) {
			std::string sTargetPartURI = pKeyStoreRelation->getTargetPartURI();
			POpcPackagePart pKeystorePart = m_pPackageReader->createPart(sTargetPartURI);
			if (pKeystorePart == nullptr)
				throw CNMRException(NMR_ERROR_OPCCOULDNOTGETKEYSTORESTREAM);
			return pKeystorePart->getImportStream();
		}
		return nullptr;
	}

	void CKeyStoreOpcPackageReader::parseKeyStore(NMR::PImportStream keyStoreStream, NMR::PProgressMonitor pProgressMonitor) {

		PXmlReader pXMLReader = fnCreateXMLReaderInstance(keyStoreStream, pProgressMonitor);
		nfBool bHasModel = false;
		NMR::PModelReaderWarnings m_pWarnings;
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

			// Compare with Model Node Name
			if (strcmp(pszLocalName, XML_3MF_ELEMENT_KEYSTORE) == 0) {
				if (bHasModel)
					throw CNMRException(NMR_ERROR_DUPLICATEMODELNODE);
				bHasModel = true;

				//				m_pModel->setCurrentPath(m_pModel->rootPath());
				PModelReaderNode_KeyStore pXMLNode = std::make_shared<CModelReaderNode_KeyStore>(m_KeyStore.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader.get());
				/*
				if (!pXMLNode->getHasResources())
				throw CNMRException(NMR_ERROR_NORESOURCES);
				if (!pXMLNode->getHasBuild())
				throw CNMRException(NMR_ERROR_NOBUILD);*/
			}

		}
	}
}