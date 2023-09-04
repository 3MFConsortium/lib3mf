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

Abstract: This is the class declaration of CFunctionReference

*/


#ifndef __LIB3MF_FUNCTIONREFERENCE
#define __LIB3MF_FUNCTIONREFERENCE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Common/NMR_FunctionReference.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CFunctionReference 
**************************************************************************************************************************/

class CFunctionReference : public virtual IFunctionReference, public virtual CBase {
private:

	/**
	* Put private members here.
	*/
	
protected:

	/**
	* Put protected members here.
	*/
	NMR::PFunctionReference m_pFunctionReference;
public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CFunctionReference(NMR::PFunctionReference pFunctionReference);

	/**
	* Public member functions to implement.
	*/

	/**
	* IFunctionReference::GetFunctionResourceID - Returns the UniqueResourceID of the Function.
	* @return returns the UniqueResourceID.
	*/
	Lib3MF_uint32 GetFunctionResourceID() override;

	/**
	* IFunctionReference::SetFunctionResourceID - Sets the UniqueResourceID to refer to.
	* @param[in] nUniqueResourceID - UniqueResourceID of the function
	*/
	void SetFunctionResourceID(const Lib3MF_uint32 nUniqueResourceID) override;


	/**
	* IFunctionReference::GetTransform - Returns the transformation matrix into the coordinate system of the referenced Function.
	* @return the transformation matrix
	*/
	Lib3MF::sTransform GetTransform() override;

	/**
	* IFunctionReference::SetTransform - Sets the transformation matrix into the coordinate system of the referenced Function.
	* @param[in] Transform - new transformation matrix
	*/
	void SetTransform(const Lib3MF::sTransform Transform) override;

		/**
	* IFunctionReference::GetOutputName - Returns the name of the function output to use.
	* @return the name of the function output
	*/
	std::string GetOutputName() override;

	/**
	* IFunctionReference::SetOutputName - Sets the name of the function output to use.
	* @param[in] sOutputName - new name of the function output
	*/
	void SetOutputName(const std::string & sOutputName) override;
};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_FUNCTIONREFERENCE
