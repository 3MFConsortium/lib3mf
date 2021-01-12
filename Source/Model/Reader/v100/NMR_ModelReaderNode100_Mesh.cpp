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

NMR_ModelReaderNode100_Mesh.cpp implements the Model Reader Mesh Node Class.
A mesh reader model node is a parser for the mesh node of an XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Mesh.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Vertices.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangles.h"
#include "Model/Reader/BeamLattice1702/NMR_ModelReaderNode_BeamLattice1702_BeamLattice.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Mesh::CModelReaderNode100_Mesh(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelWarnings pWarnings,
		_In_ PProgressMonitor pProgressMonitor, _In_ PPackageResourceID pObjectLevelPropertyID, _In_ ModelResourceIndex nDefaultPropertyIndex)
		: CModelReaderNode(pWarnings, pProgressMonitor)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);

		m_pObjectLevelPropertyID = pObjectLevelPropertyID;
		m_nObjectLevelPropertyIndex = nDefaultPropertyIndex;

		m_pMesh = pMesh;
		m_pModel = pModel;

		m_bHasClippingMeshID = false;
		m_nClippingMeshID = 0;
		m_eClipMode = eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE;
		m_bHasRepresentationMeshID = false;
		m_nRepresentationMeshID = 0;
	}

	void CModelReaderNode100_Mesh::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Mesh::retrieveClippingInfo(_Out_ eModelBeamLatticeClipMode &eClipMode, _Out_ nfBool & bHasClippingMode, _Out_ ModelResourceID & nClippingMeshID)
	{
		bHasClippingMode = m_bHasClippingMeshID;
		nClippingMeshID = m_nClippingMeshID;
		eClipMode = m_eClipMode;
	}
	
	void CModelReaderNode100_Mesh::retrieveRepresentationInfo(_Out_ nfBool & bHasRepresentation, _Out_ ModelResourceID & nRepresentationMeshID)
	{
		bHasRepresentation = m_bHasRepresentationMeshID;
		nRepresentationMeshID = m_nRepresentationMeshID;
	}

	void CModelReaderNode100_Mesh::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode100_Mesh::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {

			if (strcmp(pChildName, XML_3MF_ELEMENT_VERTICES) == 0)
			{
				if (m_pMesh->getNodeCount() % PROGRESS_READUPDATE == PROGRESS_READUPDATE - 1) {
					m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READMESH);
					m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
				}
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Vertices>(m_pMesh, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_TRIANGLES) == 0)
			{
				if (m_pMesh->getFaceCount() % PROGRESS_READUPDATE == PROGRESS_READUPDATE - 1) {
					m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READMESH);
					m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
				}
				PModelReaderNode100_Triangles pXMLNode = std::make_shared<CModelReaderNode100_Triangles>(m_pModel, m_pMesh, m_pWarnings,
					m_pObjectLevelPropertyID, m_nObjectLevelPropertyIndex);
				pXMLNode->parseXML(pXMLReader);
				if (m_pObjectLevelPropertyID && m_pObjectLevelPropertyID->getPackageModelPath() == 0) {
					// warn, if object does not have an object-level property, but a triangle has one
					if (pXMLNode->getUsedPropertyID() != 0) {
						m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGOBJECTLEVELPID), mrwMissingMandatoryValue);
					}
					// Try and define an object-level property as some PropertyID used by a triangle in the meshobject
					ModelResourceID nObjectLevelPropertyID = pXMLNode->getUsedPropertyID();
					m_pObjectLevelPropertyID = m_pModel->findPackageResourceID(m_pModel->currentPath(), nObjectLevelPropertyID);
					m_nObjectLevelPropertyIndex = 0;
				}
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}


		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_BEAMLATTICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BEAMLATTICE) == 0)
			{
				PModelReaderNode_BeamLattice1702_BeamLattice pXMLNode = std::make_shared<CModelReaderNode_BeamLattice1702_BeamLattice>(m_pModel, m_pMesh, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				pXMLNode->retrieveClippingInfo(m_eClipMode, m_bHasClippingMeshID, m_nClippingMeshID);
				pXMLNode->retrieveRepresentationInfo(m_bHasRepresentationMeshID, m_nRepresentationMeshID);
				pXMLNode->validateBallOptions(m_pWarnings);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}

}
