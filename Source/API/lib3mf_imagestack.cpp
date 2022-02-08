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

Abstract: This is a stub class definition of CImageStack

*/

#include "lib3mf_imagestack.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_attachment.hpp"

#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_ImportStream_Native.h"

#include "Common/NMR_StringUtils.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImageStack 
**************************************************************************************************************************/

CImageStack::CImageStack(NMR::PModelImageStack pModelImageStack)
	:CResource(pModelImageStack), CImage3D(pModelImageStack), m_pModelImageStack(pModelImageStack)
{
}


Lib3MF_uint32 CImageStack::GetRowCount()
{
	return m_pModelImageStack->getRowCount();
}

void CImageStack::SetRowCount(const Lib3MF_uint32 nRowCount)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CImageStack::GetColumnCount()
{
	return m_pModelImageStack->getColumnCount();
}

void CImageStack::SetColumnCount(const Lib3MF_uint32 nColumnCount)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CImageStack::GetSheetCount()
{
	return m_pModelImageStack->getSheetCount();
}

IAttachment * CImageStack::GetSheet(const Lib3MF_uint32 nIndex)
{
	auto pAttachment = m_pModelImageStack->getSheet(nIndex);
	return new CAttachment(pAttachment);
}

void CImageStack::SetSheet(const Lib3MF_uint32 nIndex, IAttachment* pSheet)
{
	if (pSheet == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CAttachment* pAttachment = dynamic_cast<CAttachment*> (pSheet);
	if (pAttachment == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pModelImageStack->setSheet(nIndex, pAttachment->getModelAttachment());
}

IAttachment * CImageStack::CreateEmptySheet(const Lib3MF_uint32 nIndex, const std::string & sPath)
{
	NMR::PImportStream pStream = std::make_shared<NMR::CImportStream_Unique_Memory>();
	auto pAttachment = m_pModelImageStack->createSheet(nIndex, sPath, pStream);
	return new CAttachment(pAttachment);
}

IAttachment * CImageStack::CreateSheetFromBuffer(const Lib3MF_uint32 nIndex, const std::string & sPath, const Lib3MF_uint64 nDataBufferSize, const Lib3MF_uint8 * pDataBuffer)
{
	NMR::PImportStream pStream = std::make_shared<NMR::CImportStream_Unique_Memory>(pDataBuffer, nDataBufferSize);
	auto pAttachment = m_pModelImageStack->createSheet(nIndex, sPath, pStream);
	return new CAttachment(pAttachment);
}

IAttachment * CImageStack::CreateSheetFromFile(const Lib3MF_uint32 nIndex, const std::string & sPath, const std::string & sFileName)
{
	std::wstring sUTF16FileName = NMR::fnUTF8toUTF16(sFileName);
	NMR::PImportStream pFileStream = std::make_shared<NMR::CImportStream_Native>(sUTF16FileName.c_str());
	auto pAttachment = m_pModelImageStack->createSheet(nIndex, sPath, pFileStream->copyToMemory());
	return new CAttachment(pAttachment);
}

