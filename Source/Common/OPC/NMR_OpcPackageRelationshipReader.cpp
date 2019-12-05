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

#include "Common/OPC/NMR_OpcPackageRelationshipReader.h"
#include "Common/OPC/NMR_OpcPackageTypes.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_WinTypes.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR {

	COpcPackageRelationshipReader::COpcPackageRelationshipReader(_In_ PImportStream pImportStream, _In_ PProgressMonitor pProgressMonitor)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (pProgressMonitor) {
			pProgressMonitor->ReportProgressAndQueryCancelled(true);
		}
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
			if (strcmp(pszLocalName, OPC_RELS_RELATIONSHIP_CONTAINER) == 0) {
				parseRootNode(pXMLReader.get());
			}
		}
	}


	COpcPackageRelationshipReader::~COpcPackageRelationshipReader()
	{

	}

	void COpcPackageRelationshipReader::parseRootNode(_In_ CXmlReader * pXMLReader)
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
					if (strcmp(pszNameSpaceURI, OPCPACKAGE_SCHEMA_RELATIONSHIPS) == 0) {
						if (strcmp(pszLocalName, OPC_RELS_RELATIONSHIP_NODE) == 0) {
							parseChildNode(pXMLReader);
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

				if (strcmp(pszLocalName, OPC_RELS_RELATIONSHIP_CONTAINER) == 0) {
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


	void COpcPackageRelationshipReader::parseChildNode(_In_ CXmlReader * pXMLReader)
	{
		if (pXMLReader == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (!pXMLReader->MoveToFirstAttribute())
			return;

		std::string sTarget;
		std::string sID;
		std::string sType;

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
					if (strcmp(pszLocalName, OPC_RELS_ATTRIB_TARGET) == 0)
						sTarget = pszValue;
					if (strcmp(pszLocalName, OPC_RELS_ATTRIB_TYPE) == 0)
						sType = pszValue;
					if (strcmp(pszLocalName, OPC_RELS_ATTRIB_ID) == 0)
						sID = pszValue;
				}

			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}
		
		// must be non-empty and absolute getPath
		if (sTarget.empty()) // || (sTarget[0] != L'/') )
			throw CNMRException(NMR_ERROR_INVALIDOPCPARTURI);

		if (sType == PACKAGE_PRINT_TICKET_RELATIONSHIP_TYPE) {
			for (auto itRel : m_IDToRelationShips) {
				if (itRel.second->getType() == sType) {
					throw CNMRException(NMR_ERROR_DUPLICATE_PRINTTICKET);
				}
			}
		}
		
		auto found = m_IDToRelationShips.find(sID);
		if (found != m_IDToRelationShips.end()) {
			throw CNMRException(NMR_ERROR_OPC_DUPLICATE_RELATIONSHIP_ID);
		}
		POpcPackageRelationship pRelationShip = std::make_shared<COpcPackageRelationship> (sID, sType, sTarget);
		m_IDToRelationShips.insert(std::make_pair(sID, pRelationShip));

		pXMLReader->CloseElement();
	}

	nfUint32 COpcPackageRelationshipReader::getCount()
	{
		return (nfUint32)m_IDToRelationShips.size();
	}

	POpcPackageRelationship COpcPackageRelationshipReader::getRelationShip(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = (nfUint32)m_IDToRelationShips.size();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto it = m_IDToRelationShips.begin();
		std::advance(it, nIndex);
		return it->second;
	}

	void COpcPackageRelationshipReader::parseAttributes(_In_ CXmlReader * pXMLReader)
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


