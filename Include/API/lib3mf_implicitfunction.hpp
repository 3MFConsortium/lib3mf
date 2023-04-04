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

Abstract: This is the class declaration of CImplicitFunction

*/


#ifndef __LIB3MF_IMPLICITFUNCTION
#define __LIB3MF_IMPLICITFUNCTION

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CImplicitFunction 
**************************************************************************************************************************/

class CImplicitFunction : public virtual IImplicitFunction, public virtual CBase {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	/**
	* IImplicitFunction::GetIdentifier - Retrieves the identifier of the function
	* @return the identifier
	*/
	std::string GetIdentifier() override;

	/**
	* IImplicitFunction::SetIdentifier - Sets the identifier of the function
	* @param[in] sIdentifier - the identifier
	*/
	void SetIdentifier(const std::string & sIdentifier) override;

	/**
	* IImplicitFunction::GetDisplayName - Retrieves the display name of the function
	* @return the display name
	*/
	std::string GetDisplayName() override;

	/**
	* IImplicitFunction::SetDisplayName - Sets the display name of the function
	* @param[in] sDisplayName - the display name
	*/
	void SetDisplayName(const std::string & sDisplayName) override;

	/**
	* IImplicitFunction::AddNode - Add a node
	* @param[in] sNodeType - the type of the node
	* @param[in] sIdentifier - the identifier of the input
	* @param[in] sDisplayName - the display name of the input
	*/
	void AddNode(const std::string & sNodeType, const std::string & sIdentifier, const std::string & sDisplayName) override;

	/**
	* IImplicitFunction::GetNodes - Retrieves the nodes
	* @return the accessor to the nodes
	*/
	INodeAccessor * GetNodes() override;

	/**
	* IImplicitFunction::AddInput - Add an input
	* @param[in] sIdentifier - the identifier of the input
	* @param[in] sDisplayName - the display name of the input
	*/
	void AddInput(const std::string & sIdentifier, const std::string & sDisplayName) override;

	/**
	* IImplicitFunction::GetInputs - Retrieves the inputs
	* @return the accessor to the inputs
	*/
	IImplicitPortAccessor * GetInputs() override;

	/**
	* IImplicitFunction::AddOutput - Add an output
	* @param[in] sIdentifier - the identifier of the output
	* @param[in] sDisplayName - the display name of the output
	*/
	void AddOutput(const std::string & sIdentifier, const std::string & sDisplayName) override;

	/**
	* IImplicitFunction::GetOutputs - Retrieves the outputs
	* @return the accessor to the outputs
	*/
	IImplicitPortAccessor * GetOutputs() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMPLICITFUNCTION
