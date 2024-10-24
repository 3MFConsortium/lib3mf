/*++

Copyright (C) 2019 3MF Consortium

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


--*/

#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Segment.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Hatch.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Point.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_ZHatch.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_ZPoint.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CToolpathReaderNode_Segment::CToolpathReaderNode_Segment(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData, _In_ std::string sBinaryIndexStreamPath)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
		m_pReadData(pReadData), 
		m_nPartID (0), 
		m_nProfileID (0),
		m_bHasSegmentType (false),
		m_nOverrideFraction (XML_3MF_DEFAULTOVERRIDEFRACTION),
		m_bHasOverrideFraction (false),	
		m_eSegmentType (eModelToolpathSegmentType::HatchSegment),
		m_sBinaryIndexStreamPath(sBinaryIndexStreamPath)
	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}


	void CToolpathReaderNode_Segment::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Set custom attributes to default value (0.0)
		m_pReadData->clearSegmentAttributes();

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasSegmentType)
			throw CNMRException(NMR_ERROR_MISSINGTYPEATTRIBUTE);
		if (!hasPartID ())
			throw CNMRException(NMR_ERROR_MISSINGID);
		if (!hasProfileID())
			throw CNMRException(NMR_ERROR_MISSINGID);

		m_pReadData->beginSegment(m_eSegmentType, getProfileID(), getPartID(), m_nOverrideFraction);

		// Parse Content
		parseContent(pXMLReader);

		m_pReadData->endSegment();

	}

	void CToolpathReaderNode_Segment::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PROFILEID) == 0) {
			if (hasProfileID())
				throw CNMRException(NMR_ERROR_DUPLICATEID);

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nProfileID = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PARTID) == 0) {
			if (hasPartID())
				throw CNMRException(NMR_ERROR_DUPLICATEID);

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPartID = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PARTID) == 0) {
			if (m_bHasOverrideFraction)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEFRACTION);

			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXOVERRIDEFRACTION)) {
				m_nOverrideFraction = nValue;
				m_bHasOverrideFraction = true;
			}
			else {
				throw CNMRException(NMR_ERROR_INVALIDOVERRIDEFRACTION);
			}
		}
		

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TYPE) == 0) {
			if (m_bHasSegmentType)
				throw CNMRException(NMR_ERROR_DUPLICATETYPEATTRIBUTE);

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_HATCH) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::HatchSegment;
			}

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_LOOP) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::LoopSegment;
			}

			if (strcmp(pAttributeValue, XML_3MF_TOOLPATHTYPE_POLYLINE) == 0) {
				m_bHasSegmentType = true;
				m_eSegmentType = eModelToolpathSegmentType::PolylineSegment;
			}

			if (!m_bHasSegmentType)
				throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

		}

	}

	void CToolpathReaderNode_Segment::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pNameSpace);

		m_pReadData->storeSegmentAttribute(pNameSpace, pAttributeName, pAttributeValue);

	}

	void CToolpathReaderNode_Segment::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TOOLPATHSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_HATCH) == 0) {

				if (m_eSegmentType != eModelToolpathSegmentType::HatchSegment)
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_Hatch pXMLNode = std::make_shared<CToolpathReaderNode_Hatch>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				m_pReadData->addDiscretePoint(pXMLNode->getX1(), pXMLNode->getY1(), pXMLNode->getTag (), pXMLNode->getFactorF1 (), pXMLNode->getFactorG1(), pXMLNode->getFactorH1());
				m_pReadData->addDiscretePoint(pXMLNode->getX2(), pXMLNode->getY2(), pXMLNode->getTag(), pXMLNode->getFactorF2(), pXMLNode->getFactorG2(), pXMLNode->getFactorH2());

			}
			else if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_POINT) == 0) {

				if ((m_eSegmentType != eModelToolpathSegmentType::LoopSegment) && (m_eSegmentType != eModelToolpathSegmentType::PolylineSegment))
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_Point pXMLNode = std::make_shared<CToolpathReaderNode_Point>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				m_pReadData->addDiscretePoint(pXMLNode->getX(), pXMLNode->getY(), pXMLNode->getTag(), pXMLNode->getFactorF (), pXMLNode->getFactorG (), pXMLNode->getFactorH ());

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

/*		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BINARYSPEC) == 0) {

			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_POINT) == 0) {
				if ((m_eSegmentType != eModelToolpathSegmentType::LoopSegment) && (m_eSegmentType != eModelToolpathSegmentType::PolylineSegment))
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_ZPoint pXMLNode = std::make_shared<CToolpathReaderNode_ZPoint>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				nfInt32 nXID, nYID;
				pXMLNode->getBinaryIDs(nXID, nYID);

				if ((m_pBinaryStreamCollection.get() == nullptr) || (m_sBinaryIndexStreamPath.empty()))
					throw CNMRException(NMR_ERROR_NOBINARYSTREAMAVAILABLE);

			/*if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_HATCH) == 0) {
				if (m_eSegmentType != eModelToolpathSegmentType::HatchSegment)
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_ZHatch pXMLNode = std::make_shared<CToolpathReaderNode_ZHatch>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				nfInt32 nX1ID, nY1ID, nX2ID, nY2ID;
				pXMLNode->getBinaryIDs(nX1ID, nY1ID, nX2ID, nY2ID);

				if ((m_pBinaryStreamCollection.get() == nullptr) || (m_sBinaryStreamPath.empty()))
					throw CNMRException(NMR_ERROR_NOBINARYSTREAMAVAILABLE);

				auto pReader = m_pBinaryStreamCollection->findReader(m_sBinaryStreamPath);
				if (pReader == nullptr)
					throw CNMRException(NMR_ERROR_BINARYSTREAMNOTFOUND);

				nfUint32 nX1Count = pReader->getTypedChunkEntryCount(nX1ID, edtFloatArray);
				nfUint32 nY1Count = pReader->getTypedChunkEntryCount(nY1ID, edtFloatArray);
				nfUint32 nX2Count = pReader->getTypedChunkEntryCount(nX2ID, edtFloatArray);
				nfUint32 nY2Count = pReader->getTypedChunkEntryCount(nY2ID, edtFloatArray);

				if ((nX1Count != nY1Count) || (nX1Count != nX2Count) || (nX1Count != nY1Count) || (nX1Count != nY2Count))
					throw CNMRException(NMR_ERROR_INCONSISTENTBINARYSTREAMCOUNT);
				nfUint32 nCount = nX1Count;

				if (nCount > 0) {

					std::vector<nfFloat> X1Values;
					std::vector<nfFloat> Y1Values;
					std::vector<nfFloat> X2Values;
					std::vector<nfFloat> Y2Values;

					X1Values.resize(nCount);
					Y1Values.resize(nCount);
					X2Values.resize(nCount);
					Y2Values.resize(nCount);

					nfFloat * pX1 = X1Values.data();
					nfFloat * pY1 = Y1Values.data();
					nfFloat * pX2 = X2Values.data();
					nfFloat * pY2 = Y2Values.data();

					pReader->readFloatArray(nX1ID, pX1, nCount);
					pReader->readFloatArray(nY1ID, pY1, nCount);
					pReader->readFloatArray(nX2ID, pX2, nCount);
					pReader->readFloatArray(nY2ID, pY2, nCount);

					for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
						m_pReadData->addPoint(*pX1, *pY1);
						m_pReadData->addPoint(*pX2, *pY2);

						pX1++; pY1++; pX2++; pY2++;
					}
				}

			} 

			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_POINT) == 0) {
				if ((m_eSegmentType != eModelToolpathSegmentType::LoopSegment) && (m_eSegmentType != eModelToolpathSegmentType::PolylineSegment))
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				PToolpathReaderNode_ZPoint pXMLNode = std::make_shared<CToolpathReaderNode_ZPoint>(m_pWarnings, m_pProgressMonitor, m_pReadData);
				pXMLNode->parseXML(pXMLReader);

				nfInt32 nXID, nYID;
				pXMLNode->getBinaryIDs(nXID, nYID);

				if ((m_pBinaryStreamCollection.get() == nullptr) || (m_sBinaryStreamPath.empty()))
					throw CNMRException(NMR_ERROR_NOBINARYSTREAMAVAILABLE);

				auto pReader = m_pBinaryStreamCollection->findReader(m_sBinaryStreamPath);
				if (pReader == nullptr)
					throw CNMRException(NMR_ERROR_BINARYSTREAMNOTFOUND);

				nfUint32 nXCount = pReader->getTypedChunkEntryCount(nXID, edtFloatArray);
				nfUint32 nYCount = pReader->getTypedChunkEntryCount(nYID, edtFloatArray);

				if (nXCount != nYCount)
					throw CNMRException(NMR_ERROR_INCONSISTENTBINARYSTREAMCOUNT);
				nfUint32 nCount = nXCount;

				if (nCount > 0) {

					std::vector<nfFloat> XValues;
					std::vector<nfFloat> YValues;

					XValues.resize(nCount);
					YValues.resize(nCount);

					nfFloat * pX = XValues.data();
					nfFloat * pY = YValues.data();

					pReader->readFloatArray(nXID, pX, nCount);
					pReader->readFloatArray(nYID, pY, nCount);

					for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++) {
						m_pReadData->addPoint(*pX, *pY);

						pX++; pY++;
					}
				}

			}  

		} */

	}

	nfUint32 CToolpathReaderNode_Segment::getProfileID()
	{
		return m_nProfileID;
	}

	bool CToolpathReaderNode_Segment::hasProfileID()
	{
		return (m_nProfileID != 0);
	}

	nfUint32 CToolpathReaderNode_Segment::getPartID()
	{
		return m_nPartID;
	}

	bool CToolpathReaderNode_Segment::hasPartID()
	{
		return (m_nPartID != 0);
	}

	bool CToolpathReaderNode_Segment::hasOverrideFraction()
	{
		return m_bHasOverrideFraction;
	}

	nfUint32 CToolpathReaderNode_Segment::getOverrideFraction()
	{
		return m_nOverrideFraction;
	}

}
