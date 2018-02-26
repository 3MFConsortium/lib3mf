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

NMR_ModelReaderNode100_Colors.cpp implements the Model Reader Color Group Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_Tex2DGroup.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Tex2Coord.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_Tex2DGroup::CModelReaderNode100_Tex2DGroup(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, _In_ PModelReader_TexCoordMapping pTexCoordMapping)
		: CModelReaderNode(pWarnings)
	{
		if (!pTexCoordMapping.get())
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		// Initialize variables
		m_nID = 0;
		m_nTextureID = 0;
		m_nTexCoordIndex = 0;

		m_pModel = pModel;
		m_pTexCoordMapping = pTexCoordMapping;
	}

	void CModelReaderNode100_Tex2DGroup::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode100_Tex2DGroup::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TEX2DGROUP_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnWStringToUint32(pAttributeValue);
		}
		else if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_TEX2DGROUP_TEXTUREID) == 0) {
			if (m_nTextureID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			ModelResourceID nID = fnWStringToUint32(pAttributeValue);
			if (nID == 0)
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDMODELRESOURCE), mrwInvalidMandatoryValue);

			m_nTextureID = nID;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}


	void CModelReaderNode100_Tex2DGroup::OnNSChildElement(_In_z_ const nfWChar * pChildName, _In_z_ const nfWChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (wcscmp(pNameSpace, XML_3MF_NAMESPACE_MATERIALSPEC) == 0) {
			if (wcscmp(pChildName, XML_3MF_ELEMENT_TEX2COORD) == 0) {
				PModelReaderNode100_Tex2Coord pXMLNode = std::make_shared<CModelReaderNode100_Tex2Coord>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				m_pTexCoordMapping->registerTexCoords(m_nID, m_nTexCoordIndex, m_nTextureID, pXMLNode->getU(), pXMLNode->getV());
				m_nTexCoordIndex++;

				if (m_nTexCoordIndex > XML_3MF_MAXRESOURCEINDEX)
					throw CNMRException(NMR_ERROR_INVALIDINDEX);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
