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

NMR_OpcPackageWriter.cpp defines an OPC Package writer in a portable way.

--*/

#include "Common/NMR_Exception.h" 
#include "Common/NMR_StringUtils.h" 

#include "Model/Classes/NMR_ModelConstants.h"

#include "Model/Writer/NMR_KeyStoreOpcPackageWriter.h" 
#include "Model/Writer/SecureContent085/NMR_ModelWriterNode_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/Platform/NMR_ExportStream_Compressed.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Common/OPC/NMR_OpcPackageWriter.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelContext.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_SecureContext.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/Platform/NMR_ExportStream_Encrypted.h"
#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/Platform/NMR_ExportStream.h"

#include "Model/Classes/NMR_KeyStoreFactory.h"

namespace NMR {


	CKeyStoreOpcPackageWriter::CKeyStoreOpcPackageWriter(_In_ PExportStream pImportStream, _In_ CModelContext * context)
		:m_pContext(context)
	{
		if (nullptr == m_pContext
		 || nullptr == m_pContext->getKeyStore()
		 || nullptr == m_pContext->getSecureContext()
		 || nullptr == m_pContext->getProgressMonitor()
		 || nullptr == m_pContext->getModel())
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);

		m_pPackageWriter = std::make_shared<COpcPackageWriter>(pImportStream);
		refreshAllResourceData();
	}

	void CKeyStoreOpcPackageWriter::refreshAllResourceData() {
		PModel model = m_pContext->getModel();
		PKeyStore keyStore = m_pContext->getKeyStore();
		for (nfUint32 i = 0; i < keyStore->getResourceDataCount(); i++) {
			NMR::PKeyStoreResourceData rd = keyStore->getResourceData(i);
			//TODO: refresh iv and key on all resource datas from all resourcedatagroups
			//CEKPARAMS cv = rd->getCipherValue();
			//if (!rd->isClosed()){
			//	if (rd->isNew()) {
			//		model->generateRandomBytes(cv.m_key.data(), cv.m_key.size());
			//		rd->open(cv.m_key);
			//	}
			//	model->generateRandomBytes(cv.m_iv.data(), cv.m_iv.size());
			//	rd->refreshIV(cv.m_iv);
			//}
		}
	}

	void CKeyStoreOpcPackageWriter::wrapPartStream(PKeyStoreResourceData rd, POpcPackagePart part) {
		PSecureContext secureContext = m_pContext->getSecureContext();
		ContentEncryptionDescriptor p = secureContext->getDekCtx();
		PKeyStoreResourceDataGroup rdg = m_pContext->getKeyStore()->findResourceDataGroupByResourceDataPath(rd->getPath());
		p.m_sDekDecryptData.m_sParams = CKeyStoreFactory::makeContentEncryptionParams(rd, rdg);

		PExportStream stream;
		PExportStream encryptStream = std::make_shared<CExportStream_Encrypted>(part->getExportStream(), p);
		if (rd->isCompressed()) {
			PExportStream compressStream = std::make_shared<CExportStream_Compressed>(encryptStream);
			stream = compressStream;
		} else {
			stream = encryptStream;
		}
		part->setExportStream(stream);
	}

	void CKeyStoreOpcPackageWriter::refreshResourceDataTag(PKeyStoreResourceData rd) {
		PSecureContext secureContext = m_pContext->getSecureContext();
		ContentEncryptionDescriptor dekCtx = secureContext->getDekCtx();
		PKeyStoreResourceDataGroup rdg = m_pContext->getKeyStore()->findResourceDataGroupByResourceDataPath(rd->getPath());
		dekCtx.m_sDekDecryptData.m_sParams = CKeyStoreFactory::makeContentEncryptionParams(rd, rdg);
		dekCtx.m_fnCrypt(0, nullptr, nullptr, dekCtx.m_sDekDecryptData);
		rd->setAuthTag(dekCtx.m_sDekDecryptData.m_sParams->getAuthTag());
	}

	//TODO: fix access right refresh
	//void CKeyStoreOpcPackageWriter::updateDecryptRightCipher(PKeyStoreDecryptRight dr, PKeyStoreResourceData rd) {
	//	PSecureContext secureContext = m_pContext->getSecureContext();
	//	try {
	//		KeyWrappingDescriptor ctx = secureContext->getKekCtx(dr->getConsumer()->getConsumerID());
	//		ctx.m_sKekDecryptData.m_sConsumerId = dr->getConsumer()->getConsumerID();
	//		ctx.m_sKekDecryptData.m_sResourcePath = rd->getPath()->getPath();
	//		nfUint64 encrypted = ctx.m_fnWrap(rd->getCipherValue().m_key, ctx.m_sKekDecryptData);
	//		if (encrypted > 0) {
	//			CEKPARAMS cipherValue = rd->getCipherValue();
	//			cipherValue.m_key = ctx.m_sKekDecryptData.m_KeyBuffer;
	//			dr->open(cipherValue);
	//		}
	//	} catch (CNMRException const e) {
	//		if (e.getErrorCode() == NMR_ERROR_KEKDESCRIPTORNOTFOUND) {
	//			if (dr->isNew())
	//				throw;
	//			CEKPARAMS drCipherValue = dr->getCipherValue();
	//			CEKPARAMS rdCipherValue = rd->getCipherValue();
	//			drCipherValue.m_iv = rdCipherValue.m_iv;
	//			drCipherValue.m_tag = rdCipherValue.m_tag;
	//			dr->open(drCipherValue);
	//		} else {
	//			throw e;
	//		}
	//	}
	//}

	POpcPackagePart CKeyStoreOpcPackageWriter::addPart(_In_ std::string sPath)
	{
		PSecureContext secureContext = m_pContext->getSecureContext();
		PKeyStore keyStore = m_pContext->getKeyStore();

		auto pPart = m_pPackageWriter->addPart(sPath);
		NMR::PKeyStoreResourceData rd = keyStore->findResourceData(sPath);
		if (nullptr != rd) {
			if (secureContext->hasDekCtx()) {
				wrapPartStream(rd, pPart);
			} else {
				throw CNMRException(NMR_ERROR_DEKDESCRIPTORNOTFOUND);
			}
		}
		return pPart;
	}

	void CKeyStoreOpcPackageWriter::close() {
		PSecureContext secureContext = m_pContext->getSecureContext();
		PKeyStore keyStore = m_pContext->getKeyStore();

		for (nfUint32 i = 0; i < keyStore->getResourceDataCount(); i++) {
			PKeyStoreResourceData rd = keyStore->getResourceData(i);
			if(secureContext->hasDekCtx()){
				refreshResourceDataTag(rd);
			}
			//TODO update accessrights
			//for (nfUint32 j = 0; j < rd->getDecryptRightCount(); j++) {
			//	PKeyStoreDecryptRight dr = rd->getDecryptRight(j);
			//	updateDecryptRightCipher(dr, rd);
			//}
		}

		if (!keyStore->empty()) {
			POpcPackagePart pKeyStorePart = m_pPackageWriter->addPart(PACKAGE_3D_KEYSTORE_URI);
			m_pPackageWriter->addContentType(pKeyStorePart, PACKAGE_KEYSTORE_CONTENT_TYPE);
			m_pPackageWriter->addRootRelationship(PACKAGE_KEYSTORE_RELATIONSHIP_TYPE, pKeyStorePart.get());
			m_pPackageWriter->addRootRelationship(PACKAGE_MUST_PRESERVE_RELATIONSHIP_TYPE, pKeyStorePart.get());
			PXmlWriter_Native pXMLWriter4KeyStore = std::make_shared<CXmlWriter_Native>(pKeyStorePart->getExportStream());
			writeKeyStoreStream(pXMLWriter4KeyStore.get());
		}
	}

	void CKeyStoreOpcPackageWriter::addContentType(std::string sExtension, std::string sContentType)
	{
		m_pPackageWriter->addContentType(sExtension, sContentType);
	}

	void CKeyStoreOpcPackageWriter::addContentType(POpcPackagePart pOpcPackagePart, std::string sContentType)
	{
		m_pPackageWriter->addContentType(pOpcPackagePart, sContentType);
	}

	POpcPackageRelationship CKeyStoreOpcPackageWriter::addRootRelationship(std::string sType, COpcPackagePart * pTargetPart)
	{
		return m_pPackageWriter->addRootRelationship(sType, pTargetPart);
	}

	POpcPackageRelationship CKeyStoreOpcPackageWriter::addPartRelationship(POpcPackagePart pOpcPackagePart, std::string sType, COpcPackagePart * pTargetPart)
	{
		return m_pPackageWriter->addPartRelationship(pOpcPackagePart, sType, pTargetPart);
	}

	void CKeyStoreOpcPackageWriter::writeKeyStoreStream(_In_ CXmlWriter * pXMLWriter) {
		if (pXMLWriter == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pXMLWriter->WriteStartDocument();

		CModelWriterNode_KeyStore XMLNode4KeyStore(pXMLWriter, m_pContext->getProgressMonitor(), m_pContext->getKeyStore());
		XMLNode4KeyStore.writeToXML();

		pXMLWriter->WriteEndDocument();

		pXMLWriter->Flush();
	}

}
