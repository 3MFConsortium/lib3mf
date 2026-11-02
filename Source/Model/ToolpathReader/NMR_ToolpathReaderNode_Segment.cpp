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

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CToolpathReaderNode_Segment::CToolpathReaderNode_Segment(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
		m_pReadData(pReadData), 
		m_nPartID (0), 
		m_nProfileID (0),
		m_bHasSegmentType (false),
		m_eSegmentType (eModelToolpathSegmentType::HatchSegment),
		m_StandardAttributes ()
	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}

	static nfUint32 parseSegmentAttributeValue(_In_z_ const nfChar * pAttributeValue, bool bMustBePositive)
	{
		// strtoul silently wraps negative numbers, so only accept a plain digit sequence.
		const nfChar * pChar = pAttributeValue;
		while (*pChar == ' ')
			pChar++;
		if ((*pChar < '0') || (*pChar > '9'))
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEVALUE);

		nfUint32 nValue = fnStringToUint32(pChar);
		if (bMustBePositive && (nValue == 0))
			throw CNMRException(NMR_ERROR_INVALIDSEGMENTATTRIBUTEVALUE);

		return nValue;
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

		m_pReadData->beginSegment(m_eSegmentType, getProfileID(), getPartID(), m_StandardAttributes);

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

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_LASERINDEX) == 0) {
			m_StandardAttributes.m_nLaserIndex = parseSegmentAttributeValue(pAttributeValue, false);
			m_StandardAttributes.m_nFlags |= TOOLPATHSEGMENTATTRIBUTE_HASLASERINDEX;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_LASERSYNC) == 0) {
			m_StandardAttributes.m_nLaserSync = parseSegmentAttributeValue(pAttributeValue, true);
			m_StandardAttributes.m_nFlags |= TOOLPATHSEGMENTATTRIBUTE_HASLASERSYNC;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TIMEPREDICTION) == 0) {
			m_StandardAttributes.m_nTimePrediction = parseSegmentAttributeValue(pAttributeValue, false);
			m_StandardAttributes.m_nFlags |= TOOLPATHSEGMENTATTRIBUTE_HASTIMEPREDICTION;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_JUMPPREDICTION) == 0) {
			m_StandardAttributes.m_nJumpPrediction = parseSegmentAttributeValue(pAttributeValue, false);
			m_StandardAttributes.m_nFlags |= TOOLPATHSEGMENTATTRIBUTE_HASJUMPPREDICTION;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TAG) == 0) {
			m_StandardAttributes.m_nTag = parseSegmentAttributeValue(pAttributeValue, false);
			m_StandardAttributes.m_nFlags |= TOOLPATHSEGMENTATTRIBUTE_HASTAG;
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

		if (XML_3MF_ISTOOLPATHNAMESPACE(pNameSpace)) {
			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_HATCH) == 0) {

				if (m_eSegmentType != eModelToolpathSegmentType::HatchSegment)
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				uint32_t nOverrideInterpolationStart = m_pReadData->getGlobalOverrideInterpolationCount();

				CToolpathReaderNode_Hatch xmlNode (m_pWarnings, m_pProgressMonitor, m_pReadData);
				xmlNode.parseXML(pXMLReader);

				uint32_t nOverrideInterpolationEnd = m_pReadData->getGlobalOverrideInterpolationCount();
				if (nOverrideInterpolationEnd < nOverrideInterpolationStart)
					throw CNMRException(NMR_ERROR_OVERRIDEINTERPOLATIONOVERFLOW);

				uint32_t nOverrideInterpolationCount = nOverrideInterpolationEnd - nOverrideInterpolationStart;

				m_pReadData->addDiscretePoint(xmlNode.getX1(), xmlNode.getY1(), xmlNode.getTag(), xmlNode.hasFactorF1(), xmlNode.getFactorF1(), xmlNode.hasFactorG1(), xmlNode.getFactorG1(), xmlNode.hasFactorH1(), xmlNode.getFactorH1(), xmlNode.hasFactorE1(), xmlNode.getFactorE1(), nOverrideInterpolationStart, nOverrideInterpolationCount);
				m_pReadData->addDiscretePoint(xmlNode.getX2(), xmlNode.getY2(), xmlNode.getTag(), xmlNode.hasFactorF2(), xmlNode.getFactorF2(), xmlNode.hasFactorG2(), xmlNode.getFactorG2(), xmlNode.hasFactorH2(), xmlNode.getFactorH2(), xmlNode.hasFactorE2(), xmlNode.getFactorE2(), nOverrideInterpolationStart, nOverrideInterpolationCount);

			}
			else if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_POINT) == 0) {

				if ((m_eSegmentType != eModelToolpathSegmentType::LoopSegment) && (m_eSegmentType != eModelToolpathSegmentType::PolylineSegment))
					throw CNMRException(NMR_ERROR_INVALIDTYPEATTRIBUTE);

				uint32_t nOverrideInterpolationStart = m_pReadData->getGlobalOverrideInterpolationCount();

				CToolpathReaderNode_Point xmlNode (m_pWarnings, m_pProgressMonitor, m_pReadData);
				xmlNode.parseXML(pXMLReader);

				uint32_t nOverrideInterpolationEnd = m_pReadData->getGlobalOverrideInterpolationCount();
				if (nOverrideInterpolationEnd < nOverrideInterpolationStart)
					throw CNMRException(NMR_ERROR_OVERRIDEINTERPOLATIONOVERFLOW);

				uint32_t nOverrideInterpolationCount = nOverrideInterpolationEnd - nOverrideInterpolationStart;

				m_pReadData->addDiscretePoint(xmlNode.getX(), xmlNode.getY(), xmlNode.getTag(), xmlNode.hasFactorF (), xmlNode.getFactorF (), xmlNode.hasFactorG(), xmlNode.getFactorG (), xmlNode.hasFactorH(), xmlNode.getFactorH (), xmlNode.hasFactorE(), xmlNode.getFactorE (), nOverrideInterpolationStart, nOverrideInterpolationCount);

			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

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

}
