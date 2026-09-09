/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CDisp2DGroup

*/


#ifndef __LIB3MF_DISP2DGROUP
#define __LIB3MF_DISP2DGROUP

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelDisp2DGroup.h"


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CDisp2DGroup
**************************************************************************************************************************/

class CDisp2DGroup : public virtual IDisp2DGroup, public virtual CResource {
private:

	/**
	* Put private members here.
	*/

protected:

	NMR::PModelDisp2DGroupResource disp2DGroup();

public:

	CDisp2DGroup(NMR::PModelDisp2DGroupResource pResource);


	/**
	* Public member functions to implement.
	*/

	IDisplacement2D * GetDisplacement2D() override;

	INormVectorGroup * GetNormalVectorGroup() override;

	Lib3MF_double GetHeight() override;

	void SetHeight(const Lib3MF_double dHeight) override;

	Lib3MF_double GetOffset() override;

	void SetOffset(const Lib3MF_double dOffset) override;

	Lib3MF_uint32 GetCount() override;

	Lib3MF_uint32 AddCoordinate(const Lib3MF::sDisplacement2DCoordinate Coordinate) override;

	Lib3MF::sDisplacement2DCoordinate GetCoordinate(const Lib3MF_uint32 nIndex) override;

	void SetCoordinate(const Lib3MF_uint32 nIndex, const Lib3MF::sDisplacement2DCoordinate Coordinate) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_DISP2DGROUP
