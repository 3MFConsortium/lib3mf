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

Abstract: This is the class declaration of CSliceStack

*/


#ifndef __LIB3MF_SLICESTACK
#define __LIB3MF_SLICESTACK

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Model/Classes/NMR_ModelSliceStack.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CSliceStack 
**************************************************************************************************************************/

class CSliceStack : public virtual ISliceStack, public virtual CResource {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/
	NMR::PModelSliceStack sliceStack();

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CSliceStack(NMR::PModelSliceStack pSliceStack);


	/**
	* Public member functions to implement.
	*/

	double GetBottomZ();

	Lib3MF_uint64 GetSliceCount ();

	ISlice * AddSlice(const double fZTop);

	ISlice * GetSlice (const Lib3MF_uint64 nSliceIndex);

	Lib3MF_uint64 GetSliceRefCount();

	void AddSliceStackReference(ISliceStack* pTheSliceStack) ;

	ISliceStack * GetSliceStackReference(const Lib3MF_uint64 nSliceRefIndex);

	void CollapseSliceReferences();

	virtual void SetOwnPath(const std::string & sPath);

	virtual std::string GetOwnPath();
};

} // namespace Impl
} // namespace Lib3MF

#pragma warning( pop )
#endif // __LIB3MF_SLICESTACK
