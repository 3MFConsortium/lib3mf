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

Abstract: This is a stub class definition of CBinaryStream

*/

#include "lib3mf_binarystream.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/NMR_UUID.h"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBinaryStream 
**************************************************************************************************************************/

CBinaryStream::CBinaryStream(std::string sPackageIndexPath, std::string sPackageBinaryPath, NMR::PChunkedBinaryStreamWriter pStreamWriter)
	: m_sPackageIndexPath (sPackageIndexPath), m_sPackageBinaryPath (sPackageBinaryPath), m_pStreamWriter (pStreamWriter)
{
	if (pStreamWriter.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::CUUID uuid;
	m_sUUID = uuid.toString();

}



std::string CBinaryStream::GetBinaryPath()
{
	return m_sPackageBinaryPath;
}

std::string CBinaryStream::GetIndexPath()
{
	return m_sPackageIndexPath;
}

std::string CBinaryStream::GetUUID()
{
	return m_sUUID;
}

void CBinaryStream::DisableDiscretizedArrayCompression()
{
	m_pStreamWriter->setDefaultCompressionMode(false, 0.001, NMR::eChunkedBinaryPredictionType::eptNoPredicition);
}

void CBinaryStream::EnableDiscretizedArrayCompression(const Lib3MF_double dUnits, const Lib3MF::eBinaryStreamPredictionType ePredictionType)
{
	switch (ePredictionType) {
		case eBinaryStreamPredictionType::NoPrediction: 
			m_pStreamWriter->setDefaultCompressionMode(true, dUnits, NMR::eChunkedBinaryPredictionType::eptNoPredicition);
			break;
		case eBinaryStreamPredictionType::DeltaPrediction:
			m_pStreamWriter->setDefaultCompressionMode(true, dUnits, NMR::eChunkedBinaryPredictionType::eptDeltaPredicition);
			break;
		default:
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}
}

void CBinaryStream::EnableLZ4(const Lib3MF_uint32 nCompressionLevel)
{
	m_pStreamWriter->setCompressionType(NMR::eChunkedBinaryCompressionType::ctLZ4, nCompressionLevel);
}

void CBinaryStream::EnableZLib(const Lib3MF_uint32 nCompressionLevel)
{
	m_pStreamWriter->setCompressionType(NMR::eChunkedBinaryCompressionType::ctZLib, nCompressionLevel);
}

void CBinaryStream::EnableZstd(const Lib3MF_uint32 nCompressionLevel)
{
	m_pStreamWriter->setCompressionType(NMR::eChunkedBinaryCompressionType::ctZstd, nCompressionLevel);
}
