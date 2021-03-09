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

NMR_ModelReaderNode100_BaseMaterial.cpp implements the Model Reader Base Material Node Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterial.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_BaseMaterial::CModelReaderNode100_BaseMaterial(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{

		m_pModel = pModel;
		m_bHasName = false;
	    m_bHasDisplayColor = false;
		m_cDisplayColor = 0xffffffff;
	}

	void CModelReaderNode100_BaseMaterial::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode100_BaseMaterial::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BASEMATERIAL_NAME) == 0) {
			m_sName = pAttributeValue;
			m_bHasName = true;
		}
		
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BASEMATERIAL_DISPLAYCOLOR) == 0) {
			// Convert to color and make a input and range check!
			m_bHasDisplayColor = fnStringToSRGBColor(pAttributeValue, m_cDisplayColor);
		}
	}


	nfColor CModelReaderNode100_BaseMaterial::getDisplayColor()
	{
		return m_cDisplayColor;
	}

	std::string CModelReaderNode100_BaseMaterial::getMaterialName()
	{
		return m_sName;
	}

	nfBool CModelReaderNode100_BaseMaterial::hasDisplayColor()
	{
		return m_bHasDisplayColor;
	}

	nfBool CModelReaderNode100_BaseMaterial::hasName()
	{
		return m_bHasName;
	}

}
