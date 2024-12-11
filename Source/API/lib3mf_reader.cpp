/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CReader 
**************************************************************************************************************************/

void CReader::ReadFromFile(const std::string & sFilename)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::ReadFromBuffer(const Lib3MF_uint64 nBufferBufferSize, const Lib3MF_uint8 * pBufferBuffer)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::ReadFromCallback(const Lib3MF::ReadCallback pTheReadCallback, const Lib3MF_uint64 nStreamSize, const Lib3MF::SeekCallback pTheSeekCallback, const Lib3MF_pvoid pUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::SetProgressCallback(const Lib3MF::ProgressCallback pProgressCallback, const Lib3MF_pvoid pUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::AddRelationToRead(const std::string & sRelationShipType)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::RemoveRelationToRead(const std::string & sRelationShipType)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::SetStrictModeActive(const bool bStrictModeActive)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CReader::GetStrictModeActive()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CReader::GetWarning(const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nErrorCode)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CReader::GetWarningCount()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::AddKeyWrappingCallback(const std::string & sConsumerID, const Lib3MF::KeyWrappingCallback pTheCallback, const Lib3MF_pvoid pUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CReader::SetContentEncryptionCallback(const Lib3MF::ContentEncryptionCallback pTheCallback, const Lib3MF_pvoid pUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

