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

#include "Model/Reader/Slice1507/NMR_ModelReader_Slice1507_SliceRef.h"

#include "Model/Classes/NMR_ModelAttachment.h"

#include "Common/NMR_StringUtils.h"

#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {
	void CModelReaderNode_Slice1507_SliceRef::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue) {
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEREF_ID) == 0) {
			m_SliceStackId = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEREF_PATH) == 0) {
			m_Path = pAttributeValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_SLICEZTOP) == 0) {
			// according to spec, the attribute "ztop" is not allowed in a SliceRef
			// We we only warn here
			m_pWarnings->addException(CNMRException(NMR_ERROR_SLICE_INVALIDATTRIBUTE), mrwInvalidMandatoryValue);
		}
		else
			throw CNMRException(NMR_ERROR_SLICE_INVALIDATTRIBUTE);
	}

	void CModelReaderNode_Slice1507_SliceRef::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader) {

	}

	CModelReaderNode_Slice1507_SliceRef::CModelReaderNode_Slice1507_SliceRef(_In_ PModelWarnings pWarnings) : CModelReaderNode(pWarnings) {
		m_SliceStackId = -1;
		m_Path = "";
	}

	void CModelReaderNode_Slice1507_SliceRef::parseXML(_In_ CXmlReader * pXMLReader) {
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	std::string CModelReaderNode_Slice1507_SliceRef::Path()
	{
		return m_Path;
	}
	nfUint32 CModelReaderNode_Slice1507_SliceRef::SliceStackId()
	{
		return m_SliceStackId;
	}
}
