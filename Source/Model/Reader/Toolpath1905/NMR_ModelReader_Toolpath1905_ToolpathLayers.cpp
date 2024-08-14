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

NMR_ModelReaderNode_Toolpath1905_ToolpathLayers.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathLayers.h"
#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathLayer.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathLayers::CModelReaderNode_Toolpath1905_ToolpathLayers (_In_ CModel * pModel, _In_ CModelToolpath * pToolpath, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_pToolpath (pToolpath), m_pModel(pModel)
	{
		if (pToolpath == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayers::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
	}


	void CModelReaderNode_Toolpath1905_ToolpathLayers::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayers::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathLayers::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TOOLPATHSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_TOOLPATHLAYER) == 0) {

				PModelReaderNode_Toolpath1905_ToolpathLayer pXMLNode = std::make_shared<CModelReaderNode_Toolpath1905_ToolpathLayer>(
					m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				pXMLNode->createLayer(m_pToolpath);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}
	

}

