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

NMR_ModelReaderNode_BeamLattice1702_Beam.cpp covers the official 3MF beamlattice extension.
(Initial version based on private beam extension)


--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beam.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	eModelBeamLatticeCapMode stringToCapMode(const nfChar * capModeStr) {
		if (strcmp(capModeStr, XML_3MF_BEAMLATTICE_CAPMODE_BUTT) == 0)
			return eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_BUTT;
		if ((strcmp(capModeStr, XML_3MF_BEAMLATTICE_CAPMODE_HEMISPHERE) == 0) || (strcmp(capModeStr, XML_3MF_BEAMLATTICE_CAPMODE_ROUND) == 0))
			return eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_HEMISPHERE;
		// if (strcmp(capModeStr, XML_3MF_BEAMLATTICE_CAPMODE_SPHERE) == 0)
		//	return eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
		return eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
	}

	CModelReaderNode_BeamLattice1702_Beam::CModelReaderNode_BeamLattice1702_Beam(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_nIndex1 = -1;
		m_nIndex2 = -1;

		m_bHasRadius1 = false;
		m_bHasRadius2 = false;
		m_dRadius1 = 0;
		m_dRadius2 = 0;

		m_bHasCap1 = false;
		m_bHasCap2 = false;

		m_bHasTag = false;
		m_nTag = -1;
	}

	void CModelReaderNode_BeamLattice1702_Beam::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_BeamLattice1702_Beam::retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, nfInt32 nNodeCount)
	{
		if ((m_nIndex1 < 0) || (m_nIndex2 < 0))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		if ((m_nIndex1 >= nNodeCount) || (m_nIndex2 >= nNodeCount))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		if ( m_nIndex1 == m_nIndex2 )
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);
		
		nIndex1 = m_nIndex1;
		nIndex2 = m_nIndex2;
	}

	void CModelReaderNode_BeamLattice1702_Beam::retrieveRadii(_Out_ nfBool & bHasRadius1, _Out_ nfDouble & dRadius1, _Out_ nfBool & bHasRadius2, _Out_ nfDouble & dRadius2) {
		bHasRadius1 = m_bHasRadius1;
		if (bHasRadius1)
			dRadius1 = m_dRadius1;
		bHasRadius2 = m_bHasRadius2;
		if (bHasRadius2)
			dRadius2 = m_dRadius2;
	}

	void CModelReaderNode_BeamLattice1702_Beam::retrieveCapModes(_Out_ nfBool & bHasCapMode1, _Out_ eModelBeamLatticeCapMode & eCapMode1, _Out_ nfBool & bHasCapMode2, _Out_ eModelBeamLatticeCapMode & eCapMode2)
	{
		bHasCapMode1 = m_bHasCap1;
		if (bHasCapMode1)
			eCapMode1 = m_eCapMode1;
		bHasCapMode2 = m_bHasCap2;
		if (bHasCapMode2)
			eCapMode2 = m_eCapMode2;
	}

	
	void CModelReaderNode_BeamLattice1702_Beam::retrieveTag(_Out_ nfBool & bHasTag, _Out_ nfInt32 & nTag) {
		bHasTag = m_bHasTag;
		if (bHasTag)
			nTag = m_nTag;
	}

	void CModelReaderNode_BeamLattice1702_Beam::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_V1) == 0) {
			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex1 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_V2) == 0) {
			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex2 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_R1) == 0) {
			nfFloat fValue = fnStringToFloat(pAttributeValue);
			if ((fValue >= 0) && (fValue < XML_3MF_MAXIMUMBEAMRADIUSVALUE)) {
				m_dRadius1 = fValue;
				m_bHasRadius1 = true;
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_R2) == 0) {
			nfFloat fValue = fnStringToFloat(pAttributeValue);
			if ((fValue >= 0) && (fValue < XML_3MF_MAXIMUMBEAMRADIUSVALUE)) {
				m_dRadius2 = fValue;
				m_bHasRadius2 = true;
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP1) == 0) {
			m_bHasCap1 = true;
			m_eCapMode1 = stringToCapMode(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP2) == 0) {
			m_bHasCap2 = true;
			m_eCapMode2 = stringToCapMode(pAttributeValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_BeamLattice1702_Beam::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		
	}
	
}
