/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CNurbsCurve

*/


#ifndef __LIB3MF_LIB3MFNURBSCURVE
#define __LIB3MF_LIB3MFNURBSCURVE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Model/Classes/NMR_ModelNurbsCurve.h"
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CNurbsCurve 
**************************************************************************************************************************/

class CNurbsCurve : public virtual INurbsCurve, public virtual CResource {
private:

	NMR::PModelNurbsCurve m_pNurbsCurve;
protected:

public:

	CNurbsCurve() = delete;
	CNurbsCurve(NMR::PModelNurbsCurve pNurbsCurve);
	~CNurbsCurve();

	Lib3MF_uint32 GetDegree ();

	bool CheckValidity ();

	void GetKnots (Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer);

	void SetKnots (const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer);

	void AddKnot (const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue);

	void SetControlPoint (const Lib3MF_uint32 nIndex, const Lib3MF_double dX, const Lib3MF_double dY, const Lib3MF_double dZ, const Lib3MF_double dW);

	void GetControlPoint (const Lib3MF_uint32 nIndex, Lib3MF_double & dX, Lib3MF_double & dY, Lib3MF_double & dZ, Lib3MF_double & dW);

};

} // namespace Impl
} // namespace Lib3MF

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFNURBSCURVE
