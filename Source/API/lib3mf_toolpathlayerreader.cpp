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
#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_builditem.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_customdomtree.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLayerReader
**************************************************************************************************************************/

CToolpathLayerReader::CToolpathLayerReader(NMR::PModelToolpathLayerReadData pReadData, NMR::PModelToolpath pModelToolpath)
	: m_pReadData(pReadData), m_pModelToolpath(pModelToolpath)
{
	if (pReadData.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pModelToolpath.get() == nullptr)
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

IToolpathProfile* CToolpathLayerReader::GetSegmentDefaultProfile(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID = 0;
	uint32_t nPointCount = 0;
	uint32_t nPartID = 0;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	auto pProfile = m_pModelToolpath->getProfileByUUID(m_pReadData->mapIDtoUUID(nProfileID));
	return new CToolpathProfile (pProfile);
}

std::string CToolpathLayerReader::GetSegmentDefaultProfileUUID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID = 0;
	uint32_t nPartID = 0;
	uint32_t nPointCount = 0;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return m_pReadData->mapIDtoUUID(nProfileID);
}

Lib3MF_uint32 CToolpathLayerReader::GetSegmentDefaultProfileID(const Lib3MF_uint32 nSegmentIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID = 0;
	uint32_t nPartID = 0;
	uint32_t nPointCount = 0;
	m_pReadData->getSegmentInfo(nSegmentIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return nPartID;
}

std::string CToolpathLayerReader::GetProfileUUIDByLocalProfileID(const Lib3MF_uint32 nLocalProfileID)
{
	return m_pReadData->mapIDtoUUID(nLocalProfileID);
}


bool CToolpathLayerReader::SegmentHasUniformProfile(const Lib3MF_uint32 nSegmentIndex)
{
	return m_pReadData->segmentHasUniformProfile(nSegmentIndex);
}

bool CToolpathLayerReader::SegmentHasOverrideFactors(const Lib3MF_uint32 nSegmentIndex, const Lib3MF::eToolpathProfileOverrideFactor eOverrideFactor)
{
	NMR::eModelToolpathProfileOverrideFactor internalOverrideFactor;
	switch (eOverrideFactor) {
		case Lib3MF::eToolpathProfileOverrideFactor::FactorF:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorF;
			break;

		case Lib3MF::eToolpathProfileOverrideFactor::FactorG:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorG;
			break;

		case Lib3MF::eToolpathProfileOverrideFactor::FactorH:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorH;
			break;

		default:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfNone;

	}

	return m_pReadData->segmentHasOverrideFactors(nSegmentIndex, internalOverrideFactor);
}


IBuildItem* CToolpathLayerReader::GetSegmentBuildItem(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);
	std::string sUUID = m_pReadData->mapIDtoUUID(nPartID);

	auto pModel = m_pModelToolpath->getModel();
	auto pBuildItemInstance = pModel->findBuildItemByUUID(sUUID, true);

	return new CBuildItem(pBuildItemInstance);


}

std::string CToolpathLayerReader::GetSegmentBuildItemUUID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return m_pReadData->mapIDtoUUID(nPartID);
}

Lib3MF_uint32 CToolpathLayerReader::GetSegmentPartID(const Lib3MF_uint32 nIndex)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	return nPartID;
}

std::string CToolpathLayerReader::GetBuildItemUUIDByLocalPartID(const Lib3MF_uint32 nLocalPartID)
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
				NMR::TOOLPATHREADPOINT& point = m_pReadData->getSegmentPoint(nIndex, nPointIndex);
				pPoint->m_Coordinates[0] = (float) (point.m_nX * dUnits);
				pPoint->m_Coordinates[1] = (float) (point.m_nY * dUnits);
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
				NMR::TOOLPATHREADPOINT & point = m_pReadData->getSegmentPoint(nIndex, nPointIndex);
				pPoint->m_Coordinates[0] = point.m_nX;
				pPoint->m_Coordinates[1] = point.m_nY;
				pPoint++;
			}
		}
	}
}

void CToolpathLayerReader::GetSegmentPointOverrideFactors(const Lib3MF_uint32 nSegmentIndex, const Lib3MF::eToolpathProfileOverrideFactor eOverrideFactor, Lib3MF_uint64 nFactorValuesBufferSize, Lib3MF_uint64* pFactorValuesNeededCount, Lib3MF_double* pFactorValuesBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nSegmentIndex, eNMRType, nProfileID, nPartID, nPointCount);

	if (pFactorValuesNeededCount != nullptr) {
		*pFactorValuesNeededCount = nPointCount;
	}

	if (pFactorValuesBuffer != nullptr) {
		if (nFactorValuesBufferSize < nPointCount)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		bool bFillWithZeros = false;

		uint32_t nDenominator = m_pReadData->getSegmentOverrideDenominator(nSegmentIndex);
		if (nDenominator > 0) {
			double * pTarget = pFactorValuesBuffer;

			switch (eOverrideFactor) {
				case Lib3MF::eToolpathProfileOverrideFactor::FactorF:
					for (uint32_t nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++) {

						NMR::TOOLPATHREADPOINT& point = m_pReadData->getSegmentPoint(nSegmentIndex, nPointIndex);
						if (point.m_nFactorF < 0) {
							*pTarget = 0.0;
						}
						else if ((uint32_t)point.m_nFactorF >= nDenominator) {
							*pTarget = 1.0;
						}
						else {
							*pTarget = (double)point.m_nFactorF / (double)nDenominator;
						}

						pTarget++;

					}
					break;

				case Lib3MF::eToolpathProfileOverrideFactor::FactorG:
					for (uint32_t nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++) {

						NMR::TOOLPATHREADPOINT& point = m_pReadData->getSegmentPoint(nSegmentIndex, nPointIndex);
						if (point.m_nFactorG < 0) {
							*pTarget = 0.0;
						}
						else if ((uint32_t)point.m_nFactorG >= nDenominator) {
							*pTarget = 1.0;
						}
						else {
							*pTarget = (double)point.m_nFactorG / (double)nDenominator;
						}

						pTarget++;

					}
					break;

				case Lib3MF::eToolpathProfileOverrideFactor::FactorH:
					for (uint32_t nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++) {

						NMR::TOOLPATHREADPOINT& point = m_pReadData->getSegmentPoint(nSegmentIndex, nPointIndex);
						if (point.m_nFactorH < 0) {
							*pTarget = 0.0;
						}
						else if ((uint32_t)point.m_nFactorH >= nDenominator) {
							*pTarget = 1.0;
						}
						else {
							*pTarget = (double)point.m_nFactorH / (double)nDenominator;
						}

						pTarget++;

					}
					break;

				default:
					bFillWithZeros = true;

			}

		}
		else {
			bFillWithZeros = true;
		}

		if (bFillWithZeros) {
			for (uint32_t nPointIndex = 0; nPointIndex < nPointCount; nPointIndex++)
				pFactorValuesBuffer[nPointIndex] = 0.0;
		}

	}
}


void CToolpathLayerReader::GetSegmentHatchDataInModelUnits(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nHatchDataBufferSize, Lib3MF_uint64* pHatchDataNeededCount, Lib3MF::sHatch2D* pHatchDataBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	if (eNMRType == NMR::eModelToolpathSegmentType::HatchSegment) {
		uint32_t nHatchCount = nPointCount / 2;

		if (pHatchDataNeededCount != nullptr) {
			*pHatchDataNeededCount = nHatchCount;
		}

		if (pHatchDataBuffer != nullptr) {
			if (nHatchDataBufferSize < nHatchCount)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

			if (nHatchCount > 0) {

				double dUnits = m_pReadData->getUnits();

				uint32_t nHatchIndex;
				Lib3MF::sHatch2D* pHatch = pHatchDataBuffer;
				for (nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {
					auto & point1 = m_pReadData->getSegmentPoint(nIndex, nHatchIndex * 2);
					auto & point2 = m_pReadData->getSegmentPoint(nIndex, nHatchIndex * 2 + 1);
					pHatch->m_Point1Coordinates[0] = point1.m_nX * dUnits;
					pHatch->m_Point1Coordinates[1] = point1.m_nY * dUnits;
					pHatch->m_Point2Coordinates[0] = point2.m_nX * dUnits;
					pHatch->m_Point2Coordinates[1] = point2.m_nY * dUnits;
					pHatch->m_Tag = point1.m_nTag;
					pHatch++;
				}
			}
		}

	}
	else {

		if (pHatchDataNeededCount != nullptr) 
			*pHatchDataNeededCount = 0;

	}

}

void CToolpathLayerReader::GetSegmentHatchDataDiscrete(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nHatchDataBufferSize, Lib3MF_uint64* pHatchDataNeededCount, Lib3MF::sDiscreteHatch2D* pHatchDataBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nPointCount;
	m_pReadData->getSegmentInfo(nIndex, eNMRType, nProfileID, nPartID, nPointCount);

	if (eNMRType == NMR::eModelToolpathSegmentType::HatchSegment) {
		uint32_t nHatchCount = nPointCount / 2;

		if (pHatchDataNeededCount != nullptr) {
			*pHatchDataNeededCount = nHatchCount;
		}

		if (pHatchDataBuffer != nullptr) {
			if (nHatchDataBufferSize < nHatchCount)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

			if (nHatchCount > 0) {

				uint32_t nHatchIndex;
				Lib3MF::sDiscreteHatch2D* pHatch = pHatchDataBuffer;
				for (nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {
					auto& point1 = m_pReadData->getSegmentPoint(nIndex, nHatchIndex * 2);
					auto& point2 = m_pReadData->getSegmentPoint(nIndex, nHatchIndex * 2 + 1);
					pHatch->m_Point1Coordinates[0] = point1.m_nX;
					pHatch->m_Point1Coordinates[1] = point1.m_nY;
					pHatch->m_Point2Coordinates[0] = point2.m_nX;
					pHatch->m_Point2Coordinates[1] = point2.m_nY;
					pHatch->m_Tag = point1.m_nTag;
					pHatch++;
				}
			}
		}

	}
	else {

		if (pHatchDataNeededCount != nullptr)
			*pHatchDataNeededCount = 0;

	}

}


void CToolpathLayerReader::GetSegmentHatchOverrideFactors(const Lib3MF_uint32 nSegmentIndex, const Lib3MF::eToolpathProfileOverrideFactor eOverrideFactor, Lib3MF_uint64 nFactorValuesBufferSize, Lib3MF_uint64* pFactorValuesNeededCount, Lib3MF::sHatch2DOverrides* pFactorValuesBuffer)
{
	NMR::eModelToolpathSegmentType eNMRType;
	uint32_t nProfileID;
	uint32_t nPartID;
	uint32_t nDoubleHatchCount;
	m_pReadData->getSegmentInfo(nSegmentIndex, eNMRType, nProfileID, nPartID, nDoubleHatchCount);

	if (eNMRType == NMR::eModelToolpathSegmentType::HatchSegment) {
		uint32_t nHatchCount = nDoubleHatchCount / 2;

		bool bFillWithZeros = false;

		if (pFactorValuesNeededCount != nullptr) {
			*pFactorValuesNeededCount = nHatchCount;
		}

		if ((pFactorValuesBuffer != nullptr) && (nHatchCount > 0)) {
			if (nFactorValuesBufferSize < nHatchCount)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);


			uint32_t nDenominator = m_pReadData->getSegmentOverrideDenominator(nSegmentIndex);
			if (nDenominator > 0) {
				Lib3MF::sHatch2DOverrides* pTarget = pFactorValuesBuffer;

				switch (eOverrideFactor) {
				case Lib3MF::eToolpathProfileOverrideFactor::FactorF:
					for (uint32_t nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {

						NMR::TOOLPATHREADPOINT& point1 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2);
						if (point1.m_nFactorF < 0) {
							pTarget->m_Point1Override = 0.0;
						}
						else if ((uint32_t)point1.m_nFactorF >= nDenominator) {
							pTarget->m_Point1Override = 1.0;
						}
						else {
							pTarget->m_Point1Override = (double)point1.m_nFactorF / (double)nDenominator;
						}

						NMR::TOOLPATHREADPOINT& point2 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2 + 1);
						if (point2.m_nFactorF < 0) {
							pTarget->m_Point2Override = 0.0;
						}
						else if ((uint32_t)point2.m_nFactorF >= nDenominator) {
							pTarget->m_Point2Override = 1.0;
						}
						else {
							pTarget->m_Point2Override = (double)point2.m_nFactorF / (double)nDenominator;
						}

						pTarget++;

					}
					break;

				case Lib3MF::eToolpathProfileOverrideFactor::FactorG:
					for (uint32_t nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {

						NMR::TOOLPATHREADPOINT& point1 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2);
						if (point1.m_nFactorG < 0) {
							pTarget->m_Point1Override = 0.0;
						}
						else if ((uint32_t)point1.m_nFactorG >= nDenominator) {
							pTarget->m_Point1Override = 1.0;
						}
						else {
							pTarget->m_Point1Override = (double)point1.m_nFactorG / (double)nDenominator;
						}

						NMR::TOOLPATHREADPOINT& point2 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2 + 1);
						if (point2.m_nFactorG < 0) {
							pTarget->m_Point2Override = 0.0;
						}
						else if ((uint32_t)point2.m_nFactorG >= nDenominator) {
							pTarget->m_Point2Override = 1.0;
						}
						else {
							pTarget->m_Point2Override = (double)point2.m_nFactorG / (double)nDenominator;
						}

						pTarget++;

					}
					break;


				case Lib3MF::eToolpathProfileOverrideFactor::FactorH:
					for (uint32_t nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {

						NMR::TOOLPATHREADPOINT& point1 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2);
						if (point1.m_nFactorH < 0) {
							pTarget->m_Point1Override = 0.0;
						}
						else if ((uint32_t)point1.m_nFactorH >= nDenominator) {
							pTarget->m_Point1Override = 1.0;
						}
						else {
							pTarget->m_Point1Override = (double)point1.m_nFactorH / (double)nDenominator;
						}

						NMR::TOOLPATHREADPOINT& point2 = m_pReadData->getSegmentPoint(nSegmentIndex, nHatchIndex * 2 + 1);
						if (point2.m_nFactorH < 0) {
							pTarget->m_Point2Override = 0.0;
						}
						else if ((uint32_t)point2.m_nFactorH >= nDenominator) {
							pTarget->m_Point2Override = 1.0;
						}
						else {
							pTarget->m_Point2Override = (double)point2.m_nFactorH / (double)nDenominator;
						}

						pTarget++;

					}
					break;

				default:
					bFillWithZeros = true;

				}

			}
			else {
				bFillWithZeros = true;
			}

			if (bFillWithZeros) {
				for (uint32_t nHatchIndex = 0; nHatchIndex < nHatchCount; nHatchIndex++) {
					pFactorValuesBuffer[nHatchIndex].m_Point1Override = 0.0;
					pFactorValuesBuffer[nHatchIndex].m_Point2Override = 0.0;
				}
			}

		}

	}
	else {

		if (pFactorValuesNeededCount != nullptr)
			*pFactorValuesNeededCount = 0;

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

void CToolpathLayerReader::FindSegmentAttributeInfoByName(const std::string& sNameSpace, const std::string& sAttributeName, Lib3MF_uint32& nID, Lib3MF::eToolpathAttributeType& eAttributeType)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findSegmentAttribute(sNameSpace, sAttributeName, true);
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


Lib3MF_uint32 CToolpathLayerReader::FindSegmentAttributeIDByName(const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findSegmentAttribute(sNameSpace, sAttributeName, true);
	return attributeInfo.first;
}

Lib3MF::eToolpathAttributeType CToolpathLayerReader::FindSegmentAttributeTypeByName(const std::string& sNameSpace, const std::string& sAttributeName)
{
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findSegmentAttribute(sNameSpace, sAttributeName, true);

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
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findSegmentAttribute(sNameSpace, sAttributeName, true);
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
	std::pair<uint32_t, NMR::eModelToolpathSegmentAttributeType> attributeInfo = m_pReadData->findSegmentAttribute(sNameSpace, sAttributeName, true);
	if (attributeInfo.second != NMR::eModelToolpathSegmentAttributeType::SegmentAttributeDouble)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDATTRIBUTETYPE);

	return m_pReadData->getSegmentDoubleAttribute(nIndex, attributeInfo.first);
}

Lib3MF_uint32 CToolpathLayerReader::GetPartCount()
{
	return m_pReadData->getPartCount();
}

void CToolpathLayerReader::GetPartInformation(const Lib3MF_uint32 nPartIndex, Lib3MF_uint32& nPartID, std::string& sBuildItemUUID) 
{
	m_pReadData->getPartInformation(nPartIndex, nPartID, sBuildItemUUID);
}

IBuildItem* CToolpathLayerReader::GetPartBuildItem(const Lib3MF_uint32 nPartIndex)
{
	std::string sBuildItemUUID;
	uint32_t nPartID = 0;
	m_pReadData->getPartInformation(nPartIndex, nPartID, sBuildItemUUID);

	auto pModel = m_pModelToolpath->getModel();
	auto pBuildItemInstance = pModel->findBuildItemByUUID(sBuildItemUUID, true);

	return new CBuildItem(pBuildItemInstance);

}


