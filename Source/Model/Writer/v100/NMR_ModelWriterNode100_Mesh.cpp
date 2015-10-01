/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

#ifdef __GCC
#include <stdio.h>
#endif // __GCC

namespace NMR {

	CModelWriterNode100_Mesh::CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter, _In_ PModelWriter_ColorMapping pColorMapping, _In_ PModelWriter_TexCoordMappingContainer pTextureMappingContainer)
		:CModelWriterNode(pModelMeshObject->getModel(), pXMLWriter)
	{
		__NMRASSERT(pModelMeshObject != nullptr);
		if (!pColorMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (!pTextureMappingContainer.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModelMeshObject = pModelMeshObject;
		m_pColorMapping = pColorMapping;
		m_pTextureMappingContainer = pTextureMappingContainer;

		// Initialize buffer arrays
		m_pszFloatFormat = "%.3f";
		m_nTriangleBufferPos = 0;
		m_nVertexBufferPos = 0;
		putVertexString(MODELWRITERMESH100_VERTEXLINESTART);
		putTriangleString(MODELWRITERMESH100_TRIANGLELINESTART);
	}

	void CModelWriterNode100_Mesh::writeToXML()
	{
		__NMRASSERT(m_pXMLWriter);
		__NMRASSERT(m_pModel);

		CMesh * pMesh = m_pModelMeshObject->getMesh();

		nfUint32 nNodeCount = pMesh->getNodeCount ();
		nfUint32 nFaceCount = pMesh->getFaceCount();
		nfUint32 nNodeIndex, nFaceIndex;

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

			// Get Node Colors
			pInformation = pMeshInformationHandler->getInformationByType(0, emiNodeColors);
			if (pInformation)
				pNodeColors = dynamic_cast<CMeshInformation_NodeColors *> (pInformation);

			// Get Tex Coords
			pInformation = pMeshInformationHandler->getInformationByType(0, emiTexCoords);
			if (pInformation)
				pTexCoords = dynamic_cast<CMeshInformation_TexCoords *> (pInformation);

		}


		// Write Triangles
		writeStartElement(XML_3MF_ELEMENT_TRIANGLES);
		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			// Get Mesh Face
			MESHFACE * pMeshFace = pMesh->getFace(nFaceIndex);

			ModelResourceID nPropertyID = 0;
			ModelResourceIndex nPropertyIndex1 = 0;
			ModelResourceIndex nPropertyIndex2 = 0;
			ModelResourceIndex nPropertyIndex3 = 0;

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

			if (nPropertyID != 0) {
				if ((nPropertyIndex1 != nPropertyIndex2) || (nPropertyIndex1 != nPropertyIndex3)) {
					writeFaceData_ThreeProperties(pMeshFace, nPropertyID, nPropertyIndex1, nPropertyIndex2, nPropertyIndex3);
				}
				else {
					writeFaceData_OneProperty(pMeshFace, nPropertyID, nPropertyIndex1);
				}
			}
			else
			{
				writeFaceData_Plain(pMeshFace);

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

	void CModelWriterNode100_Mesh::putVertexFloat(_In_ const nfFloat fValue)
	{

#ifdef __GCC
		int nCount = sprintf(&m_VertexLine[m_nVertexBufferPos], m_pszFloatFormat, fValue);
#else
		int nCount = sprintf_s(&m_VertexLine[m_nVertexBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nVertexBufferPos, m_pszFloatFormat, fValue);
#endif // __GCC

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nVertexBufferPos += nCount;
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

#ifdef __GCC
		int nCount = sprintf(&m_TriangleLine[m_nTriangleBufferPos], "%d", nValue);
#else
		int nCount = sprintf_s(&m_TriangleLine[m_nTriangleBufferPos], MODELWRITERMESH100_LINEBUFFERSIZE - m_nTriangleBufferPos, "%d", nValue);
#endif // __GCC

		if (nCount < 1)
			throw CNMRException(NMR_ERROR_COULDNOTCONVERTNUMBER);
		m_nTriangleBufferPos += nCount;
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

	void CModelWriterNode100_Mesh::writeFaceData_Plain(_In_ MESHFACE * pFace)
	{
		__NMRASSERT(pFace);
		m_nTriangleBufferPos = MODELWRITERMESH100_TRIANGLELINESTARTLENGTH;
		putTriangleUInt32(pFace->m_nodeindices[0]);
		putTriangleString("\" v2=\"");
		putTriangleUInt32(pFace->m_nodeindices[1]);
		putTriangleString("\" v3=\"");
		putTriangleUInt32(pFace->m_nodeindices[2]);
		putTriangleString("\" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}

	void CModelWriterNode100_Mesh::writeFaceData_OneProperty(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex)
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
		putTriangleString("\" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}

	void CModelWriterNode100_Mesh::writeFaceData_ThreeProperties(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex1, _In_ const ModelResourceIndex nPropertyIndex2, _In_ const ModelResourceIndex nPropertyIndex3)
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
		putTriangleString("\" />");
		m_pXMLWriter->WriteRawLine(&m_TriangleLine[0], m_nTriangleBufferPos);
	}

}
