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
NMR_ModelReaderNode100_Vertex.cpp implements the Model Reader Vertex Node Class.
A vertex reader model node is a parser for the vertex node of an XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Vertex.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>
#include <climits>
#include <stdlib.h>

namespace NMR {

	CModelReaderNode100_Vertex::CModelReaderNode100_Vertex(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
		m_bHasX = false;
		m_bHasY = false;
		m_bHasZ = false;
	}

	void CModelReaderNode100_Vertex::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Vertex::retrievePosition(_Out_ nfFloat & fX, _Out_ nfFloat & fY, _Out_ nfFloat & fZ)
	{
		// Model Coordinate is missing
		if ((!m_bHasX) || (!m_bHasY) || (!m_bHasZ))
			throw CNMRException(NMR_ERROR_MODELCOORDINATEMISSING);

		fX = m_fX;
		fY = m_fY;
		fZ = m_fZ;
	}

	void CModelReaderNode100_Vertex::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_X) == 0) {
			m_fX = strtof(pAttributeValue, nullptr);
			if (std::isnan (m_fX))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs (m_fX) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_bHasX = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_Y) == 0) {
			m_fY = strtof(pAttributeValue, nullptr);
			if (std::isnan (m_fY))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_fY) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_bHasY = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_VERTEX_Z) == 0) {
			m_fZ = strtof(pAttributeValue, nullptr);
			if (std::isnan (m_fZ))
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			if (fabs(m_fZ) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELCOORDINATES);
			m_bHasZ = true;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}


}
