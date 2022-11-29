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

NMR_ModelReaderNode_Toolpath1905_ToolpathResource.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathResource.h"
#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathProfiles.h"
#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathLayers.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathResource::CModelReaderNode_Toolpath1905_ToolpathResource (_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), m_nID (0), m_bHasID (false), m_dUnitFactor(1.0), m_bHasUnitFactor (false), m_pModel (pModel)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathResource::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		if (!m_bHasID)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);
		if (!m_bHasUnitFactor)
			throw CNMRException(NMR_ERROR_MISSINGUNITFACTOR);

		m_pToolpath = std::make_shared<CModelToolpath> (m_nID, m_pModel, m_dUnitFactor);
		m_pModel->addResource(m_pToolpath);

		// Parse Content
		parseContent(pXMLReader);

		
	}


	void CModelReaderNode_Toolpath1905_ToolpathResource::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{

		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);


		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATH_ID) == 0) {
			if (m_bHasID)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
			m_bHasID = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATH_UNITFACTOR) == 0) {
			if (m_bHasUnitFactor)
				throw CNMRException(NMR_ERROR_DUPLICATEVALUE);

			m_dUnitFactor = strtod(pAttributeValue, nullptr);
			if (std::isnan(m_dUnitFactor))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_dUnitFactor) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDUNITS);
			m_bHasUnitFactor = true;
		}

	}
	
	
	void CModelReaderNode_Toolpath1905_ToolpathResource::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TOOLPATHSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_TOOLPATHPROFILES) == 0) {

				PModelReaderNode_Toolpath1905_ToolpathProfiles pXMLNode = std::make_shared<CModelReaderNode_Toolpath1905_ToolpathProfiles>(
					m_pModel, m_pToolpath.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);				


			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TOOLPATHLAYERS) == 0) {

				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_Toolpath1905_ToolpathLayers>(
					m_pModel, m_pToolpath.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}
	

}

