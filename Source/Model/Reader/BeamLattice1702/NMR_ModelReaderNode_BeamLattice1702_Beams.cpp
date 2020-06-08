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

NMR_ModelReaderNode_BeamLattice1702_Beams.cpp covers the official 3MF beamlattice extension.
(Initial version based on private beam extension)


--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beams.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beam.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode_BeamLattice1702_Beams::CModelReaderNode_BeamLattice1702_Beams(_In_ CModel * pModel, _In_ CMesh * pMesh,
		_In_ nfDouble defaultRadius, _In_ eModelBeamLatticeCapMode defaultCapMode,
		_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);

		m_pModel = pModel;
		m_pMesh = pMesh;
		m_dDefaultRadius = defaultRadius;
		m_eDefaultCapMode = defaultCapMode;
	}

	void CModelReaderNode_BeamLattice1702_Beams::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode_BeamLattice1702_Beams::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode_BeamLattice1702_Beams::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BEAM) == 0) {
				// Parse XML
				PModelReaderNode_BeamLattice1702_Beam pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_Beam>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Retrieve node indices
				nfInt32 nIndex1, nIndex2;
				pXMLNode->retrieveIndices(nIndex1, nIndex2, m_pMesh->getNodeCount());

				MESHNODE* pNode1 = m_pMesh->getNode(nIndex1);
				MESHNODE* pNode2 = m_pMesh->getNode(nIndex2);
				
				if (fnVEC3_length(fnVEC3_sub(pNode1->m_position, pNode2->m_position)) < m_pMesh->getBeamLatticeMinLength())
					m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICENODESTOOCLOSE), mrwInvalidMandatoryValue);

				nfInt32 nTag;
				nfBool bHasTag, bHasRadius1, bHasRadius2;
				nfDouble dRadius1, dRadius2;
				pXMLNode->retrieveTag(bHasTag, nTag);
				pXMLNode->retrieveRadii(bHasRadius1, dRadius1, bHasRadius2, dRadius2);
				nfDouble dDefaultValueForR2 = m_dDefaultRadius;
				if (bHasRadius1) {
					dDefaultValueForR2 = dRadius1;
				}
				else {
					dRadius1 = m_dDefaultRadius;
				}

				if (!bHasRadius2) {
					dRadius2 = dDefaultValueForR2;
				}

				nfBool bHasCapMode1, bHasCapMode2;
				eModelBeamLatticeCapMode eCap1, eCap2;
				nfInt32 nCap1, nCap2;
				pXMLNode->retrieveCapModes(bHasCapMode1, eCap1, bHasCapMode2, eCap2);
				nCap1 = bHasCapMode1 ? eCap1 : m_eDefaultCapMode;
				nCap2 = bHasCapMode2 ? eCap2 : m_eDefaultCapMode;

				// Create beam if valid
				if (nIndex1 != nIndex2) {
					MESHNODE * pNode1 = m_pMesh->getNode(nIndex1);
					MESHNODE * pNode2 = m_pMesh->getNode(nIndex2);
					m_pMesh->addBeam(pNode1, pNode2, dRadius1, dRadius2, nCap1, nCap2);
				}
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
