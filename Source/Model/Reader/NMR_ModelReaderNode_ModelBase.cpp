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

NMR_ModelReaderNode_ModelBase.cpp implements the Model Reader Node Class.
A model reader node is an abstract base class for all XML nodes of a 3MF Model Stream.

--*/

#include "Model/Reader/NMR_ModelReaderNode_ModelBase.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_Resources.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_Build.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h" 

#include "Model/Reader/v093/NMR_ModelReaderNode093_Resources.h" 
#include "Model/Reader/v093/NMR_ModelReaderNode093_Build.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CModelReaderNode_ModelBase::CModelReaderNode_ModelBase(_In_ CModel * pModel, _In_ PModelWarnings pWarnings, const std::string sPath,
		_In_ PProgressMonitor pProgressMonitor)
		: CModelReaderNode(pWarnings, pProgressMonitor), m_bIgnoreBuild(false), m_bIgnoreMetaData(false), m_bHaveWarnedAboutV093(false)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_sRequiredExtensions = "";

		m_pModel->setLanguage("und");

		m_bHasResources = false;
		m_bHasBuild = false;
		m_bWithinIgnoredElement = false;

		m_sPath = sPath;
	}
	
	void CModelReaderNode_ModelBase::CheckRequiredExtensions() {
		// check, whether all required extensions are available in this implementation before actually parsing the file
		std::istringstream iss(m_sRequiredExtensions);
		std::vector<std::string> tokens{ std::istream_iterator<std::string,  char>{iss},
			std::istream_iterator<std::string,  char>{} };
		for (auto token : tokens) {
			// is the extension listed?
			if (m_ListedExtensions.count(token) < 1) {
				throw CNMRException(NMR_ERROR_INVALIDREQUIREDEXTENSIONPREFIX);
			}
			// is the extension supported?
			std::string sExtensionURI = m_ListedExtensions[token];
			if (strcmp(sExtensionURI.c_str(), PACKAGE_XMLNS_100) != 0 &&
				strcmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_MATERIALSPEC) != 0 &&
				strcmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_PRODUCTIONSPEC) != 0 &&
				strcmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_SLICESPEC) != 0 &&
				strcmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_BEAMLATTICESPEC) != 0 &&
				strcmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_SECURECONTENTSPEC) != 0 )
			{
				m_pWarnings->addWarning(NMR_ERROR_REQUIREDEXTENSIONNOTSUPPORTED, mrwInvalidMandatoryValue);
			}
		}
	}

	void CModelReaderNode_ModelBase::parseXML(_In_ CXmlReader * pXMLReader)
	{
		// Parse name
		parseName(pXMLReader);

		// Parse attribute
		parseAttributes(pXMLReader);

		// Check required extensions
		CheckRequiredExtensions();

		// Parse Content
		parseContent(pXMLReader);

	}

	void CModelReaderNode_ModelBase::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_UNIT) == 0) {

			// set unit string  and set validity
			try {
				m_pModel->setUnitString(pAttributeValue);
			}
			catch (CNMRException & e) {
				m_pWarnings->addException(e, mrwInvalidMandatoryValue);
			}
		}
		else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS) == 0) {
			m_sRequiredExtensions = std::string(pAttributeValue);
		}
	}

	void CModelReaderNode_ModelBase::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_XML) == 0) {
			if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_LANG) == 0) {
				m_pModel->setLanguage(std::string(pAttributeValue));
			}
			else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_SPACE) == 0)
			{
				throw CNMRException(NMR_ERROR_FORBIDDENXMLATTRITIBUTE);
			}
			// lax handling here for other XML_3MF_NAMESPACE_XML-attributes
		}
		else if (strcmp(pNameSpace, XML_3MF_NAMESPACE_XMLNS) == 0) {
			m_ListedExtensions.insert(std::pair<std::string, std::string>(std::string(pAttributeName), std::string(pAttributeValue)));
		}
	}

	void CModelReaderNode_ModelBase::ReadMetaDataNode(_In_ CXmlReader * pXMLReader)
	{
		PModelReaderNode100_MetaData pXMLNode = std::make_shared<CModelReaderNode100_MetaData>(m_pWarnings);
		pXMLNode->parseXML(pXMLReader);

		std::string sKey = pXMLNode->getKey();
		std::string sValue = pXMLNode->getValue();
		std::string sType = pXMLNode->getType();
		nfBool bPreserve = pXMLNode->getPreserve();

		if (!sKey.empty()) {
			if (m_pModel->hasMetaData(sKey)) {
				m_pWarnings->addWarning(NMR_ERROR_DUPLICATEMETADATA, mrwInvalidOptionalValue);
			}
			else {
				std::string sNameSpace, sName;
				decomposeKeyIntoNamespaceAndName(sKey, sNameSpace, sName);
				if (!sNameSpace.empty()) {
					std::string sNameSpaceURI;
					if (!pXMLReader->GetNamespaceURI(sNameSpace, sNameSpaceURI)) {
						m_pWarnings->addException(CNMRException(NMR_ERROR_METADATA_COULDNOTGETNAMESPACE), mrwInvalidOptionalValue);
						sNameSpaceURI = sNameSpace;
					}
					m_pModel->addMetaData(sNameSpaceURI, sName, sValue, sType, bPreserve);
				}
				else {
					// default namespace
					if (CModelMetaData::isValidNamespaceAndName("", sName)) {
						m_pModel->addMetaData("", sName, sValue, sType, bPreserve);
					}
					else
						m_pWarnings->addException(CNMRException(NMR_ERROR_UNKNOWNMETADATA), mrwInvalidOptionalValue);
				}
			}
		}
		else {
			m_pWarnings->addWarning(NMR_ERROR_INVALIDMETADATA, mrwInvalidOptionalValue);
		}
	}

	void CModelReaderNode_ModelBase::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (strcmp(pChildName, XML_3MF_ELEMENT_RESOURCES) == 0) {
				m_bWithinIgnoredElement = false;

				m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READRESOURCES);
				m_pProgressMonitor->ReportProgressAndQueryCancelled(true);
				
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Resources>(m_pModel, m_pWarnings, m_sPath.c_str(), m_pProgressMonitor);
				if (m_bHasResources)
					throw CNMRException(NMR_ERROR_DUPLICATERESOURCES);
				pXMLNode->parseXML(pXMLReader);
				m_bHasResources = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BUILD) == 0) {
				if (m_bHasBuild)
					throw CNMRException(NMR_ERROR_DUPLICATEBUILDSECTION);
				if (m_bIgnoreBuild) {
					m_bWithinIgnoredElement = true;
				}
				else {
					m_bWithinIgnoredElement = false;
					m_pProgressMonitor->SetProgressIdentifier(ProgressIdentifier::PROGRESS_READBUILD);
					m_pProgressMonitor->ReportProgressAndQueryCancelled(true);

					PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Build>(m_pModel, m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
				}
				m_bHasBuild = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_METADATA) == 0) {
				if (m_bIgnoreMetaData) {
					m_bWithinIgnoredElement = true;
				} else {
					m_bWithinIgnoredElement = false;
					ReadMetaDataNode(pXMLReader);
				}
			}
			else {
				if (!m_bWithinIgnoredElement)
					m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}
		else if ((strcmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (strcmp(pNameSpace, "") == 0)) {
			
			if (!m_bHaveWarnedAboutV093) {
				m_pWarnings->addException(CNMRException(NMR_ERROR_VERSION093_NOT_SUPPORTED), mrwInvalidOptionalValue);
				m_bHaveWarnedAboutV093 = true;
			}
			

			if (strcmp(pChildName, XML_3MF_ELEMENT_RESOURCES) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Resources>(m_pModel, m_pWarnings);
				if (m_bHasResources)
					throw CNMRException(NMR_ERROR_DUPLICATERESOURCES);

				pXMLNode->parseXML(pXMLReader);
				m_bHasResources = true;
			}
			else if (strcmp(pChildName, XML_3MF_ELEMENT_BUILD) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Build>(m_pModel, m_pWarnings);
				if (m_bHasBuild)
					throw CNMRException(NMR_ERROR_DUPLICATEBUILDSECTION);

				pXMLNode->parseXML(pXMLReader);
				m_bHasBuild = true;
			}
			else if ((strcmp(pChildName, XML_3MF_ELEMENT_METADATA) == 0) || (strcmp(pChildName, XML_3MF_ELEMENT_METADATA_ENRTY) == 0)) {
				ReadMetaDataNode(pXMLReader);
			}
			else
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}
		else {
			// ignore
		}
	}

	nfBool CModelReaderNode_ModelBase::getHasResources()
	{
		return m_bHasResources;
	}

	nfBool CModelReaderNode_ModelBase::getHasBuild()
	{
		return m_bHasBuild;
	}

	nfBool CModelReaderNode_ModelBase::ignoreBuild()
	{
		return m_bIgnoreBuild;
	}
	void CModelReaderNode_ModelBase::setIgnoreBuild(bool bIgnoreBuild)
	{
		m_bIgnoreBuild = bIgnoreBuild;
	}

	nfBool CModelReaderNode_ModelBase::ignoreMetaData()
	{
		return m_bIgnoreMetaData;
	}
	void CModelReaderNode_ModelBase::setIgnoreMetaData(bool bIgnoreMetaData)
	{
		m_bIgnoreMetaData = bIgnoreMetaData;
	}

}
