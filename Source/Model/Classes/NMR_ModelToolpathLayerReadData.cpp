/*++

Copyright (C) 2018 3MF Consortium

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

Abstract:

NMR_ModelToolpathLayerReadData.cpp defines the Model Toolpath Layer Read Data.

--*/

#include "Model/Classes/NMR_ModelToolpathLayerReadData.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelObject.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include "Common/NMR_UUID.h"

#include <sstream>
#include <algorithm>

namespace NMR {

	CModelToolpathLayerReadData::CModelToolpathLayerReadData(_In_ PModelToolpath pModelToolpath)
		: m_pModelToolpath (pModelToolpath), m_pCurrentSegment(nullptr), 
		m_pCurrentAttributePage (nullptr), m_nCurrentPositionOnPage (TOOLPATHSEGMENTATTRIBUTEPAGESIZE)
	{
		if (pModelToolpath.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}

	CModelToolpathLayerReadData::~CModelToolpathLayerReadData()
	{

	}

	std::string CModelToolpathLayerReadData::getUUID()
	{
		return m_sUUID;
	}

	void CModelToolpathLayerReadData::beginSegment(eModelToolpathSegmentType eType, nfUint32 nProfileID, nfUint32 nPartID)
	{
		if (m_pCurrentSegment != nullptr)
			throw CNMRException(NMR_ERROR_LAYERSEGMENTALREADYOPEN);

		m_pCurrentSegment = m_Segments.allocData();
		m_pCurrentSegment->m_eType = eType;
		m_pCurrentSegment->m_nPartID = nPartID;
		m_pCurrentSegment->m_nProfileID = nProfileID;
		m_pCurrentSegment->m_nStartPoint = m_Points.getCount ();
		m_pCurrentSegment->m_nPointCount = 0;

		size_t nNumberOfUint32Attributes = m_Uint32AttributesOfNextSegment.size();
		size_t nNumberOfDoubleAttributes = m_DoubleAttributesOfNextSegment.size();

		m_pCurrentSegment->m_pUint32Attributes = (uint32_t*) allocAttributeBuffer((uint32_t)(sizeof (uint32_t) * nNumberOfUint32Attributes));
		m_pCurrentSegment->m_pDoubleAttributes = (double*)allocAttributeBuffer((uint32_t) (sizeof(double) * nNumberOfDoubleAttributes));

		for (size_t nIndex = 0; nIndex < nNumberOfUint32Attributes; nIndex++)
			m_pCurrentSegment->m_pUint32Attributes[nIndex] = m_Uint32AttributesOfNextSegment.at(nIndex);
		for (size_t nIndex = 0; nIndex < nNumberOfDoubleAttributes; nIndex++)
			m_pCurrentSegment->m_pDoubleAttributes[nIndex] = m_DoubleAttributesOfNextSegment.at(nIndex);

	}

	void CModelToolpathLayerReadData::endSegment()
	{
		if (m_pCurrentSegment == nullptr)
			throw CNMRException(NMR_ERROR_LAYERSEGMENTNOTOPEN);

		nfUint32 nCount = m_Points.getCount();

		__NMRASSERT(m_pCurrentSegment->m_nStartPoint <= nCount);

		m_pCurrentSegment->m_nPointCount = nCount - m_pCurrentSegment->m_nStartPoint;

		m_pCurrentSegment = nullptr;
	}

	void CModelToolpathLayerReadData::addPoint(nfFloat fX, nfFloat fY)
	{
		if (m_pCurrentSegment == nullptr)
			throw CNMRException(NMR_ERROR_LAYERSEGMENTNOTOPEN);

		NVEC2 * pVec = m_Points.allocData();
		pVec->m_values.x = fX;
		pVec->m_values.y = fY;
	}

	nfUint32 CModelToolpathLayerReadData::getSegmentCount()
	{
		return m_Segments.getCount();
	}

	void CModelToolpathLayerReadData::getSegmentInfo(nfUint32 nSegmentIndex, eModelToolpathSegmentType & eType, nfUint32 & nProfileID, nfUint32 & nPartID, nfUint32 & nPointCount)
	{
		TOOLPATHREADSEGMENT * pSegment = m_Segments.getData(nSegmentIndex);
		__NMRASSERT(pSegment != nullptr);
		eType = pSegment->m_eType;
		nPartID = pSegment->m_nPartID;
		nProfileID = pSegment->m_nProfileID;
		nPointCount = pSegment->m_nPointCount;
	}

	NVEC2 CModelToolpathLayerReadData::getSegmentPoint(nfUint32 nSegmentIndex, nfUint32 nPointIndex)
	{
		TOOLPATHREADSEGMENT * pSegment = m_Segments.getData(nSegmentIndex);
		__NMRASSERT(pSegment != nullptr);
		if (nPointIndex >= pSegment->m_nPointCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Points.getDataRef(pSegment->m_nStartPoint + nPointIndex);

	}

	void CModelToolpathLayerReadData::registerUUID(nfUint32 nID, std::string sUUID)
	{
		auto iIter = m_UUIDMap.find(nID);
		if (iIter != m_UUIDMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEID);

		m_UUIDMap.insert(std::make_pair (nID, sUUID));
	}

	std::string CModelToolpathLayerReadData::mapIDtoUUID(nfUint32 nID)
	{
		auto iIter = m_UUIDMap.find(nID);
		if (iIter == m_UUIDMap.end())
			throw CNMRException(NMR_ERROR_MISSINGID);

		return iIter->second;
	}

	uint32_t CModelToolpathLayerReadData::getCustomXMLDataCount()
	{
		return (uint32_t) m_CustomXMLData.size();
	}

	PCustomXMLTree CModelToolpathLayerReadData::getCustomXMLData(uint32_t nIndex)
	{
		if (nIndex >= m_CustomXMLData.size ())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_CustomXMLData.at(nIndex);
	}

	void CModelToolpathLayerReadData::addCustomXMLData(PCustomXMLTree pCustomXMLTree)
	{
		if (pCustomXMLTree.get () == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_CustomXMLData.push_back(pCustomXMLTree);
	}

	uint32_t CModelToolpathLayerReadData::registerCustomSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, eModelToolpathSegmentAttributeType eSegmentType)
	{
		if (m_Segments.getCount() > 0)
			throw CNMRException(NMR_ERROR_SEGMENTATTRIBUTEDEFINEDAFTERREADING);
		if (sNameSpace.empty ())
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTENAMESPACE);
		if (sAttributeName.empty())
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTENAME);

		uint32_t nNewAttributeID;
		
		switch (eSegmentType) {
		case eModelToolpathSegmentAttributeType::SegmentAttributeUint32:
			m_Uint32AttributesOfNextSegment.push_back (0);
			nNewAttributeID = (uint32_t)m_Uint32AttributesOfNextSegment.size();
			m_SegmentAttributesDefinition.insert(std::make_pair (std::make_pair (sNameSpace, sAttributeName), std::make_pair (nNewAttributeID, eSegmentType)));
			return nNewAttributeID;

		case eModelToolpathSegmentAttributeType::SegmentAttributeDouble:
			m_DoubleAttributesOfNextSegment.push_back (0.0);
			nNewAttributeID = (uint32_t)m_DoubleAttributesOfNextSegment.size();
			m_SegmentAttributesDefinition.insert(std::make_pair(std::make_pair(sNameSpace, sAttributeName), std::make_pair(nNewAttributeID, eSegmentType)));
			return nNewAttributeID;

		default:
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTETYPE);
		}

	}

	void CModelToolpathLayerReadData::clearSegmentAttributes()
	{
		for (auto& attribute : m_Uint32AttributesOfNextSegment)
			attribute = 0;
		for (auto& attribute : m_DoubleAttributesOfNextSegment)
			attribute = 0.0;
	}

	void CModelToolpathLayerReadData::storeSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, const std::string& sAttributeValue)
	{

		auto iIter = m_SegmentAttributesDefinition.find(std::make_pair (sNameSpace, sAttributeName));

		if (iIter != m_SegmentAttributesDefinition.end()) {
			auto nAttributeID = iIter->second.first;
			switch (iIter->second.second) {
			case eModelToolpathSegmentAttributeType::SegmentAttributeUint32:
				if ((nAttributeID < 1) && (nAttributeID > m_Uint32AttributesOfNextSegment.size ()))
					throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEID);

				m_Uint32AttributesOfNextSegment.at (nAttributeID - 1) = fnStringToUint32(sAttributeValue.c_str ());

				break;
			case eModelToolpathSegmentAttributeType::SegmentAttributeDouble:
				if ((nAttributeID < 1) && (nAttributeID > m_DoubleAttributesOfNextSegment.size ()))
					throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEID);
				__NMRASSERT(m_pCurrentSegment->m_pDoubleAttributes != nullptr);

				m_DoubleAttributesOfNextSegment.at (nAttributeID - 1) = fnStringToDouble(sAttributeValue.c_str());
				break;
			}
			
		}
	}

	uint32_t CModelToolpathLayerReadData::getSegmentUint32Attribute(nfUint32 nSegmentIndex, uint32_t nAttributeID)
	{
		TOOLPATHREADSEGMENT* pSegment = m_Segments.getData(nSegmentIndex);
		__NMRASSERT(pSegment != nullptr);

		if ((nAttributeID < 1) && (nAttributeID > m_Uint32AttributesOfNextSegment.size ()))
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEID);
		__NMRASSERT(pSegment->m_pUint32Attributes != nullptr);

		return pSegment->m_pUint32Attributes[nAttributeID - 1];

	}

	double CModelToolpathLayerReadData::getSegmentDoubleAttribute(nfUint32 nSegmentIndex, uint32_t nAttributeID)
	{
		TOOLPATHREADSEGMENT* pSegment = m_Segments.getData(nSegmentIndex);
		__NMRASSERT(pSegment != nullptr);

		if ((nAttributeID < 1) && (nAttributeID > m_DoubleAttributesOfNextSegment.size ()))
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEID);
		__NMRASSERT(pSegment->m_pDoubleAttributes != nullptr);

		return pSegment->m_pDoubleAttributes[nAttributeID - 1];

	}


	nfByte* CModelToolpathLayerReadData::allocAttributeBuffer(uint32_t nSize)
	{
		if (nSize == 0)
			return nullptr;

		if (nSize >= TOOLPATHSEGMENTATTRIBUTEPAGESIZE)
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEDATASIZE);

		if (m_nCurrentPositionOnPage + nSize > TOOLPATHSEGMENTATTRIBUTEPAGESIZE) {
			m_AttributeBuffer.push_back (CToolpathLayerAttributePage ());
			m_pCurrentAttributePage = &(*m_AttributeBuffer.rbegin());
			m_pCurrentAttributePage->resize(TOOLPATHSEGMENTATTRIBUTEPAGESIZE);
			m_nCurrentPositionOnPage = 0;
		}

		__NMRASSERT(m_pCurrentAttributePage != nullptr);

		nfByte* pData = &m_pCurrentAttributePage->at(m_nCurrentPositionOnPage);
		m_nCurrentPositionOnPage += nSize;

		return pData;

	}

	uint32_t CModelToolpathLayerReadData::findUint32Attribute(const std::string& sNameSpace, const std::string& sAttributeName, bool bMustExist)
	{
		auto iIter = m_SegmentAttributesDefinition.find(std::make_pair(sNameSpace, sAttributeName));

		if (iIter != m_SegmentAttributesDefinition.end()) {
			if (iIter->second.second == eModelToolpathSegmentAttributeType::SegmentAttributeUint32)
				return iIter->second.first;

			if (bMustExist)
				throw CNMRException(NMR_ERROR_SEGMENTATTRIBUTEISNOTUINT32);
		}

		if (bMustExist)
			throw CNMRException(NMR_ERROR_SEGMENTATTRIBUTENOTFOUND);

		return 0;
	}

	uint32_t CModelToolpathLayerReadData::findDoubleAttribute(const std::string& sNameSpace, const std::string& sAttributeName, bool bMustExist)
	{
		auto iIter = m_SegmentAttributesDefinition.find(std::make_pair(sNameSpace, sAttributeName));

		if (iIter != m_SegmentAttributesDefinition.end()) {
			if (iIter->second.second == eModelToolpathSegmentAttributeType::SegmentAttributeDouble)
				return iIter->second.first;

			if (bMustExist)
				throw CNMRException(NMR_ERROR_SEGMENTATTRIBUTEISNOTUINT32);
		}

		if (bMustExist)
			throw CNMRException(NMR_ERROR_SEGMENTATTRIBUTENOTFOUND);

		return 0;
	}

}
