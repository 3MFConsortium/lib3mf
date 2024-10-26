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

NMR_ModelReaderNode_Toolpath1905_ToolpathModifier.h covers the official 3MF Toolpath extension.


--*/

#include "Model/Reader/Toolpath1905/NMR_ModelReader_Toolpath1905_ToolpathModifier.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode_Toolpath1905_ToolpathModifier::CModelReaderNode_Toolpath1905_ToolpathModifier(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings), 
		m_bHasAttribute(false),
		m_bHasDelta(false),
		m_dDelta (0.0),
		m_OverrideFactor (NMR::eModelToolpathProfileOverrideFactor::pfNone),
		m_pModel(pModel)

	{
	}


	
	void CModelReaderNode_Toolpath1905_ToolpathModifier::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
		
	}


	void CModelReaderNode_Toolpath1905_ToolpathModifier::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{

		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);


		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_ATTRIBUTE) == 0) {
			if (m_bHasAttribute)
				throw CNMRException(NMR_ERROR_DUPLICATEMODIFIERATTRIBUTE);

			m_sAttribute = pAttributeValue;
			m_bHasAttribute = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_DELTA) == 0) {
			if (m_bHasDelta)
				throw CNMRException(NMR_ERROR_DUPLICATEMODIFIERDELTA);

			m_dDelta = fnStringToFloat(pAttributeValue);
			if (std::isnan(m_dDelta))
				throw CNMRException(NMR_ERROR_INVALIDTOOLPATHPROFILEDELTA);
			if (fabs(m_dDelta) > XML_3MF_MAXIMUMPROFILEDELTA)
				throw CNMRException(NMR_ERROR_INVALIDTOOLPATHPROFILEDELTA);

			m_bHasDelta = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_FACTOR) == 0) {

			if (m_OverrideFactor != NMR::eModelToolpathProfileOverrideFactor::pfNone)
				throw CNMRException(NMR_ERROR_DUPLICATEMODIFIERFACTOR);

			if (strcmp(pAttributeValue, "f") == 0) {
				m_OverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorF;
			} else if (strcmp(pAttributeValue, "g") == 0) {
				m_OverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorG;
			} else if (strcmp(pAttributeValue, "h") == 0) {
				m_OverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorH;
			} else 
				throw CNMRException(NMR_ERROR_INVALIDOVERRIDEFACTOR);

		}

	}
	
	void CModelReaderNode_Toolpath1905_ToolpathModifier::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pNameSpace);
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathModifier::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
	}
	
	void CModelReaderNode_Toolpath1905_ToolpathModifier::addToProfile(CModelToolpathProfile* pProfile, CXmlReader* pXMLReader)
	{
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pProfile);

		if (!m_bHasAttribute)
			throw CNMRException(NMR_ERROR_MISSINGMODIFIERPROFILEATTRIBUTE);

		if (!m_bHasDelta)
			throw CNMRException(NMR_ERROR_MISSINGMODIFIERPROFILEDELTA);

		if (m_sAttribute.empty())
			throw CNMRException(NMR_ERROR_EMPTYMODIFIERPROFILEATTRIBUTE);

		size_t nPos = m_sAttribute.find(":");
		if (nPos == std::string::npos) {
			pProfile->addModifier("", m_sAttribute, m_dDelta, m_OverrideFactor);
		}
		else {
			std::string sNameSpacePrefix = m_sAttribute.substr(0, nPos);
			std::string sValueName = m_sAttribute.substr(nPos + 1);
			std::string sNameSpace;
			if (!pXMLReader->GetNamespaceURI(sNameSpacePrefix, sNameSpace))
				throw CNMRException(NMR_ERROR_INVALIDPROFILEMODIFIERNAMESPACE);

			pProfile->addModifier(sNameSpace, sValueName, m_dDelta, m_OverrideFactor);
		}

		


	}


}

