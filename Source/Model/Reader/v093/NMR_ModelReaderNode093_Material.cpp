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
NMR_ModelReaderNode093_Material.cpp implements the Model Reader Material Node Class.
A material reader model node is a parser for the material node of an XML Model Stream.

--*/

#include "Model/Reader/v093/NMR_ModelReaderNode093_Material.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include <cmath>
#include <climits>

namespace NMR {

	CModelReaderNode093_Material::CModelReaderNode093_Material(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		m_nResourceID = 0;
		m_nColorID = 0;
		m_sName = "material";
	}

	void CModelReaderNode093_Material::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}

	std::string CModelReaderNode093_Material::retrieveName()
	{
		return m_sName;
	}
			
	ModelResourceID CModelReaderNode093_Material::retrieveColorID()
	{
		return m_nColorID;
	}
	

	ModelResourceID CModelReaderNode093_Material::retrieveID()
	{
		try {
			if (m_nResourceID == 0)
				throw CNMRException(NMR_ERROR_MISSINGMODELMATERIALID);
		}
		catch (CNMRException & Exception) {
			m_pWarnings->addException(Exception, mrwMissingMandatoryValue);
		}

		return m_nResourceID;
	}

	
	
	void CModelReaderNode093_Material::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MATERIAL_ID) == 0) {
			if (m_nResourceID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATEMATERIALID);

			// Convert to integer and make a input and range check!
			m_nResourceID = fnStringToUint32(pAttributeValue);
			m_nResourceID++;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MATERIAL_COLORID) == 0) {
			// Convert to integer and make a input and range check!
			m_nColorID = fnStringToUint32(pAttributeValue);
			m_nColorID++;
		}

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MATERIAL_NAME) == 0) {
			// Convert to integer and make a input and range check!
			m_sName = pAttributeValue;
		}

	}


}

