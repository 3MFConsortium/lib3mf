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

Abstract: This is the class declaration of CFunction

*/


#ifndef __LIB3MF_FUNCTION
#define __LIB3MF_FUNCTION

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelFunction.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CFunction 
**************************************************************************************************************************/

class CFunction : public virtual IFunction, public virtual CResource {
private:
	/**
	 * Put private members here.
	 */
	NMR::CModelFunction* function();

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CFunction() = delete;
	CFunction(NMR::PModelResource pResource);

	/**
	* Public member functions to implement.
	*/

	/**
	* IFunction::GetDisplayName - Retrieves the display name of the function
	* @return the display name
	*/
	std::string GetDisplayName() override;

	/**
	* IFunction::SetDisplayName - Sets the display name of the function
	* @param[in] sDisplayName - the display name
	*/
	void SetDisplayName(const std::string & sDisplayName) override;

	/**
	* IFunction::AddInput - Add an input
	* @param[in] sIdentifier - the identifier of the input
	* @param[in] sDisplayName - the display name of the input
	* @param[in] eType - the type of the input
	* @return The added input port
	*/
	IImplicitPort * AddInput(const std::string & sIdentifier, const std::string & sDisplayName, const Lib3MF::eImplicitPortType eType) override;

	/**
	* IFunction::GetInputs - Retrieves the inputs
	* @return iterator for the list of inputs
	*/
	IImplicitPortIterator * GetInputs() override;

	/**
	* IFunction::RemoveInput - Removes an input
	* @param[in] pInput - The input to be removed
	*/
	void RemoveInput(IImplicitPort* pInput) override;

	/**
	* IFunction::AddOutput - Add an output
	* @param[in] sIdentifier - the identifier of the output
	* @param[in] sDisplayName - the display name of the output
	* @param[in] eType - the type of the input
	* @return The added input port
	*/
	IImplicitPort * AddOutput(const std::string & sIdentifier, const std::string & sDisplayName, const Lib3MF::eImplicitPortType eType) override;

	/**
	* IFunction::GetOutputs - Retrieves the outputs
	* @return iterator for the outputs
	*/
	IImplicitPortIterator * GetOutputs() override;

	/**
	* IFunction::RemoveOutput - Removes an output
	* @param[in] pOutput - The output to be removed
	*/
	void RemoveOutput(IImplicitPort* pOutput) override;

	/**
	* IFunction::FindInput - Retrieves an input
	* @param[in] sIdentifier - the identifier of the input
	* @return the input port
	*/
	IImplicitPort * FindInput(const std::string & sIdentifier) override;

	/**
	* IFunction::FindOutput - Retrieves an output
	* @param[in] sIdentifier - the identifier of the output
	* @return the output port
	*/
	IImplicitPort * FindOutput(const std::string & sIdentifier) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_FUNCTION
