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

NMR_ModelWriterNode_Mesh.h defines the Model Writer Mesh Node Class.
This is the class for exporting the 3mf mesh node.

--*/

#ifndef __NMR_MODELWRITERNODE100_MESH
#define __NMR_MODELWRITERNODE100_MESH

#include "Model/Writer/NMR_ModelWriterNode.h" 
#include "Model/Writer/NMR_ModelWriter_ColorMapping.h" 
#include "Model/Writer/NMR_ModelWriter_TexCoordMappingContainer.h" 
#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMeshObject.h" 
#include "Common/Platform/NMR_XmlWriter.h"
#include <array>

#define MODELWRITERMESH100_LINEBUFFERSIZE 512
#define MODELWRITERMESH100_VERTEXLINESTART "<vertex x=\""
#define MODELWRITERMESH100_TRIANGLELINESTART "<triangle v1=\""
#define MODELWRITERMESH100_VERTEXLINESTARTLENGTH 11
#define MODELWRITERMESH100_TRIANGLELINESTARTLENGTH 14

namespace NMR {

	class CModelWriterNode100_Mesh : public CModelWriterNode {
	protected:
		CModelMeshObject * m_pModelMeshObject;
		PModelWriter_ColorMapping m_pColorMapping;
		PModelWriter_TexCoordMappingContainer m_pTextureMappingContainer;

		// Internal functions for an efficient and buffered output of raw XML data
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_VertexLine;
		std::array<nfChar, MODELWRITERMESH100_LINEBUFFERSIZE> m_TriangleLine;
		nfUint32 m_nVertexBufferPos;
		nfUint32 m_nTriangleBufferPos;

		__NMR_INLINE void putVertexString(_In_ const nfChar * pszString);
		__NMR_INLINE void putVertexFloat(_In_ const nfFloat fValue);

		__NMR_INLINE void putTriangleString(_In_ const nfChar * pszString);
		__NMR_INLINE void putTriangleUInt32(_In_ const nfUint32 nValue);

		__NMR_INLINE void writeVertexData(_In_ MESHNODE * pNode);
		__NMR_INLINE void writeFaceData_Plain(_In_ MESHFACE * pFace);
		__NMR_INLINE void writeFaceData_OneProperty(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex);
		__NMR_INLINE void writeFaceData_ThreeProperties(_In_ MESHFACE * pFace, _In_ const ModelResourceID nPropertyID, _In_ const ModelResourceIndex nPropertyIndex1, _In_ const ModelResourceIndex nPropertyIndex2, _In_ const ModelResourceIndex nPropertyIndex3);
	public:
		CModelWriterNode100_Mesh() = delete;
		CModelWriterNode100_Mesh(_In_ CModelMeshObject * pModelMeshObject, _In_ CXmlWriter * pXMLWriter, _In_ PModelWriter_ColorMapping pColorMapping, _In_ PModelWriter_TexCoordMappingContainer pTextureMappingContainer);
		
		virtual void writeToXML();
	};

}

#endif // __NMR_MODELWRITERNODE100_MESH
