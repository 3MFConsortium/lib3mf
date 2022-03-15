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

Abstract: This is a stub class definition of CPersistent3MFPackage

*/

#include "lib3mf_persistent3mfpackage.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_keystore.hpp"

#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#include "Model/Reader/NMR_KeyStoreOpcPackageReader.h"

#include "Common/Platform/NMR_ImportStream_Native.h"
// Include custom headers here.
#include "Common/Platform/NMR_Platform.h"


using namespace Lib3MF::Impl;

CPersistent3MFPackage::CPersistent3MFPackage(NMR::PModel pModel, const std::string& sFileName)
	: m_pModel (pModel), m_sFileName (sFileName)
{

	if (pModel == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

CPersistent3MFPackage::~CPersistent3MFPackage()
{

}

IKeyStore * CPersistent3MFPackage::ExtractKeyStore()
{
	NMR::PKeyStore pKeyStore = std::make_shared<NMR::CKeyStore>();

	NMR::PImportStream pPackageStream = NMR::fnCreateImportStreamInstance(m_sFileName.c_str());

	auto pReader = std::make_shared<NMR::CModelReader_3MF_Native>(m_pModel);
	auto pKeyStoreReader = std::make_shared<NMR::CKeyStoreOpcPackageReader>(pPackageStream, *pReader);

	/*auto pKeyStoreStream = pKeyStoreReader->findKeyStoreStream();
	auto pKeyStoreInMemory = pKeyStoreStream->copyToMemory();
	auto nSize = pKeyStoreInMemory->retrieveSize();
	pKeyStoreInMemory->seekPosition(0, true);

	std::vector<uint8_t> buffer;
	buffer.resize(nSize + 1);
	pKeyStoreInMemory->readIntoBuffer(buffer.data(), nSize, true);
	buffer[nSize] = 0;

	m_sKeyStoreDataString = (char*)buffer.data(); */
	return new CKeyStore(m_pModel, pReader->keyStore());
}

void CPersistent3MFPackage::UpdateKeyStore(IKeyStore* pKeyStoreInstance)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CPersistent3MFPackage::GetKeyStoreString()
{
	return m_sKeyStoreDataString;
}

