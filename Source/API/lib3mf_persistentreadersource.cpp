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

Abstract: This is a stub class definition of CPersistentReaderSource

*/

#include "lib3mf_persistentreadersource.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Callback.h"
#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Common/Platform/NMR_ImportStream_Native.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CPersistentReaderSource 
**************************************************************************************************************************/
CPersistentReaderSource::CPersistentReaderSource(NMR::PModelPersistentDataSource pDataSource)
	: m_pDataSource (pDataSource)
{
	if (pDataSource.get () == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	
}

CPersistentReaderSource::~CPersistentReaderSource()
{

}


Lib3MF::ePersistentReaderSourceType CPersistentReaderSource::GetSourceType()
{
	auto pStream = m_pDataSource->getImportStream();

	auto pMemoryStream = dynamic_cast<NMR::CImportStream_Memory *> (pStream.get());
	if (pMemoryStream != nullptr)
		return Lib3MF::ePersistentReaderSourceType::MemoryBuffer;

	auto pCallbackStream = dynamic_cast<NMR::CImportStream_Callback*> (pStream.get());
	if (pCallbackStream != nullptr)
		return Lib3MF::ePersistentReaderSourceType::Callback;

	auto pFileStream = dynamic_cast<NMR::CImportStream_Native*> (pStream.get());
	if (pFileStream != nullptr)
		return Lib3MF::ePersistentReaderSourceType::FileOnDisk;

	return Lib3MF::ePersistentReaderSourceType::Unknown;
}

void CPersistentReaderSource::InvalidateSourceData()
{
	m_pDataSource->invalidateSource();
}

bool CPersistentReaderSource::SourceDataIsValid()
{
	return m_pDataSource->isValid();
}

NMR::PModelPersistentDataSource CPersistentReaderSource::getDataSource()
{
	return m_pDataSource;
}
