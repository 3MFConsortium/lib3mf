
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

NMR_ModelReaderNode_KeyStore.h defines the Model Reader Node class that is related to <keystore>.

--*/

#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStore.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreConsumer.h"
#include "Model/Reader/SecureContent085/NMR_ModelReaderNode_KeyStoreResourceData.h"

#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode_KeyStore::CModelReaderNode_KeyStore(CKeyStore * pKeyStore, PModelReaderWarnings pWarnings)
		: CModelReaderNode_KeyStoreBase(pKeyStore, pWarnings)
	{

	}

	void CModelReaderNode_KeyStore::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Parse Content
		parseContent(pXMLReader);

		/*if (!m_bHasID)
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
		*/

		// TODO: check consumerindex

		// TODO: a path must not show up in more than one resourcedata element

		// Set references
		if (!m_UUID.get()) {
			// We do not have to check for secure content spec, because it is the base spec of a keystore
			m_pWarnings->addException(CNMRException(NMR_ERROR_MISSINGUUID), mrwMissingMandatoryValue);
			m_UUID = std::make_shared<CUUID>();
		}
		m_pKeyStore->setUUID(m_UUID);
		//pBuildItem->setPath(m_sPath);
	}

	void CModelReaderNode_KeyStore::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(XML_3MF_SECURE_CONTENT_UUID, pAttributeName) == 0) {
			if (m_UUID.get())
				m_pWarnings->addException(CNMRException(NMR_ERROR_DUPLICATEUUID), eModelReaderWarningLevel::mrwInvalidMandatoryValue);
			m_UUID = std::make_shared<CUUID>(pAttributeValue);
		}
	}

	void CModelReaderNode_KeyStore::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);
		__NMRASSERT(pNameSpace);
	}

	void CModelReaderNode_KeyStore::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pChildName);
		__NMRASSERT(pXMLReader);
		__NMRASSERT(pNameSpace);

		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_SECURECONTENTSPEC) == 0) {
			// Read a consumer
			if (strcmp(pChildName, XML_3MF_ELEMENT_CONSUMER) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_KeyStoreConsumer>(m_pKeyStore, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			} else if (strcmp(pChildName, XML_3MF_ELEMENT_RESOURCEDATA) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode_KeyStoreResourceData>(m_pKeyStore, m_pWarnings);
				pXMLNode->parseXML(pXMLReader);
			} else {
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
	}

}
