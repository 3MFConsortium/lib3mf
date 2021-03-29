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

NMR_ModelReaderNode_BeamLattice1702_BeamLattice.cpp covers the official 3MF beamlattice extension.
(Initial version based on private beam extension)

--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamLattice.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beams.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Beam.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamSets.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Balls.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ball.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>

namespace NMR {

	CModelReaderNode_BeamLattice1702_BeamLattice::CModelReaderNode_BeamLattice1702_BeamLattice(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_pModel = pModel;
		m_pMesh = pMesh;
		m_pWarnings = pWarnings;
		m_bHasClippingMeshID = false;
		m_nClippingMeshID = 0;
		m_bHasRepresentationMeshID = false;
		m_nRepresentationMeshID = 0;
		m_eClipMode = eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE;
		m_dDefaultRadius = 0.0001;
		m_eDefaultCapMode = eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE;
		m_dDefaultBallRadius = 0.0;
	}

	void CModelReaderNode_BeamLattice1702_BeamLattice::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_BeamLattice1702_BeamLattice::retrieveClippingInfo(_Out_ eModelBeamLatticeClipMode &eClipMode, _Out_ nfBool & bHasClippingMode, _Out_ ModelResourceID & nClippingMeshID)
	{
		bHasClippingMode = m_bHasClippingMeshID;
		nClippingMeshID = m_nClippingMeshID;
		eClipMode = m_eClipMode;
	}

	void CModelReaderNode_BeamLattice1702_BeamLattice::retrieveRepresentationInfo(_Out_ nfBool & bHasRepresentation, _Out_ ModelResourceID & nRepresentationMeshID)
	{
		bHasRepresentation = m_bHasRepresentationMeshID;
		nRepresentationMeshID = m_nRepresentationMeshID;
	}

	void CModelReaderNode_BeamLattice1702_BeamLattice::validateBallOptions(_In_ PModelWarnings pWarnings)
	{
		if (m_pMesh->getBeamLatticeBallMode() != eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_NONE) {
			if (m_pMesh->getDefaultBallRadius() <= 0 && m_pMesh->getDefaultBallRadius() >= XML_3MF_MAXIMUMCOORDINATEVALUE) {
				pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE), mrwInvalidOptionalValue);
			}
		}
	}
	
	void CModelReaderNode_BeamLattice1702_BeamLattice::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_RADIUS) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if ( std::isnan(dValue) || (dValue <= 0) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE) )
				throw CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE);
			m_dDefaultRadius = dValue;
		}
		else if ( (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_MINLENGTH) == 0) ||
			(strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_PRECISION) == 0) )	// legacy
		{
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue <= 0) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE);
			m_pMesh->setBeamLatticeMinLength(dValue);
		}
		else if ((strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMODE) == 0) ||
			(strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPING) == 0)) // legacy
		{
			if (strcmp(pAttributeValue, XML_3MF_BEAMLATTICE_CLIPMODE_INSIDE) == 0) {
				m_eClipMode = MODELBEAMLATTICECLIPMODE_INSIDE;
			}
			else if (strcmp(pAttributeValue, XML_3MF_BEAMLATTICE_CLIPMODE_OUTSIDE) == 0) {
				m_eClipMode = MODELBEAMLATTICECLIPMODE_OUTSIDE;
			}
			else {
				m_eClipMode = MODELBEAMLATTICECLIPMODE_NONE;
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMESH) == 0) {
			m_nClippingMeshID = fnStringToUint32(pAttributeValue);
			m_bHasClippingMeshID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_REPRESENTATIONMESH) == 0) {
			m_nRepresentationMeshID = fnStringToUint32(pAttributeValue);
			m_bHasRepresentationMeshID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_ACCURACY) == 0) {
			// legacy, ignore it
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_CAPMODE) == 0) {
			m_eDefaultCapMode = stringToCapMode(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLMODE) == 0) {
			m_pMesh->setBeamLatticeBallMode(stringToBallMode(pAttributeValue));
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLRADIUS) == 0) {
			nfDouble dValue = fnStringToDouble(pAttributeValue);
			if (std::isnan(dValue) || (dValue <= 0) || (dValue > XML_3MF_MAXIMUMCOORDINATEVALUE))
				throw CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE);
			m_dDefaultBallRadius = dValue;
			m_pMesh->setDefaultBallRadius(dValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode_BeamLattice1702_BeamLattice::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		

	}
	
	void CModelReaderNode_BeamLattice1702_BeamLattice::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BEAMS) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_Beams>(m_pModel, m_pMesh, m_dDefaultRadius, m_eDefaultCapMode, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BEAMSETS) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_BeamSets>(m_pMesh, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BALLS) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_Balls>(m_pModel, m_pMesh, m_dDefaultBallRadius, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}
}
