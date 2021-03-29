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
NMR_ModelReaderNode_Volumetric1907_Image3DSheet.cpp covers the official 3MF volumetric extension.

--*/

#include "Model/Reader/Volumetric1907/NMR_ModelReaderNode_Volumetric1907_Image3DSheet.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {


	CModelReaderNode_Volumetric1907_Image3DSheet::CModelReaderNode_Volumetric1907_Image3DSheet(_In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)		
		
	{
	}

	void CModelReaderNode_Volumetric1907_Image3DSheet::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);
	}
	
	
	void CModelReaderNode_Volumetric1907_Image3DSheet::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3DSHEET_PATH) == 0) {
			m_sPath = pAttributeValue;
		}
    else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3DSHEET_MIN_VAL) == 0) {
      m_minValue = std::stod(pAttributeValue);
    }
    else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMAGE3DSHEET_MAX_VAL) == 0) {
      m_maxValue = std::stod(pAttributeValue);
    }
	}

	std::string CModelReaderNode_Volumetric1907_Image3DSheet::getPath()
	{	
		return m_sPath;
	}

  nfDouble CModelReaderNode_Volumetric1907_Image3DSheet::getMinValue()
  {
    return m_minValue;
  }

  nfDouble CModelReaderNode_Volumetric1907_Image3DSheet::getMaxValue()
  {
    return m_maxValue;
  }

}
