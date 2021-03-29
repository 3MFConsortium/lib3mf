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
NMR_ModelReaderNode093_TextureVertex.cpp implements the Model Reader Texture Vertex Node Class.
A texture vertex reader model node is a parser for the vertex node of an XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_TextureVertex.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include <cmath>
#include <climits>
#include <stdlib.h>

namespace NMR {

	CModelReaderNode093_TextureVertex::CModelReaderNode093_TextureVertex(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_fU = 0.0f;
		m_fV = 0.0f;
		m_bHasU = false;
		m_bHasV = false;
	}

	void CModelReaderNode093_TextureVertex::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_TextureVertex::retrieveTexCoords(_Out_ nfFloat & fU, _Out_ nfFloat & fV)
	{
		try
		{
			// Texture Coordinate is missing
			if ((!m_bHasU) || (!m_bHasV))
				throw CNMRException(NMR_ERROR_MODELTEXTURECOORDINATEMISSING);
		} 
		catch (CNMRException & Exception)
		{
			m_pWarnings->addException(Exception, mrwInvalidOptionalValue);
		}

		fU = m_fU;
		fV = m_fV;
	}

	void CModelReaderNode093_TextureVertex::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE_U) == 0) {
			m_fU = strtof(pAttributeValue, nullptr);
			if (std::isnan (m_fU))
				throw CNMRException(NMR_ERROR_INVALIDMODELTEXTURECOORDINATES);
			if (fabs (m_fU) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELTEXTURECOORDINATES);
			m_bHasU = true;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TEXTURE_V) == 0) {
			m_fV = strtof(pAttributeValue, nullptr);
			if (std::isnan (m_fV))
				throw CNMRException(NMR_ERROR_INVALIDMODELTEXTURECOORDINATES);
			if (fabs(m_fV) > XML_3MF_MAXIMUMCOORDINATEVALUE)
				throw CNMRException(NMR_ERROR_INVALIDMODELTEXTURECOORDINATES);
			m_bHasV = true;
		}

	}


}
