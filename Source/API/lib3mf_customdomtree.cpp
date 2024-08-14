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

Abstract: This is a stub class definition of CCustomDOMTree

*/

#include "lib3mf_customdomtree.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_customxmlnode.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CCustomDOMTree 
**************************************************************************************************************************/

CCustomDOMTree::CCustomDOMTree(NMR::PCustomXMLTree pXMLTree)
	: m_pXMLTree (pXMLTree)
{
	if (pXMLTree.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

CCustomDOMTree::~CCustomDOMTree()
{

}

NMR::PCustomXMLTree CCustomDOMTree::getXMLTreeInstance()
{
	return m_pXMLTree;
}

std::string CCustomDOMTree::GetNameSpace()
{
	return m_pXMLTree->getNameSpace();
}

ICustomXMLNode * CCustomDOMTree::GetRootNode()
{
	return new CCustomXMLNode(m_pXMLTree, m_pXMLTree->getRootNode ());
}

std::string CCustomDOMTree::SaveToString()
{
	return m_pXMLTree->saveToString();
}

