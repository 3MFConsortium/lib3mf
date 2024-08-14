/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CCustomXMLNode

*/

#include "lib3mf_customxmlnode.hpp"
#include "lib3mf_customxmlnodes.hpp"
#include "lib3mf_customxmlattribute.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CCustomXMLNode 
**************************************************************************************************************************/
CCustomXMLNode::CCustomXMLNode(NMR::PCustomXMLTree pXMLTree, NMR::PCustomXMLNode pXMLNode)
	: m_pXMLTree (pXMLTree), m_pXMLNode (pXMLNode)
{
	if (pXMLTree.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pXMLNode.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}

NMR::PCustomXMLNode CCustomXMLNode::getNodeInstance()
{
	return m_pXMLNode;
}


CCustomXMLNode::~CCustomXMLNode()
{

}

std::string CCustomXMLNode::GetName()
{
	return m_pXMLNode->getName();
}

std::string CCustomXMLNode::GetNameSpace()
{
	return m_pXMLTree->getNameSpace();
}

Lib3MF_uint64 CCustomXMLNode::GetAttributeCount()
{
	return m_pXMLNode->getAttributeCount();
}

ICustomXMLAttribute * CCustomXMLNode::GetAttribute(const Lib3MF_uint64 nIndex)
{
	if (nIndex >= m_pXMLNode->getAttributeCount())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDATTRIBUTEINDEX);

	return new CCustomXMLAttribute(m_pXMLNode, m_pXMLNode->getAttributeByIndex (nIndex));
}

bool CCustomXMLNode::HasAttribute(const std::string & sName)
{
	return m_pXMLNode->hasAttribute(sName);
}

ICustomXMLAttribute * CCustomXMLNode::FindAttribute(const std::string & sName, const bool bMustExist)
{
	auto pAttribute = m_pXMLNode->findAttribute(sName, bMustExist);
	if (pAttribute.get() == nullptr)
		return nullptr;

	return new CCustomXMLAttribute(m_pXMLNode, pAttribute);
}

bool CCustomXMLNode::RemoveAttribute(const std::string & sName)
{
	return m_pXMLNode->removeAttribute(sName);
}

bool CCustomXMLNode::RemoveAttributeByIndex(const Lib3MF_uint64 nIndex)
{
	return m_pXMLNode->removeAttributeByIndex(nIndex);
}

void CCustomXMLNode::AddAttribute(const std::string & sName, const std::string & sValue)
{
	m_pXMLNode->addAttribute(sName, sValue);
}

void CCustomXMLNode::AddIntegerAttribute(const std::string & sName, const Lib3MF_int64 nValue)
{
	m_pXMLNode->addAttribute(sName, std::to_string (nValue));
}

void CCustomXMLNode::AddDoubleAttribute(const std::string & sName, const Lib3MF_double dValue)
{
	m_pXMLNode->addAttribute(sName, std::to_string(dValue));
}

void CCustomXMLNode::AddBoolAttribute(const std::string & sName, const bool bValue)
{
	if (bValue)
		m_pXMLNode->addAttribute(sName, "true");
	else
		m_pXMLNode->addAttribute(sName, "false");
}

ICustomXMLNodes * CCustomXMLNode::GetChildren()
{
	return new CCustomXMLNodes (m_pXMLTree, m_pXMLNode->getChildren());
}

Lib3MF_uint64 CCustomXMLNode::CountChildrenByName(const std::string & sName)
{
	auto childList = m_pXMLNode->findChildrenWithName(sName);
	return childList.size ();
}

ICustomXMLNodes * CCustomXMLNode::GetChildrenByName(const std::string & sName)
{
	auto childList = m_pXMLNode->findChildrenWithName(sName);
	return new CCustomXMLNodes(m_pXMLTree, childList);
}

bool CCustomXMLNode::HasChild(const std::string & sName)
{
	return m_pXMLNode->hasChild(sName);
}

bool CCustomXMLNode::HasUniqueChild(const std::string & sName)
{
	return m_pXMLNode->hasUniqueChild(sName);
}

ICustomXMLNode * CCustomXMLNode::FindChild(const std::string & sName, const bool bMustExist)
{
	auto pChild = m_pXMLNode->findUniqueChild(sName, bMustExist);
	if (pChild.get() == nullptr)
		return nullptr;

	return new CCustomXMLNode(m_pXMLTree, pChild);
}

ICustomXMLNode * CCustomXMLNode::AddChild(const std::string & sName)
{
	auto pChild = m_pXMLNode->addChild(sName);
	return new CCustomXMLNode (m_pXMLTree, pChild);
}

void CCustomXMLNode::RemoveChild(ICustomXMLNode* pChildInstance)
{
	if (pChildInstance == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CCustomXMLNode* pChildInstanceImpl = dynamic_cast<CCustomXMLNode*> (pChildInstance);
	if (pChildInstanceImpl == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pXMLNode->removeChild(pChildInstanceImpl->getNodeInstance ().get());
}

uint64_t CCustomXMLNode::RemoveChildrenWithName(const std::string & sName)
{
	return m_pXMLNode->removeChildrenWithName(sName);
}

void CCustomXMLNode::Remove()
{
	m_pXMLNode->remove();
}

