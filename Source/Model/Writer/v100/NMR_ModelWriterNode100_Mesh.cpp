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

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelWriterNode100_Mesh::CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter)
		:CModelWriterNode(pModelMeshObject->getModel(), pXMLWriter)
	{
		__NMRASSERT(pModelMeshObject != nullptr);
		m_pModelMeshObject = pModelMeshObject;
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

			// Write Vertex
			writeStartElement(XML_3MF_ELEMENT_VERTEX);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_X, pMeshNode->m_position.m_values.x);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_Y, pMeshNode->m_position.m_values.y);
			writeFloatAttribute(XML_3MF_ATTRIBUTE_VERTEX_Z, pMeshNode->m_position.m_values.z);
			writeEndElement();
		}
		writeFullEndElement();

		// Write Triangles
		writeStartElement(XML_3MF_ELEMENT_TRIANGLES);
		for (nFaceIndex = 0; nFaceIndex < nFaceCount; nFaceIndex++) {
			// Get Mesh Face
			MESHFACE * pMeshFace = pMesh->getFace(nFaceIndex);

			// Write Triangle
			writeStartElement(XML_3MF_ELEMENT_TRIANGLE);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V1, pMeshFace->m_nodeindices[0]);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V2, pMeshFace->m_nodeindices[1]);
			writeIntAttribute(XML_3MF_ATTRIBUTE_TRIANGLE_V3, pMeshFace->m_nodeindices[2]);
			writeEndElement();
		}
		writeFullEndElement();

		// Finish Mesh Element
		writeFullEndElement();
	}

}
