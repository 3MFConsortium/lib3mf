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

NMR_ModelReaderNode.cpp implements the Model Reader Node Class.
A model reader node is an abstract base class for all XML nodes of a
3MF Model Stream.

--*/

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelReaderNode::CModelReaderNode(_In_ PModelWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor)
	{
		m_bParsedAttributes = false;
		m_bParsedContent = false;
		m_bIsEmptyElement = false;

		if (pProgressMonitor) {
			m_pProgressMonitor = pProgressMonitor;
		}
		else {
			m_pProgressMonitor = std::make_shared<CProgressMonitor>();
		}

		if (pWarnings) {
			m_pWarnings = pWarnings;
		}
		else {
			m_pWarnings = std::make_shared<CModelWarnings>();
		}
	}

	void CModelReaderNode::parseName(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		LPCSTR pszName = nullptr;
		pXMLReader->GetLocalName(&pszName, nullptr);
		if (!pszName)
			throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

		std::string sName(pszName);
		m_sName = sName;

		if (m_sName == "")
			throw CNMRException(NMR_ERROR_NODENAMEISEMPTY);

		m_bIsEmptyElement = pXMLReader->IsEmptyElement() != 0;
	}

	std::string CModelReaderNode::getName()
	{
		return m_sName;
	}

	PModelWarnings CModelReaderNode::getWarnings() const
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

				if (nNameCount > 0) {
					if (nNameSpaceCount == 0) {
						OnAttribute(pszLocalName, pszValue);
					}
					else {
						OnNSAttribute(pszLocalName, pszValue, pszNameSpaceURI);
					}
				}
			}

			bContinue = pXMLReader->MoveToNextAttribute();
		}
	}

	void CModelReaderNode::parseContent(_In_ CXmlReader * pXMLReader)
	{
		__NMRASSERT(pXMLReader);

		if (m_sName == "")
			throw CNMRException(NMR_ERROR_NODENAMEISEMPTY);

		if (m_bParsedContent)
			throw CNMRException(NMR_ERROR_ALREADYPARSEDXMLNODE);
		m_bParsedContent = true;

		if (m_bIsEmptyElement) {
			pXMLReader->CloseElement();
			return;
		}

		while (!pXMLReader->IsEOF()) {
			LPCSTR pszLocalName = nullptr;
			LPCSTR pszNameSpaceURI = nullptr;
			LPCSTR pszText = nullptr;
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
						if (pszNameSpaceURI == nullptr) {
							OnNSChildElement(pszLocalName, "", pXMLReader);
						}
						else {
							OnNSChildElement(pszLocalName, pszNameSpaceURI, pXMLReader);
						}
					}
				break;

			case XMLREADERNODETYPE_TEXT:
				pXMLReader->GetValue(&pszText, &nCount);
				if (!pszText)
					throw CNMRException(NMR_ERROR_COULDNOTGETXMLTEXT);

				if (nCount > 0)
					OnText(pszText, pXMLReader);
				break;

			case XMLREADERNODETYPE_ENDELEMENT:
				pXMLReader->GetLocalName(&pszLocalName, &nCount);
				if (!pszLocalName)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				if (strcmp(pszLocalName, m_sName.c_str()) == 0) {
					OnEndElement (pXMLReader);

					pXMLReader->CloseElement();
					return;
				}
				break;

			case XMLREADERNODETYPE_UNKNOWN:
				break;
			}
		}
	}

	void CModelReaderNode::OnAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnText(_In_z_ const nfChar * pText, _In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnEndElement(_In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnNSAttribute(_In_z_ const nfChar * pAttributeName, _In_z_ const nfChar * pAttributeValue, _In_z_ const nfChar * pNameSpace)
	{
		// empty on purpose, to be implemented by child classes
	}

	void CModelReaderNode::OnNSChildElement(_In_z_ const nfChar * pChildName, _In_z_ const nfChar * pNameSpace, _In_ CXmlReader * pXMLReader)
	{
		// empty on purpose, to be implemented by child classes

	}


}
