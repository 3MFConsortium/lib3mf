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

Abstract: This is a stub class definition of CToolpathLayerReader

*/

#include "lib3mf_toolpathlayerreader.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_customdomtree.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLayerReader
**************************************************************************************************************************/

CToolpathLayerReader::CToolpathLayerReader(NMR::PModelToolpathLayerReadData pReadData)
	: m_pReadData(pReadData)
{
	if (pReadData.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}

CToolpathLayerReader::~CToolpathLayerReader()
{

}

std::string CToolpathLayerReader::GetLayerDataUUID()
{
	return m_pReadData->getUUID();
}

Lib3MF_uint32 CToolpathLayerReader::GetSegmentCount()
{
	return m_pReadData->getSegmentCount();
}

void CToolpathLayerReader::GetSegmentInfo(const Lib3MF_uint32 nIndex, Lib3MF::eToolpathSegmentType& eType, Lib3MF_uint32& nPointCount)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	switch (eNMRType) {
	case NMR::eModelToolpathSegmentType::HatchSegment: eType = eToolpathSegmentType::Hatch; break;
	case NMR::eModelToolpathSegmentType::LoopSegment: eType = eToolpathSegmentType::Loop; break;
	case NMR::eModelToolpathSegmentType::PolylineSegment: eType = eToolpathSegmentType::Polyline; break;
	default:
		eType = eToolpathSegmentType::Unknown;
	}
}

IToolpathProfile* CToolpathLayerReader::GetSegmentProfile(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CToolpathLayerReader::GetSegmentProfileUUID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return m_pReadData->mapIDtoUUID(nProfileID);
}

IBuildItem* CToolpathLayerReader::GetSegmentPart(const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CToolpathLayerReader::GetSegmentPartUUID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return m_pReadData->mapIDtoUUID(nPartID);
}

void CToolpathLayerReader::GetSegmentPointData(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nPointDataBufferSize, Lib3MF_uint64* pPointDataNeededCount, Lib3MF::sPosition2D* pPointDataBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	if (pPointDataNeededCount != nullptr) {
		*pPointDataNeededCount = nPointCount;
	}

	if (pPointDataBuffer != nullptr) {
		if (nPointDataBufferSize < nPointCount)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		if (nPointCount > 0) {

			uint32_t nPointIndex;
			Lib3MF::sPosition2D* pPoint = pPointDataBuffer;
			for (nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++) {
				NMR::NVEC2 position = m_pReadData->getSegmentPoint(nIndex, nPointIndex);
				pPoint->m_Coordinates[0] = position.m_values.x;
				pPoint->m_Coordinates[1] = position.m_values.y;
				pPoint++;
			}
		}
	}
}

Lib3MF_uint32 CToolpathLayerReader::GetCustomDataCount()
{
	return m_pReadData->getCustomXMLDataCount();
}

ICustomDOMTree* CToolpathLayerReader::GetCustomData(const Lib3MF_uint32 nIndex)
{
	return new CCustomDOMTree(m_pReadData->getCustomXMLData (nIndex));
}

void CToolpathLayerReader::GetCustomDataName(const Lib3MF_uint32 nIndex, std::string& sNameSpace, std::string& sDataName)
{
	auto pCustomData = m_pReadData->getCustomXMLData(nIndex);
	sNameSpace = pCustomData->getNameSpace();
	sDataName = pCustomData->getRootName();
}

Lib3MF_uint32 CToolpathLayerReader::FindUint32AttributeID(const std::string& sNameSpace, const std::string& sAttributeName)
{
	return m_pReadData->findUint32Attribute(sNameSpace, sAttributeName, true);
}


Lib3MF_uint32 CToolpathLayerReader::GetSegmentUint32AttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID)
{
	return m_pReadData->getSegmentUint32Attribute(nIndex, nID);
}

Lib3MF_uint32 CToolpathLayerReader::GetSegmentUint32AttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName)
{
	uint32_t nID = m_pReadData->findUint32Attribute(sNameSpace, sAttributeName, true);
	return m_pReadData->getSegmentUint32Attribute(nIndex, nID);
}

Lib3MF_uint32 CToolpathLayerReader::FindDoubleAttributeID(const std::string& sNameSpace, const std::string& sAttributeName)
{
	return m_pReadData->findDoubleAttribute(sNameSpace, sAttributeName, true);
}


Lib3MF_double CToolpathLayerReader::GetSegmentDoubleAttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID)
{
	return m_pReadData->getSegmentDoubleAttribute(nIndex, nID);
}

Lib3MF_double CToolpathLayerReader::GetSegmentDoubleAttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName)
{
	uint32_t nID = m_pReadData->findDoubleAttribute(sNameSpace, sAttributeName, true);
	return m_pReadData->getSegmentDoubleAttribute(nIndex, nID);
}

