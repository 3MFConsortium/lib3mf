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

NMR_OpcPackageReader.cpp defines an OPC Package reader of .rels files in a portable way.

--*/

#include "Common/OPC/NMR_OpcPackageContentTypesReader.h"
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	COpcPackageContentTypesReader::COpcPackageContentTypesReader(_In_ PImportStream pImportStream, PProgressMonitor pProgressMonitor)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pProgressMonitor.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pProgressMonitor->ReportProgressAndQueryCancelled(true);
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pImportStream, pProgressMonitor);

		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCSTR pszLocalName = nullptr;
			pXMLReader->GetLocalName(&pszLocalName, nullptr);
			if (!pszLocalName)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

			// Compare with Model Node Name
			if (strcmp(pszLocalName, OPC_CONTENTTYPES_CONTAINER) == 0) {
				parseRootNode(pXMLReader.get());
			}
		}
	}


	COpcPackageContentTypesReader::~COpcPackageContentTypesReader()
	{

	}

	void COpcPackageContentTypesReader::parseRootNode(_In_ CXmlReader * pXMLReader)
	{
		if (pXMLReader == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		parseAttributes(pXMLReader);

		while (!pXMLReader->IsEOF()) {
			LPCSTR pszLocalName = nullptr;
			LPCSTR pszNameSpaceURI = nullptr;
			UINT nCount = 0;
			UINT nNameSpaceCount = 0;

			eXmlReaderNodeType NodeType;
			pXMLReader->Read(NodeType);

			switch (NodeType) {
			case XMLREADERNODETYPE_STARTELEMENT:
				pXMLReader->GetLocalName(&pszLocalName, &nCount);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				pXMLReader->GetNamespaceURI(&pszNameSpaceURI, &nNameSpaceCount);
				if (!pszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				if (nCount > 0) {
					if (strcmp(pszNameSpaceURI, OPCPACKAGE_SCHEMA_CONTENTTYPES) == 0) {
						if (strcmp(pszLocalName, OPC_CONTENTTYPES_NODE) == 0) {
							parseChildNode(pXMLReader, false);
						}
						else if (strcmp(pszLocalName, OPC_CONTENTTYPES_NODE_OVERRIDE) == 0) {
							parseChildNode(pXMLReader, true);
						}
						else
							throw CNMRException(NMR_ERROR_NAMESPACE_INVALID_ELEMENT);
					}
				}
				break;

			case XMLREADERNODETYPE_ENDELEMENT:
				pXMLReader->GetLocalName(&pszLocalName, &nCount);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				if (strcmp(pszLocalName, OPC_CONTENTTYPES_CONTAINER) == 0) {
					return;
				}
				pXMLReader->CloseElement();
				break;

			case XMLREADERNODETYPE_UNKNOWN:
				break;

			case XMLREADERNODETYPE_TEXT:
				break;
			}
		}
	}


	void COpcPackageContentTypesReader::parseChildNode(_In_ CXmlReader * pXMLReader, _In_ bool bOverride)
	{
		if (pXMLReader == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!pXMLReader->MoveToFirstAttribute())
			return;

		std::string sExtension;
		std::string sPartName;
		std::string sContentType;

		nfBool bContinue = true;
		while (bContinue) {

			if (!pXMLReader->IsDefault()) {
				LPCSTR pszLocalName = nullptr;
				LPCSTR pszNameSpaceURI = nullptr;
				LPCSTR pszValue = nullptr;
				UINT nNameCount = 0;
				UINT nValueCount = 0;
				UINT nNameSpaceCount = 0;

				// Get Attribute Name
				pXMLReader->GetNamespaceURI(&pszNameSpaceURI, &nNameSpaceCount);
				if (!pszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				pXMLReader->GetLocalName(&pszLocalName, &nNameCount);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				// Get Attribute Value
				pXMLReader->GetValue(&pszValue, &nValueCount);
				if (!pszValue)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);

				if (nNameSpaceCount == 0) {
					if (strcmp(pszLocalName, OPC_CONTENTTYPES_ATTRIB_EXTENSION) == 0)
						sExtension = pszValue;
					if (strcmp(pszLocalName, OPC_CONTENTTYPES_ATTRIB_CONTENTTYPE) == 0)
						sContentType = pszValue;
					if (strcmp(pszLocalName, OPC_CONTENTTYPES_ATTRIB_PARTNAME) == 0)
						sPartName = pszValue;
				}

			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}

		if (sContentType.empty())
			throw CNMRException(NMR_ERROR_CONTENTTYPE_EMPTY_CONTENTTYPE);

		if (bOverride) {
			if (sPartName.empty())
				throw CNMRException(NMR_ERROR_CONTENTTYPE_EMPTY_PARTNAME);

			std::string sRelativePartName = fnRemoveLeadingPathDelimiter(sPartName);
			for (auto it : m_Overrides) {
				if (it->m_partName == sRelativePartName)
					throw CNMRException(NMR_ERROR_DUPLICATE_CONTENTTYPE);
			}
			POpcPackageContentType_Override pRelationShip = std::make_shared<COpcPackageContentType_Override>(sRelativePartName, sContentType);
			m_Overrides.push_back(pRelationShip);
		}
		else {
			if (sExtension.empty())
				throw CNMRException(NMR_ERROR_CONTENTTYPE_EMPTY_EXTENSION);

			POpcPackageContentType pContentType = std::make_shared<COpcPackageContentType>(sExtension, sContentType);
			for (auto it : m_ContentTypes) {
				if (it->m_extension == sExtension)
					throw CNMRException(NMR_ERROR_DUPLICATE_CONTENTTYPE);
			}
			m_ContentTypes.push_back(pContentType);
		}

		

		pXMLReader->CloseElement();
	}

	nfUint32 COpcPackageContentTypesReader::getCount()
	{
		return (nfUint32)m_ContentTypes.size();
	}

	POpcPackageContentType COpcPackageContentTypesReader::getContentType(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = (nfUint32)m_ContentTypes.size();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_ContentTypes[nIndex];
	}

	nfUint32 COpcPackageContentTypesReader::getOverrideCount()
	{
		return (nfUint32)m_Overrides.size();
	}

	POpcPackageContentType_Override COpcPackageContentTypesReader::getOverrideContentType(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = (nfUint32)m_Overrides.size();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Overrides[nIndex];
	}

	void COpcPackageContentTypesReader::parseAttributes(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		if (!pXMLReader->MoveToFirstAttribute())
			return;

		nfBool bContinue = true;
		while (bContinue) {

			if (!pXMLReader->IsDefault()) {
				LPCSTR pszLocalName = nullptr;
				LPCSTR pszNameSpaceURI = nullptr;
				LPCSTR pszValue = nullptr;
				UINT nNameCount = 0;
				UINT nValueCount = 0;
				UINT nNameSpaceCount = 0;

				// Get Attribute Name
				pXMLReader->GetNamespaceURI(&pszNameSpaceURI, &nNameSpaceCount);
				if (!pszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				pXMLReader->GetLocalName(&pszLocalName, &nNameCount);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				// Get Attribute Value
				pXMLReader->GetValue(&pszValue, &nValueCount);
				if (!pszValue)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);
			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}
	}


}


