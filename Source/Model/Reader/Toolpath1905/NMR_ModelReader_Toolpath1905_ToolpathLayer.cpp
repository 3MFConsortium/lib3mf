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

Abstract:

NMR_ModelReaderNode_Toolpath1905_ToolpathLayer.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathLayer.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathLayer::CModelReaderNode_Toolpath1905_ToolpathLayer (_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings),
		m_bHasPath (false),
		m_bHasZTop (false),
		m_nZTop (0),
		m_pModel(pModel)

	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayer::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
	}


	void CModelReaderNode_Toolpath1905_ToolpathLayer::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLAYER_PATH) == 0) {
			if (m_bHasPath)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			// Convert to integer and make a input and range check!
			m_sPath = pAttributeValue;
			m_bHasPath = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHLAYER_ZTOP) == 0) {
			if (m_bHasZTop)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			// Convert to integer and make a input and range check!
			m_nZTop = fnStringToUint32(pAttributeValue);
			m_bHasZTop = true;
		}

	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayer::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{


	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayer::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{

	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayer::createLayer(CModelToolpath * pToolpath, bool bIgnoreBottomLayer)
	{
		__NMRASSERT(pToolpath);

		if (!m_bHasPath)
			throw CNMRException(NMR_ERROR_MISSINGPATH);
		if (!m_bHasZTop)
			throw CNMRException(NMR_ERROR_MISSINGZTOP);

		bool bIgnoreLayer = false;

		if (bIgnoreBottomLayer) {
			if (pToolpath->layersAreEmpty() && (pToolpath->getBottomZ() == m_nZTop))
				bIgnoreLayer = true;
		}
			
		if (!bIgnoreLayer) {
			pToolpath->addLayer(m_sPath, m_nZTop);
		}
	}


}

