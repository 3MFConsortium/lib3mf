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

#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Point.h" 
#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Override.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CToolpathReaderNode_Point::CToolpathReaderNode_Point(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
		m_pReadData(pReadData),
		m_bHasX (false),
		m_bHasY (false),
		m_nX (0),
		m_nY (0),
		m_nTag (0),
		m_dFactorF1 (0),
		m_dFactorG1 (0),
		m_dFactorH1 (0),
		m_dFactorE1 (0),
		m_dFactorF2 (0),
		m_dFactorG2 (0),
		m_dFactorH2 (0),
		m_dFactorE2 (0),
		m_bHasConstantFactorF (false),
		m_bHasConstantFactorG (false),
		m_bHasConstantFactorH (false),
		m_bHasConstantFactorE (false),
		m_bHasFactorF1 (false),
		m_bHasFactorF2 (false),
		m_bHasFactorG1 (false),
		m_bHasFactorG2 (false),
		m_bHasFactorH1 (false),
		m_bHasFactorH2 (false),
		m_bHasFactorE1 (false),
		m_bHasFactorE2 (false),
		m_bHasOverrideInterpolation (false),
		m_dLastSubParameter (0.0)
	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}


	void CToolpathReaderNode_Point::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// A linear factor must carry both its start and end value.
		validateLinearFactors();

	}

	void CToolpathReaderNode_Point::validateLinearFactors()
	{
		if (m_bHasFactorF1 != m_bHasFactorF2)
			throw CNMRException(NMR_ERROR_INCOMPLETELINEARSCALEFACTOR);
		if (m_bHasFactorG1 != m_bHasFactorG2)
			throw CNMRException(NMR_ERROR_INCOMPLETELINEARSCALEFACTOR);
		if (m_bHasFactorH1 != m_bHasFactorH2)
			throw CNMRException(NMR_ERROR_INCOMPLETELINEARSCALEFACTOR);
		if (m_bHasFactorE1 != m_bHasFactorE2)
			throw CNMRException(NMR_ERROR_INCOMPLETELINEARSCALEFACTOR);
	}

	void CToolpathReaderNode_Point::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_X) == 0) {
			if (m_bHasX)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_nX = fnStringToInt32(pAttributeValue);
			m_bHasX = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_Y) == 0) {
			if (m_bHasY)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_nY = fnStringToInt32(pAttributeValue);
			m_bHasY = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TAG) == 0) {
			m_nTag = fnStringToInt32(pAttributeValue);
		}
		// Constant factors: set both the start and the end value.
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF) == 0) {
			if (m_bHasConstantFactorF || m_bHasFactorF1 || m_bHasFactorF2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF);

			m_dFactorF1 = fnStringToDouble(pAttributeValue);
			m_dFactorF2 = m_dFactorF1;
			m_bHasConstantFactorF = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG) == 0) {
			if (m_bHasConstantFactorG || m_bHasFactorG1 || m_bHasFactorG2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG);

			m_dFactorG1 = fnStringToDouble(pAttributeValue);
			m_dFactorG2 = m_dFactorG1;
			m_bHasConstantFactorG = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH) == 0) {
			if (m_bHasConstantFactorH || m_bHasFactorH1 || m_bHasFactorH2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH);

			m_dFactorH1 = fnStringToDouble(pAttributeValue);
			m_dFactorH2 = m_dFactorH1;
			m_bHasConstantFactorH = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE) == 0) {
			if (m_bHasConstantFactorE || m_bHasFactorE1 || m_bHasFactorE2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE);

			m_dFactorE1 = fnStringToDouble(pAttributeValue);
			m_dFactorE2 = m_dFactorE1;
			m_bHasConstantFactorE = true;
		}
		// Linear factors: start value (x1) and end value (x2). Must not combine with a constant.
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF1) == 0) {
			if (m_bHasFactorF1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF1);
			if (m_bHasConstantFactorF)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF);
			m_dFactorF1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorF1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF2) == 0) {
			if (m_bHasFactorF2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF2);
			if (m_bHasConstantFactorF)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF);
			m_dFactorF2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorF2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG1) == 0) {
			if (m_bHasFactorG1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG1);
			if (m_bHasConstantFactorG)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG);
			m_dFactorG1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorG1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG2) == 0) {
			if (m_bHasFactorG2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG2);
			if (m_bHasConstantFactorG)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG);
			m_dFactorG2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorG2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH1) == 0) {
			if (m_bHasFactorH1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH1);
			if (m_bHasConstantFactorH)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH);
			m_dFactorH1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorH1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH2) == 0) {
			if (m_bHasFactorH2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH2);
			if (m_bHasConstantFactorH)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH);
			m_dFactorH2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorH2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE1) == 0) {
			if (m_bHasFactorE1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE1);
			if (m_bHasConstantFactorE)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE);
			m_dFactorE1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorE1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE2) == 0) {
			if (m_bHasFactorE2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE2);
			if (m_bHasConstantFactorE)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE);
			m_dFactorE2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorE2 = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);

	}

	void CToolpathReaderNode_Point::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{

	}

	void CToolpathReaderNode_Point::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (XML_3MF_ISTOOLPATHNAMESPACE(pNameSpace)) {
			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_OVERRIDEINTERPOLATION) == 0) {
				CToolpathReaderNode_Override xmlNode(m_pWarnings, m_pProgressMonitor, m_pReadData);

				xmlNode.parseXML(pXMLReader);

				nfDouble dParameter = xmlNode.getParameter();

				// Support point parameters must be strictly within (0,1) and strictly increasing.
				if ((dParameter <= 0.0) || (dParameter >= 1.0))
					m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDOVERRIDESUBPARAMETER), mrwInvalidOptionalValue);
				else if (m_bHasOverrideInterpolation && (dParameter <= m_dLastSubParameter))
					m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDOVERRIDESUBPARAMETER), mrwInvalidOptionalValue);

				m_dLastSubParameter = dParameter;
				m_bHasOverrideInterpolation = true;

				m_pReadData->addOverrideInterpolation(dParameter,
					xmlNode.hasValueF(), xmlNode.getValueF(false),
					xmlNode.hasValueG(), xmlNode.getValueG(false),
					xmlNode.hasValueH(), xmlNode.getValueH(false),
					xmlNode.hasValueE(), xmlNode.getValueE(false));
			}
		}
	}

	nfInt32 CToolpathReaderNode_Point::getX() const
	{
		if (!m_bHasX)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nX;
	}

	nfInt32 CToolpathReaderNode_Point::getY() const
	{
		if (!m_bHasY)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nY;
	}

	nfInt32 CToolpathReaderNode_Point::getTag() const
	{
		return m_nTag;
	}


	nfDouble CToolpathReaderNode_Point::getStartFactorF() const
	{
		return m_dFactorF1;
	}

	nfDouble CToolpathReaderNode_Point::getStartFactorG() const
	{
		return m_dFactorG1;
	}

	nfDouble CToolpathReaderNode_Point::getStartFactorH() const
	{
		return m_dFactorH1;
	}

	nfDouble CToolpathReaderNode_Point::getStartFactorE() const
	{
		return m_dFactorE1;
	}

	nfDouble CToolpathReaderNode_Point::getEndFactorF() const
	{
		return m_dFactorF2;
	}

	nfDouble CToolpathReaderNode_Point::getEndFactorG() const
	{
		return m_dFactorG2;
	}

	nfDouble CToolpathReaderNode_Point::getEndFactorH() const
	{
		return m_dFactorH2;
	}

	nfDouble CToolpathReaderNode_Point::getEndFactorE() const
	{
		return m_dFactorE2;
	}

	bool CToolpathReaderNode_Point::hasFactorF() const
	{
		return m_bHasConstantFactorF || m_bHasFactorF1 || m_bHasFactorF2;
	}

	bool CToolpathReaderNode_Point::hasFactorG() const
	{
		return m_bHasConstantFactorG || m_bHasFactorG1 || m_bHasFactorG2;
	}

	bool CToolpathReaderNode_Point::hasFactorH() const
	{
		return m_bHasConstantFactorH || m_bHasFactorH1 || m_bHasFactorH2;
	}

	bool CToolpathReaderNode_Point::hasFactorE() const
	{
		return m_bHasConstantFactorE || m_bHasFactorE1 || m_bHasFactorE2;
	}

	bool CToolpathReaderNode_Point::isLinearFactorF() const
	{
		return m_bHasFactorF1 && m_bHasFactorF2;
	}

	bool CToolpathReaderNode_Point::isLinearFactorG() const
	{
		return m_bHasFactorG1 && m_bHasFactorG2;
	}

	bool CToolpathReaderNode_Point::isLinearFactorH() const
	{
		return m_bHasFactorH1 && m_bHasFactorH2;
	}

	bool CToolpathReaderNode_Point::isLinearFactorE() const
	{
		return m_bHasFactorE1 && m_bHasFactorE2;
	}

	bool CToolpathReaderNode_Point::hasOverrideInterpolation() const
	{
		return m_bHasOverrideInterpolation;
	}

}
