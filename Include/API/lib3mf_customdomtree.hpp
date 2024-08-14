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

Abstract: This is the class declaration of CCustomDOMTree

*/


#ifndef __LIB3MF_CUSTOMDOMTREE
#define __LIB3MF_CUSTOMDOMTREE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_CustomXMLTree.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CCustomDOMTree 
**************************************************************************************************************************/

class CCustomDOMTree : public virtual ICustomDOMTree, public virtual CBase {
private:

	NMR::PCustomXMLTree m_pXMLTree;

public:
	CCustomDOMTree(NMR::PCustomXMLTree pXMLTree);

	virtual ~CCustomDOMTree();

	NMR::PCustomXMLTree getXMLTreeInstance ();

	/**
	* ICustomDOMTree::GetNameSpace - Returns the namespace identifier for the DOM Tree.
	* @return returns the namespace of the DOM Tree.
	*/
	std::string GetNameSpace() override;

	/**
	* ICustomDOMTree::GetRootNode - Returns root node of the document.
	* @return Root node of the document.
	*/
	ICustomXMLNode * GetRootNode() override;

	/**
	* ICustomDOMTree::SaveToString - Saves the XML document into a string.
	* @param[in] bAddLineBreaks - If true, line breaks and indentation will be added to the output string.
	* @return String with the XML Content.
	*/
	std::string SaveToString() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_CUSTOMDOMTREE
