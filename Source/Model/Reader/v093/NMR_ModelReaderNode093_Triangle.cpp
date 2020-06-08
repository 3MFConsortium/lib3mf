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

NMR_ModelReaderNode093_Triangle.cpp implements the Model Reader Triangle Node
Class. A triangle reader model node is a parser for the triangle node of an
XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Triangle.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <climits>
#include <cmath>
#include <wchar.h>

namespace NMR {

	CModelReaderNode093_Triangle::CModelReaderNode093_Triangle(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialise default values
		m_nIndex1 = -1;
		m_nIndex2 = -1;
		m_nIndex3 = -1;

		m_nTextureIndex1 = -1;
		m_nTextureIndex2 = -1;
		m_nTextureIndex3 = -1;
		m_nColorID1 = 0;
		m_nColorID2 = 0;
		m_nColorID3 = 0;
		m_nMaterialID = 0;
		m_nTextureID = 0;
	}

	void CModelReaderNode093_Triangle::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode093_Triangle::retrieveIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3, nfInt32 nNodeCount)
	{
		if ((m_nIndex1 < 0) || (m_nIndex2 < 0) || (m_nIndex3 < 0))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		if ((m_nIndex1 >= nNodeCount) || (m_nIndex2 >= nNodeCount) || (m_nIndex3 >= nNodeCount))
			throw CNMRException(NMR_ERROR_INVALIDMODELNODEINDEX);

		nIndex1 = m_nIndex1;
		nIndex2 = m_nIndex2;
		nIndex3 = m_nIndex3;
	}

	nfBool CModelReaderNode093_Triangle::retrieveTextureIndices(_Out_ nfInt32 & nIndex1, _Out_ nfInt32 & nIndex2, _Out_ nfInt32 & nIndex3)
	{

		nIndex1 = m_nTextureIndex1;
		nIndex2 = m_nTextureIndex2;
		nIndex3 = m_nTextureIndex3;

		return ((nIndex1 >= 0) && (nIndex2 >= 0) && (nIndex3 >= 0));
	}



	void CModelReaderNode093_Triangle::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		nfInt32 nValue;

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V1) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex1 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V2) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex2 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_V3) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nIndex3 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_CV1) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nTextureIndex1 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_CV2) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nTextureIndex2 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_CV3) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nTextureIndex3 = nValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_COLORID) == 0) {
			std::string sValue(pAttributeValue);
			std::string sSubStr = sValue.substr(0, 4);
			if (sSubStr == "tex(") {
				if (sValue.length() > 4) {
					std::string sTexID = sValue.substr(4, sValue.length() - sSubStr.length() - 1);
					nfInt32 nValue = fnStringToInt32(sTexID.c_str());
					if ((nValue < 0) || (nValue >= XML_3MF_MAXRESOURCEINDEX))
						throw CNMRException(NMR_ERROR_INVALIDTEXTUREREFERENCE);

					m_nTextureID = nValue + 1;

				}
				else {
					throw CNMRException(NMR_ERROR_INVALIDTEXTUREREFERENCE);
				}

			}
			else {
				const nfChar * pCommaValue = strchr(pAttributeValue, ',');

				// Check, if we have a single value
				if (pCommaValue == nullptr) {

					nValue = fnStringToInt32(pAttributeValue);
					if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX)) {
						m_nColorID1 = nValue + 1;
						m_nColorID2 = nValue + 1;
						m_nColorID3 = nValue + 1;
					}
				}
				else {
					// Check, if we have a color triplet
					nfInt32 nColorID1 = -1;
					nfInt32 nColorID2 = -1;
					nfInt32 nColorID3 = -1;

					fnStringToCommaSeparatedIntegerTriplet(pAttributeValue, nColorID1, nColorID2, nColorID3);

					if ((nColorID1 >= 0) && (nColorID1 < XML_3MF_MAXRESOURCEINDEX))
						m_nColorID1 = nColorID1 + 1;
					if ((nColorID2 >= 0) && (nColorID2 < XML_3MF_MAXRESOURCEINDEX))
						m_nColorID2 = nColorID2 + 1;
					if ((nColorID3 >= 0) && (nColorID3 < XML_3MF_MAXRESOURCEINDEX))
						m_nColorID3 = nColorID3 + 1;
				}
			}
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_TRIANGLE_MATERIALID) == 0) {
			nValue = fnStringToInt32(pAttributeValue);
			if ((nValue >= 0) && (nValue < XML_3MF_MAXRESOURCEINDEX))
				m_nMaterialID = nValue + 1;
		}

	}

	nfBool CModelReaderNode093_Triangle::retrieveColorIDs(_Out_ nfInt32 & nColorID1, _Out_ nfInt32 & nColorID2, _Out_ nfInt32 & nColorID3)
	{
		nColorID1 = m_nColorID1;
		nColorID2 = m_nColorID2;
		nColorID3 = m_nColorID3;
		return ((nColorID1 != 0) && (nColorID2 != 0) && (nColorID3 != 0));
	}

	nfInt32 CModelReaderNode093_Triangle::retrieveTextureID()
	{
		return m_nTextureID;
	}

	nfInt32 CModelReaderNode093_Triangle::retrieveMaterialID()
	{
		return m_nMaterialID;
	}

}
