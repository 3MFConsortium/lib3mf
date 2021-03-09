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

NMR_ModelReaderNode100_BaseMaterials.cpp implements the Model Reader Base Materials Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterials.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_BaseMaterial.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_BaseMaterials::CModelReaderNode100_BaseMaterials(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialize variables
		m_nID = 0;

		m_pModel = pModel;
	}

	void CModelReaderNode100_BaseMaterials::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		// Create Resource
		m_pBaseMaterialResource = std::make_shared<CModelBaseMaterialResource> (m_nID, m_pModel);

		m_pModel->addResource(m_pBaseMaterialResource);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode100_BaseMaterials::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BASEMATERIALS_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}

	}

	void CModelReaderNode100_BaseMaterials::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_BASE) == 0) {
				PModelReaderNode100_BaseMaterial pXMLNode = std::make_shared<CModelReaderNode100_BaseMaterial>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (m_pBaseMaterialResource.get() != nullptr) {
					m_pBaseMaterialResource->addBaseMaterial(pXMLNode->getMaterialName(), pXMLNode->getDisplayColor());
				}
			}
		}
	}

}
