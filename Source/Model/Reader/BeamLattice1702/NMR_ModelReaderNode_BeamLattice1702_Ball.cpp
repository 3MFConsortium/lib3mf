/*++

Copyright (C) 2020 3MF Consortium

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

NMR_ModelReaderNode_BeamLattice1702_Ball.cpp covers the official 3MF beamlattice extension.

--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ball.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	eModelBeamLatticeBallMode stringToBallMode(const nfChar* ballModeStr) 
	{
		if (strcmp(ballModeStr, XML_3MF_BEAMLATTICE_BALLMODE_MIXED) == 0)
			return eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_MIXED;
		if (strcmp(ballModeStr, XML_3MF_BEAMLATTICE_BALLMODE_ALL) == 0)
			return eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_ALL;
		return eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_NONE;
	}

	CModelReaderNode_BeamLattice1702_Ball::CModelReaderNode_BeamLattice1702_Ball(_In_ CModel* pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_nIndex = -1;

		m_bHasRadius = false;
		m_dRadius = 0;

		m_bHasRadius = false;

		m_bHasTag = false;
		m_nTag = -1;
	}

	void CModelReaderNode_BeamLattice1702_Ball::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode_BeamLattice1702_Ball::retrieveIndex(_Out_ nfInt32 & nIndex, nfInt32 nNodeCount)
	{
		if ((m_nIndex < 0) || m_nIndex >= nNodeCount)
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		nIndex = m_nIndex;
	}

	void CModelReaderNode_BeamLattice1702_Ball::retrieveRadius(_Out_ nfBool & bHasRadius, _Out_ nfDouble & dRadius)
	{
		bHasRadius = m_bHasRadius;
		if (bHasRadius)
			dRadius = m_dRadius;
	}

	void CModelReaderNode_BeamLattice1702_Ball::retrieveTag(_Out_ nfBool & bHasTag, _Out_ nfInt32 & nTag)
	{
		bHasTag = m_bHasTag;
		if (bHasTag)
			nTag = m_nTag;
	}

	void CModelReaderNode_BeamLattice1702_Ball::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_BALL_VINDEX) == 0) {
			nfInt32 nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_BALL_R) == 0) {
			nfFloat fValue = fnStringToFloat(pAttributeValue);
			if ((fValue >= 0) && (fValue < XML_3MF_MAXIMUMBALLRADIUSVALUE)) {
				m_dRadius = fValue;
				m_bHasRadius = true;
			}
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_BeamLattice1702_Ball::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{

	}
}