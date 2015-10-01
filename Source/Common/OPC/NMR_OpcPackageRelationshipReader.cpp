/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

namespace NMR {

	COpcPackageRelationshipReader::COpcPackageRelationshipReader(_In_ PImportStream pImportStream)
	{
		if (pImportStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pImportStream);

		eXmlReaderNodeType NodeType;
		// Read all XML Root Nodes
		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(NodeType))
				break;

			// Get Node Name
			LPCWSTR pwszLocalName = nullptr;
			pXMLReader->GetLocalName(&pwszLocalName, nullptr);
			if (!pwszLocalName)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

			// Compare with Model Node Name
			if (wcscmp(pwszLocalName, OPC_RELS_RELATIONSHIP_CONTAINER) == 0) {
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
			LPCWSTR pwszLocalName = nullptr;
			LPCWSTR pwszNameSpaceURI = nullptr;
			UINT nCount = 0;
			UINT nNameSpaceCount = 0;

			eXmlReaderNodeType NodeType;
			pXMLReader->Read(NodeType);

			switch (NodeType) {
			case XMLREADERNODETYPE_STARTELEMENT:
				pXMLReader->GetLocalName(&pwszLocalName, &nCount);
				if (!pwszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				pXMLReader->GetNamespaceURI(&pwszNameSpaceURI, &nNameSpaceCount);
				if (!pwszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				if (nCount > 0) {
					if (wcscmp(pwszNameSpaceURI, OPCPACKAGE_SCHEMA_RELATIONSHIPS) == 0) {
						if (wcscmp(pwszLocalName, OPC_RELS_RELATIONSHIP_NODE) == 0) {
							parseChildNode(pXMLReader);
						}
					}
				}
				break;

			case XMLREADERNODETYPE_ENDELEMENT:
				pXMLReader->GetLocalName(&pwszLocalName, &nCount);
				if (!pwszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				if (wcscmp(pwszLocalName, OPC_RELS_RELATIONSHIP_CONTAINER) == 0) {
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

		std::wstring sTarget;
		std::wstring sID;
		std::wstring sType;

		nfBool bContinue = true;
		while (bContinue) {

			if (!pXMLReader->IsDefault()) {
				LPCWSTR pwszLocalName = nullptr;
				LPCWSTR pwszNameSpaceURI = nullptr;
				LPCWSTR pwszValue = nullptr;
				UINT nNameCount = 0;
				UINT nValueCount = 0;
				UINT nNameSpaceCount = 0;

				// Get Attribute Name
				pXMLReader->GetNamespaceURI(&pwszNameSpaceURI, &nNameSpaceCount);
				if (!pwszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				pXMLReader->GetLocalName(&pwszLocalName, &nNameCount);
				if (!pwszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				// Get Attribute Value
				pXMLReader->GetValue(&pwszValue, &nValueCount);
				if (!pwszValue)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);

				if (nNameSpaceCount == 0) {
					if (wcscmp(pwszLocalName, OPC_RELS_ATTRIB_TARGET) == 0)
						sTarget = pwszValue;
					if (wcscmp(pwszLocalName, OPC_RELS_ATTRIB_TYPE) == 0)
						sType = pwszValue;
					if (wcscmp(pwszLocalName, OPC_RELS_ATTRIB_ID) == 0)
						sID = pwszValue;
				}

			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}

		POpcPackageRelationship pRelationShip = std::make_shared<COpcPackageRelationship> (sID, sType, sTarget);
		m_RelationShips.push_back(pRelationShip);

		pXMLReader->CloseElement();
	}

	nfUint32 COpcPackageRelationshipReader::getCount()
	{
		return (nfUint32)m_RelationShips.size();
	}

	POpcPackageRelationship COpcPackageRelationshipReader::getRelationShip(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = (nfUint32)m_RelationShips.size();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_RelationShips[nIndex];
	}

	void COpcPackageRelationshipReader::parseAttributes(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		if (!pXMLReader->MoveToFirstAttribute())
			return;

		nfBool bContinue = true;
		while (bContinue) {

			if (!pXMLReader->IsDefault()) {
				LPCWSTR pwszLocalName = nullptr;
				LPCWSTR pwszNameSpaceURI = nullptr;
				LPCWSTR pwszValue = nullptr;
				UINT nNameCount = 0;
				UINT nValueCount = 0;
				UINT nNameSpaceCount = 0;

				// Get Attribute Name
				pXMLReader->GetNamespaceURI(&pwszNameSpaceURI, &nNameSpaceCount);
				if (!pwszNameSpaceURI)
					throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

				pXMLReader->GetLocalName(&pwszLocalName, &nNameCount);
				if (!pwszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				// Get Attribute Value
				pXMLReader->GetValue(&pwszValue, &nValueCount);
				if (!pwszValue)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);
			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}
	}


}


