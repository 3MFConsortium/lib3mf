/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CComposeMatrixNode

*/


#ifndef __LIB3MF_COMPOSEMATRIXNODE
#define __LIB3MF_COMPOSEMATRIXNODE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_implicitnode.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CComposeMatrixNode 
**************************************************************************************************************************/

class CComposeMatrixNode : public virtual IComposeMatrixNode, public virtual CImplicitNode {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:
	CComposeMatrixNode(NMR::PModelImplicitNode pImplicitNode);

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	/**
	* IComposeMatrixNode::GetInputM00 - Retrieves the input for the element 0 0
	* @return the input for the m00 element
	*/
	IImplicitPort * GetInputM00() override;

	/**
	* IComposeMatrixNode::GetInputM01 - Retrieves the input for the element 0 1
	* @return the input for the m01 element
	*/
	IImplicitPort * GetInputM01() override;

	/**
	* IComposeMatrixNode::GetInputM02 - Retrieves the input for the element 0 2
	* @return the input for the m02 element
	*/
	IImplicitPort * GetInputM02() override;

	/**
	* IComposeMatrixNode::GetInputM03 - Retrieves the input for the element 0 3
	* @return the input for the m03 element
	*/
	IImplicitPort * GetInputM03() override;

	/**
	* IComposeMatrixNode::GetInputM10 - Retrieves the input for the element 1 0
	* @return the input for the m10 element
	*/
	IImplicitPort * GetInputM10() override;

	/**
	* IComposeMatrixNode::GetInputM11 - Retrieves the input for the element 1 1
	* @return the input for the m11 element
	*/
	IImplicitPort * GetInputM11() override;

	/**
	* IComposeMatrixNode::GetInputM12 - Retrieves the input for the element 1 2
	* @return the input for the m12 element
	*/
	IImplicitPort * GetInputM12() override;

	/**
	* IComposeMatrixNode::GetInputM13 - Retrieves the input for the element 1 3
	* @return the input for the m3 element
	*/
	IImplicitPort * GetInputM13() override;

	/**
	* IComposeMatrixNode::GetInputM20 - Retrieves the input for the element 2 0
	* @return the input for the m2 element
	*/
	IImplicitPort * GetInputM20() override;

	/**
	* IComposeMatrixNode::GetInputM21 - Retrieves the input for the element 2 1
	* @return 
	*/
	IImplicitPort * GetInputM21() override;

	/**
	* IComposeMatrixNode::GetInputM22 - Retrieves the input for the element 2 2
	* @return the input for the m22 element
	*/
	IImplicitPort * GetInputM22() override;

	/**
	* IComposeMatrixNode::GetInputM23 - Retrieves the input for the element 2 3
	* @return the input for the m23 element
	*/
	IImplicitPort * GetInputM23() override;

	/**
	* IComposeMatrixNode::GetInputM30 - Retrieves the input for the element 3 0
	* @return the input for the m30 element
	*/
	IImplicitPort * GetInputM30() override;

	/**
	* IComposeMatrixNode::GetInputM31 - Retrieves the input for the element 3 1
	* @return the input for the m31 element
	*/
	IImplicitPort * GetInputM31() override;

	/**
	* IComposeMatrixNode::GetInputM32 - Retrieves the input for the element 3 2
	* @return the input for the m32 element
	*/
	IImplicitPort * GetInputM32() override;

	/**
	* IComposeMatrixNode::GetInputM33 - Retrieves the input for the element 3 3
	* @return the input for the m33 element
	*/
	IImplicitPort * GetInputM33() override;

	/**
	* IComposeMatrixNode::GetOutputMatrix - Retrieves the output
	* @return the output
	*/
	IImplicitPort * GetOutputResult() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_COMPOSEMATRIXNODE
