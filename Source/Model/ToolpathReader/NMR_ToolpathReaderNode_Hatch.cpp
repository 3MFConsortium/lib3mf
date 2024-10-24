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
		m_nFactorF1(0),
		m_nFactorG1(0),
		m_nFactorH1(0),
		m_nFactorF2(0),
		m_nFactorG2(0),
		m_nFactorH2(0)


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
			m_nFactorF1 = fnStringToInt32(pAttributeValue);
			m_nFactorF2 = m_nFactorF1;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF1) == 0) {
			m_nFactorF1 = fnStringToInt32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF2) == 0) {
			m_nFactorF2 = fnStringToInt32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG) == 0) {
			m_nFactorG1 = fnStringToInt32(pAttributeValue);
			m_nFactorG2 = m_nFactorF1;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG1) == 0) {
			m_nFactorG1 = fnStringToInt32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG2) == 0) {
			m_nFactorG2 = fnStringToInt32(pAttributeValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);

	}

	void CToolpathReaderNode_Hatch::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
	}

	void CToolpathReaderNode_Hatch::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
	}

	nfInt32 CToolpathReaderNode_Hatch::getX1()
	{
		if (!m_bHasX1)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nX1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getY1()
	{
		if (!m_bHasY1)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nY1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getX2()
	{
		if (!m_bHasX2)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nX2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getY2()
	{
		if (!m_bHasY2)
			throw CNMRException(NMR_ERROR_MISSINGCOORDINATE);
		return m_nY2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getTag()
	{
		return m_nTag;
	}

	
	nfInt32 CToolpathReaderNode_Hatch::getFactorF1()
	{
		return m_nFactorF1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getFactorG1()
	{
		return m_nFactorG1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getFactorH1()
	{
		return m_nFactorH1;
	}

	nfInt32 CToolpathReaderNode_Hatch::getFactorF2()
	{
		return m_nFactorF2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getFactorG2()
	{
		return m_nFactorG2;
	}

	nfInt32 CToolpathReaderNode_Hatch::getFactorH2()
	{
		return m_nFactorH2;
	}

}
