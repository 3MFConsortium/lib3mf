/*++

Copyright (C) 2017 Autodesk Inc.

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

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_Segment.h"

#include "Model/COM/NMR_COMInterface_ModelReader.h"
#include "Model/Classes/NMR_ModelAttachment.h"
#ifdef NMR_COM_NATIVE
#include "Common/Platform/NMR_COM_Native.h"
#include "Model/Reader/NMR_ModelReader_3MF_OPC.h"
#else
#include "Common/Platform/NMR_COM_Emulation.h"
#include "Model/Reader/NMR_ModelReader_3MF_Native.h"
#endif

#include "Common/NMR_StringUtils.h"

#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {
	void CModelReaderNode_Slices1507_Segment::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) {
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICESEGMENT_V2) == 0) {
			m_pSlice->addPolygonIndex(m_PolygonIndex, fnStringToInt32(pAttributeValue));
		}
	}

	void CModelReaderNode_Slices1507_Segment::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) {

	}

	CModelReaderNode_Slices1507_Segment::CModelReaderNode_Slices1507_Segment(_In_ CSlice *pSlice, nfUint32 nPolygonIndex, _In_ PModelReaderWarnings pWarnings) : CModelReaderNode(pWarnings) {
		m_pSlice = pSlice;
		m_PolygonIndex = nPolygonIndex;
	}

	void CModelReaderNode_Slices1507_Segment::parseXML(_In_ CXmlReader * pXMLReader) {
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}
}
