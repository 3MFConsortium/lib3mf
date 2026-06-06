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

	CToolpathReaderNode_Override::CToolpathReaderNode_Override(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), 
			m_pReadData (pReadData),
			m_dParameter (0),
			m_dValueF(0),
			m_dValueG(0),
			m_dValueH(0),
			m_dValueE(0),
			m_bHasParameter (false),
			m_bHasValueF (false),
			m_bHasValueG (false),
			m_bHasValueH (false),
			m_bHasValueE (false)

	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}


	void CToolpathReaderNode_Override::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CToolpathReaderNode_Override::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_PARAMETERVALUE) == 0) {
			if (m_bHasParameter)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEPARAMETERVALUE);

			m_dParameter = fnStringToDouble(pAttributeValue);
			m_bHasParameter = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORF) == 0) {
			if (m_bHasValueF)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEFACTORF);
			m_dValueF = fnStringToDouble(pAttributeValue);
			m_bHasValueF = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORG) == 0) {
			if (m_bHasValueG)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEFACTORG);
			m_dValueG = fnStringToDouble(pAttributeValue);
			m_bHasValueG = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORH) == 0) {
			if (m_bHasValueH)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEFACTORH);
			m_dValueH = fnStringToDouble(pAttributeValue);
			m_bHasValueH = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_SCALEFACTORE) == 0) {
			if (m_bHasValueE)
				throw CNMRException(NMR_ERROR_DUPLICATEOVERRIDEFACTORE);
			m_dValueE = fnStringToDouble(pAttributeValue);
			m_bHasValueE = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);

	}

	void CToolpathReaderNode_Override::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
	}

	void CToolpathReaderNode_Override::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
	}

	nfDouble CToolpathReaderNode_Override::getParameter()
	{
		if (!m_bHasParameter)
			throw CNMRException(NMR_ERROR_MISSINGOVERRIDEPARAMETERVALUE);
		return m_dParameter;
	}

	nfDouble CToolpathReaderNode_Override::getValueF(bool bMustExist)
	{
		if (!m_bHasValueF) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_MISSINGOVERRIDEFACTORF);
			return 0.0;
		}
		return m_dValueF;
	}

	nfDouble CToolpathReaderNode_Override::getValueG(bool bMustExist)
	{
		if (!m_bHasValueG) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_MISSINGOVERRIDEFACTORG);
			return 0.0;
		}
		return m_dValueG;
	}

	nfDouble CToolpathReaderNode_Override::getValueH(bool bMustExist)
	{
		if (!m_bHasValueH) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_MISSINGOVERRIDEFACTORH);
			return 0.0;
		}
		return m_dValueH;
	}

	nfDouble CToolpathReaderNode_Override::getValueE(bool bMustExist)
	{
		if (!m_bHasValueE) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_MISSINGOVERRIDEFACTORE);
			return 0.0;
		}
		return m_dValueE;
	}

}
