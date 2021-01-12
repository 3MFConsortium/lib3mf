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

NMR_ModelReaderNode100_BuildItem.cpp implements the Model Reader BuildItem Node Class.
A builditem reader model node is a parser for the builditem node of an XML Model Stream.

--*/

#include "Model/Reader/v100/NMR_ModelReaderNode100_BuildItem.h"
#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaDataGroup.h"
#include "Model/Classes/NMR_ModelBuildItem.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode100_BuildItem::CModelReaderNode100_BuildItem(_In_ CModel * pModel, _In_ PModelWarnings pWarnings)
		: CModelReaderNode(pWarnings)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_ObjectID = 0;
		m_bHasID = false;
		m_sPartNumber = "";

		m_hasPath = false;

		m_mTransform = fnMATRIX3_identity();
	}

	void CModelReaderNode100_BuildItem::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		if (!m_bHasID)
			throw CNMRException(NMR_ERROR_MISSINGBUILDITEMOBJECTID);

		CModelObject * pObject;
		PPackageResourceID pID;
		if (m_hasPath) {
			if (m_pModel->currentPath() != m_pModel->rootPath() )
				throw CNMRException(NMR_ERROR_REFERENCESTOODEEP);
			pID = m_pModel->findPackageResourceID(m_sPath, m_ObjectID);
		}
		else
			pID = m_pModel->findPackageResourceID(m_pModel->currentPath(), m_ObjectID);
		if (!pID)
			throw CNMRException(NMR_ERROR_COULDNOTFINDBUILDITEMOBJECT);

		pObject = m_pModel->findObject(pID->getUniqueID());
		if (!pObject)
			throw CNMRException(NMR_ERROR_COULDNOTFINDBUILDITEMOBJECT);

		if (MODELOBJECTTYPE_OTHER == pObject->getObjectType()) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_BUILDITEMOBJECT_MUSTNOTBE_OTHER), mrwInvalidMandatoryValue);
		}

		// Create Build Item
		PModelBuildItem pBuildItem = std::make_shared<CModelBuildItem>(pObject, m_mTransform, m_pModel->createHandle());
		if (!pBuildItem->isValidForSlices()) {
			m_pWarnings->addException(CNMRException(NMR_ERROR_SLICETRANSFORMATIONPLANAR), mrwInvalidMandatoryValue);
		}
		
		if (m_MetaDataGroup.get()) {
			pBuildItem->metaDataGroup()->mergeMetaData(m_MetaDataGroup.get());
		}

		m_pModel->addBuildItem(pBuildItem);

		// Set Item Reference
		pBuildItem->setPartNumber(m_sPartNumber);

		// Set Production references
		if (!m_UUID.get()) {
			if (pXMLReader->NamespaceRegistered(XML_3MF_NAMESPACE_PRODUCTIONSPEC)) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGUUID), mrwMissingMandatoryValue);
			}
			m_UUID = std::make_shared<CUUID>();
		}
		pBuildItem->setUUID(m_UUID);
		pBuildItem->setPath(m_sPath);
	}

	void CModelReaderNode100_BuildItem::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_ITEM_OBJECTID) == 0) {
			if (m_bHasID)
				throw CNMRException(NMR_ERROR_DUPLICATEBUILDITEMOBJECTID);

			m_ObjectID = fnStringToUint32(pAttributeValue);
			m_bHasID = true;
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_ITEM_TRANSFORM) == 0) {
			m_mTransform = fnMATRIX3_fromString(pAttributeValue);
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_ITEM_PARTNUMBER) == 0) {
			m_sPartNumber = std::string(pAttributeValue);
		}
		else
			m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE), mrwInvalidOptionalValue);
	}

	void CModelReaderNode100_BuildItem::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_PRODUCTIONSPEC) == 0) {
			if (strcmp(pAttributeName, XML_3MF_PRODUCTION_UUID) == 0) {
				if (m_UUID.get())
					throw CNMRException(NMR_ERROR_DUPLICATEUUID);
				m_UUID = std::make_shared<CUUID>(pAttributeValue);
			}
			if (strcmp(pAttributeName, XML_3MF_PRODUCTION_PATH) == 0) {
				if (m_hasPath)
					throw CNMRException(NMR_ERROR_DUPLICATEPATH);
				m_sPath = std::string(pAttributeValue);
				if (!fnStartsWithPathDelimiter(m_sPath)) {
					m_pWarnings->addException(CNMRException(NMR_ERROR_PATH_NOT_ABSOLUTE), mrwInvalidOptionalValue);
				}
				m_hasPath = true;
			}
		}
	}

	void CModelReaderNode100_BuildItem::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			// Read a metadatagroup object
			if (strcmp(pChildName, XML_3MF_ELEMENT_METADATAGROUP) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_MetaDataGroup>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				if (m_MetaDataGroup.get()) {
					m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATEMETADATAGROUP), mrwInvalidOptionalValue);
				}
				m_MetaDataGroup = dynamic_cast<CModelReaderNode100_MetaDataGroup*>(pXMLNode.get())->getMetaDataGroup();
			}
			else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
	}

}
