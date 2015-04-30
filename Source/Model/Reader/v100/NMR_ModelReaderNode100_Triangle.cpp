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

NMR_ModelReaderNode100_Triangle.cpp implements the Model Reader Triangle Node
Class. A triangle reader model node is a parser for the triangle node of an
XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <math.h>

namespace NMR {

	CModelReaderNode100_Triangle::CModelReaderNode100_Triangle(_In_ PModelReaderWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialise default values
		m_nPropertyID = 0;
		m_nPropertyIndex1 = -1;
		m_nPropertyIndex2 = -1;
		m_nPropertyIndex3 = -1;
		m_nIndex1 = -1;
		m_nIndex2 = -1;
		m_nIndex3 = -1;
	}

	void CModelReaderNode100_Triangle::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_Triangle::retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3, nfInt32 nNodeCount)
	{
		if ((m_nIndex1 < 0) || (m_nIndex2 < 0) || (m_nIndex3 < 0))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		if ((m_nIndex1 >= nNodeCount) || (m_nIndex2 >= nNodeCount) || (m_nIndex3 >= nNodeCount))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		nIndex1 = m_nIndex1;
		nIndex2 = m_nIndex2;
		nIndex3 = m_nIndex3;
	}

	nfBool CModelReaderNode100_Triangle::retrieveProperties(_Out_ ModelResourceID & nPropertyID, _Out_ ModelResourceIndex & nPropertyIndex1, _Out_ ModelResourceIndex & nPropertyIndex2, _Out_ ModelResourceIndex & nPropertyIndex3)
	{
		if (m_nPropertyID == 0)
			return false;
		if ((m_nPropertyIndex1 < 0) || (m_nPropertyIndex2 < 0) ||
			(m_nPropertyIndex3 < 0))
			return false;

		nPropertyID = m_nPropertyID;
		nPropertyIndex1 = m_nPropertyIndex1;
		nPropertyIndex2 = m_nPropertyIndex2;
		nPropertyIndex3 = m_nPropertyIndex3;

		return true;
	}

	void CModelReaderNode100_Triangle::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		nfInt32 nValue;

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V1) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex1 = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V2) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex2 = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V3) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex3 = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_PID) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEID))
				m_nPropertyID = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P1) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex1 = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P2) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex2 = nValue;
		}

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P3) == 0) {
			nValue = fnWStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex3 = nValue;
		}
	}

	void CModelReaderNode100_Triangle::OnChildElement(_In_z_ const nfWChar * pChildName, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
	}

}
