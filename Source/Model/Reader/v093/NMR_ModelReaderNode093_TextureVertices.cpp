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

NMR_ModelReaderNode093_TextureVertices.cpp defines the Model Reader Texture Vertices Node Class.
A texture vertices reader model node is a parser for the texture vertices node of an
XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_TextureVertices.h"
#include "Model/Reader/v093/NMR_ModelReaderNode093_TextureVertex.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode093_TextureVertices::CModelReaderNode093_TextureVertices(_In_ CMesh * pMesh, _In_ PModelWarnings pWarnings, _In_ PModelReader_TexCoordMapping pTexCoordMapping)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pMesh);
		if (pTexCoordMapping.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pMesh = pMesh;
		m_pTexCoordMapping = pTexCoordMapping;
		m_nTexCoordIndex = 0;
	}

	void CModelReaderNode093_TextureVertices::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse Name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_TextureVertices::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
	}

	void CModelReaderNode093_TextureVertices::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_TEXTUREVERTEX) == 0)
			{
				PModelReaderNode093_TextureVertex pXMLNode = std::make_shared<CModelReaderNode093_TextureVertex>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				// Create Mesh Node
				nfFloat fU, fV;
				pXMLNode->retrieveTexCoords(fU, fV);

				m_pTexCoordMapping->registerTexCoords(1, m_nTexCoordIndex, 1, fU, fV);
				m_nTexCoordIndex++;

				if (m_nTexCoordIndex > XML_3MF_MAXRESOURCEINDEX)
					throw CNMRException(NMR_ERROR_INVALIDINDEX);
			}
		}
	}

}
