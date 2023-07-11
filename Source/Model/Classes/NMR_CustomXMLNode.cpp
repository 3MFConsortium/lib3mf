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

--*/

#include "Model/Classes/NMR_CustomXMLNode.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CCustomXMLNode::CCustomXMLNode(CCustomXMLNode * pParentNode, const std::string& sName)
		: m_sName (sName), m_bHasBeenInvalidated (false), m_pParentNode (pParentNode)
	{

	}

	CCustomXMLNode::~CCustomXMLNode()
	{
		invalidateNodeAndAllChildren();
	}

	std::string CCustomXMLNode::getName()
	{
		checkInvalidation();

		return m_sName;
	}

	CCustomXMLNode* CCustomXMLNode::getParentNode()
	{
		checkInvalidation();

		return m_pParentNode;
	}


	std::vector<PCustomXMLNode> CCustomXMLNode::getChildren()
	{
		checkInvalidation();

		return m_Children;
	}

	PCustomXMLNode CCustomXMLNode::getChildByIndex(const size_t nIndex)
	{
		checkInvalidation();

		if (nIndex >= m_Children.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Children.at(nIndex);
	}

	size_t CCustomXMLNode::getChildCount()
	{
		checkInvalidation();

		return m_Children.size();
	}

	bool CCustomXMLNode::hasChild(const std::string& sName)
	{
		checkInvalidation();

		auto iIter = m_ChildMap.find(sName);
		return iIter != m_ChildMap.end();
	}

	bool CCustomXMLNode::hasUniqueChild(const std::string& sName)
	{
		checkInvalidation();

		auto iIter = m_ChildMap.find(sName);
		if (iIter != m_ChildMap.end()) {
			return (iIter->second.size () == 1);
		}

		return false;

	}

	PCustomXMLNode CCustomXMLNode::addChild(const std::string& sName)
	{
		checkInvalidation();

		auto pChildNode = std::make_shared<CCustomXMLNode>(this, sName);

		m_Children.push_back(pChildNode);

		auto iIter = m_ChildMap.find(sName);
		if (iIter != m_ChildMap.end()) {
			iIter->second.insert(std::make_pair (pChildNode.get (), pChildNode));
		}
		else {
			std::map<CCustomXMLNode *, PCustomXMLNode> childSet;
			childSet.insert(std::make_pair(pChildNode.get(), pChildNode));
			m_ChildMap.insert(std::make_pair(sName, childSet));
		}
			
		return pChildNode;
	}


	PCustomXMLNode CCustomXMLNode::findUniqueChild(const std::string& sName, bool bMustExist)
	{
		checkInvalidation();

		PCustomXMLNode pChild = nullptr;

		auto iIter = m_ChildMap.find(sName);
		if (iIter != m_ChildMap.end()) {
			if (iIter->second.size() == 1) {
				pChild = iIter->second.begin()->second;
			}
			else {
				throw CNMRException(NMR_ERROR_XMLCHILDNODEISNOTUNIQUE);
			}
		}

		if (bMustExist)
			throw CNMRException(NMR_ERROR_XMLCHILDNODENOTFOUND);

		return nullptr;

	}

	size_t CCustomXMLNode::removeChildrenWithName(const std::string& sName)
	{
		checkInvalidation();

		size_t nRemoved = 0;
		auto iNameIter = m_ChildMap.find(sName);
		if (iNameIter != m_ChildMap.end()) {
			for (auto iChild : iNameIter->second) {
				iChild.second->invalidateNodeAndAllChildren();
			}
		}
		m_ChildMap.erase(sName);

		m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(), [sName, &nRemoved](const PCustomXMLNode& pChild)
		{
			if (pChild->getName() == sName) {
				nRemoved++;
				return true;
			}

			return false;
		}));

		return nRemoved;
	}

	void CCustomXMLNode::removeChild(CCustomXMLNode* pXMLNode)
	{
		checkInvalidation();

		if (pXMLNode == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		pXMLNode->invalidateNodeAndAllChildren();

		auto iNameIter = m_ChildMap.find(pXMLNode->getName());
		if (iNameIter != m_ChildMap.end()) {
			iNameIter->second.erase(pXMLNode);
		}

		m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(), [pXMLNode](const PCustomXMLNode& pChild)
		{
			return (pChild.get() == pXMLNode);
		}));

	}

	void CCustomXMLNode::remove()
	{
		checkInvalidation();

		if (m_pParentNode == nullptr)
			throw CNMRException(NMR_ERROR_ROOTNODECANNOTBEDELETED);

		m_pParentNode->removeChild(this);
	}


	void CCustomXMLNode::checkInvalidation()
	{
		if (m_bHasBeenInvalidated)
			throw CNMRException(NMR_ERROR_TRIEDTOACCESSINVALIDATEDNODE);
	}

	void CCustomXMLNode::invalidateNodeAndAllChildren()
	{
		if (!m_bHasBeenInvalidated) {
			m_bHasBeenInvalidated = true;
			for (auto pChild : m_Children)
				pChild->invalidateNodeAndAllChildren();
		}
	}

	std::vector<PCustomXMLNode> CCustomXMLNode::findChildrenWithName(const std::string& sName)
	{
		std::vector<PCustomXMLNode> childList;
		auto iNameIter = m_ChildMap.find(sName);
		if (iNameIter != m_ChildMap.end()) {
			for (auto iChild : iNameIter->second)
					childList.push_back(iChild.second);

		}

		return childList;
	}

	size_t CCustomXMLNode::getAttributeCount()
	{
		return m_Attributes.size ();
	}

	PCustomXMLAttribute CCustomXMLNode::getAttributeByIndex(size_t nIndex)
	{
		if (nIndex >= m_Attributes.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Attributes.at(nIndex);
	}


	bool CCustomXMLNode::hasAttribute(const std::string& sName)
	{
		auto iIter = m_AttributeMap.find(sName);
		return iIter != m_AttributeMap.end();
	}

	PCustomXMLAttribute CCustomXMLNode::findAttribute(const std::string& sName, bool bMustExist)
	{
		auto iIter = m_AttributeMap.find(sName);
		if (iIter != m_AttributeMap.end()) {
			return iIter->second;
		}
		else {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_XMLATTRIBUTENOTFOUND);

			return nullptr;
		}

	}

	PCustomXMLAttribute CCustomXMLNode::addAttribute(const std::string& sName, const std::string& sValue)
	{
		if (hasAttribute (sName))
			throw CNMRException(NMR_ERROR_DUPLICATEXMLATTRIBUTE);

		auto pAttribute = std::make_shared<CCustomXMLAttribute>(sName, sValue);
		m_Attributes.push_back(pAttribute);
		m_AttributeMap.insert(std::make_pair (sName, pAttribute));

		return pAttribute;
	}

	bool CCustomXMLNode::removeAttribute(const std::string& sName)
	{
		auto iIter = m_AttributeMap.find(sName);
		if (iIter != m_AttributeMap.end()) {
			m_AttributeMap.erase(iIter);
			m_Attributes.erase(std::remove_if(m_Attributes.begin(), m_Attributes.end(), [sName](const PCustomXMLAttribute& pAttribute)
			{
				return (pAttribute->getName () == sName);
			}));

			return true;
		}
		else {
			return false;
		}

	}


	bool CCustomXMLNode::removeAttributeByIndex(const uint64_t nIndex)
	{
		if (nIndex >= m_Attributes.size())
			return false;

		auto iIter = m_Attributes.begin();
		iIter += nIndex;

		std::string sName = (*iIter)->getName();

		m_Attributes.erase(iIter);
		m_AttributeMap.erase(sName);

		return true;
	}

	void CCustomXMLNode::writeContentToXML(CXmlWriter* pXMLWriter)
	{
		__NMRASSERT(pXMLWriter);

		for (auto pAttribute : m_Attributes) {
			std::string sName = pAttribute->getName();
			std::string sValue = pAttribute->getValue();
			pXMLWriter->WriteAttributeString(nullptr, sName.c_str(), nullptr, sValue.c_str());			
		}

		for (auto pChild : m_Children) {
			pXMLWriter->WriteStartElement(nullptr, pChild->getName ().c_str(), nullptr);
			pChild->writeContentToXML(pXMLWriter);
			pXMLWriter->WriteEndElement();
		}


	}


}

