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

Lib3MF_uint32 CToolpathLayerReader::GetSegmentLocalPartID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return nPartID;
}

std::string CToolpathLayerReader::GetPartUUIDByLocalPartID(const Lib3MF_uint32 nLocalPartID)
{
	return m_pReadData->mapIDtoUUID(nLocalPartID);
}


void CToolpathLayerReader::GetSegmentPointDataInModelUnits(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nPointDataBufferSize, Lib3MF_uint64* pPointDataNeededCount, Lib3MF::sPosition2D* pPointDataBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	double dUnits = m_pReadData->getUnits();

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
				NMR::NVEC2I position = m_pReadData->getSegmentPoint(nIndex, nPointIndex);
				pPoint->m_Coordinates[0] = (float) (position.m_values.x * dUnits);
				pPoint->m_Coordinates[1] = (float) (position.m_values.y * dUnits);
				pPoint++;
			}
		}
	}
}

void CToolpathLayerReader::GetSegmentPointDataDiscrete(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nPointDataBufferSize, Lib3MF_uint64* pPointDataNeededCount, Lib3MF::sDiscretePosition2D* pPointDataBuffer)
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
			Lib3MF::sDiscretePosition2D* pPoint = pPointDataBuffer;
			for (nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++) {
				NMR::NVEC2I position = m_pReadData->getSegmentPoint(nIndex, nPointIndex);
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

void CToolpathLayerReader::FindAttributeInfoByName(const std::string& sNameSpace, const std::string& sAttributeName, Lib3MF_uint32& nID, Lib3MF::eToolpathAttributeType& eAttributeType)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findAttribute(sNameSpace, sAttributeName, true);
	nID = attributeInfo.first;
	
	switch (attributeInfo.second) {
	case NMR::eModelToolpathSegmentAttributeType::SegmentAttributeInt64:
		eAttributeType = eToolpathAttributeType::Integer;
		break;
	case NMR::eModelToolpathSegmentAttributeType::SegmentAttributeDouble:
		eAttributeType = eToolpathAttributeType::Double;
		break;
	default:
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDATTRIBUTETYPE);


	}
}


Lib3MF_uint32 CToolpathLayerReader::FindAttributeIDByName(const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findAttribute(sNameSpace, sAttributeName, true);
	return attributeInfo.first;
}

Lib3MF::eToolpathAttributeType CToolpathLayerReader::FindAttributeValueByName(const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findAttribute(sNameSpace, sAttributeName, true);

	switch (attributeInfo.second) {
	case NMR::eModelToolpathSegmentAttributeType::SegmentAttributeInt64:
		return eToolpathAttributeType::Integer;
	case NMR::eModelToolpathSegmentAttributeType::SegmentAttributeDouble:
		return eToolpathAttributeType::Double;
	default:
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDATTRIBUTETYPE);

	}

}

Lib3MF_int64 CToolpathLayerReader::GetSegmentIntegerAttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID)
{
	return m_pReadData->getSegmentIntegerAttribute (nIndex, nID);
}

Lib3MF_int64 CToolpathLayerReader::GetSegmentIntegerAttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findAttribute(sNameSpace, sAttributeName, true);
	if (attributeInfo.second != NMR::eModelToolpathSegmentAttributeType::SegmentAttributeInt64)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDATTRIBUTETYPE);

	return m_pReadData->getSegmentIntegerAttribute(nIndex, attributeInfo.first);

}





Lib3MF_double CToolpathLayerReader::GetSegmentDoubleAttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID)
{
	return m_pReadData->getSegmentDoubleAttribute(nIndex, nID);
}

Lib3MF_double CToolpathLayerReader::GetSegmentDoubleAttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findAttribute(sNameSpace, sAttributeName, true);
	if (attributeInfo.second != NMR::eModelToolpathSegmentAttributeType::SegmentAttributeDouble)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDATTRIBUTETYPE);

	return m_pReadData->getSegmentDoubleAttribute(nIndex, attributeInfo.first);
}

