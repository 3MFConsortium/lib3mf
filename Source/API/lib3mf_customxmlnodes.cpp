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

Abstract: This is a stub class definition of CCustomXMLNodes

*/

#include "lib3mf_customxmlnodes.hpp"
#include "lib3mf_customxmlnode.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CCustomXMLNodes 
**************************************************************************************************************************/

CCustomXMLNodes::CCustomXMLNodes(NMR::PCustomXMLTree pXMLTree, std::vector<NMR::PCustomXMLNode> pXMLNodes)
	: m_pXMLTree (pXMLTree), m_pXMLNodes (pXMLNodes)
{
	if (pXMLTree.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

CCustomXMLNodes::~CCustomXMLNodes()
{

}


Lib3MF_uint64 CCustomXMLNodes::GetNodeCount()
{
	return m_pXMLNodes.size();
}

ICustomXMLNode * CCustomXMLNodes::GetNode(const Lib3MF_uint64 nIndex)
{
	if (nIndex >= m_pXMLNodes.size ())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDNODEINDEX);

	return new CCustomXMLNode(m_pXMLTree, m_pXMLNodes.at(nIndex));
}

Lib3MF_uint64 CCustomXMLNodes::CountNodesByName(const std::string & sName)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICustomXMLNodes * CCustomXMLNodes::GetNodesByName(const std::string & sName)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CCustomXMLNodes::HasNode(const std::string & sName)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CCustomXMLNodes::HasUniqueNode(const std::string & sName)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICustomXMLNode * CCustomXMLNodes::FindNode(const std::string & sName, const bool bMustExist)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

