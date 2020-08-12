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

NMR_ModelReaderNode_BeamLattice1702_BeamSet.cpp covers the official 3MF beamlattice extension.


--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSet.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ref.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BallRef.h"


#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode_BeamLattice1702_BeamSet::CModelReaderNode_BeamLattice1702_BeamSet(_In_ BEAMSET * pBeamSet, _In_ std::unordered_set<std::string> * pUniqueIdentifiers, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_pBeamSet = pBeamSet;
		m_pUniqueIdentifiers = pUniqueIdentifiers;
	}

	void CModelReaderNode_BeamLattice1702_BeamSet::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_BeamLattice1702_BeamSet::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_NAME) == 0) {
			m_pBeamSet->m_sName = pAttributeValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_IDENTIFIER) == 0) {
			std::string attributeString(pAttributeValue);
			if (m_pUniqueIdentifiers->find(attributeString) == m_pUniqueIdentifiers->end()) {
				m_pUniqueIdentifiers->insert(attributeString);
				m_pBeamSet->m_sIdentifier = pAttributeValue;
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMSET_IDENTIFIER_NOT_UNIQUE), mrwInvalidOptionalValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_BeamLattice1702_BeamSet::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pNameSpace);

	}
	
	void CModelReaderNode_BeamLattice1702_BeamSet::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_REF) == 0) {
				PModelReaderNode_BeamLattice1702_Ref pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_Ref>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				nfInt32 nIndex;
				pXMLNode->retrieveIndex(nIndex);
				m_pBeamSet->m_Refs.push_back(nIndex);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BALLREF) == 0) {
				PModelReaderNode_BeamLattice1702_BallRef pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_BallRef>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
				nfInt32 nIndex;
				pXMLNode->retrieveIndex(nIndex);
				m_pBeamSet->m_BallRefs.push_back(nIndex);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);

		}
	}

}
