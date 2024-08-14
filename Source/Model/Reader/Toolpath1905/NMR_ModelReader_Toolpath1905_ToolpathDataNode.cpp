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

NMR_ModelReaderNode_Toolpath1905_ToolpathProfiles.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathData.h"
#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathDataNode.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathDataNode::CModelReaderNode_Toolpath1905_ToolpathDataNode(_In_ CModel * pModel, _In_ CModelToolpath * pToolpath, _In_ PModelWarnings pWarnings, const std::string& sNameSpace, PCustomXMLNode pXMLNode)
		: CModelReaderNode(pWarnings), m_pToolpath (pToolpath), m_pModel(pModel), m_sNameSpace(sNameSpace), m_pXMLNode(pXMLNode)
	{
		if (pToolpath == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pXMLNode == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathDataNode::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
	}


	void CModelReaderNode_Toolpath1905_ToolpathDataNode::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		m_pXMLNode->addAttribute(pAttributeName, pAttributeValue);
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathDataNode::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		std::string sAttributeNameSpace = pNameSpace;
		if (m_sNameSpace == sAttributeNameSpace)
			m_pXMLNode->addAttribute(pAttributeName, pAttributeValue);
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathDataNode::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		std::string sChildNameSpace = pNameSpace;
		if (m_sNameSpace == sChildNameSpace) {

			auto pSubNode = m_pXMLNode->addChild(pChildName);
			PModelReaderNode_Toolpath1905_ToolpathDataNode pXMLReaderNode = std::make_shared<CModelReaderNode_Toolpath1905_ToolpathDataNode>(m_pModel, m_pToolpath, m_pWarnings, m_sNameSpace, pSubNode);
			pXMLReaderNode->parseXML(pXMLReader);

		}


	}
	



}

