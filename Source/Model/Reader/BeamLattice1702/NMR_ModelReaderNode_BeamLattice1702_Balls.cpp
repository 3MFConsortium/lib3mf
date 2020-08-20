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

NMR_ModelReaderNode_BeamLattice1702_Balls.cpp covers the official 3MF beamlattice extension.

--*/

#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Balls.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_Ball.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {
	
	CModelReaderNode_BeamLattice1702_Balls::CModelReaderNode_BeamLattice1702_Balls(_In_ CModel * pModel, _In_ CMesh * pMesh,
		_In_ nfDouble defaultBallRadius, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);

		m_pModel = pModel;
		m_pMesh = pMesh;
		m_dDefaultBallRadius = defaultBallRadius;
	}

	void CModelReaderNode_BeamLattice1702_Balls::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse Attributes
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode_BeamLattice1702_Balls::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode_BeamLattice1702_Balls::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BALL) == 0) {
				// Parse XML
				PModelReaderNode_BeamLattice1702_Ball pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_Ball>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Retrieve node index
				nfInt32 nIndex;
				pXMLNode->retrieveIndex(nIndex, m_pMesh->getNodeCount());

				nfInt32 nTag;
				nfBool bHasTag, bHasRadius;
				nfDouble dRadius;
				pXMLNode->retrieveTag(bHasTag, nTag);
				pXMLNode->retrieveRadius(bHasRadius, dRadius);
				
				if (!bHasRadius) {
					dRadius = m_dDefaultBallRadius;
				}

				// Create ball
				MESHNODE * pNode = m_pMesh->getNode(nIndex);
				m_pMesh->addBall(pNode, dRadius);
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
	}
}