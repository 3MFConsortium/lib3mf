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

NMR_ModelReaderNode100_MultiProperties.cpp implements the Model Reader MultiProperties Class.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_MultiProperties.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_Multi.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_Model.h"

#include "Common/NMR_StringUtils.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode100_MultiProperties::CModelReaderNode100_MultiProperties(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		// Initialize variables
		m_nID = 0;

		m_pModel = pModel;
	}

	void CModelReaderNode100_MultiProperties::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Use parameter and assign to model Object
		if (m_nID == 0)
			throw CNMRException(NMR_ERROR_MISSINGMODELRESOURCEID);

		std::vector<MODELMULTIPROPERTYLAYER> vctLayers;

		if (m_pPIDs) {
			if (!m_pBlendMethods) {
				m_pBlendMethods = std::make_shared<std::vector<eModelBlendMethod>>();
				m_pBlendMethods->resize(m_pPIDs->size() - 1);
				for (size_t i = 0; i < m_pBlendMethods->size(); i++) {
					(*m_pBlendMethods)[i] = eModelBlendMethod::MODELBLENDMETHOD_MIX;
				}
			}

			if (m_pBlendMethods->size() != m_pPIDs->size() - 1) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_MULTIPROPERTIES_DIFFERNT_NUMBER_OF_BLENDMETHODS_AND_PIDS), mrwInvalidMandatoryValue);
			}

			for (size_t i = 0; i < m_pPIDs->size(); i++) {
				PPackageResourceID pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), (*m_pPIDs)[i]);
				if (!pID)
					throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);

				eModelBlendMethod method = eModelBlendMethod::MODELBLENDMETHOD_MIX;
				if (m_pBlendMethods && (i > 0) && (i < m_pBlendMethods->size() + 1 )) {
					method = (*m_pBlendMethods)[i-1];
				}
				vctLayers.push_back(MODELMULTIPROPERTYLAYER({ pID->getUniqueID() , method }));
			}
		}
		else {
			{
				m_pWarnings->addException(CNMRException(NMR_ERROR_MULTIPROPERTIES_MISSING_PIDS), mrwInvalidMandatoryValue);
			}
		}
		
		m_pMultiPropertyGroup = std::make_shared<CModelMultiPropertyGroupResource>(m_nID, m_pModel);
		m_pModel->addResource(m_pMultiPropertyGroup);
		
		for (auto layer : vctLayers) {
			PModelResource pResource = m_pModel->findResource(layer.m_nUniqueResourceID);
			if (!pResource) {
				throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
			}
			pResource->buildResourceIndexMap();

			m_pMultiPropertyGroup->addLayer(layer);
		}

		// Parse Content
		parseContent(pXMLReader);
	}

	void CModelReaderNode100_MultiProperties::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MULTIPROPERTIES_ID) == 0) {
			if (m_nID != 0)
				throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

			// Convert to integer and make a input and range check!
			m_nID = fnStringToUint32(pAttributeValue);
		} else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MULTIPROPERTIES_PIDS) == 0) {
			if (m_pPIDs)
				throw CNMRException(NMR_ERROR_DUPLICATE_PIDS);

			m_pPIDs = std::make_shared<std::vector<ModelResourceID>>(fnVctType_fromString<ModelResourceID>(std::string(pAttributeValue)));
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MULTIPROPERTIES_BLENDMETHODS) == 0) {
			if (m_pBlendMethods)
				throw CNMRException(NMR_ERROR_DUPLICATE_BLENDMETHOS);

			std::vector<std::string> vctBlendMethodStrings = fnVctType_fromString<std::string>(std::string(pAttributeValue));

			m_pBlendMethods = std::make_shared<std::vector<eModelBlendMethod>>();
			m_pBlendMethods->reserve(vctBlendMethodStrings.size());
			for (std::string blendMethodString : vctBlendMethodStrings) {
				eModelBlendMethod method = MODELBLENDMETHOD_MIX;
				try {
					method = CModelMultiPropertyGroupResource::stringToBlendMethod(blendMethodString);
				} catch (CNMRException e) {
					m_pWarnings->addException(e, mrwInvalidMandatoryValue);
				}
				m_pBlendMethods->push_back(method);
			}

		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}


	void CModelReaderNode100_MultiProperties::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_MATERIALSPEC) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_MULTI) == 0) {
				PModelReaderNode100_Multi pXMLNode = std::make_shared<CModelReaderNode100_Multi>(m_pModel, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::vector<ModelResourceIndex> vctPIndices =  pXMLNode->getPIndices();

				nfUint32 nLayers = m_pMultiPropertyGroup->getLayerCount();

				while (vctPIndices.size() < nLayers) {
					vctPIndices.push_back(0);
				}
				PModelMultiProperty pMultiProperty = std::make_shared<CModelMultiProperty>(nLayers);

				for (nfUint32 iLayer=0; iLayer < nLayers; iLayer++) {
					PModelResource pResource = m_pModel->findResource(m_pMultiPropertyGroup->getLayer(iLayer).m_nUniqueResourceID);
					if (!pResource) {
						throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
					}
					ModelPropertyID nPropertyID;
					if (!pResource->mapResourceIndexToPropertyID(vctPIndices[iLayer], nPropertyID)) {
						throw CNMRException(NMR_ERROR_INVALID_RESOURCE_INDEX);
					}
					(*pMultiProperty)[iLayer] = nPropertyID;
				}

				m_pMultiPropertyGroup->addMultiProperty(pMultiProperty);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
	}

}
