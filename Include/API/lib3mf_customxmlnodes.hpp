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

Abstract: This is the class declaration of CCustomXMLNodes

*/


#ifndef __LIB3MF_CUSTOMXMLNODES
#define __LIB3MF_CUSTOMXMLNODES

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
 Class declaration of CCustomXMLNodes 
**************************************************************************************************************************/

class CCustomXMLNodes : public virtual ICustomXMLNodes, public virtual CBase {
private:
	
	NMR::PCustomXMLTree m_pXMLTree;
	std::vector<NMR::PCustomXMLNode> m_pXMLNodes;


protected:

public:

	CCustomXMLNodes(NMR::PCustomXMLTree pXMLTree, std::vector<NMR::PCustomXMLNode> pXMLNodes);

	virtual ~CCustomXMLNodes();

	Lib3MF_uint64 GetNodeCount() override;

	/**
	* ICustomXMLNodes::GetNode - Returns node instance. Fails if Index is out of range.
	* @param[in] nIndex - Index of the node to return (0-based).
	* @return XML Node node.
	*/
	ICustomXMLNode * GetNode(const Lib3MF_uint64 nIndex) override;

	/**
	* ICustomXMLNodes::CountNodesByName - Returns how many nodes of the XML Node have a specific name.
	* @param[in] sName - Name of the node.
	* @return returns the number of nodes with the specified name.
	*/
	Lib3MF_uint64 CountNodesByName(const std::string & sName) override;

	/**
	* ICustomXMLNodes::GetNodesByName - Returns all the nodes nodes of the XML Node with a specific name.
	* @param[in] sName - Name of the node.
	* @return returns the list of node nodes.
	*/
	ICustomXMLNodes * GetNodesByName(const std::string & sName) override;

	/**
	* ICustomXMLNodes::HasNode - Returns if a node with a specific name exist.
	* @param[in] sName - Name of the node.
	* @return returns if a node with a specific name exists.
	*/
	bool HasNode(const std::string & sName) override;

	/**
	* ICustomXMLNodes::HasUniqueNode - Returns if a node with a specific name exist once and only once.
	* @param[in] sName - Name of the node.
	* @return returns if a node with a specific name exists once and only once.
	*/
	bool HasUniqueNode(const std::string & sName) override;

	/**
	* ICustomXMLNodes::FindNode - Returns node with a specific name. Throws an error if name does not exist once and only once.
	* @param[in] sName - Name of the node.
	* @param[in] bMustExist - If true, the call fails if node does not exist. If falls, the call will return null if the node does not exist.
	* @return returns node instance.
	*/
	ICustomXMLNode * FindNode(const std::string & sName, const bool bMustExist) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_CUSTOMXMLNODES
