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

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_TriangleSet.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_TriangleSetRef.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>

namespace NMR {

	CModelReaderNode100_TriangleSet::CModelReaderNode100_TriangleSet(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
	}

	void CModelReaderNode100_TriangleSet::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		m_pTriangleSet = std::make_shared<CModelTriangleSet>(m_sName, m_sIdentifier);

		// Parse Content
		parseContent(pXMLReader);
	}

	
	void CModelReaderNode100_TriangleSet::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue); 
		
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLESET_NAME) == 0) {
            m_sName = pAttributeValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLESET_IDENTIFIER) == 0) {
			m_sIdentifier = pAttributeValue;
		}
	}

	void CModelReaderNode100_TriangleSet::OnNSChildElement(_In_z_ const nfChar* pChildName, _In_z_ const nfChar* pNameSpace, _In_ CXmlReader* pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_TRIANGLESETS) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_REF) == 0)
			{
				PModelReaderNode100_TriangleSetRef pXMLNode = std::make_shared<CModelReaderNode100_TriangleSetRef>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pTriangleSet->addTriangle(pXMLNode->getIndex());
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}

	}


	PModelTriangleSet CModelReaderNode100_TriangleSet::createTriangleSet()
	{
		return m_pTriangleSet;
	}

}
