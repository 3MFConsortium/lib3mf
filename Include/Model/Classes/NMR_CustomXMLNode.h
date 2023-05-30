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

#ifndef __NMR_CUSTOMXMLNODE
#define __NMR_CUSTOMXMLNODE

#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>

#include "Common/Platform/NMR_XmlWriter.h"
#include "NMR_CustomXMLAttribute.h"

namespace NMR {

	class CCustomXMLNode;
	typedef std::shared_ptr <CCustomXMLNode> PCustomXMLNode;

	class CCustomXMLNode {
	private:
	
		std::string m_sName;

		// Parent of node (null means rootNode)
		CCustomXMLNode * m_pParentNode;

		// Flag if Node has been invalidated (e.g. removed)
		bool m_bHasBeenInvalidated;
		
		// Ordered list of Children
		std::vector<PCustomXMLNode> m_Children;

		// Child multi-map
		std::map<std::string, std::map<CCustomXMLNode*, PCustomXMLNode>> m_ChildMap;

		// Attributes
		std::vector<PCustomXMLAttribute> m_Attributes;
		std::map<std::string, PCustomXMLAttribute> m_AttributeMap;

		// Throws an error if the node has been deleted (flagged invalidated)
		void checkInvalidation();

		// Invalidates the node and all of its children
		void invalidateNodeAndAllChildren();

	public:
		CCustomXMLNode() = delete;
		CCustomXMLNode(CCustomXMLNode* pParentNode, const std::string & sName);
		
		~CCustomXMLNode();
		
		std::string getName ();

		CCustomXMLNode* getParentNode();
		
		std::vector<PCustomXMLNode> getChildren ();
		
		std::vector<PCustomXMLNode> findChildrenWithName (const std::string & sName);

		size_t getChildCount ();
		
		bool hasChild (const std::string & sName);
		
		bool hasUniqueChild(const std::string& sName);

		PCustomXMLNode addChild (const std::string & sName);

		PCustomXMLNode findUniqueChild (const std::string & sName, bool bMustExist);

		size_t removeChildrenWithName(const std::string& sName);

		void removeChild (CCustomXMLNode * pXMLNode);

		void remove();

		size_t getAttributeCount();

		PCustomXMLAttribute getAttributeByIndex(size_t nIndex);

		bool hasAttribute(const std::string& sName);

		PCustomXMLAttribute findAttribute(const std::string& sName, bool bMustExist);

		PCustomXMLAttribute addAttribute(const std::string& sName, const std::string& sValue);

		bool removeAttribute(const std::string& sName);

		bool removeAttributeByIndex(const uint64_t nIndex);

		void writeContentToXML (CXmlWriter * pXMLWriter);
	};

}

#endif // __NMR_CUSTOMXMLNODE
