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
NMR_ModelReaderNode093_Color.cpp implements the Model Reader Color Node Class.
A color reader model node is a parser for the color node of an XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Color.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <cmath>
#include <climits>

namespace NMR {

	CModelReaderNode093_Color::CModelReaderNode093_Color(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_nResourceID = 0;
		m_nTextureID = 0;
	}

	void CModelReaderNode093_Color::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		// Parse Color
		parseColor();
	}

	nfColor CModelReaderNode093_Color::retrieveColor()
	{
		return m_cColor;
	}

	void CModelReaderNode093_Color::parseColor()
	{
		m_cColor = 0;

		try {
			if (m_sColorString.length() > 0) {
				nfChar bFirstChar = m_sColorString[0];
				if (bFirstChar == '#') {
					if (fnStringToSRGBColor(m_sColorString.c_str(), m_cColor)) {
						return;
					}
				}

				if (bFirstChar == 's') {
					std::string sSubStr = m_sColorString.substr(0, 5);
					if (sSubStr == "scRGB") {
						// parse scRGB String
						throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
					}
				}

				if (bFirstChar == L't') {
					std::string sSubStr = m_sColorString.substr(0, 4);
					if (sSubStr == "tex(") {
						// parse Texture String

						if (m_sColorString.length() > 4) {
							std::string sTexID = m_sColorString.substr(4, m_sColorString.length() - sSubStr.length() - 1);
							nfInt32 nValue = fnStringToInt32(sTexID.c_str());
							if ((nValue < 0) || (nValue >= XML_3MF_MAXRESOURCEINDEX))
								throw CNMRException(NMR_ERROR_INVALIDTEXTUREREFERENCE);

							m_nTextureID = nValue + 1;
							return;
						}
						else {
							throw CNMRException(NMR_ERROR_INVALIDTEXTUREREFERENCE);
						}
					}
				}

				if (bFirstChar == 'C') {
					std::string sSubStr4 = m_sColorString.substr(0, 4);
					if (sSubStr4 == "CMYK") {
						throw CNMRException(NMR_ERROR_NOTSUPPORTINGLEGACYCMYK);
					}
				}

			}

			throw CNMRException(NMR_ERROR_INVALIDVALUEINCOLORSTRING);
		}
		catch (CNMRException & Exception) {
			m_pWarnings->addException(Exception, mrwMissingMandatoryValue);
		}

	}


	ModelResourceID CModelReaderNode093_Color::retrieveID()
	{
		try {
			//if (m_nUniqueResourceID == 0)
				//throw CNMRException(NMR_ERROR_MISSINGMODELCOLORID);
		}
		catch (CNMRException & Exception) {
			m_pWarnings->addException(Exception, mrwMissingMandatoryValue);
		}

		return m_nResourceID;
	}

	void CModelReaderNode093_Color::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COLOR_VALUE) == 0) {
			m_sColorString = pAttributeValue;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COLOR_ID) == 0) {
			if (m_nResourceID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATECOLORID);

			// Convert to integer and make a input and range check!
			m_nResourceID = fnStringToUint32(pAttributeValue);
			m_nResourceID++;
		}

	}

	ModelResourceID CModelReaderNode093_Color::retrieveTextureID()
	{
		return m_nTextureID;
	}

}
