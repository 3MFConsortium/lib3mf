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

--*/

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertex.h"
#include "Common/NMR_StringUtils.h"
#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {
	void CModelReaderNode_Slices1507_Vertex::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) {
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEVERTEX_X) == 0) {
			m_x = fnStringToFloat(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEVERTEX_Y) == 0) {
			m_y = fnStringToFloat(pAttributeValue);
		}
		else
			throw CNMRException(NMR_ERROR_SLICE_INVALIDATTRIBUTE);
	}

	void CModelReaderNode_Slices1507_Vertex::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) {

	}

	CModelReaderNode_Slices1507_Vertex::CModelReaderNode_Slices1507_Vertex(_In_ CSlice *pSlice, _In_ PModelWarnings pWarnings) : CModelReaderNode(pWarnings) {
		m_pSlice = pSlice;
	}

	void CModelReaderNode_Slices1507_Vertex::parseXML(_In_ CXmlReader * pXMLReader) {
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		m_pSlice->addVertex(m_x, m_y);
	}
}
