/*++

Copyright (C) 2026 3MF Consortium

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

NMR_ToolpathViewableJSONSerializer.cpp implements deterministic XML-to-JSON
conversion for toolpath layer viewables.

--*/

#include "Model/ToolpathReader/NMR_ToolpathViewableJSONSerializer.h"

#include "Common/NMR_Exception.h"
#include "Common/Platform/NMR_Platform.h"
#include "Common/Platform/NMR_XmlReader.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include <cctype>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace NMR {

	namespace {

		const char * XML_NAMESPACE_DECL_URI = "http://www.w3.org/2000/xmlns/";

		struct sJSONAttribute {
			std::string m_sName;
			std::string m_sValue;
		};

		struct sJSONNode {
			std::string m_sName;
			std::vector<sJSONAttribute> m_Attributes;
			std::vector<sJSONNode> m_Children;
			std::vector<std::string> m_Texts;
		};

		struct sNamespaceContext {
			std::string m_sDefaultNamespaceURI;
			std::vector<std::pair<std::string, std::string>> m_Bindings;
		};

		struct sRawAttribute {
			std::string m_sLocalName;
			std::string m_sNamespaceURI;
			std::string m_sValue;
		};

		static std::string trim(const std::string & sValue)
		{
			size_t nStart = 0;
			while ((nStart < sValue.size()) && std::isspace(static_cast<unsigned char>(sValue[nStart])))
				nStart++;

			size_t nEnd = sValue.size();
			while ((nEnd > nStart) && std::isspace(static_cast<unsigned char>(sValue[nEnd - 1])))
				nEnd--;

			return sValue.substr(nStart, nEnd - nStart);
		}

		static bool isAllWhitespace(const std::string & sValue)
		{
			for (char c : sValue) {
				if (!std::isspace(static_cast<unsigned char>(c)))
					return false;
			}
			return true;
		}

		static void appendEscapedJSONString(const std::string & sValue, std::string & sJSON)
		{
			sJSON.push_back('\"');
			for (unsigned char c : sValue) {
				switch (c) {
				case '\"': sJSON += "\\\""; break;
				case '\\': sJSON += "\\\\"; break;
				case '\b': sJSON += "\\b"; break;
				case '\f': sJSON += "\\f"; break;
				case '\n': sJSON += "\\n"; break;
				case '\r': sJSON += "\\r"; break;
				case '\t': sJSON += "\\t"; break;
				default:
					if (c < 0x20) {
						static const char * pHex = "0123456789abcdef";
						sJSON += "\\u00";
						sJSON.push_back(pHex[(c >> 4) & 0x0f]);
						sJSON.push_back(pHex[c & 0x0f]);
					}
					else {
						sJSON.push_back(static_cast<char>(c));
					}
					break;
				}
			}
			sJSON.push_back('\"');
		}

		static bool isJSONNumberLexeme(const std::string & sValue)
		{
			if (sValue.empty())
				return false;

			size_t nPos = 0;
			if ((sValue[nPos] == '-') || (sValue[nPos] == '+'))
				nPos++;
			if (nPos >= sValue.size())
				return false;

			if (sValue[nPos] == '0') {
				nPos++;
			}
			else {
				if (!std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					return false;
				while ((nPos < sValue.size()) && std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					nPos++;
			}

			if ((nPos < sValue.size()) && (sValue[nPos] == '.')) {
				nPos++;
				if ((nPos >= sValue.size()) || !std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					return false;
				while ((nPos < sValue.size()) && std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					nPos++;
			}

			if ((nPos < sValue.size()) && ((sValue[nPos] == 'e') || (sValue[nPos] == 'E'))) {
				nPos++;
				if ((nPos < sValue.size()) && ((sValue[nPos] == '+') || (sValue[nPos] == '-')))
					nPos++;
				if ((nPos >= sValue.size()) || !std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					return false;
				while ((nPos < sValue.size()) && std::isdigit(static_cast<unsigned char>(sValue[nPos])))
					nPos++;
			}

			return nPos == sValue.size();
		}

		static void appendJSONScalarFromLexeme(const std::string & sValue, std::string & sJSON)
		{
			std::string sTrimmed = trim(sValue);
			if ((sTrimmed == "true") || (sTrimmed == "false") || (sTrimmed == "null") || isJSONNumberLexeme(sTrimmed)) {
				sJSON += sTrimmed;
			}
			else {
				appendEscapedJSONString(sValue, sJSON);
			}
		}

		static std::string findPrefixForNamespace(const sNamespaceContext & context, const std::string & sNamespaceURI)
		{
			for (size_t nIndex = context.m_Bindings.size(); nIndex > 0; nIndex--) {
				const auto & binding = context.m_Bindings[nIndex - 1];
				if (binding.second == sNamespaceURI)
					return binding.first;
			}
			return std::string();
		}

		static std::string resolveElementQName(const std::string & sLocalName, const std::string & sNamespaceURI, const sNamespaceContext & context)
		{
			if (sNamespaceURI.empty())
				return sLocalName;

			if (sNamespaceURI == context.m_sDefaultNamespaceURI)
				return sLocalName;

			std::string sPrefix = findPrefixForNamespace(context, sNamespaceURI);
			if (sPrefix.empty())
				return sLocalName;

			return sPrefix + ":" + sLocalName;
		}

		static std::string resolveAttributeQName(const std::string & sLocalName, const std::string & sNamespaceURI, const sNamespaceContext & context)
		{
			if (sNamespaceURI.empty())
				return sLocalName;

			std::string sPrefix = findPrefixForNamespace(context, sNamespaceURI);
			if (sPrefix.empty())
				return sLocalName;

			return sPrefix + ":" + sLocalName;
		}

		static void appendNodeObject(const sJSONNode & node, std::string & sJSON)
		{
			sJSON.push_back('{');
			bool bHasPrevious = false;

			for (const auto & attribute : node.m_Attributes) {
				if (bHasPrevious)
					sJSON.push_back(',');
				bHasPrevious = true;

				appendEscapedJSONString(attribute.m_sName, sJSON);
				sJSON.push_back(':');
				appendJSONScalarFromLexeme(attribute.m_sValue, sJSON);
			}

			std::vector<std::string> childOrder;
			std::unordered_map<std::string, std::vector<const sJSONNode *>> groupedChildren;
			childOrder.reserve(node.m_Children.size());

			for (const auto & child : node.m_Children) {
				auto iIter = groupedChildren.find(child.m_sName);
				if (iIter == groupedChildren.end()) {
					childOrder.push_back(child.m_sName);
					groupedChildren.insert(std::make_pair(child.m_sName, std::vector<const sJSONNode *>()));
				}

				groupedChildren[child.m_sName].push_back(&child);
			}

			for (const auto & sChildName : childOrder) {
				const auto & children = groupedChildren[sChildName];

				if (bHasPrevious)
					sJSON.push_back(',');
				bHasPrevious = true;

				appendEscapedJSONString("_" + sChildName, sJSON);
				sJSON.push_back(':');

				if (children.size() == 1) {
					appendNodeObject(*children[0], sJSON);
				}
				else {
					sJSON.push_back('[');
					for (size_t nIndex = 0; nIndex < children.size(); nIndex++) {
						if (nIndex > 0)
							sJSON.push_back(',');
						appendNodeObject(*children[nIndex], sJSON);
					}
					sJSON.push_back(']');
				}
			}

			if (!node.m_Texts.empty()) {
				if (bHasPrevious)
					sJSON.push_back(',');

				appendEscapedJSONString("_text", sJSON);
				sJSON.push_back(':');
				if (node.m_Texts.size() == 1) {
					appendJSONScalarFromLexeme(node.m_Texts[0], sJSON);
				}
				else {
					sJSON.push_back('[');
					for (size_t nIndex = 0; nIndex < node.m_Texts.size(); nIndex++) {
						if (nIndex > 0)
							sJSON.push_back(',');
						appendJSONScalarFromLexeme(node.m_Texts[nIndex], sJSON);
					}
					sJSON.push_back(']');
				}
			}

			sJSON.push_back('}');
		}

		static sJSONNode parseCurrentElement(CXmlReader * pXMLReader, const sNamespaceContext & parentContext)
		{
			const nfChar * pLocalName = nullptr;
			const nfChar * pNamespaceURI = nullptr;

			pXMLReader->GetLocalName(&pLocalName, nullptr);
			pXMLReader->GetNamespaceURI(&pNamespaceURI, nullptr);

			if (pLocalName == nullptr)
				throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);
			if (pNamespaceURI == nullptr)
				throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

			std::string sCurrentLocalName = pLocalName;
			std::string sCurrentNamespaceURI = pNamespaceURI;

			std::vector<sRawAttribute> rawAttributes;
			std::vector<std::pair<std::string, std::string>> declaredPrefixes;
			std::string sDeclaredDefaultNamespace;
			bool bHasDeclaredDefaultNamespace = false;

			if (pXMLReader->MoveToFirstAttribute()) {
				bool bContinue = true;
				while (bContinue) {
					const nfChar * pAttributeLocalName = nullptr;
					const nfChar * pAttributeNamespaceURI = nullptr;
					const nfChar * pAttributeValue = nullptr;

					pXMLReader->GetLocalName(&pAttributeLocalName, nullptr);
					pXMLReader->GetNamespaceURI(&pAttributeNamespaceURI, nullptr);
					pXMLReader->GetValue(&pAttributeValue, nullptr);

					if (pAttributeLocalName == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);
					if (pAttributeNamespaceURI == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);
					if (pAttributeValue == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETXMLVALUE);

					sRawAttribute rawAttribute;
					rawAttribute.m_sLocalName = pAttributeLocalName;
					rawAttribute.m_sNamespaceURI = pAttributeNamespaceURI;
					rawAttribute.m_sValue = pAttributeValue;
					rawAttributes.push_back(rawAttribute);

					if ((rawAttribute.m_sLocalName == "xmlns") && rawAttribute.m_sNamespaceURI.empty()) {
						sDeclaredDefaultNamespace = rawAttribute.m_sValue;
						bHasDeclaredDefaultNamespace = true;
					}
					else if (rawAttribute.m_sNamespaceURI == XML_NAMESPACE_DECL_URI) {
						declaredPrefixes.push_back(std::make_pair(rawAttribute.m_sLocalName, rawAttribute.m_sValue));
					}

					bContinue = pXMLReader->MoveToNextAttribute();
				}
			}

			sNamespaceContext currentContext = parentContext;
			if (bHasDeclaredDefaultNamespace)
				currentContext.m_sDefaultNamespaceURI = sDeclaredDefaultNamespace;
			for (const auto & declaredPrefix : declaredPrefixes) {
				currentContext.m_Bindings.push_back(declaredPrefix);
			}

			sJSONNode node;
			node.m_sName = resolveElementQName(sCurrentLocalName, sCurrentNamespaceURI, currentContext);

			for (const auto & rawAttribute : rawAttributes) {
				sJSONAttribute attribute;
				if ((rawAttribute.m_sLocalName == "xmlns") && rawAttribute.m_sNamespaceURI.empty()) {
					attribute.m_sName = "xmlns";
				}
				else if (rawAttribute.m_sNamespaceURI == XML_NAMESPACE_DECL_URI) {
					attribute.m_sName = "xmlns:" + rawAttribute.m_sLocalName;
				}
				else {
					attribute.m_sName = resolveAttributeQName(rawAttribute.m_sLocalName, rawAttribute.m_sNamespaceURI, currentContext);
				}
				attribute.m_sValue = rawAttribute.m_sValue;
				node.m_Attributes.push_back(attribute);
			}

			eXmlReaderNodeType nodeType;
			while (!pXMLReader->IsEOF()) {
				if (!pXMLReader->Read(nodeType))
					break;

				switch (nodeType) {
				case XMLREADERNODETYPE_STARTELEMENT:
					node.m_Children.push_back(parseCurrentElement(pXMLReader, currentContext));
					break;

				case XMLREADERNODETYPE_TEXT:
				{
					const nfChar * pText = nullptr;
					pXMLReader->GetValue(&pText, nullptr);
					if (pText == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETXMLTEXT);

					std::string sText = pText;
					if (!isAllWhitespace(sText))
						node.m_Texts.push_back(sText);
					break;
				}

				case XMLREADERNODETYPE_ENDELEMENT:
				{
					const nfChar * pEndLocalName = nullptr;
					const nfChar * pEndNamespaceURI = nullptr;
					pXMLReader->GetLocalName(&pEndLocalName, nullptr);
					pXMLReader->GetNamespaceURI(&pEndNamespaceURI, nullptr);

					if (pEndLocalName == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);
					if (pEndNamespaceURI == nullptr)
						throw CNMRException(NMR_ERROR_COULDNOTGETNAMESPACE);

					if ((sCurrentLocalName == pEndLocalName) && (sCurrentNamespaceURI == pEndNamespaceURI)) {
						pXMLReader->CloseElement();
						return node;
					}
					break;
				}

				case XMLREADERNODETYPE_UNKNOWN:
				default:
					break;
				}
			}

			return node;
		}

	}

	std::string CToolpathViewableJSONSerializer::serializeLayerXMLToJSON(_In_ PImportStream pLayerDataStream)
	{
		if (pLayerDataStream.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pLayerDataStream->seekPosition(0, true);

		PProgressMonitor pProgressMonitor = std::make_shared<CProgressMonitor>();
		PXmlReader pXMLReader = fnCreateXMLReaderInstance(pLayerDataStream, pProgressMonitor);

		eXmlReaderNodeType nodeType;
		sJSONNode rootNode;
		bool bFoundRoot = false;

		sNamespaceContext rootContext;
		rootContext.m_sDefaultNamespaceURI = "";

		while (!pXMLReader->IsEOF()) {
			if (!pXMLReader->Read(nodeType))
				break;

			if (nodeType == XMLREADERNODETYPE_STARTELEMENT) {
				const nfChar * pLocalName = nullptr;
				pXMLReader->GetLocalName(&pLocalName, nullptr);
				if (pLocalName == nullptr)
					throw CNMRException(NMR_ERROR_COULDNOTGETLOCALXMLNAME);

				// Skip processing instruction nodes represented as pseudo-elements.
				if (std::string(pLocalName) == XML_3MF_ATTRIBUTE_PREFIX_XML)
					continue;

				rootNode = parseCurrentElement(pXMLReader.get(), rootContext);
				bFoundRoot = true;
				break;
			}
		}

		if (!bFoundRoot)
			throw CNMRException(NMR_ERROR_NOMODELNODE);

		std::string sJSON;
		sJSON.reserve(65536);
		sJSON.push_back('{');
		appendEscapedJSONString(rootNode.m_sName, sJSON);
		sJSON.push_back(':');
		appendNodeObject(rootNode, sJSON);
		sJSON.push_back('}');

		return sJSON;
	}

}

