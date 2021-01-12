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

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Slice.h"
#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Vertices.h"
#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Polygon.h"

#include "Common/NMR_StringUtils.h"
#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {
	void CModelReaderNode_Slices1507_Slice::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) {
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEZTOP) == 0) {
			if (m_bHasZTop)
				throw CNMRException(NMR_ERROR_DUPLICATETEZTOP);
			m_TopZ = fnStringToFloat(pAttributeValue);
			m_bHasZTop = true;
		}
	}

	void CModelReaderNode_Slices1507_Slice::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) {
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SLICESPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_SLICEVERTICES) == 0) {
				PModelReaderNode_Slices1507_Vertices pXMLNode = std::make_shared<CModelReaderNode_Slices1507_Vertices>(m_Slice.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_SLICEPOLYGON) == 0) {
				PModelReaderNode_Slices1507_Polygon pXMLNode = std::make_shared<CModelReaderNode_Slices1507_Polygon>(m_Slice.get(), m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

	CModelReaderNode_Slices1507_Slice::CModelReaderNode_Slices1507_Slice(_In_ CModelSliceStack *pSliceStack, _In_ PModelWarnings pWarnings) : CModelReaderNode(pWarnings) {
		m_pSliceStack = pSliceStack;
		m_bHasZTop = false;
	}

	void CModelReaderNode_Slices1507_Slice::parseXML(_In_ CXmlReader * pXMLReader) {
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		m_Slice = m_pSliceStack->AddSlice(m_TopZ);

		// Parse Content
		parseContent(pXMLReader);
		if (!m_bHasZTop)
			throw CNMRException(NMR_ERROR_MISSINGTEZTOP);
	}
}
