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

Abstract: This is the class declaration of CComposeMatrixFromRowVectorsNode

*/


#ifndef __LIB3MF_COMPOSEMATRIXFROMROWVECTORSNODE
#define __LIB3MF_COMPOSEMATRIXFROMROWVECTORSNODE

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
 Class declaration of CComposeMatrixFromRowVectorsNode 
**************************************************************************************************************************/

class CComposeMatrixFromRowVectorsNode : public virtual IComposeMatrixFromRowVectorsNode, public virtual CImplicitNode {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:
	CComposeMatrixFromRowVectorsNode(NMR::PModelImplicitNode pImplicitNode);
	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	/**
	* IComposeMatrixFromRowVectorsNode::GetInputRow0 - Retrieves the input for the first row
	* @return the input for the first row
	*/
	IImplicitPort * GetInputRow0() override;

	/**
	* IComposeMatrixFromRowVectorsNode::GetInputRow1 - Retrieves the input for the second row
	* @return the input for the second row
	*/
	IImplicitPort * GetInputRow1() override;

	/**
	* IComposeMatrixFromRowVectorsNode::GetInputRow2 - Retrieves the input for the third row
	* @return the input for the third row
	*/
	IImplicitPort * GetInputRow2() override;

	/**
	* IComposeMatrixFromRowVectorsNode::GetInputRow3 - Retrieves the input for the fourth row
	* @return the input for the fourth row
	*/
	IImplicitPort * GetInputRow3() override;

	/**
	* IComposeMatrixFromRowVectorsNode::GetOutputMatrix - Retrieves the output
	* @return the output
	*/
	IImplicitPort * GetOutputMatrix() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_COMPOSEMATRIXFROMROWVECTORSNODE
