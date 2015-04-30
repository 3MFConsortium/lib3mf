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

NMR_ModelReaderNode.cpp implements the Model Reader Node Class.
A model reader node is an abstract base class for all XML nodes of a
3MF Model Stream.

--*/

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CModelReaderNode::CModelReaderNode(PModelReaderWarnings pWarnings)
	{
		m_bParsedAttributes = false;
		m_bParsedContent = false;
		m_bIsEmptyElement = false;
		if (pWarnings.get()) {
			m_pWarnings = pWarnings;
		}
		else {
			m_pWarnings = std::make_shared<CModelReaderWarnings>();
		}
	}

	void CModelReaderNode::parseName(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		LPCWSTR pwszName = nullptr;
		pXMLReader->GetQualifiedName(&pwszName, nullptr);
		if (!pwszName)
			throw CNMRException(NMR_ERROR_COULDNOTGETQUALIFIEDXMLNAME);

		std::wstring sName(pwszName);
		m_sName = sName;

		if (m_sName == L"")
			throw CNMRException(NMR_ERROR_NODENAMEISEMPTY);

		m_bIsEmptyElement = pXMLReader->IsEmptyElement() != 0;
	}

	std::wstring CModelReaderNode::getName()
	{
		return m_sName;
	}

	PModelReaderWarnings CModelReaderNode::getWarnings()
	{
		return m_pWarnings;
	}

	void CModelReaderNode::parseAttributes(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		if (m_bParsedAttributes)
			throw CNMRException(NMR_ERROR_ALREADYPARSEDXMLNODE);
		m_bParsedAttributes = true;

		if (!pXMLReader->MoveToFirstAttribute())
			return;

		nfBool bContinue = true;
		while (bContinue) {

			if (!pXMLReader->IsDefault()) {
				LPCWSTR pwszQualifiedName = nullptr;
				LPCWSTR pwszValue = nullptr;
				UINT nNameCount = 0;
				UINT nValueCount = 0;

				// Get Attribute Name
				pXMLReader->GetQualifiedName(&pwszQualifiedName, &nNameCount);
				if (!pwszQualifiedName)
					throw CNMRException(NMR_ERROR_COULDNOTGETQUALIFIEDXMLNAME);

				// Get Attribute Value
				pXMLReader->GetValue(&pwszValue, &nValueCount);
				if (!pwszValue)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);

				if (nNameCount > 0)
					OnAttribute(pwszQualifiedName, pwszValue);
			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}
	}

	void CModelReaderNode::parseContent(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		if (m_sName == L"")
			throw CNMRException(NMR_ERROR_NODENAMEISEMPTY);

		if (m_bParsedContent)
			throw CNMRException(NMR_ERROR_ALREADYPARSEDXMLNODE);
		m_bParsedContent = true;

		if (m_bIsEmptyElement)
			return;

		while (!pXMLReader->IsEOF()) {
			LPCWSTR pwszQualifiedName = nullptr;
			LPCWSTR pwszText = nullptr;
			UINT nCount = 0;

			eXmlReaderNodeType NodeType;
			pXMLReader->Read(NodeType);

			switch (NodeType) {
			case XMLREADERNODETYPE_STARTELEMENT:
				pXMLReader->GetQualifiedName(&pwszQualifiedName, &nCount);
				if (!pwszQualifiedName)
					throw CNMRException(NMR_ERROR_COULDNOTGETQUALIFIEDXMLNAME);

				if (nCount > 0)
					OnChildElement(pwszQualifiedName, pXMLReader);
				break;

			case XMLREADERNODETYPE_TEXT:
				pXMLReader->GetValue(&pwszText, &nCount);
				if (!pwszText)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLTEXT);

				if (nCount > 0)
					OnText(pwszText, pXMLReader);
				break;

			case XMLREADERNODETYPE_ENDELEMENT:
				pXMLReader->GetQualifiedName(&pwszQualifiedName, &nCount);
				if (!pwszQualifiedName)
					throw CNMRException(NMR_ERROR_COULDNOTGETQUALIFIEDXMLNAME);

				if (wcscmp(pwszQualifiedName, m_sName.c_str()) == 0) {
					OnEndElement (pXMLReader);
					return;
				}
				break;

			case XMLREADERNODETYPE_UNKNOWN:
				break;
			}
		}
	}

	void CModelReaderNode::OnAttribute(_In_z_ const nfWChar * pAttributeName, _In_z_ const nfWChar * pAttributeValue)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnChildElement(_In_z_ const nfWChar * pChildName, _In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnText(_In_z_ const nfWChar * pText, _In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnEndElement(_In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes
	}

}
