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

NMR_ModelReaderNode093_Mesh.cpp implements the Model Reader Mesh Node Class.
A mesh reader model node is a parser for the mesh node of an XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Mesh.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Vertices.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangles.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_TextureVertices.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_Mesh::CModelReaderNode093_Mesh(_In_ CModel * pModel, _In_ CMesh * pMesh, _In_ PModelReader_ColorMapping pColorMapping, _In_ PModelBaseMaterialResource pMaterialResource, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		__NMRASSERT(pModel);
		__NMRASSERT(pColorMapping.get() != nullptr);

		m_pMesh = pMesh;
		m_pModel = pModel;

		m_pTexCoordMapping = std::make_shared<CModelReader_TexCoordMapping>();
		m_pColorMapping = pColorMapping;
		m_pMaterialResource = pMaterialResource;
	}

	void CModelReaderNode093_Mesh::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_Mesh::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode093_Mesh::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {

			if (strcmp(pChildName, XML_3MF_ELEMENT_VERTICES) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Vertices>(m_pMesh, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}

			if (strcmp(pChildName, XML_3MF_ELEMENT_TRIANGLES) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Triangles>(m_pModel, m_pMesh, m_pColorMapping, m_pTexCoordMapping, m_pMaterialResource, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}

			if (strcmp(pChildName, XML_3MF_ELEMENT_TEXTUREVERTICES) == 0)
			{
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_TextureVertices>(m_pMesh, m_pWarnings, m_pTexCoordMapping);
				pXMLNode->parseXML(pXMLReader);
			}

		}
	}

}
