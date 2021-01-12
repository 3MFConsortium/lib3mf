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

// Include custom headers here.
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_accessright.hpp"
#include "lib3mf_contentencryptionparams.hpp"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Callback.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/NMR_SecureContext.h"
#include "Model/Classes/NMR_KeyStore.h"


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
		reader().warnings()->setCriticalWarningLevel(NMR::mrwInvalidOptionalValue);
	else
		reader().warnings()->setCriticalWarningLevel(NMR::mrwFatal);
}

bool CReader::GetStrictModeActive ()
{
	return reader().warnings()->getCriticalWarningLevel() == NMR::mrwInvalidOptionalValue;
}

std::string CReader::GetWarning (const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nErrorCode)
{
	auto warning = reader().warnings()->getWarning(nIndex);
	nErrorCode = warning->getErrorCode();
	return warning->getMessage();
}

Lib3MF_uint32 CReader::GetWarningCount ()
{
	return reader().warnings()->getWarningCount();
}

void Lib3MF::Impl::CReader::AddKeyWrappingCallback(const std::string &sConsumerID, const Lib3MF::KeyWrappingCallback pTheCallback, const Lib3MF_pvoid pUserData) {
	NMR::KeyWrappingDescriptor descriptor;
	descriptor.m_sKekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnWrap =
		[this, pTheCallback](
			std::vector<NMR::nfByte> const & cipher,
			std::vector<NMR::nfByte> & plain,
			NMR::KeyWrappingContext & ctx) {

		std::shared_ptr<IAccessRight> pAccessRight = std::make_shared<CAccessRight>(ctx.m_pAccessRight);
		IBase * pBaseEntity(nullptr);
		pBaseEntity = pAccessRight.get();
		Lib3MF_AccessRight entityHandle = pBaseEntity;

		//figure out the size of the key.
		Lib3MF_uint64 needed = 0;
		Lib3MF_uint64 result = 0;
		(*pTheCallback)(entityHandle, cipher.size(), cipher.data(),
			0, &needed, nullptr, ctx.m_pUserData, &result);
		if (result == 0)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);

		plain.resize(needed, 0);

		result = 0;
		(*pTheCallback)(entityHandle, cipher.size(), cipher.data(),
			plain.size(), nullptr, plain.data(), ctx.m_pUserData, &result);
		if (result == 0)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		return (NMR::nfUint64)result;
	};
	reader().secureContext()->addKekCtx(sConsumerID, descriptor);
}
void Lib3MF::Impl::CReader::SetContentEncryptionCallback(const Lib3MF::ContentEncryptionCallback pTheCallback, const Lib3MF_pvoid pUserData) {
	NMR::ContentEncryptionDescriptor descriptor;
	descriptor.m_sDekDecryptData.m_pUserData = pUserData;
	descriptor.m_fnCrypt = [this, pTheCallback](NMR::nfUint64 size, NMR::nfByte const * cipher, NMR::nfByte * plain, NMR::ContentEncryptionContext & ctx) {
		std::shared_ptr<CContentEncryptionParams> pCekParams = std::make_shared<CContentEncryptionParams>(ctx.m_sParams);
		IBase * pBaseEntity(nullptr);
		pBaseEntity = pCekParams.get();
		Lib3MF_ContentEncryptionParams entityHandle = pBaseEntity;
		Lib3MF_uint64 result = 0;
		(*pTheCallback)(entityHandle, (Lib3MF_uint64)size, cipher, (Lib3MF_uint64)size, nullptr, plain, ctx.m_pUserData, &result);
		if (result == 0)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_CALCULATIONABORTED);
		return (NMR::nfUint64)result;
	};
	reader().secureContext()->setDekCtx(descriptor);
}

