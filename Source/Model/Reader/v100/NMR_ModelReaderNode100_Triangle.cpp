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
#include <cmath>

namespace NMR {

	CModelReaderNode100_Triangle::CModelReaderNode100_Triangle(_In_ PModelWarnings pWarnings)
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

	nfBool CModelReaderNode100_Triangle::retrieveProperties(_Inout_ ModelResourceID & nPropertyID, _Inout_ ModelResourceIndex & nPropertyIndex1, _Inout_ ModelResourceIndex & nPropertyIndex2, _Inout_ ModelResourceIndex & nPropertyIndex3)
	{

		if (m_nPropertyID == 0)
			return false;
		if (m_nPropertyIndex1 < 0)
			return false;

		// See Core Spec 4.1.3.1 (Triangle)
		nPropertyID = m_nPropertyID;
		nPropertyIndex1 = m_nPropertyIndex1;

		if (m_nPropertyIndex2 >= 0) {
			nPropertyIndex2 = m_nPropertyIndex2;
		}
		else {
			nPropertyIndex2 = m_nPropertyIndex1;
		}

		if (m_nPropertyIndex3 >= 0) {
			nPropertyIndex3 = m_nPropertyIndex3;
		}
		else {
			nPropertyIndex3 = m_nPropertyIndex1;
		}

		return true;

	}

	void CModelReaderNode100_Triangle::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		nfInt32 nValue;

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V1) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex1 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V2) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex2 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V3) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex3 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_PID) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEID))
				m_nPropertyID = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P1) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex1 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P2) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex2 = nValue;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_P3) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nPropertyIndex3 = nValue;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

}
