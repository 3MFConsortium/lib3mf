/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFReader

*/

#include "lib3mf_reader.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFReader 
**************************************************************************************************************************/

CLib3MFReader::CLib3MFReader(std::string sReaderClass, NMR::PModel model)
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

NMR::CModelReader& CLib3MFReader::reader()
{
	return *m_pReader;
}

void CLib3MFReader::ReadFromFile (const std::string & sFilename)
{
	NMR::PImportStream pImportStream = NMR::fnCreateImportStreamInstance(sFilename.c_str());
	reader().readStream(pImportStream);
}

void CLib3MFReader::ReadFromBuffer (const Lib3MF_uint64 nBufferBufferSize, const Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PImportStream pImportStream = std::make_shared<NMR::CImportStream_Memory>(pBufferBuffer, nBufferBufferSize);
	reader().readStream(pImportStream);
}

void CLib3MFReader::SetProgressCallback(const Lib3MFProgressCallback pProgressCallback, const Lib3MF_int64 nUserData)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFReader::AddRelationToRead (const std::string & sRelationShipType)
{
	reader().addRelationToRead(sRelationShipType);
}

void CLib3MFReader::RemoveRelationToRead (const std::string & sRelationShipType)
{
	reader().removeRelationToRead(sRelationShipType);
}

void CLib3MFReader::SetStrictModeActive (const bool bStrictModeActive)
{
	if (bStrictModeActive)
		reader().getWarnings()->setCriticalWarningLevel(NMR::mrwInvalidOptionalValue);
	else
		reader().getWarnings()->setCriticalWarningLevel(NMR::mrwFatal);
}

bool CLib3MFReader::GetStrictModeActive ()
{
	return reader().getWarnings()->getCriticalWarningLevel() == NMR::mrwInvalidOptionalValue;
}

std::string CLib3MFReader::GetWarning (const Lib3MF_uint32 nIndex, Lib3MF_uint32 & nErrorCode)
{
	auto warning = reader().getWarnings()->getWarning(nIndex);
	nErrorCode = warning->getErrorCode();
	return warning->getMessage();
}

Lib3MF_uint32 CLib3MFReader::GetWarningCount ()
{
	return reader().getWarnings()->getWarningCount();
}

