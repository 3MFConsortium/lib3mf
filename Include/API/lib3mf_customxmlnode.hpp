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

Abstract: This is the class declaration of CCustomXMLNode

*/


#ifndef __LIB3MF_CUSTOMXMLNODE
#define __LIB3MF_CUSTOMXMLNODE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_CustomXMLNode.h"
#include "Model/Classes/NMR_CustomXMLTree.h"


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CCustomXMLNode 
**************************************************************************************************************************/

class CCustomXMLNode : public virtual ICustomXMLNode, public virtual CBase {
private:

	NMR::PCustomXMLTree m_pXMLTree;
	NMR::PCustomXMLNode m_pXMLNode;

public:

	CCustomXMLNode(NMR::PCustomXMLTree pXMLTree, NMR::PCustomXMLNode pXMLNode);

	virtual ~CCustomXMLNode();

	NMR::PCustomXMLNode getNodeInstance();

	/**
	* ICustomXMLNode::GetName - Retrieves name of the node.
	* @return returns the name of the node.
	*/
	std::string GetName() override;

	/**
	* ICustomXMLNode::GetNameSpace - Retrieves namespace of the node.
	* @return returns the namespace of the node.
	*/
	std::string GetNameSpace() override;

	/**
	* ICustomXMLNode::GetAttributeCount - Returns number of attributes.
	* @return returns the number of attributes.
	*/
	Lib3MF_uint64 GetAttributeCount() override;

	/**
	* ICustomXMLNode::GetAttribute - Returns attribute instance. Fails if Index is out of range.
	* @param[in] nIndex - Index of the attribute to return (0-based).
	* @return XML Document attribute.
	*/
	ICustomXMLAttribute * GetAttribute(const Lib3MF_uint64 nIndex) override;

	/**
	* ICustomXMLNode::HasAttribute - Returns if attribute of a specific name exists.
	* @param[in] sName - Name of the attribute.
	* @return Returns if the attribute exists.
	*/
	bool HasAttribute(const std::string & sName) override;

	/**
	* ICustomXMLNode::FindAttribute - Returns attribute instance of a specific name. 
	* @param[in] sName - Name of the attribute.
	* @param[in] bMustExist - If true, the call fails if attribute does not exist. If falls, the call will return null if the attribute does not exist.
	* @return XML Document attribute.
	*/
	ICustomXMLAttribute * FindAttribute(const std::string & sName, const bool bMustExist) override;

	/**
	* ICustomXMLNode::RemoveAttribute - Removes the attribute with a specific name. Does nothing if attribute does not exist.
	* @param[in] sName - Name of the attribute.
	*/
	bool RemoveAttribute(const std::string & sName) override;

	/**
	* ICustomXMLNode::RemoveAttributeByIndex - Removes the attribute with a specific index. Does nothing if attribute does not exist.
	* @param[in] nIndex - Index of the attribute to remove (0-based).
	*/
	bool RemoveAttributeByIndex(const Lib3MF_uint64 nIndex) override;

	/**
	* ICustomXMLNode::AddAttribute - Adds an attribute with a specific name and string value. Fails if attribute already exists.
	* @param[in] sName - Name of the attribute.
	* @param[in] sValue - Value of the attribute.
	*/
	void AddAttribute(const std::string & sName, const std::string & sValue) override;

	/**
	* ICustomXMLNode::AddIntegerAttribute - Adds an attribute with a specific name and integer value. Fails if attribute already exists.
	* @param[in] sName - Name of the attribute.
	* @param[in] nValue - Value of the attribute.
	*/
	void AddIntegerAttribute(const std::string & sName, const Lib3MF_int64 nValue) override;

	/**
	* ICustomXMLNode::AddDoubleAttribute - Adds an attribute with a specific name and double value. Fails if attribute already exists.
	* @param[in] sName - Name of the attribute.
	* @param[in] dValue - Value of the attribute.
	*/
	void AddDoubleAttribute(const std::string & sName, const Lib3MF_double dValue) override;

	/**
	* ICustomXMLNode::AddBoolAttribute - Adds an attribute with a specific name and bool value. Fails if attribute already exists.
	* @param[in] sName - Name of the attribute.
	* @param[in] bValue - Value of the attribute.
	*/
	void AddBoolAttribute(const std::string & sName, const bool bValue) override;

	/**
	* ICustomXMLNode::GetChildren - Returns all the child nodes of the XML Node.
	* @return returns the list of child nodes.
	*/
	ICustomXMLNodes * GetChildren() override;

	/**
	* ICustomXMLNode::CountChildrenByName - Returns how many children of the XML Node have a specific name.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the node.
	* @return returns the number children with the specified name.
	*/
	Lib3MF_uint64 CountChildrenByName(const std::string & sName) override;

	/**
	* ICustomXMLNode::GetChildrenByName - Returns all the child nodes of the XML Node with a specific name.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the child.
	* @return returns the list of child nodes.
	*/
	ICustomXMLNodes * GetChildrenByName(const std::string & sName) override;

	/**
	* ICustomXMLNode::HasChild - Returns if a child with a specific name exist.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the child.
	* @return returns if a child with a specific name exists.
	*/
	bool HasChild(const std::string & sName) override;

	/**
	* ICustomXMLNode::HasUniqueChild - Returns if a child with a specific name exist once and only once.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the child.
	* @return returns if a child with a specific name exists once and only once.
	*/
	bool HasUniqueChild(const std::string & sName) override;

	/**
	* ICustomXMLNode::FindChild - Returns child with a specific name. Throws an error if name does not exist once and only once.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the child.
	* @param[in] bMustExist - If true, the call fails if child does not exist. If falls, the call will return null if the child does not exist.
	* @return returns child instance or null.
	*/
	ICustomXMLNode * FindChild(const std::string & sName, const bool bMustExist) override;

	/**
	* ICustomXMLNode::AddChild - Adds a new child with a specific name.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the child.
	* @return returns child instance.
	*/
	ICustomXMLNode * AddChild(const std::string & sName) override;

	/**
	* ICustomXMLNode::RemoveChild - Removes a child with a specific name. All subsequent calls to the child will fail after the call.
	* @param[in] pChildInstance - child instance to remove. Fails if given instance is not a child of the node.
	*/
	void RemoveChild(ICustomXMLNode* pChildInstance) override;

	/**
	* ICustomXMLNode::RemoveChildrenWithName - Removes all children with a specific name. Does nothing if no child with the name exists. . All subsequent calls to the deleted children will fail after the call.
	* @param[in] sNameSpace - New namespace of the child. MUST be either an empty string for the root namespace, or previously being registered with the document.
	* @param[in] sName - Name of the children.
	*/
	uint64_t RemoveChildrenWithName(const std::string & sName) override;

	/**
	* ICustomXMLNode::Remove - Removes the node from its parent. The root node of the document can not be removed.
	*/
	void Remove() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_CUSTOMXMLNODE
