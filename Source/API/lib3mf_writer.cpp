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

Abstract: This is a stub class definition of CWriter

*/

#include "lib3mf_writer.hpp"
#include "lib3mf_interfaceexception.hpp"
#include <memory>

// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ExportStream_Callback.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"
#include "Common/Platform/NMR_ExportStream_Dummy.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/NMR_SecureContext.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "API/lib3mf_consumer.hpp"
#include "lib3mf_cipherdata.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CWriter 
**************************************************************************************************************************/

CWriter::CWriter(std::string sWriterClass, NMR::PModel model)
{
	m_pWriter = nullptr;

	// Create specified writer instance
	if (sWriterClass.compare("3mf") == 0) {
		m_pWriter = std::make_shared<NMR::CModelWriter_3MF_Native>(model);
	}
	else if (sWriterClass.compare("stl") == 0) {
		m_pWriter = std::make_shared<NMR::CModelWriter_STL>(model);
	}

	if (!m_pWriter)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_WRITERCLASSUNKNOWN);
}

NMR::CModelWriter& CWriter::writer()
{
	return *m_pWriter;
}

void CWriter::WriteToFile (const std::string & sFilename)
{
	setlocale(LC_ALL, "C");
	NMR::PExportStream pStream = NMR::fnCreateExportStreamInstance(sFilename.c_str());
	try {
		writer().exportToStream(pStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		} else if (e.getErrorCode() == NMR_ERROR_DEKDESCRIPTORNOTFOUND
				|| e.getErrorCode() == NMR_ERROR_KEKDESCRIPTORNOTFOUND) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_SECURECONTEXTNOTREGISTERED);
		} else throw e;
	}
}

Lib3MF_uint64 CWriter::GetStreamSize ()
{
	// Write to a special dummy stream just to calculate the size
	NMR::PExportStreamDummy pStream = std::make_shared<NMR::CExportStreamDummy>();
	try {
		writer().exportToStream(pStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		} else if (e.getErrorCode() == NMR_ERROR_DEKDESCRIPTORNOTFOUND
			|| e.getErrorCode() == NMR_ERROR_KEKDESCRIPTORNOTFOUND) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_SECURECONTEXTNOTREGISTERED);
		} else throw e;
	}

	return pStream->getDataSize();
}

void CWriter::WriteToBuffer(Lib3MF_uint64 nBufferBufferSize, Lib3MF_uint64* pBufferNeededCount, Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PExportStreamMemory pStream;
	if (!momentBuffer || momentBuffer->getDataSize() > nBufferBufferSize) {
		pStream = std::make_shared<NMR::CExportStreamMemory>();
		try {
			writer().exportToStream(pStream);
		} catch (NMR::CNMRException&e) {
			if (e.getErrorCode() == NMR_USERABORTED) {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
			} else if (e.getErrorCode() == NMR_ERROR_DEKDESCRIPTORNOTFOUND
				|| e.getErrorCode() == NMR_ERROR_KEKDESCRIPTORNOTFOUND) {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_SECURECONTEXTNOTREGISTERED);
			} else throw e;
		}
	} else {
		pStream = momentBuffer;
	}

	Lib3MF_uint64 cbStreamSize = pStream->getDataSize();
	if (pBufferNeededCount)
		*pBufferNeededCount = cbStreamSize;

	if (nBufferBufferSize >= cbStreamSize) {
		// TODO eliminate this copy, perhaps by allowing CExportStreamMemory to use existing buffers
		std::memcpy(pBufferBuffer, pStream->getData(), static_cast<size_t>(cbStreamSize));
		momentBuffer.reset();
	} else {
		momentBuffer = pStream;
	}
}

void CWriter::WriteToCallback(const Lib3MFWriteCallback pTheWriteCallback, const Lib3MFSeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)
{
	NMR::ExportStream_WriteCallbackType lambdaWriteCallback =
		[pTheWriteCallback](NMR::nfByte* pData, NMR::nfUint64 cbBytes, void* pUserData)
	{
		(*pTheWriteCallback)(reinterpret_cast<Lib3MF_uint64>(pData), cbBytes, pUserData );
		return 0;
	};

	NMR::ExportStream_SeekCallbackType lambdaSeekCallback =
		[pTheSeekCallback](NMR::nfUint64 nPosition, void* pUserData)
	{
		(*pTheSeekCallback)(nPosition, pUserData);
		return 0;
	};

	NMR::PExportStream pStream = std::make_shared<NMR::CExportStream_Callback>(lambdaWriteCallback, lambdaSeekCallback, pUserData);
	try {
		writer().exportToStream(pStream);
	}
	catch (NMR::CNMRException&e) {
		if (e.getErrorCode() == NMR_USERABORTED) {
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		}
		else throw e;
	}
}

void CWriter::SetProgressCallback(const Lib3MFProgressCallback callback, const Lib3MF_pvoid pUserData)
{
	NMR::Lib3MFProgressCallback lambdaCallback = 
		[callback](int progressStep, NMR::ProgressIdentifier identifier, void* pUserData)
		{
			bool ret;
			(*callback)(&ret, progressStep /100.0f, eLib3MFProgressIdentifier(identifier), pUserData);
			return ret;
		};
	m_pWriter->SetProgressCallback(lambdaCallback, reinterpret_cast<void*>(pUserData));
}

Lib3MF_uint32 CWriter::GetDecimalPrecision()
{
	return m_pWriter->GetDecimalPrecision();
}

void CWriter::SetDecimalPrecision(const Lib3MF_uint32 nDecimalPrecision)
{
	m_pWriter->SetDecimalPrecision(nDecimalPrecision);
}

void Lib3MF::Impl::CWriter::AddKeyWrappingCallback(const Lib3MF::KeyWrappingCallback pTheCallback, const std::string &sConsumerID, const Lib3MF_pvoid pUserData) {
	NMR::KeyWrappingDescriptor descriptor;
	descriptor.m_sKekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnWrap =
		[this, pTheCallback](
			std::vector<NMR::nfByte> const & plain,
			NMR::KeyWrappingContext & ctx) {
		NMR::PKeyStore keystore = this->writer().getKeyStore();
		NMR::PKeyStoreConsumer consumer = keystore->findConsumerById(ctx.m_sConsumerId);
		__NMRASSERT(nullptr != consumer);
		NMR::PKeyStoreResourceData resourceData = keystore->findResourceDataByPath(ctx.m_sResourcePath);
		__NMRASSERT(nullptr != resourceData);
		NMR::PKeyStoreDecryptRight decryptRight = resourceData->findDecryptRightByConsumer(consumer);
		__NMRASSERT(nullptr != decryptRight);

		eEncryptionAlgorithm algorithm = (decryptRight->getEncryptionAlgorithm() == NMR::eKeyStoreEncryptAlgorithm::RsaOaepMgf1p)
			? eEncryptionAlgorithm::RsaOaepMgf1p : eEncryptionAlgorithm::Aes256Gcm;

		NMR::nfUint64 result = 0;
		std::shared_ptr<IConsumer> pConsumer = std::make_shared<CConsumer>(consumer);
		IBase * pBaseConsumer(nullptr);
		pBaseConsumer = pConsumer.get();
		Lib3MF_Consumer handle = pBaseConsumer;
		(*pTheCallback)(handle, algorithm, plain.size(), plain.data(),
			ctx.m_KeyBuffer.size(), nullptr, ctx.m_KeyBuffer.data(),
			ctx.m_pUserData, &result);

		if (result < 0)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);

		return result;
	};
	writer().getSecureContext()->addKekCtx(sConsumerID, descriptor);
}

void Lib3MF::Impl::CWriter::SetContentEncryptionCallback(const Lib3MF::ContentEncryptionCallback pTheCallback, const Lib3MF_pvoid pUserData)
{
	NMR::ContentEncryptionDescriptor descriptor;
	descriptor.m_sDekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnCrypt = [this, pTheCallback](
			NMR::nfUint64 size,
			NMR::nfByte const * plain,
			NMR::nfByte * cipher,
			NMR::ContentEncryptionContext & ctx) {
		Lib3MF::sAes256CipherValue cipherDataValue;
		__NMRASSERT(ctx.m_sCipherValue.m_iv.size() == sizeof(cipherDataValue.m_IV));
		std::copy(ctx.m_sCipherValue.m_iv.begin(), ctx.m_sCipherValue.m_iv.end(), cipherDataValue.m_IV);

		__NMRASSERT(ctx.m_sCipherValue.m_key.size() == sizeof(cipherDataValue.m_Key));
		std::copy(ctx.m_sCipherValue.m_key.begin(), ctx.m_sCipherValue.m_key.end(), cipherDataValue.m_Key);

		__NMRASSERT(ctx.m_sCipherValue.m_tag.size() == sizeof(cipherDataValue.m_Tag));
		std::copy(ctx.m_sCipherValue.m_tag.begin(), ctx.m_sCipherValue.m_tag.end(), cipherDataValue.m_Tag);

		std::shared_ptr<CCipherData> pCipherData = std::make_shared<CCipherData>(cipherDataValue, ctx.m_nfHandler);
		IBase * pBaseCipherData(nullptr);
		pBaseCipherData = pCipherData.get();
		Lib3MF_CipherData handle = pBaseCipherData;
		NMR::nfUint64 result = 0;
		(*pTheCallback)(eEncryptionAlgorithm::Aes256Gcm, handle, size, plain, size, nullptr, cipher, ctx.m_pUserData, &result);
		if (result < 0)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		else if (result == 0) {
			std::copy(&cipherDataValue.m_Tag[0], &cipherDataValue.m_Tag[16], ctx.m_sCipherValue.m_tag.begin());
		}
		return result;
	};
	m_pWriter->getSecureContext()->setDekCtx(descriptor);
}
