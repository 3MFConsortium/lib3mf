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
#include "Common/NMR_ErrorConst.h"
#include "Model/Writer/NMR_KeyStoreOpcPackageWriter.h" 
#include "Model/Writer/SecureContent101/NMR_ModelWriterNode_KeyStore.h"
#include "Common/Platform/NMR_ExportStream_Compressed.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Common/OPC/NMR_OpcPackageWriter.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelContext.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Common/NMR_SecureContext.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/Platform/NMR_ExportStream_Encrypted.h"
#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/NMR_ModelWarnings.h"
#include "Model/Classes/NMR_KeyStoreFactory.h"

namespace NMR {


	CKeyStoreOpcPackageWriter::CKeyStoreOpcPackageWriter(_In_ PExportStream pImportStream, _In_ CModelContext const & context)
		:m_pContext(context)
	{
		if (!context.isComplete())
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);

		m_pPackageWriter = std::make_shared<COpcPackageWriter>(pImportStream);
		refreshAllResourceDataGroups();
	}

	void CKeyStoreOpcPackageWriter::refreshAllResourceDataGroups() {
		PKeyStore const & keyStore = m_pContext.keyStore();
		for (nfUint64 i = 0; i < keyStore->getResourceDataGroupCount(); ++i) {
			PKeyStoreResourceDataGroup rdg = keyStore->getResourceDataGroup(i);
			if (rdg->isOpen()) {
				for (nfUint64 j = 0; j < rdg->getAccessRightCount(); ++j) {
					PKeyStoreAccessRight ar = rdg->getAccessRight(j);
					refreshAccessRight(ar, rdg->getKey());
				}
			}
		}

		PModel const & model = m_pContext.model();
		if (!model->hasCryptoRandCallbak())
			m_pContext.warnings()->addWarning(NMR_ERROR_RNGCALLBACKNOTCRYPTOSTRONG, eModelWarningLevel::mrwInvalidOptionalValue);
		for (nfUint64 i = 0; i < keyStore->getResourceDataCount(); ++i) {
			PKeyStoreResourceData rd = keyStore->getResourceData(i);
			if (rd->getGroup()->isOpen()) {
				std::vector<nfByte> newIv(fnGetAlgorithmInitVectorSize(rd->getEncryptionAlgorithm()), 0);
				model->generateRandomBytes(newIv.data(), newIv.size());
				rd->setInitVector(newIv);
			}
		}
	}

	bool CKeyStoreOpcPackageWriter::pathIsEncrypted(_In_ std::string sPath)
	{
		PSecureContext const& secureContext = m_pContext.secureContext();
		PKeyStore const& keyStore = m_pContext.keyStore();

		NMR::PKeyStoreResourceData rd = keyStore->findResourceData(sPath);
		if (nullptr != rd) {
			if (secureContext->hasDekCtx()) {
				return true;
			}
			else {
				m_pContext.warnings()->addWarning(NMR_ERROR_DEKDESCRIPTORNOTFOUND, eModelWarningLevel::mrwMissingMandatoryValue);
			}
		}
		return false;
	}

	POpcPackagePart CKeyStoreOpcPackageWriter::wrapPartStream(PKeyStoreResourceData rd, POpcPackagePart part) {
		PSecureContext const & secureContext = m_pContext.secureContext();
		ContentEncryptionDescriptor p = secureContext->getDekCtx();
		PKeyStoreResourceDataGroup rdg = m_pContext.keyStore()->findResourceDataGroupByResourceDataPath(rd->packagePath());
		p.m_sDekDecryptData.m_sParams = CKeyStoreFactory::makeContentEncryptionParams(rd, rdg);

		PExportStream stream;
		PExportStream encryptStream = std::make_shared<CExportStream_Encrypted>(part->getExportStream(), p);
		if (rd->isCompressed()) {
			PExportStream compressStream = std::make_shared<CExportStream_Compressed>(encryptStream);
			stream = compressStream;
		} else {
			stream = encryptStream;
		}
		return std::make_shared<COpcPackagePart>(*part, stream);
	}

	void CKeyStoreOpcPackageWriter::refreshResourceDataTag(PKeyStoreResourceData rd) {
		ContentEncryptionDescriptor dekCtx = m_pContext.secureContext()->getDekCtx();
		PKeyStoreResourceDataGroup rdg = m_pContext.keyStore()->findResourceDataGroupByResourceDataPath(rd->packagePath());
		dekCtx.m_sDekDecryptData.m_sParams = CKeyStoreFactory::makeContentEncryptionParams(rd, rdg);
		dekCtx.m_fnCrypt(0, nullptr, nullptr, dekCtx.m_sDekDecryptData);
		rd->setAuthTag(dekCtx.m_sDekDecryptData.m_sParams->getAuthTag());
	}

	void CKeyStoreOpcPackageWriter::refreshAccessRight(PKeyStoreAccessRight ar, std::vector<nfByte> const & key) {
		try {
			KeyWrappingDescriptor ctx = m_pContext.secureContext()->getKekCtx(ar->getConsumer()->getConsumerID());
			ctx.m_sKekDecryptData.m_pAccessRight = ar;
			std::vector<nfByte> closedKey;
			//give consumer a chance to (re)encrypt this key
			nfUint64 wrapped = ctx.m_fnWrap(key, closedKey, ctx.m_sKekDecryptData);
			ar->setCipherValue(closedKey);
		} catch (CNMRException const & e) {
			if (ar->isNew()) {
				m_pContext.warnings()->addException(e, eModelWarningLevel::mrwFatal);
			}
			//here, either there is no registered consumer client or client decided not to refresh the value
		}
	}

	POpcPackagePart CKeyStoreOpcPackageWriter::addPart(_In_ std::string sPath)
	{
		PSecureContext const & secureContext = m_pContext.secureContext();
		PKeyStore const & keyStore = m_pContext.keyStore();

		auto pPart = m_pPackageWriter->addPart(sPath);
		NMR::PKeyStoreResourceData rd = keyStore->findResourceData(sPath);
		if (nullptr != rd) {
			if (secureContext->hasDekCtx()) {
				return wrapPartStream(rd, pPart);
			} else {
				m_pContext.warnings()->addWarning(NMR_ERROR_DEKDESCRIPTORNOTFOUND, eModelWarningLevel::mrwFatal);
			}
		}
		return pPart;
	}

	void CKeyStoreOpcPackageWriter::close() {
		PSecureContext const & secureContext = m_pContext.secureContext();
		PKeyStore const & keyStore = m_pContext.keyStore();

		for (nfUint32 i = 0; i < keyStore->getResourceDataCount(); i++) {
			PKeyStoreResourceData rd = keyStore->getResourceData(i);
			if(secureContext->hasDekCtx()){
				refreshResourceDataTag(rd);
			}
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

	std::list<POpcPackageRelationship> CKeyStoreOpcPackageWriter::addWriterSpecificRelationships(_In_ POpcPackagePart pOpcPackagePart, _In_ COpcPackagePart* pTargetPart)
	{
		std::list<POpcPackageRelationship> list;
		std::string sPath = fnIncludeLeadingPathDelimiter(pTargetPart->getURI());
		if (pathIsEncrypted(sPath))
		{
			list.push_back(addPartRelationship(pOpcPackagePart, PACKAGE_ENCRYPTED_FILE_RELATIONSHIP, pTargetPart));
		}

		list.merge(m_pPackageWriter->addWriterSpecificRelationships(pOpcPackagePart, pTargetPart));
		return list;
	}

	void CKeyStoreOpcPackageWriter::writeKeyStoreStream(_In_ CXmlWriter * pXMLWriter) {
		pXMLWriter->WriteStartDocument();

		CModelWriterNode_KeyStore XMLNode4KeyStore(pXMLWriter, m_pContext.monitor(), m_pContext.keyStore());
		XMLNode4KeyStore.writeToXML();

		pXMLWriter->WriteEndDocument();

		pXMLWriter->Flush();
	}

}
