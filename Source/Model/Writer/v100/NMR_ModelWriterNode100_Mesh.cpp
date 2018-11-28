/*++

Copyright (C) 2018 3MF Consortium

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
#include "Common/MeshInformation/NMR_MeshInformation_BaseMaterials.h"
#include "Common/MeshInformation/NMR_MeshInformation_NodeColors.h"
#include "Common/MeshInformation/NMR_MeshInformation_TexCoords.h"

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
	const int CModelWriterNode100_Mesh::m_snPutDoubleFactor = (int)(pow(10, CModelWriterNode100_Mesh::m_snPosAfterDecPoint));

	CModelWriterNode100_Mesh::CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter, _In_ CProgressMonitor * pProgressMonitor,
		_In_ PModelWriter_ColorMapping pColorMapping, _In_ PModelWriter_TexCoordMappingContainer pTextureMappingContainer, _In_ nfBool bWriteMaterialExtension, _In_ nfBool bWriteBeamLatticeExtension)
		:CModelWriterNode(pModelMeshObject->getModel(), pXMLWriter, pProgressMonitor)
	{
		__NMRASSERT(pModelMeshObject != nullptr);
		if (!pColorMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!pTextureMappingContainer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_bWriteMaterialExtension = bWriteMaterialExtension;
		m_bWriteBeamLatticeExtension = bWriteBeamLatticeExtension;

		m_pModelMeshObject = pModelMeshObject;
		m_pColorMapping = pColorMapping;
		m_pTextureMappingContainer = pTextureMappingContainer;

		// Initialize buffer arrays
		m_nTriangleBufferPos = 0;
		m_nVertexBufferPos = 0;
		m_nBeamBufferPos = 0;
		m_nBeamRefBufferPos = 0;
		putVertexString(MODELWRITERMESH100_VERTEXLINESTART);
		putTriangleString(MODELWRITERMESH100_TRIANGLELINESTART);
		putBeamString(MODELWRITERMESH100_BEAMLATTICE_BEAMLINESTART);
		putBeamRefString(MODELWRITERMESH100_BEAMLATTICE_REFLINESTART);
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

	void CModelWriterNode100_Mesh::writeToXML()
	{
		__NMRASSERT(m_pXMLWriter);
		__NMRASSERT(m_pModel);

		CMesh * pMesh = m_pModelMeshObject->getMesh();

		const nfUint32 nNodeCount = pMesh->getNodeCount ();
		const nfUint32 nFaceCount = pMesh->getFaceCount();
		const nfUint32 nBeamCount = pMesh->getBeamCount();
		nfUint32 nNodeIndex, nFaceIndex, nBeamIndex;

		// Write Mesh Element
		writeStartElement(XML_3MF_ELEMENT_MESH);

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
				if (m_pProgressMonitor && !m_pProgressMonitor->Progress(-1, ProgressIdentifier::PROGRESS_WRITENODES) )
					throw CNMRException(NMR_USERABORTED);
			}
		}
		writeFullEndElement();

		// Retrieve Mesh Informations
		CMeshInformation_BaseMaterials * pBaseMaterials = NULL;
		CMeshInformation_NodeColors * pNodeColors = NULL;
		CMeshInformation_TexCoords * pTexCoords = NULL;

		CMeshInformationHandler * pMeshInformationHandler = pMesh->getMeshInformationHandler();
		if (pMeshInformationHandler) {
			CMeshInformation * pInformation;

			// Get Base Materials
			pInformation = pMeshInformationHandler->getInformationByType(0, emiBaseMaterials);
			if (pInformation)
				pBaseMaterials = dynamic_cast<CMeshInformation_BaseMaterials *> (pInformation);

			if (m_bWriteMaterialExtension) {
				// Get Node Colors
				pInformation = pMeshInformationHandler->getInformationByType(0, emiNodeColors);
				if (pInformation)
					pNodeColors = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);

				// Get Tex Coords
				pInformation = pMeshInformationHandler->getInformationByType(0, emiTexCoords);
				if (pInformation)
					pTexCoords = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);
			}
		}

		// if there is a PID, but no DefaultProperty
		if ( (pBaseMaterials) || (pNodeColors) || (pTexCoords) ) {
			if ( !(m_pModelMeshObject->getDefaultProperty()) ) {
				throw CNMRException(NMR_ERROR_MISSINGDEFAULTPID);
			}
		}

		// Write Triangles
		writeStartElement(XML_3MF_ELEMENT_TRIANGLES);
		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			if (nFaceIndex % PROGRESS_TRIANGLEUPDATE == PROGRESS_TRIANGLEUPDATE - 1) {
				if (m_pProgressMonitor && !m_pProgressMonitor->Progress(-1, ProgressIdentifier::PROGRESS_WRITETRIANGLES) )
					throw CNMRException(NMR_USERABORTED);
			}

			// Get Mesh Face
			MESHFACE * pMeshFace = pMesh->getFace(nFaceIndex);

			ModelResourceID nPropertyID = 0;
			ModelResourceIndex nPropertyIndex1 = 0;
			ModelResourceIndex nPropertyIndex2 = 0;
			ModelResourceIndex nPropertyIndex3 = 0;

			nfChar * pAdditionalString = nullptr;
			// Retrieve Base Material
			if (pBaseMaterials) {
				MESHINFORMATION_BASEMATERIAL* pFaceData = (MESHINFORMATION_BASEMATERIAL*)pBaseMaterials->getFaceData(nFaceIndex);
				if (pFaceData->m_nMaterialGroupID) {
					nPropertyID = pFaceData->m_nMaterialGroupID;
					nPropertyIndex1 = pFaceData->m_nMaterialIndex;
					nPropertyIndex2 = pFaceData->m_nMaterialIndex;
					nPropertyIndex3 = pFaceData->m_nMaterialIndex;
				}
			}

			if (m_bWriteMaterialExtension) {
				// Retrieve Node Colors
				if (pNodeColors) {
					MESHINFORMATION_NODECOLOR* pFaceData = (MESHINFORMATION_NODECOLOR*)pNodeColors->getFaceData(nFaceIndex);
					if ((pFaceData->m_cColors[0] != 0) || (pFaceData->m_cColors[1] != 0) || (pFaceData->m_cColors[2] != 0)) {

						ModelResourceIndex nColorIndex1 = 0;
						ModelResourceIndex nColorIndex2 = 0;
						ModelResourceIndex nColorIndex3 = 0;
						nfBool colorsFound = m_pColorMapping->findColor(pFaceData->m_cColors[0], nColorIndex1) &&
							m_pColorMapping->findColor(pFaceData->m_cColors[1], nColorIndex2) &&
							m_pColorMapping->findColor(pFaceData->m_cColors[2], nColorIndex3);

						if (colorsFound) {
							nPropertyID = m_pColorMapping->getResourceID();
							nPropertyIndex1 = nColorIndex1;
							nPropertyIndex2 = nColorIndex2;
							nPropertyIndex3 = nColorIndex3;
						}
					}
				}

				// Retrieve TexCoords
				if (pTexCoords) {
					MESHINFORMATION_TEXCOORDS* pFaceData = (MESHINFORMATION_TEXCOORDS*)pTexCoords->getFaceData(nFaceIndex);
					if (pFaceData->m_TextureID != 0) {
						ModelResourceIndex nTextureIndex1 = 0;
						ModelResourceIndex nTextureIndex2 = 0;
						ModelResourceIndex nTextureIndex3 = 0;

						PModelWriter_TexCoordMapping pMapping = m_pTextureMappingContainer->findTexture(pFaceData->m_TextureID);
						if (pMapping.get() != nullptr) {
							nfBool textureFound = pMapping->findTexCoords(pFaceData->m_vCoords[0].m_fields[0], pFaceData->m_vCoords[0].m_fields[1], nTextureIndex1) &&
								pMapping->findTexCoords(pFaceData->m_vCoords[1].m_fields[0], pFaceData->m_vCoords[1].m_fields[1], nTextureIndex2) &&
								pMapping->findTexCoords(pFaceData->m_vCoords[2].m_fields[0], pFaceData->m_vCoords[2].m_fields[1], nTextureIndex3);

							if (textureFound) {
								nPropertyID = pMapping->getResourceID();
								nPropertyIndex1 = nTextureIndex1;
								nPropertyIndex2 = nTextureIndex2;
								nPropertyIndex3 = nTextureIndex3;
							}

						}

					}
				}
			}

			if (nPropertyID != 0) {
				if ((nPropertyIndex1 != nPropertyIndex2) || (nPropertyIndex1 != nPropertyIndex3)) {
					writeFaceData_ThreeProperties(pMeshFace, nPropertyID, nPropertyIndex1, nPropertyIndex2, nPropertyIndex3, pAdditionalString);
				}
				else {
					writeFaceData_OneProperty(pMeshFace, nPropertyID, nPropertyIndex1, pAdditionalString);
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

		if (m_bWriteBeamLatticeExtension) {
			if (nBeamCount > 0) {
				// write beamlattice
				writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMLATTICE, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
				writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_RADIUS, float(pMesh->getDefaultBeamRadius()));
				writeFloatAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_MINLENGTH, float(pMesh->getBeamLatticeMinLength()));

				if (m_pModelMeshObject->getBeamLatticeAttributes()->m_bHasClippingMeshID) {
					writeStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPING, clipModeToString(m_pModelMeshObject->getBeamLatticeAttributes()->m_eClipMode));
					writeIntAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMESH, m_pModelMeshObject->getBeamLatticeAttributes()->m_nClippingMeshID->getUniqueID());
				}

				eModelBeamLatticeCapMode eDefaultCapMode = pMesh->getBeamLatticeCapMode();
				writeConstStringAttribute(XML_3MF_ATTRIBUTE_BEAMLATTICE_CAPMODE, capModeToString(eDefaultCapMode).c_str());
				{
					// write beamlattice: beams
					writeStartElementWithPrefix(XML_3MF_ELEMENT_BEAMS, XML_3MF_NAMESPACEPREFIX_BEAMLATTICE);
					for (nBeamIndex = 0; nBeamIndex < nBeamCount; nBeamIndex++) {
						// write beamlattice: beam
						MESHBEAM * pMeshBeam = pMesh->getBeam(nBeamIndex);
						writeBeamData(pMeshBeam, pMesh->getDefaultBeamRadius(), eDefaultCapMode);
					}
					writeFullEndElement();

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
		nfInt64 nAbsValue = (nfInt64)(fValue * m_snPutDoubleFactor);
		nAbsValue = MAX(nAbsValue, -nAbsValue);
		nfBool bIsNegative = fValue < 0;

		int nStart = nBufferPos;
		int nCount = 0;

		if (!nAbsValue) {
			line[nBufferPos++] = '0';
		}
		else {
			// Write the string in reverse order
			while (nAbsValue || nCount < m_snPosAfterDecPoint) {
				line[nBufferPos++] = '0' + (nAbsValue % 10);
				nAbsValue /= 10;
				nCount++;
				if (nCount == m_snPosAfterDecPoint) {
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
		nfInt64 nAbsValue = (nfInt64)(dValue * m_snPutDoubleFactor);
		nAbsValue = MAX(nAbsValue, -nAbsValue);
		nfBool bIsNegative = dValue < 0;

		int nStart = nBufferPos;
		int nCount = 0;

		if (!nAbsValue) {
			line[nBufferPos++] = '0';
		}
		else {
			// Write the string in reverse order
			while (nAbsValue || nCount < m_snPosAfterDecPoint) {
				line[nBufferPos++] = '0' + (nAbsValue % 10);
				nAbsValue /= 10;
				nCount++;
				if (nCount == m_snPosAfterDecPoint) {
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


	bool stringRepresentationsDiffer(double a, double b, double putFactor) {
		return  fabs(a - b) * putFactor > 0.1;
	}

	__NMR_INLINE void CModelWriterNode100_Mesh::writeBeamData(_In_ MESHBEAM * pBeam, _In_ nfDouble dRadius, _In_ eModelBeamLatticeCapMode eDefaultCapMode)
	{
		__NMRASSERT(pBeam);
		m_nBeamBufferPos = MODELWRITERMESH100_BEAMLATTICE_BEAMSTARTLENGTH;
		putBeamUInt32(pBeam->m_nodeindices[0]);
		const std::string sV2 = "\" " + std::string(XML_3MF_ATTRIBUTE_BEAMLATTICE_V2) + "=\"";
		putBeamString(sV2.c_str());
		putBeamUInt32(pBeam->m_nodeindices[1]);

		nfBool bWriteR2 = stringRepresentationsDiffer(pBeam->m_radius[0], pBeam->m_radius[1], m_snPutDoubleFactor);
		nfBool bWriteR1 = bWriteR2 || stringRepresentationsDiffer(pBeam->m_radius[0], dRadius, m_snPutDoubleFactor);
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

	__NMR_INLINE void CModelWriterNode100_Mesh::writeRefData(_In_ INT nRefID)
	{
		m_nBeamRefBufferPos = MODELWRITERMESH100_BEAMLATTICE_REFSTARTLENGTH;
		putBeamRefUInt32(nRefID);
		putBeamRefString("\"/>");
		m_pXMLWriter->WriteRawLine(&m_BeamRefLine[0], m_nBeamRefBufferPos);
	}

}
