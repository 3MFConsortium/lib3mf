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

NMR_ModelReaderNode_Model.cpp implements the Model Reader Node Class.
A model reader node is an abstract base class for all XML nodes of a 3MF Model Stream.

--*/

#include "Model/Reader/NMR_ModelReaderNode_Model.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_Resources.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_Build.h" 
#include "Model/Reader/v100/NMR_ModelReaderNode100_MetaData.h" 

#include "Model/Reader/v093/NMR_ModelReaderNode093_Resources.h" 
#include "Model/Reader/v093/NMR_ModelReaderNode093_Build.h" 

#include "Model/Classes/NMR_ModelConstants.h" 
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace NMR {

	CModelReaderNode_Model::CModelReaderNode_Model(_In_ CModel * pModel, _In_ PModelReaderWarnings pWarnings, const nfWChar* sPath)
		: CModelReaderNode(pWarnings), m_bIgnoreBuild(false)
	{
		__NMRASSERT(pModel);
		m_pModel = pModel;
		m_sRequiredExtensions = L"";

		m_pModel->setLanguage(L"und");

		m_bHasResources = false;
		m_bHasBuild = false;
		m_bWithinIgnoredBuild = false;

		m_sPath = sPath;
	}
	
	void CModelReaderNode_Model::CheckRequiredExtensions() {
		// check, whether all required extensions are available in this implementation before actually parsing the file
		std::wistringstream iss(m_sRequiredExtensions);
		std::vector<std::wstring> tokens{ std::istream_iterator<std::wstring,  wchar_t>{iss},
			std::istream_iterator<std::wstring,  wchar_t>{} };
		for (auto token : tokens) {
			// is the extension listed?
			if (m_ListedExtensions.count(token) < 1) {
				throw CNMRException(NMR_ERROR_INVALIDREQUIREDEXTENSIONPREFIX);
			}
			// is the extension supported?
			std::wstring sExtensionURI = m_ListedExtensions[token];
			if (wcscmp(sExtensionURI.c_str(), PACKAGE_XMLNS_100) != 0 &&
				wcscmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_MATERIALSPEC) != 0 &&
				wcscmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_PRODUCTIONSPEC) != 0 &&
				wcscmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_SLICESPEC) != 0 &&
				wcscmp(sExtensionURI.c_str(), XML_3MF_NAMESPACE_BEAMLATTICESPEC) != 0 )
			{
				m_pWarnings->addWarning(MODELREADERWARNING_REQUIREDEXTENSIONNOTSUPPORTED, NMR_ERROR_REQUIREDEXTENSIONNOTSUPPORTED, mrwInvalidMandatoryValue);
			}
		}
	}

	void CModelReaderNode_Model::parseXML(_In_ CXmlReader * pXMLReader)
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

	void CModelReaderNode_Model::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		__NMRASSERT(pAttributeName);
		__NMRASSERT(pAttributeValue);

		if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_UNIT) == 0) {

			// set unit string  and set validity
			try {
				m_pModel->setUnitString(pAttributeValue);
			}
			catch (CNMRException & e) {
				m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMODELUNIT, e.getErrorCode(), mrwInvalidMandatoryValue);
			}
		}
		else if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS) == 0) {
			m_sRequiredExtensions = std::wstring(pAttributeValue);
		}
	}

	void CModelReaderNode_Model::OnNSAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue, _In_z_ const nfWChar * pNameSpace)
	{
		if (wcscmp(pNameSpace, XML_3MF_NAMESPACE_XML) == 0) {
			if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_LANG) == 0) {
				m_pModel->setLanguage(std::wstring(pAttributeValue));
			}
			else if (wcscmp(pAttributeName, XML_3MF_ATTRIBUTE_MODEL_SPACE) == 0)
			{
				throw CNMRException(NMR_ERROR_FORBIDDENXMLATTRITIBUTE);
			}
			// lax handling here for other XML_3MF_NAMESPACE_XML-attributes
		}
		else if (wcscmp(pNameSpace, XML_3MF_NAMESPACE_XMLNS) == 0) {
			m_ListedExtensions.insert(std::pair<std::wstring, std::wstring>(std::wstring(pAttributeName), std::wstring(pAttributeValue)));
		}
	}

	bool decomposeIntoNamespaceAndName(const std::wstring &sIn, std::wstring &sNameSpace, std::wstring &sName) {
		size_t cInd = sIn.find(L":");
		if (cInd != std::wstring::npos) {
			sNameSpace = sIn.substr(0, cInd);
			sName = sIn.substr(cInd+1, sIn.length() - cInd);
			return true;
		}
		return false;
	}

	void CModelReaderNode_Model::OnNSChildElement(_In_z_ const nfWChar * pChildName, _In_z_ const nfWChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		if (wcscmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC100) == 0) {
			if (wcscmp(pChildName, XML_3MF_ELEMENT_RESOURCES) == 0) {
				m_bWithinIgnoredBuild = false;
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Resources>(m_pModel, m_pWarnings, m_sPath.c_str());
				if (m_bHasResources)
					throw CNMRException(NMR_ERROR_DUPLICATERESOURCES);

				pXMLNode->parseXML(pXMLReader);
				m_bHasResources = true;

			}
			else if (wcscmp(pChildName, XML_3MF_ELEMENT_BUILD) == 0) {
				if (m_bHasBuild)
					throw CNMRException(NMR_ERROR_DUPLICATEBUILDSECTION);
				if (!m_bIgnoreBuild) {
					m_bWithinIgnoredBuild = false;
					PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode100_Build>(m_pModel, m_pWarnings);
					pXMLNode->parseXML(pXMLReader);
				}
				else {
					m_bWithinIgnoredBuild = true;
				}
				m_bHasBuild = true;
			}
			else if (wcscmp(pChildName, XML_3MF_ELEMENT_METADATA) == 0) {
				m_bWithinIgnoredBuild = false;
				PModelReaderNode100_MetaData pXMLNode = std::make_shared<CModelReaderNode100_MetaData>(m_pWarnings);
				pXMLNode->parseXML(pXMLReader);

				std::wstring sName = pXMLNode->getName();
				std::wstring sValue = pXMLNode->getValue();
				if (sName.length() > 0) {
					if (m_pModel->hasMetaData(sName)) {
						m_pWarnings->addWarning(MODELREADERWARNING_DUPLICATEMETADATA, NMR_ERROR_DUPLICATEMETADATA, mrwInvalidOptionalValue);
					}
					std::wstring sNameSpace, sNameOnly;
					if (decomposeIntoNamespaceAndName(sName, sNameSpace, sNameOnly)) {
						std::wstring sNameSpaceURI;
						if (pXMLReader->GetNamespaceURI(sNameSpace, sNameSpaceURI))
							m_pModel->addMetaData(sNameSpaceURI+L":"+sNameOnly, sValue);
						else
							throw CNMRException(NMR_ERROR_METADATA_COULDNOTGETNAMESPACE);
					}
					else {
						// default namespace
						if ((wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_1) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_2) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_3) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_4) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_5) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_6) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_7) == 0) ||
							(wcscmp(sName.c_str(), XML_3MF_METADATA_VALUE_8) == 0)
							)
						{
							m_pModel->addMetaData(sName, sValue);
						}
						else
							m_pWarnings->addException(CNMRException(NMR_ERROR_UNKNOWNMETADATA), mrwInvalidOptionalValue);

					}
				}
				else {
					m_pWarnings->addWarning(MODELREADERWARNING_INVALIDMETADATA, NMR_ERROR_INVALIDMETADATA, mrwInvalidOptionalValue);
				}
			}
			else {
				if (!m_bWithinIgnoredBuild)
					m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
			}
		}

		if ((wcscmp(pNameSpace, XML_3MF_NAMESPACE_CORESPEC093) == 0) || (wcscmp(pNameSpace, L"") == 0)) {
		   if (wcscmp(pChildName, XML_3MF_ELEMENT_RESOURCES) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Resources>(m_pModel, m_pWarnings);
				if (m_bHasResources)
					throw CNMRException(NMR_ERROR_DUPLICATERESOURCES);

				pXMLNode->parseXML(pXMLReader);
				m_bHasResources = true;

			}
			else if (wcscmp(pChildName, XML_3MF_ELEMENT_BUILD) == 0) {
				PModelReaderNode pXMLNode = std::make_shared<CModelReaderNode093_Build>(m_pModel, m_pWarnings);
				if (m_bHasBuild)
					throw CNMRException(NMR_ERROR_DUPLICATEBUILDSECTION);

				pXMLNode->parseXML(pXMLReader);
				m_bHasBuild = true;
			}
			else if ( (wcscmp(pChildName, XML_3MF_ELEMENT_METADATA) == 0) ||  (wcscmp(pChildName, XML_3MF_ELEMENT_METADATA_ENRTY) == 0)) {
				// nothing
			}
			else 
				m_pWarnings->addException(CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT), mrwInvalidOptionalValue);
		}


	}

	nfBool CModelReaderNode_Model::getHasResources()
	{
		return m_bHasResources;
	}

	nfBool CModelReaderNode_Model::getHasBuild()
	{
		return m_bHasBuild;
	}

	nfBool CModelReaderNode_Model::ignoreBuild()
	{
		return m_bIgnoreBuild;
	}
	void CModelReaderNode_Model::setIgnoreBuild(bool bIgnoreBuild)
	{
		m_bIgnoreBuild = bIgnoreBuild;
	}

}
