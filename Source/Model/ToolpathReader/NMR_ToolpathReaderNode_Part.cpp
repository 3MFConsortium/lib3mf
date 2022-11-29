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

#include "Model/ToolpathReader/NMR_ToolpathReaderNode_Part.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CToolpathReaderNode_Part::CToolpathReaderNode_Part(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor, CModelToolpathLayerReadData * pReadData)
		: CModelReaderNode(pWarnings, pProgressMonitor), m_pReadData(pReadData), m_nPartID (0)
	{
		if (pReadData == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

	}


	void CToolpathReaderNode_Part::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CToolpathReaderNode_Part::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_ID) == 0) {
			if (hasPartID())
				throw CNMRException(NMR_ERROR_DUPLICATEID);
			
			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue > 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPartID = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_TOOLPATHATTRIBUTE_UUID) == 0) {
			if (hasUUID())
				throw CNMRException(NMR_ERROR_DUPLICATEUUID);
			m_sUUID = pAttributeValue;
		}

	}

	void CToolpathReaderNode_Part::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{

	}

	void CToolpathReaderNode_Part::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
	}


	nfUint32 CToolpathReaderNode_Part::getPartID()
	{
		return m_nPartID;
	}

	bool CToolpathReaderNode_Part::hasPartID()
	{
		return m_nPartID != 0;
	}

	std::string CToolpathReaderNode_Part::getUUID()
	{
		return m_sUUID;
	}

	bool CToolpathReaderNode_Part::hasUUID()
	{
		return (m_sUUID.length() > 0);
	}

}
