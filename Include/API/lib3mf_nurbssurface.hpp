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

Abstract: This is the class declaration of CLib3MFNurbsSurface

*/


#ifndef __LIB3MF_LIB3MFNURBSSURFACE
#define __LIB3MF_LIB3MFNURBSSURFACE

#include "lib3mf_interfaces.hpp"
#include "Model/Classes/NMR_ModelNurbsSurface.h"
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  

// Parent classes
#include "lib3mf_resource.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.


namespace Lib3MF {
namespace Impl {

/*************************************************************************************************************************
 Class declaration of CLib3MFNurbsSurface 
**************************************************************************************************************************/

class CLib3MFNurbsSurface : public virtual ILib3MFNurbsSurface, public virtual CLib3MFResource {
private:

	/**
	* Put private members here.
	*/

	NMR::PModelNurbsSurface m_pNurbsSurface;

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

	CLib3MFNurbsSurface() = delete;
	CLib3MFNurbsSurface (NMR::PModelNurbsSurface pNurbsSurface);
	~CLib3MFNurbsSurface ();

	Lib3MF_uint32 GetDegreeU ();

	Lib3MF_uint32 GetDegreeV ();

	bool CheckValidity ();

	void GetKnotsU (Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer);

	void SetKnotsU (const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer);

	void AddKnotU (const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue);

	void GetKnotsV(Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer);

	void SetKnotsV(const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer);

	void AddKnotV(const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue);

	sLib3MFNURBSUVCoordinate GetUVCoordinate(const Lib3MF_uint32 nId);

	void GetUVCoordinates(Lib3MF_uint64 nUVCoordinatesBufferSize, Lib3MF_uint64* pUVCoordinatesNeededCount, sLib3MFNURBSUVCoordinate * pUVCoordinatesBuffer);

	Lib3MF_uint32 AddUVCoordinate(const Lib3MF_double dU, const Lib3MF_double dV);

	void RemoveUVCoordinate(const Lib3MF_uint32 nId);

	bool HasUVCoordinate(const Lib3MF_uint32 nId);

	void SetControlPoint(const Lib3MF_uint32 nIndexU, const Lib3MF_uint32 nIndexV, const Lib3MF_double dX, const Lib3MF_double dY, const Lib3MF_double dZ, const Lib3MF_double dW);

	void GetControlPoint(const Lib3MF_uint32 nIndexU, const Lib3MF_uint32 nIndexV, Lib3MF_double & dX, Lib3MF_double & dY, Lib3MF_double & dZ, Lib3MF_double & dW);

	sLib3MFNURBSEdgeMapping GetEdgeMapping(const Lib3MF_uint32 nId);

	void GetEdgeMappings(Lib3MF_uint64 nEdgeMappingsBufferSize, Lib3MF_uint64* pEdgeMappingsNeededCount, sLib3MFNURBSEdgeMapping * pEdgeMappingsBuffer);
	
	Lib3MF_uint32 AddEdgeMapping(const Lib3MF_uint32 nCurveID, const Lib3MF_double dT1, const Lib3MF_double dT2);
	
	void RemoveEdgeMapping(const Lib3MF_uint32 nId);

	bool HasEdgeMapping(const Lib3MF_uint32 nId);

	void AddEdgeMappingUVCoordinate(const Lib3MF_uint32 nId, const Lib3MF_double dU, const Lib3MF_double dV);

	void ClearEdgeMappingUVCoordinates(const Lib3MF_uint32 nId);

	void GetEdgeMappingUVCoordinate(const Lib3MF_uint32 nId, const Lib3MF_uint32 nIndex, Lib3MF_double & dU, Lib3MF_double & dV);

	Lib3MF_uint32 GetEdgeMappingUVCoordinateCount(const Lib3MF_uint32 nId);


};

} // namespace Impl
} // namespace Lib3MF

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFNURBSSURFACE
