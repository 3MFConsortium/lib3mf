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
		m_dFactorF (0),
		m_dFactorG (0),
		m_dFactorH (0),
		m_dFactorE (0),
		m_bHasFactorF (false),
		m_bHasFactorG (false),
		m_bHasFactorH (false),
		m_bHasFactorE (false)
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
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF) == 0) {
			if (m_bHasFactorF)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORF);

			m_dFactorF = fnStringToDouble(pAttributeValue);
			m_bHasFactorF = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG) == 0) {
			if (m_bHasFactorG)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORG);

			m_dFactorG = fnStringToDouble(pAttributeValue);
			m_bHasFactorG = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH) == 0) {
			if (m_bHasFactorH)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORH);

			m_dFactorH = fnStringToDouble(pAttributeValue);
			m_bHasFactorH = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE) == 0) {
			if (m_bHasFactorE)
				throw CNMRException(NMR_ERROR_DUPLICATESCALEFACTORE);

			m_dFactorE = fnStringToDouble(pAttributeValue);
			m_bHasFactorE = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);

	}

	void CToolpathReaderNode_Point::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{

	}

	void CToolpathReaderNode_Point::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
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


	nfDouble CToolpathReaderNode_Point::getFactorF() const
	{
		return m_dFactorF;
	}

	nfDouble CToolpathReaderNode_Point::getFactorG() const
	{
		return m_dFactorG;
	}

	nfDouble CToolpathReaderNode_Point::getFactorH() const
	{
		return m_dFactorH;
	}

	nfDouble CToolpathReaderNode_Point::getFactorE() const
	{
		return m_dFactorE;
	}

	bool CToolpathReaderNode_Point::hasFactorF() const
	{
		return m_bHasFactorF;
	}

	bool CToolpathReaderNode_Point::hasFactorG() const
	{
		return m_bHasFactorG;
	}

	bool CToolpathReaderNode_Point::hasFactorH() const
	{
		return m_bHasFactorH;
	}

	bool CToolpathReaderNode_Point::hasFactorE() const
	{
		return m_bHasFactorE;
	}

}
