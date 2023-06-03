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

Abstract: This is the class declaration of CVector3DFieldFunction

*/


#ifndef __LIB3MF_VECTOR3DFIELDFUNCTION
#define __LIB3MF_VECTOR3DFIELDFUNCTION

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_vector3dfield.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelVector3DFieldFunction.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CVector3DFieldFunction 
**************************************************************************************************************************/

class CVector3DFieldFunction : public virtual IVector3DFieldFunction, public virtual CVector3DField {
private:

	/**
	* Put private members here.
	*/
	NMR::CModelVector3DFieldFunction* m_pVector3DFieldFunction = nullptr;
protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CVector3DFieldFunction() = delete;
	CVector3DFieldFunction(NMR::PModelVector3DFieldFunction pVector3DFieldFunction);

	/**
	* Public member functions to implement.
	*/

	/**
	* IVector3DFieldFunction::SetFunction - Sets the function to be used for the scalar field.
	* @param[in] pFunction - the function to be used for the scalar field
	*/
	void SetFunction(IImplicitFunction* pFunction) override;

	/**
	* IVector3DFieldFunction::GetFunction - Returns the function to be used for the scalar field.
	* @return the function to be used for the scalar field
	*/
	IImplicitFunction * GetFunction() override;

	/**
	* IVector3DFieldFunction::SetOutput - Sets the name of the function output to be used for the scalar field. The output must be a scalar
	* @param[in] sName - the name of the scalar function output
	*/
	void SetOutput(const std::string & sName) override;

	/**
	* IVector3DFieldFunction::GetOutput - Returns the name of the function output used for the scalar field.
	* @return the name of the scalar function output
	*/
	std::string GetOutput() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_VECTOR3DFIELDFUNCTION
