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

#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Hatch.h" 
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

	CToolpathReaderNode_Hatch::CToolpathReaderNode_Hatch(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
			m_pReadData (pReadData),
			m_nX1 (0),
			m_nY1(0),
			m_nX2(0),
			m_nY2(0),
			m_bHasX1 (false),
			m_bHasY1 (false),
			m_bHasX2 (false),
			m_bHasY2 (false),
		m_nTag(0),
		m_dFactorF1(0.0),
		m_dFactorG1(0.0),
		m_dFactorH1(0.0),
		m_dFactorE1(0.0),
		m_dFactorF2(0.0),
		m_dFactorG2(0.0),
		m_dFactorH2(0.0),
		m_dFactorE2(0.0),
		m_bHasFactorF1(false),
		m_bHasFactorF2(false),
		m_bHasFactorG1(false),
		m_bHasFactorG2(false),
		m_bHasFactorH1(false),
		m_bHasFactorH2(false),
		m_bHasFactorE1(false),
		m_bHasFactorE2(false)


	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}


	void CToolpathReaderNode_Hatch::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CToolpathReaderNode_Hatch::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_X1) == 0) {
			if (m_bHasX1)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);

			m_nX1 = fnStringToInt32(pAttributeValue);
			m_bHasX1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_Y1) == 0) {
			if (m_bHasY1)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_nY1 = fnStringToInt32(pAttributeValue);
			m_bHasY1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_X2) == 0) {
			if (m_bHasX2)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_nX2 = fnStringToInt32(pAttributeValue);
			m_bHasX2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_Y2) == 0) {
			if (m_bHasY2)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_nY2 = fnStringToInt32(pAttributeValue);
			m_bHasY2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_TAG) == 0) {
			m_nTag = fnStringToInt32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF) == 0) {
			if (m_bHasFactorF1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF1);
			if (m_bHasFactorF2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF2);

			m_dFactorF1 = fnStringToDouble(pAttributeValue);
			m_dFactorF2 = m_dFactorF1;
			m_bHasFactorF1 = true;
			m_bHasFactorF2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF1) == 0) {
			if (m_bHasFactorF1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF1);

			m_dFactorF1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorF1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF2) == 0) {
			if (m_bHasFactorF2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF2);

			m_dFactorF2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorF2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG) == 0) {
			if (m_bHasFactorG1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG1);
			if (m_bHasFactorG2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG2);

			m_dFactorG1 = fnStringToDouble(pAttributeValue);
			m_dFactorG2 = m_dFactorG1;
			m_bHasFactorG1 = true;
			m_bHasFactorG2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG1) == 0) {
			if (m_bHasFactorG1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG1);

			m_dFactorG1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorG1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG2) == 0) {
			if (m_bHasFactorG2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG2);

			m_dFactorG2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorG2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH) == 0) {
			if (m_bHasFactorH1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH1);
			if (m_bHasFactorH2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH2);

			m_dFactorH1 = fnStringToDouble(pAttributeValue);
			m_dFactorH2 = m_dFactorH1;
			m_bHasFactorH1 = true;
			m_bHasFactorH2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH1) == 0) {
			if (m_bHasFactorH1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH1);

			m_dFactorH1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorH1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH2) == 0) {
			if (m_bHasFactorH2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH2);

			m_dFactorH2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorH2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE) == 0) {
			if (m_bHasFactorE1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE1);
			if (m_bHasFactorE2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE2);

			m_dFactorE1 = fnStringToDouble(pAttributeValue);
			m_dFactorE2 = m_dFactorE1;
			m_bHasFactorE1 = true;
			m_bHasFactorE2 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE1) == 0) {
			if (m_bHasFactorE1)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE1);

			m_dFactorE1 = fnStringToDouble(pAttributeValue);
			m_bHasFactorE1 = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE2) == 0) {
			if (m_bHasFactorE2)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE2);

			m_dFactorE2 = fnStringToDouble(pAttributeValue);
			m_bHasFactorE2 = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);

	}

	void CToolpathReaderNode_Hatch::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
	}

	void CToolpathReaderNode_Hatch::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TOOLPATHSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_TOOLPATHELEMENT_OVERRIDEINTERPOLATION) == 0) {
				CToolpathReaderNode_Override xmlNode(m_pWarnings, m_pProgressMonitor, m_pReadData);

				xmlNode.parseXML(pXMLReader);

				m_pReadData->addOverrideInterpolation(xmlNode.getParameter (), xmlNode.getValueF (false), xmlNode.getValueG(false), xmlNode.getValueH(false), xmlNode.getValueE(false));

			}
		}

	}

	nfInt32 CToolpathReaderNode_Hatch::getX1() const
	{
		if (!m_bHasX1)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nX1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getY1() const
	{
		if (!m_bHasY1)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nY1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getX2() const
	{
		if (!m_bHasX2)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nX2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getY2() const
	{
		if (!m_bHasY2)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nY2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getTag() const
	{
		return m_nTag;
	}

	
	nfDouble CToolpathReaderNode_Hatch::getFactorF1() const
	{
		return m_dFactorF1;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorG1() const
	{
		return m_dFactorG1;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorH1() const
	{
		return m_dFactorH1;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorE1() const
	{
		return m_dFactorE1;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorF2() const
	{
		return m_dFactorF2;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorG2() const
	{
		return m_dFactorG2;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorH2() const
	{
		return m_dFactorH2;
	}

	nfDouble CToolpathReaderNode_Hatch::getFactorE2() const
	{
		return m_dFactorE2;
	}

	bool CToolpathReaderNode_Hatch::hasFactorF1() const
	{
		return m_bHasFactorF1;
	}

	bool CToolpathReaderNode_Hatch::hasFactorF2() const
	{
		return m_bHasFactorF2;
	}

	bool CToolpathReaderNode_Hatch::hasFactorG1() const
	{
		return m_bHasFactorG1;
	}

	bool CToolpathReaderNode_Hatch::hasFactorG2() const
	{
		return m_bHasFactorG2;
	}
	
	bool CToolpathReaderNode_Hatch::hasFactorH1() const
	{
		return m_bHasFactorH1;
	}

	bool CToolpathReaderNode_Hatch::hasFactorH2() const
	{
		return m_bHasFactorH2;
	}

	bool CToolpathReaderNode_Hatch::hasFactorE1() const
	{
		return m_bHasFactorE1;
	}

	bool CToolpathReaderNode_Hatch::hasFactorE2() const
	{
		return m_bHasFactorE2;
	}


}
