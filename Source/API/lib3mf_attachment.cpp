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

Abstract: This is a stub class definition of CAttachment

*/

#include "lib3mf_attachment.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/NMR_StringUtils.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CAttachment 
**************************************************************************************************************************/

CAttachment::CAttachment(NMR::PModelAttachment pModelAttachment)
	: m_pModelAttachment(pModelAttachment)
{
	
}

std::string CAttachment::GetPath ()
{
	return m_pModelAttachment->getPathURI();
}

void CAttachment::SetPath (const std::string & sPath)
{
	NMR::CModel * pModel = m_pModelAttachment->getModel();
	NMR::PImportStream pStream = m_pModelAttachment->getStream();
	if (pModel->getPackageThumbnail() == m_pModelAttachment) {
		// different handling for package-wide attachment
		pModel->removePackageThumbnail();
		m_pModelAttachment = pModel->addPackageThumbnail(sPath, pStream);
	}
	else {
		std::string sRelationshipType = m_pModelAttachment->getRelationShipType();
		pModel->removeAttachment(m_pModelAttachment->getPathURI());
		m_pModelAttachment = pModel->addAttachment(sPath, sRelationshipType, pStream);
	}
}

IPackagePart * CAttachment::PackagePart()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CAttachment::GetRelationShipType ()
{
	return m_pModelAttachment->getRelationShipType();
}

void CAttachment::SetRelationShipType (const std::string & sPath)
{
	m_pModelAttachment->setRelationShipType(sPath);
}

void CAttachment::WriteToFile (const std::string & sFileName)
{
	NMR::PImportStream pStream = m_pModelAttachment->getStream();

	if (pStream.get() != nullptr) {
		pStream->writeToFile(NMR::fnUTF8toUTF16(sFileName).c_str());
	} else {
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDATTACHMENTSTREAM);
	}
}

void CAttachment::ReadFromFile (const std::string & sFileName)
{
	NMR::PImportStream pImportStream = NMR::fnCreateImportStreamInstance(sFileName.c_str());

	m_pModelAttachment->setStream(pImportStream);
}

Lib3MF_uint64 CAttachment::GetStreamSize ()
{
	NMR::PImportStream pStream = m_pModelAttachment->getStream();
	return pStream->retrieveSize();
}

void CAttachment::WriteToBuffer (Lib3MF_uint64 nBufferBufferSize, Lib3MF_uint64* pBufferNeededCount, Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PImportStream pStream = m_pModelAttachment->getStream();

	Lib3MF_uint64 cbStreamSize = pStream->retrieveSize();
	if (pBufferNeededCount)
		*pBufferNeededCount = cbStreamSize;

	if (nBufferBufferSize >= cbStreamSize) {
		pStream->seekPosition(0, true);
		pStream->readBuffer(pBufferBuffer, cbStreamSize, true);
	}
}

void CAttachment::ReadFromBuffer(const Lib3MF_uint64 nBufferBufferSize, const Lib3MF_uint8 * pBufferBuffer)
{
	NMR::PImportStream pImportStream = std::make_shared<NMR::CImportStream_Unique_Memory>(pBufferBuffer, nBufferBufferSize);
	m_pModelAttachment->setStream(pImportStream);
}

