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

NMR_ModelWriterNode100_Mesh.cpp implements the Model Writer Mesh Node Class.
This is the class for exporting the 3mf mesh node.

--*/

#include "Model/Writer/v100/NMR_ModelWriterNode100_Mesh.h"
#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include "Common/3MF_ProgressMonitor.h"

#include <cmath>

#ifdef __GNUC__
#include <stdio.h>
#endif // __GNUC__

#define MAX(a,b) (((a)>(b))?(a):(b))

namespace NMR {

	CModelWriterNode100_Mesh::CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter, _In_ PProgressMonitor pProgressMonitor,
		_In_ PMeshInformation_PropertyIndexMapping pPropertyIndexMapping, _In_ int nPosAfterDecPoint, _In_ nfBool bWriteMaterialExtension, _In_ nfBool bWriteBeamLatticeExtension)
		:CModelWriterNode_ModelBase(pModelMeshObject->getModel(), pXMLWriter, pProgressMonitor), m_nPosAfterDecPoint(nPosAfterDecPoint), m_nPutDoubleFactor((int)(pow(10, CModelWriterNode100_Mesh::m_nPosAfterDecPoint)))
	{
		__NMRASSERT(pModelMeshObject != nullptr);
		if (!pPropertyIndexMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_bWriteMaterialExtension = bWriteMaterialExtension;
		m_bWriteBeamLatticeExtension = bWriteBeamLatticeExtension;

		m_pModelMeshObject = pModelMeshObject;
		m_pPropertyIndexMapping = pPropertyIndexMapping;

		// Initialize buffer arrays
		m_nTriangleBufferPos = 0;
		m_nVertexBufferPos = 0;
		m_nBeamBufferPos = 0;
		m_nBallBufferPos = 0;
		m_nBeamRefBufferPos = 0;
		m_nBallRefBufferPos = 0;
		putVertexString(MODELWRITERMESH100_VERTEXLINESTART);
		putTriangleString(MODELWRITERMESH100_TRIANGLELINESTART);
		putBeamString(MODELWRITERMESH100_BEAMLATTICE_BEAMLINESTART);
		putBallString(MODELWRITERMESH100_BEAMLATTICE_BALLLINESTART);
		putBeamRefString(MODELWRITERMESH100_BEAMLATTICE_REFLINESTART);
		putBallRefString(MODELWRITERMESH100_BEAMLATTICE_BALLREFLINESTART);
	}

	bool stringRepresentationsDiffer(double a, double b, double putFactor) {
		return  fabs(a - b) * putFactor > 0.1;
	}

	std::string capModeToString(eModelBeamLatticeCapMode eCapMode) {
		switch (eCapMode) {
		case eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_SPHERE: return XML_3MF_BEAMLATTICE_CAPMODE_SPHERE; break;
		case eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_HEMISPHERE: return XML_3MF_BEAMLATTICE_CAPMODE_HEMISPHERE; break;
		case eModelBeamLatticeCapMode::MODELBEAMLATTICECAPMODE_BUTT: return XML_3MF_BEAMLATTICE_CAPMODE_BUTT; break;
		default:
			return XML_3MF_BEAMLATTICE_CAPMODE_SPHERE;
		}
	}

	std::string clipModeToString(eModelBeamLatticeClipMode eClipMode) {
		switch (eClipMode) {
		case eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_INSIDE: return XML_3MF_BEAMLATTICE_CLIPMODE_INSIDE; break;
		case eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_OUTSIDE: return XML_3MF_BEAMLATTICE_CLIPMODE_OUTSIDE; break;
		case eModelBeamLatticeClipMode::MODELBEAMLATTICECLIPMODE_NONE: return XML_3MF_BEAMLATTICE_CLIPMODE_NONE; break;
		default:
			return XML_3MF_BEAMLATTICE_CLIPMODE_NONE;
		}
	}

	std::string ballModeToString(eModelBeamLatticeBallMode eBallMode) {
		switch (eBallMode) {
		case eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_NONE: return XML_3MF_BEAMLATTICE_BALLMODE_NONE; break;
		case eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_MIXED: return XML_3MF_BEAMLATTICE_BALLMODE_MIXED; break;
		case eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_ALL: return XML_3MF_BEAMLATTICE_BALLMODE_ALL; break;
		default:
			return XML_3MF_BEAMLATTICE_BALLMODE_NONE;
		}
	}

	void CModelWriterNode100_Mesh::writeToXML()
	{
		__NMRASSERT(m_pXMLWriter);
		__NMRASSERT(m_pModel);

		CMesh * pMesh = m_pModelMeshObject->getMesh();

		const nfUint32 nNodeCount = pMesh->getNodeCount ();
		const nfUint32 nFaceCount = pMesh->getFaceCount();
		const nfUint32 nBeamCount = pMesh->getBeamCount();
		const nfUint32 nBallCount = pMesh->getBallCount();
		nfUint32 nNodeIndex, nFaceIndex, nBeamIndex, nBallIndex;

		// Write Mesh Element
		writeStartElement(XML_3MF_ELEMENT_MESH);

		m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITENODES);
		// Write Vertices
		writeStartElement(XML_3MF_ELEMENT_VERTICES);
		for (nNodeIndex = 0; nNodeIndex < nNodeCount; nNodeIndex++) {
			// Get Mesh Node
			MESHNODE * pMeshNode = pMesh->getNode(nNodeIndex);
			writeVertexData(pMeshNode);

			/* The following works, but would be a major output speed bottleneck!

			// Write Vertex
			writeStartElement(XML_3MF_ELEMENT_VERTEX);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_X, pMeshNode->m_position.m_values.x);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_Y, pMeshNode->m_position.m_values.y);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_Z, pMeshNode->m_position.m_values.z);
			writeEndElement(); */

			if (nNodeIndex % PROGRESS_NODEUPDATE == PROGRESS_NODEUPDATE-1) {
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
			}
		}
		writeFullEndElement();

		// Retrieve Mesh Informations
		CMeshInformation_Properties * pProperties = NULL;

		UniqueResourceID nObjectLevelPropertyID = 0;
		ModelResourceIndex nObjectLevelPropertyIndex = 0;

		CMeshInformationHandler * pMeshInformationHandler = pMesh->getMeshInformationHandler();
		if (pMeshInformationHandler) {
			// Get generic property handler
			CMeshInformation *pInformation = pMeshInformationHandler->getInformationByType(0, emiProperties);
			if (pInformation) {
				pProperties = dynamic_cast<CMeshInformation_Properties *> (pInformation);
				NMR::MESHINFORMATION_PROPERTIES * pDefaultData = (NMR::MESHINFORMATION_PROPERTIES*)pProperties->getDefaultData();

				if (pDefaultData && pDefaultData->m_nUniqueResourceID != 0) {
					nObjectLevelPropertyID = pDefaultData->m_nUniqueResourceID;
					nObjectLevelPropertyIndex = m_pPropertyIndexMapping->mapPropertyIDToIndex(nObjectLevelPropertyID, pDefaultData->m_nPropertyIDs[0]);
				}
			}
		}
		
		bool bMeshHasAProperty = false;

		m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_WRITETRIANGLES);
		// Write Triangles
		writeStartElement(XML_3MF_ELEMENT_TRIANGLES);
		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			if (nFaceIndex % PROGRESS_TRIANGLEUPDATE == PROGRESS_TRIANGLEUPDATE - 1) {
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
			}

			// Get Mesh Face
			MESHFACE * pMeshFace = pMesh->getFace(nFaceIndex);

			UniqueResourceID nPropertyID = 0;
			ModelResourceIndex nPropertyIndex1 = 0;
			ModelResourceIndex nPropertyIndex2 = 0;
			ModelResourceIndex nPropertyIndex3 = 0;

			nfChar * pAdditionalString = nullptr;
			// Retrieve Property Indices
			if (pProperties != nullptr) {
				MESHINFORMATION_PROPERTIES* pFaceData = (MESHINFORMATION_PROPERTIES*)pProperties->getFaceData(nFaceIndex);
				if (pFaceData != nullptr) {
					if (pFaceData->m_nUniqueResourceID) {
						nPropertyID = pFaceData->m_nUniqueResourceID;
						nPropertyIndex1 = m_pPropertyIndexMapping->mapPropertyIDToIndex(nPropertyID, pFaceData->m_nPropertyIDs[0]);
						nPropertyIndex2 = m_pPropertyIndexMapping->mapPropertyIDToIndex(nPropertyID, pFaceData->m_nPropertyIDs[1]);
						nPropertyIndex3 = m_pPropertyIndexMapping->mapPropertyIDToIndex(nPropertyID, pFaceData->m_nPropertyIDs[2]);
					}
				}
			}


			if (nPropertyID != 0) {
				bMeshHasAProperty = true;
				// TODO: this is slow
				ModelResourceID nPropertyModelResourceID = m_pModel->findPackageResourceID(nPropertyID)->getModelResourceID();
				if ((nPropertyIndex1 != nPropertyIndex2) || (nPropertyIndex1 != nPropertyIndex3)) {
					writeFaceData_ThreeProperties(pMeshFace, nPropertyModelResourceID, nPropertyIndex1, nPropertyIndex2, nPropertyIndex3, pAdditionalString);
				}
				else {
					if ((nPropertyID == nObjectLevelPropertyID) && (nPropertyIndex1 == nObjectLevelPropertyIndex)){
						writeFaceData_Plain(pMeshFace, pAdditionalString);
					} else {
						writeFaceData_OneProperty(pMeshFace, nPropertyModelResourceID, nPropertyIndex1, pAdditionalString);
					}
				}
			}
			else
			{
				writeFaceData_Plain(pMeshFace, pAdditionalString);
			}

			/* The following works, but would be a major output speed bottleneck!

			// Write Triangle
			writeStartElement(XML_3MF_ELEMENT_TRIANGLE);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V1, pMeshFace->m_nodeindices[0]);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V2, pMeshFace->m_nodeindices[1]);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V3, pMeshFace->m_nodeindices[2]);

			// Write Property Indices
			if (nPropertyID != 0) {
				writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_PID, nPropertyID);
				writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_P1, nPropertyIndex1);
				if ((nPropertyIndex1 != nPropertyIndex2) || (nPropertyIndex1 != nPropertyIndex3)) {
					writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_P2, nPropertyIndex2);
					writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_P3, nPropertyIndex3);
				}
			}

			writeEndElement();  */
		}
		writeFullEndElement();

		if (bMeshHasAProperty && !(nObjectLevelPropertyID != 0)) {
			throw CNMRException(NMR_ERROR_MISSINGOBJECTLEVELPID);
		}

		if (m_bWriteBeamLatticeExtension) {
			if (nBeamCount > 0) {
				// write beamlattice
				writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMLATTICE, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
				nfDouble dDefaultRadius = pMesh->getDefaultBeamRadius();
				nfDouble dDefaultBallRadius = pMesh->getDefaultBallRadius();
				nfBool bWriteBallsElement = nBallCount > 0;
				writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_RADIUS, float(dDefaultRadius));
				writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_MINLENGTH, float(pMesh->getBeamLatticeMinLength()));
				eModelBeamLatticeBallMode eBallMode = pMesh->getBeamLatticeBallMode();

				if (eBallMode == eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_ALL) {
					if (dDefaultBallRadius > 0) {
						writeStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLMODE, ballModeToString(eBallMode));
						writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLRADIUS, float(dDefaultBallRadius));

						// Check if any balls do not use dDefaultBallRadius
						bWriteBallsElement = false;
						for (nfUint32 iBall = 0; iBall < nBallCount; iBall++) {
							if (stringRepresentationsDiffer(pMesh->getBall(iBall)->m_radius, dDefaultBallRadius, m_nPutDoubleFactor)) {
								bWriteBallsElement = true;
								break;
							}
						}
					}
					else {
						throw CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE);
					}
				}
				else if (nBallCount > 0) {
					if (eBallMode == eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_MIXED) {
						if (dDefaultBallRadius > 0) {
							writeStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLMODE, ballModeToString(eBallMode));
							writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_BALLRADIUS, float(dDefaultBallRadius));
						}
						else {
							throw CNMRException(NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE);
						}
					}
                }

				if (m_pModelMeshObject->getBeamLatticeAttributes()->m_bHasClippingMeshID) {
					writeStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMODE, clipModeToString(m_pModelMeshObject->getBeamLatticeAttributes()->m_eClipMode));
					PPackageResourceID pID = m_pModelMeshObject->getBeamLatticeAttributes()->m_pClippingMeshUniqueID;
					if (pID->getPath() != m_pModel->currentPath())
						throw CNMRException(NMR_ERROR_MODELRESOURCE_IN_DIFFERENT_MODEL);
					writeIntAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMESH, pID->getModelResourceID());
				}

				if (m_pModelMeshObject->getBeamLatticeAttributes()->m_bHasRepresentationMeshID) {
					PPackageResourceID pID = m_pModelMeshObject->getBeamLatticeAttributes()->m_pRepresentationUniqueID;
					if (pID->getPath() != m_pModel->currentPath())
						throw CNMRException(NMR_ERROR_MODELRESOURCE_IN_DIFFERENT_MODEL);
					writeIntAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_REPRESENTATIONMESH, pID->getModelResourceID());
				}

				eModelBeamLatticeCapMode eDefaultCapMode = pMesh->getBeamLatticeCapMode();
				writeConstStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CAPMODE, capModeToString(eDefaultCapMode).c_str());
				{
					// write beamlattice: beams
					writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMS, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
					for (nBeamIndex = 0; nBeamIndex < nBeamCount; nBeamIndex++) {
						// write beamlattice: beam
						MESHBEAM * pMeshBeam = pMesh->getBeam(nBeamIndex);
						writeBeamData(pMeshBeam, dDefaultRadius, eDefaultCapMode);
					}
					writeFullEndElement();

					// write beamlattice: balls
					// Only if there is at least 1 ball
					if (bWriteBallsElement) {
						writeStartElementWithPrefix(XML_3MF_ELEMENT_BALLS, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
						for (nBallIndex = 0; nBallIndex < nBallCount; nBallIndex++) {
							// write beamlattice: ball
							MESHBALL * pMeshBall = pMesh->getBall(nBallIndex);
							writeBallData(pMeshBall, eBallMode, dDefaultBallRadius);
						}
						writeFullEndElement();
					}

					const nfUint32 nBeamSetCount = pMesh->getBeamSetCount();
					if (nBeamSetCount > 0) {
						// write beamlattice: beamsets
						writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMSETS, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
						for (nfUint32 nBeamSetIndex = 0; nBeamSetIndex < nBeamSetCount; nBeamSetIndex++) {
							const PBEAMSET pBeamSet = pMesh->getBeamSet(nBeamSetIndex);
							{
								// write beamlattice: beamset
								writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMSET, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
								if (pBeamSet->m_sName.length()>0)
									writeConstStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_NAME, pBeamSet->m_sName.c_str());
								if (pBeamSet->m_sIdentifier.length()>0)
									writeConstStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_IDENTIFIER, pBeamSet->m_sIdentifier.c_str());
								const nfUint32 nRefCount = (nfUint32)pBeamSet->m_Refs.size();
								for (nfUint32 nRefIndex = 0; nRefIndex < nRefCount; nRefIndex++) {
									// write beamlattice: ref
									writeRefData(pBeamSet->m_Refs[nRefIndex]);
								}
								const nfUint32 nBallRefCount = (nfUint32)pBeamSet->m_BallRefs.size();
								for (nfUint32 nBallRefIndex = 0; nBallRefIndex < nBallRefCount; nBallRefIndex++) {
									// write beamlattice: ballref
									writeBallRefData(pBeamSet->m_BallRefs[nBallRefIndex]);
								}
								writeFullEndElement();
							}
						}
						writeFullEndElement();
					}
				}
				writeFullEndElement();
			}
		}

		// Finish Mesh Element
		writeFullEndElement();
	}


	void CModelWriterNode100_Mesh::putVertexString(_In_ const nfChar * pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_VertexLine[m_nVertexBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nVertexBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putFloat(_In_ const nfFloat fValue, _In_ std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> & line, _In_ nfUint32 & nBufferPos) {
		// Format float with "%.$ACCf" syntax where $ACC = m_snPosAfterDecPoint
		nfInt64 nAbsValue = (nfInt64)(fValue * m_nPutDoubleFactor);
		nAbsValue = MAX(nAbsValue, -nAbsValue);
		nfBool bIsNegative = fValue < 0;

		int nStart = nBufferPos;
		int nCount = 0;

		if (!nAbsValue) {
			line[nBufferPos++] = '0';
		}
		else {
			// Write the string in reverse order
			while (nAbsValue || nCount < m_nPosAfterDecPoint) {
				line[nBufferPos++] = '0' + (nAbsValue % 10);
				nAbsValue /= 10;
				nCount++;
				if (nCount == m_nPosAfterDecPoint) {
					line[nBufferPos++] = '.';
					if (!nAbsValue) {
						line[nBufferPos++] = '0';
					}
					nCount++;
				}
			}
			if (bIsNegative) {
				line[nBufferPos++] = '-';
				nCount++;
			}
		}

		// Reverse the float string
		int nEnd = nBufferPos - 1;
		while (nStart < nEnd) {
			char temp = line[nStart];
			line[nStart] = line[nEnd];
			line[nEnd] = temp;
			nStart++;
			nEnd--;
		}
	}

	void CModelWriterNode100_Mesh::putDouble(_In_ const nfDouble dValue, _In_ std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> & line, _In_ nfUint32 & nBufferPos) {
		// Format float with "%.$ACCf" syntax where $ACC = m_snPosAfterDecPoint
		nfInt64 nAbsValue = (nfInt64)(dValue * m_nPutDoubleFactor);
		nAbsValue = MAX(nAbsValue, -nAbsValue);
		nfBool bIsNegative = dValue < 0;

		int nStart = nBufferPos;
		int nCount = 0;

		if (!nAbsValue) {
			line[nBufferPos++] = '0';
		}
		else {
			// Write the string in reverse order
			while (nAbsValue || nCount < m_nPosAfterDecPoint) {
				line[nBufferPos++] = '0' + (nAbsValue % 10);
				nAbsValue /= 10;
				nCount++;
				if (nCount == m_nPosAfterDecPoint) {
					line[nBufferPos++] = '.';
					if (!nAbsValue) {
						line[nBufferPos++] = '0';
					}
					nCount++;
				}
			}
			if (bIsNegative) {
				line[nBufferPos++] = '-';
				nCount++;
			}
		}

		// Reverse the float string
		int nEnd = nBufferPos - 1;
		while (nStart < nEnd) {
			char temp = line[nStart];
			line[nStart] = line[nEnd];
			line[nEnd] = temp;
			nStart++;
			nEnd--;
		}
	}

	void CModelWriterNode100_Mesh::putVertexFloat(_In_ const nfFloat fValue)
	{
		putFloat(fValue, m_VertexLine, m_nVertexBufferPos);
	}


	void CModelWriterNode100_Mesh::putTriangleString(_In_ const nfChar * pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_TriangleLine[m_nTriangleBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nTriangleBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putTriangleUInt32(_In_ const nfUint32 nValue)
	{

#ifdef __GNUC__
		int nCount = sprintf(&m_TriangleLine[m_nTriangleBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_TriangleLine[m_nTriangleBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nTriangleBufferPos, "%d", nValue);
#endif // __GNUC__

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nTriangleBufferPos += nCount;
	}


	void CModelWriterNode100_Mesh::putBeamString(_In_ const nfChar * pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_BeamLine[m_nBeamBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nBeamBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putBeamUInt32(_In_ const nfUint32 nValue)
	{
#ifdef __GNUC__
		int nCount = sprintf(&m_BeamLine[m_nBeamBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_BeamLine[m_nBeamBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nBeamBufferPos, "%d", nValue);
#endif // __GNUC__

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nBeamBufferPos += nCount;
	}

	void CModelWriterNode100_Mesh::putBeamDouble(_In_ const nfDouble dValue)
	{
		putDouble(dValue, m_BeamLine, m_nBeamBufferPos);
	}

	void CModelWriterNode100_Mesh::putBallString(_In_ const nfChar * pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_BallLine[m_nBallBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nBallBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putBallUInt32(_In_ const nfUint32 nValue)
	{
#ifdef __GNUC__
		int nCount = sprintf(&m_BallLine[m_nBallBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_BallLine[m_nBallBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nBallBufferPos, "%d", nValue);
#endif // __GNUC__

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nBallBufferPos += nCount;
	}

	void CModelWriterNode100_Mesh::putBallDouble(_In_ const nfDouble dValue)
	{
		putDouble(dValue, m_BallLine, m_nBallBufferPos);
	}

	void CModelWriterNode100_Mesh::putBeamRefString(_In_ const nfChar * pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_BeamRefLine[m_nBeamRefBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nBeamRefBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putBeamRefUInt32(_In_ const nfUint32 nValue)
	{
#ifdef __GNUC__
		int nCount = sprintf(&m_BeamRefLine[m_nBeamRefBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_BeamRefLine[m_nBeamRefBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nBeamRefBufferPos, "%d", nValue);
#endif // __GNUC__

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nBeamRefBufferPos += nCount;
	}

	void CModelWriterNode100_Mesh::putBallRefString(_In_ const nfChar* pszString)
	{
		__NMRASSERT(pszString);
		const nfChar * pChar = pszString;
		nfChar * pTarget = &m_BallRefLine[m_nBallRefBufferPos];

		while (*pChar != 0) {
			*pTarget = *pChar;
			pTarget++;
			pChar++;
			m_nBallRefBufferPos++;
		}
	}

	void CModelWriterNode100_Mesh::putBallRefUInt32(_In_ const nfUint32 nValue)
	{
#ifdef __GNUC__
		int nCount = sprintf(&m_BallRefLine[m_nBallRefBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_BallRefLine[m_nBallRefBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nBallRefBufferPos, "%d", nValue);
#endif // __GNUC__

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nBallRefBufferPos += nCount;
	}

	void CModelWriterNode100_Mesh::writeVertexData(_In_ MESHNODE * pNode)
	{
		__NMRASSERT(pNode);
		m_nVertexBufferPos = MODELWRITERMESH100_VERTEXLINESTARTLENGTH;
		putVertexFloat(pNode->m_position.m_values.x);
		putVertexString("\" y=\"");
		putVertexFloat(pNode->m_position.m_values.y);
		putVertexString("\" z=\"");
		putVertexFloat(pNode->m_position.m_values.z);
		putVertexString("\" />");

		m_pXMLWriter->WriteRawLine(&m_VertexLine[0], m_nVertexBufferPos);
	}

	void CModelWriterNode100_Mesh::writeFaceData_Plain(_In_ MESHFACE * pFace, _In_opt_ const nfChar * pszAdditionalString)
	{
		__NMRASSERT(pFace);
		m_nTriangleBufferPos = MODELWRITERMESH100_TRIANGLELINESTARTLENGTH;
		putTriangleUInt32(pFace->m_nodeindices[0]);
		putTriangleString("\" v2=\"");
		putTriangleUInt32(pFace->m_nodeindices[1]);
		putTriangleString("\" v3=\"");
		putTriangleUInt32(pFace->m_nodeindices[2]);
		putTriangleString("\"");
		if (pszAdditionalString) {
			putTriangleString(pszAdditionalString);
		}
		putTriangleString(" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}

	void CModelWriterNode100_Mesh::writeFaceData_OneProperty(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex, _In_opt_ const nfChar * pszAdditionalString)
	{
		__NMRASSERT(pFace);
		m_nTriangleBufferPos = MODELWRITERMESH100_TRIANGLELINESTARTLENGTH;
		putTriangleUInt32(pFace->m_nodeindices[0]);
		putTriangleString("\" v2=\"");
		putTriangleUInt32(pFace->m_nodeindices[1]);
		putTriangleString("\" v3=\"");
		putTriangleUInt32(pFace->m_nodeindices[2]);
		if (nPropertyID != 0) {
			putTriangleString("\" pid=\"");
			putTriangleUInt32(nPropertyID);
		}
		putTriangleString("\" p1=\"");
		putTriangleUInt32(nPropertyIndex);
		putTriangleString("\"");
		if (pszAdditionalString) {
			putTriangleString(pszAdditionalString);
		}
		putTriangleString(" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}

	void CModelWriterNode100_Mesh::writeFaceData_ThreeProperties(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex1, _In_ const ModelResourceIndex nPropertyIndex2, _In_ const ModelResourceIndex nPropertyIndex3, _In_opt_ const nfChar * pszAdditionalString)
	{
		__NMRASSERT(pFace);
		m_nTriangleBufferPos = MODELWRITERMESH100_TRIANGLELINESTARTLENGTH;
		putTriangleUInt32(pFace->m_nodeindices[0]);
		putTriangleString("\" v2=\"");
		putTriangleUInt32(pFace->m_nodeindices[1]);
		putTriangleString("\" v3=\"");
		putTriangleUInt32(pFace->m_nodeindices[2]);
		if (nPropertyID != 0) {
			putTriangleString("\" pid=\"");
			putTriangleUInt32(nPropertyID);
		}
		putTriangleString("\" p1=\"");
		putTriangleUInt32(nPropertyIndex1);
		putTriangleString("\" p2=\"");
		putTriangleUInt32(nPropertyIndex2);
		putTriangleString("\" p3=\"");
		putTriangleUInt32(nPropertyIndex3);
		putTriangleString("\"");
		if (pszAdditionalString) {
			putTriangleString(pszAdditionalString);
		}
		putTriangleString(" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}	

	__NMR_INLINE void CModelWriterNode100_Mesh::writeBeamData(_In_ MESHBEAM * pBeam, _In_ nfDouble dRadius, _In_ eModelBeamLatticeCapMode eDefaultCapMode)
	{
		__NMRASSERT(pBeam);
		m_nBeamBufferPos = MODELWRITERMESH100_BEAMLATTICE_BEAMSTARTLENGTH;
		putBeamUInt32(pBeam->m_nodeindices[0]);
		const std::string sV2 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_V2) + "=\"";
		putBeamString(sV2.c_str());
		putBeamUInt32(pBeam->m_nodeindices[1]);

		nfBool bWriteR2 = stringRepresentationsDiffer(pBeam->m_radius[0], pBeam->m_radius[1], m_nPutDoubleFactor);
		nfBool bWriteR1 = bWriteR2 || stringRepresentationsDiffer(pBeam->m_radius[0], dRadius, m_nPutDoubleFactor);
		if (bWriteR1) {
			const std::string sR1 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_R1) + "=\"";
			putBeamString(sR1.c_str());
			putBeamDouble(pBeam->m_radius[0]);
		}
		if (bWriteR2) {
			const std::string sR2 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_R2) + "=\"";
			putBeamString(sR2.c_str());
			putBeamDouble(pBeam->m_radius[1]);
		}

		if ( eDefaultCapMode != pBeam->m_capMode[0] ) {
			const std::string sCap1 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP1) + "=\""
				                                           + capModeToString(eModelBeamLatticeCapMode(pBeam->m_capMode[0]));
			putBeamString(sCap1.c_str());
		}
		if (eDefaultCapMode != pBeam->m_capMode[1]) {
			const std::string sCap2 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP2) + "=\""
				+ capModeToString(eModelBeamLatticeCapMode(pBeam->m_capMode[1]));
			putBeamString(sCap2.c_str());
		}

		putBeamString("\"/>");
		m_pXMLWriter->WriteRawLine(&m_BeamLine[0], m_nBeamBufferPos);
	}

	__NMR_INLINE void CModelWriterNode100_Mesh::writeBallData(_In_ MESHBALL* pBall, _In_ eModelBeamLatticeBallMode eBallMode, _In_ nfDouble dRadius)
	{
		__NMRASSERT(pBall);

		nfBool bWriteR = stringRepresentationsDiffer(pBall->m_radius, dRadius, m_nPutDoubleFactor);
		if (!bWriteR && eBallMode == eModelBeamLatticeBallMode::MODELBEAMLATTICEBALLMODE_ALL) {
			return;
		}

		m_nBallBufferPos = MODELWRITERMESH100_BEAMLATTICE_BALLSTARTLENGTH;
		putBallUInt32(pBall->m_nodeindex);
		if (bWriteR) {
			const std::string sR = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_BALL_R) + "=\"";
			putBallString(sR.c_str());
			putBallDouble(pBall->m_radius);
		}

		putBallString("\"/>");
		m_pXMLWriter->WriteRawLine(&m_BallLine[0], m_nBallBufferPos);
	}

	__NMR_INLINE void CModelWriterNode100_Mesh::writeRefData(_In_ INT nRefID)
	{
		m_nBeamRefBufferPos = MODELWRITERMESH100_BEAMLATTICE_REFSTARTLENGTH;
		putBeamRefUInt32(nRefID);
		putBeamRefString("\"/>");
		m_pXMLWriter->WriteRawLine(&m_BeamRefLine[0], m_nBeamRefBufferPos);
	}

	__NMR_INLINE void CModelWriterNode100_Mesh::writeBallRefData(_In_ INT nBallRefID)
	{
		m_nBallRefBufferPos = MODELWRITERMESH100_BEAMLATTICE_BALLREFSTARTLENGTH;
		putBallRefUInt32(nBallRefID);
		putBallRefString("\"/>");
		m_pXMLWriter->WriteRawLine(&m_BallRefLine[0], m_nBallRefBufferPos);
	}

}
