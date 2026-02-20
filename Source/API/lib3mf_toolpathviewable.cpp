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

Abstract: This is a stub class definition of CToolpathViewable

*/

#include "lib3mf_toolpathviewable.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Model/ToolpathReader/NMR_ToolpathViewableJSONSerializer.h"
#include <cstring>


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathViewable 
**************************************************************************************************************************/

CToolpathViewable::CToolpathViewable(
	NMR::PImportStream pLayerDataStream,
	Lib3MF_uint32 nLayerIndex,
	const std::string & sLayerPath,
	Lib3MF_uint32 nLayerZMin,
	Lib3MF_uint32 nLayerZMax,
	Lib3MF_uint32 nLayerThickness)
	: m_pLayerDataStream(pLayerDataStream),
	m_nLayerIndex(nLayerIndex),
	m_sLayerPath(sLayerPath),
	m_nLayerZMin(nLayerZMin),
	m_nLayerZMax(nLayerZMax),
	m_nLayerThickness(nLayerThickness),
	m_bJSONCached(false)
{
	if (pLayerDataStream.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

CToolpathViewable::~CToolpathViewable()
{
}

void CToolpathViewable::cacheJSONIfNeeded() const
{
	if (!m_bJSONCached) {
		m_sCachedJSON = NMR::CToolpathViewableJSONSerializer::serializeLayerXMLToJSON(m_pLayerDataStream);
		m_bJSONCached = true;
	}
}

Lib3MF_uint32 CToolpathViewable::GetLayerIndex()
{
	return m_nLayerIndex;
}

std::string CToolpathViewable::GetLayerPath()
{
	return m_sLayerPath;
}

Lib3MF_uint32 CToolpathViewable::GetLayerZMin()
{
	return m_nLayerZMin;
}

Lib3MF_uint32 CToolpathViewable::GetLayerZMax()
{
	return m_nLayerZMax;
}

Lib3MF_uint32 CToolpathViewable::GetLayerThickness()
{
	return m_nLayerThickness;
}

std::string CToolpathViewable::GetJSONString()
{
	cacheJSONIfNeeded();
	return m_sCachedJSON;
}

void CToolpathViewable::GetJSONBuffer(Lib3MF_uint64 nJSONBufferBufferSize, Lib3MF_uint64* pJSONBufferNeededCount, Lib3MF_uint8 * pJSONBufferBuffer)
{
	cacheJSONIfNeeded();

	Lib3MF_uint64 nJSONSize = static_cast<Lib3MF_uint64>(m_sCachedJSON.size());
	if (pJSONBufferNeededCount)
		*pJSONBufferNeededCount = nJSONSize;

	if (pJSONBufferBuffer != nullptr) {
		if (nJSONBufferBufferSize < nJSONSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		if (nJSONSize > 0) {
			memcpy(pJSONBufferBuffer, m_sCachedJSON.data(), static_cast<size_t>(nJSONSize));
		}
	}
}

