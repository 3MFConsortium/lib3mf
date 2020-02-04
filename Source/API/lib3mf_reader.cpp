/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CReader

*/

#include "lib3mf_reader.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_cipherdata.hpp"
// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Callback.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/NMR_SecureContext.h"
using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CReader 
**************************************************************************************************************************/

CReader::CReader(std::string sReaderClass, NMR::PModel model)
{
	m_pReader = nullptr;

	// Create specified writer instance
	if (sReaderClass.compare("3mf") == 0) {
		m_pReader = std::make_shared<NMR::CModelReader_3MF_Native>(model);

	}
	else if (sReaderClass.compare("stl") == 0) {
		m_pReader = std::make_shared<NMR::CModelReader_STL>(model);
	}

	if (!m_pReader)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_READERCLASSUNKNOWN);
}

NMR::CModelReader& CReader::reader()
{
	return *m_pReader;
}

void CReader::ReadFromFile (const std::string & sFilename)
{
	NMR::PImportStream pImportStream = NMR::fnCreateImportStreamInstance(sFilename.c_str());

	try {
		reader().readStream(pImportStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		}
		else throw e;
	}
}

void CReader::ReadFromBuffer (const Lib3MF_uint64 nBufferBufferSize, const Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PImportStream pImportStream = std::make_shared<NMR::CImportStream_Shared_Memory>(pBufferBuffer, nBufferBufferSize);

	try {
		reader().readStream(pImportStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		}
		else throw e;
	}
}

void CReader::ReadFromCallback(const Lib3MFReadCallback pTheReadCallback, const Lib3MF_uint64 nStreamSize, const Lib3MFSeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)
{
	
	NMR::ImportStream_ReadCallbackType lambdaReadCallback =
		[pTheReadCallback](NMR::nfByte* pData, NMR::nfUint64 cbBytes, void* pUserData)
	{
		(*pTheReadCallback)(reinterpret_cast<Lib3MF_uint64>(pData), cbBytes, pUserData);
		return 0;
	};

	NMR::ImportStream_SeekCallbackType lambdaSeekCallback =
		[pTheSeekCallback](NMR::nfUint64 nPosition, void* pUserData)
	{
		(*pTheSeekCallback)(nPosition, pUserData);
		return 0;
	};

	NMR::PImportStream pImportStream = std::make_shared<NMR::CImportStream_Callback>(
		lambdaReadCallback, lambdaSeekCallback,
		pUserData, nStreamSize);
	try {
		reader().readStream(pImportStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		}
		else throw e;
	}
}

void CReader::SetProgressCallback(const Lib3MFProgressCallback pProgressCallback, const Lib3MF_pvoid pUserData)
{
	NMR::Lib3MFProgressCallback lambdaCallback =
		[pProgressCallback](int progressStep, NMR::ProgressIdentifier identifier, void* pUserData)
	{
		bool ret;
		(*pProgressCallback)(&ret, progressStep / 100.0f, eLib3MFProgressIdentifier(identifier), pUserData);
		return ret;
	};
	m_pReader->SetProgressCallback(lambdaCallback, pUserData);
}

void CReader::AddRelationToRead (const std::string & sRelationShipType)
{
	reader().addRelationToRead(sRelationShipType);
}

void CReader::RemoveRelationToRead (const std::string & sRelationShipType)
{
	reader().removeRelationToRead(sRelationShipType);
}

void CReader::SetStrictModeActive (const bool bStrictModeActive)
{
	if (bStrictModeActive)
		reader().getWarnings()->setCriticalWarningLevel(NMR::mrwInvalidOptionalValue);
	else
		reader().getWarnings()->setCriticalWarningLevel(NMR::mrwFatal);
}

bool CReader::GetStrictModeActive ()
{
	return reader().getWarnings()->getCriticalWarningLevel() == NMR::mrwInvalidOptionalValue;
}

std::string CReader::GetWarning (const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nErrorCode)
{
	auto warning = reader().getWarnings()->getWarning(nIndex);
	nErrorCode = warning->getErrorCode();
	return warning->getMessage();
}

Lib3MF_uint32 CReader::GetWarningCount ()
{
	return reader().getWarnings()->getWarningCount();
}

#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "lib3mf_consumer.hpp"

void Lib3MF::Impl::CReader::RegisterKEKClient(const std::string &sConsumerID, Lib3MF::KeyDecryptionCallback pDecryptionCallback, Lib3MF_pvoid pUserData) {
	//TODO: this needs to be improved - looks like too much code to be handled here
	NMR::KEKDESCRIPTOR descriptor;
	descriptor.m_sKekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnDecrypt = 
		[this, pDecryptionCallback](
			std::vector<NMR::nfByte> const & cipher, 
			std::vector<NMR::nfByte> & plain, 
			NMR::KEKDECRYPTCTX ctx) {
		NMR::PKeyStore keystore = this->reader().getKeyStore();
		NMR::PKeyStoreConsumer consumer = keystore->findConsumerById(ctx.m_sConsumerId);
		__NMRASSERT(nullptr != consumer);
		NMR::PKeyStoreResourceData resourceData = keystore->findResourceDataByPath(ctx.m_sResourcePath);
		__NMRASSERT(nullptr != resourceData);
		NMR::PKeyStoreDecryptRight decryptRight = resourceData->findDecryptRightByConsumer(consumer);
		__NMRASSERT(nullptr != decryptRight);

		eEncryptionAlgorithm algorithm = (decryptRight->getEncryptionAlgorithm() == NMR::eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) 
			? eEncryptionAlgorithm::RsaOaepMgf1p : eEncryptionAlgorithm::Aes256Gcm;

		NMR::nfUint64 neededBytes = 0;
		NMR::nfUint64 result = -1;
		std::shared_ptr<IConsumer> pConsumer = std::make_shared<CConsumer>(consumer);
		IBase * pBaseConsumer(nullptr);
		pBaseConsumer = pConsumer.get();
		Lib3MF_Consumer handle = pBaseConsumer;
		pConsumer->IncRefCount();
		(*pDecryptionCallback)(handle, algorithm, cipher.size(), cipher.data(), 0, &neededBytes, nullptr, ctx.m_pUserData, &result);
		if (result == 0 && neededBytes > 0) {
			plain.resize(neededBytes, 0);
			pConsumer->IncRefCount();
			(*pDecryptionCallback)(handle, algorithm, cipher.size(), cipher.data(), plain.size(), nullptr, plain.data(), ctx.m_pUserData, &result);
		}
		return result;
	};
	m_pReader->getSecureContext()->addKekCtx(sConsumerID, descriptor);
}

void Lib3MF::Impl::CReader::RegisterDEKClient(Lib3MF::DataDecryptionCallback pDecryptionCallback, Lib3MF_pvoid pUserData) {
	NMR::DEKDESCRIPTOR descriptor;
	descriptor.m_sDekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnDecrypt = [this, pDecryptionCallback](std::vector<NMR::nfByte> const & cipher, NMR::nfByte * plain, NMR::DEKDECRYPTCTX ctx) {
		Lib3MF::sAes256CipherValue cipherDataValue;
		std::copy(ctx.m_sCipherValue.m_iv.begin(), ctx.m_sCipherValue.m_iv.end(), cipherDataValue.m_IV);
		std::copy(ctx.m_sCipherValue.m_key.begin(), ctx.m_sCipherValue.m_key.end(), cipherDataValue.m_Key);
		std::copy(ctx.m_sCipherValue.m_tag.begin(), ctx.m_sCipherValue.m_tag.end(), cipherDataValue.m_Tag);
		std::shared_ptr<CCipherData> pCipherData = std::make_shared<CCipherData>(cipherDataValue, ctx.m_nfHandler);
		IBase * pBaseCipherData(nullptr);
		pBaseCipherData = pCipherData.get();
		Lib3MF_CipherData handle = pBaseCipherData;
		pCipherData->IncRefCount();
		(*pDecryptionCallback)(eEncryptionAlgorithm::Aes256Gcm, handle, cipher.size(), cipher.data(), cipher.size(), nullptr, plain, ctx.m_pUserData);
		return 0;
	};
	m_pReader->getSecureContext()->setDekCtx(descriptor);
}

