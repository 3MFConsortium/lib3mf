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

NMR_ModelReaderNode100_CompositeMaterials.cpp implements the Model Reader Composite Materials Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_CompositeMaterials.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Composite.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_CompositeMaterials::CModelReaderNode100_CompositeMaterials(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialize variables
		m_nID = 0;
		m_nBaseMaterialID = 0;

		m_pModel = pModel;
	}

	void CModelReaderNode100_CompositeMaterials::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		if (m_nBaseMaterialID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), m_nBaseMaterialID);
		if (!pID)
			throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);

		PModelBaseMaterialResource pBaseMaterial = m_pModel->findBaseMaterial(pID);

		// Create Resource
 		m_pCompositeMaterials = std::make_shared<CModelCompositeMaterialsResource>(m_nID, m_pModel, pBaseMaterial);
		
		if (m_pMatIndices) {
			pBaseMaterial->buildResourceIndexMap();

			if (m_pMatIndices->size() > XML_3MF_MAXRESOURCECOUNT)
				throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);


			m_vctMaterialPropertyIDs.resize(m_pMatIndices->size());
			for (size_t i = 0; i < m_vctMaterialPropertyIDs.size(); i++) {
				ModelPropertyID nPropertyID;
				if (!pBaseMaterial->mapResourceIndexToPropertyID((ModelResourceIndex)i, nPropertyID)) {
					throw CNMRException(NMR_ERROR_INVALIDBASEMATERIAL);
				}
				m_vctMaterialPropertyIDs[i] = nPropertyID;
			}
		}

		m_pModel->addResource(m_pCompositeMaterials);

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode100_CompositeMaterials::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATID) == 0) {
			if (m_nBaseMaterialID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			ModelResourceID nID = fnStringToUint32(pAttributeValue);
			if (nID == 0)
				m_pWarnings->addException(CNMRException(NMR_ERROR_INVALIDMODELRESOURCE), mrwInvalidMandatoryValue);

			m_nBaseMaterialID = nID;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATINDICES) == 0) {
			if (m_pMatIndices) {
				throw CNMRException(NMR_ERROR_DUPLICATE_MATINDICES_ATTRIBUTE);
			}
			
			m_pMatIndices = std::make_shared<std::vector<nfUint32>>( fnVctType_fromString<nfUint32>(std::string(pAttributeValue)) ) ;
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}


	void CModelReaderNode100_CompositeMaterials::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_MATERIALSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_COMPOSITE) == 0) {
				PModelReaderNode100_Composite pXMLNode = std::make_shared<CModelReaderNode100_Composite>(m_pModel, m_pWarnings, m_vctMaterialPropertyIDs);
				pXMLNode->parseXML(pXMLReader);

				PModelComposite pComposite = pXMLNode->getComposite();

				m_pCompositeMaterials->addComposite(pComposite);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
