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

Abstract: This is the class declaration of CBeamLattice

*/


#ifndef __LIB3MF_BEAMLATTICE
#define __LIB3MF_BEAMLATTICE

#include "lib3mf_interfaces.hpp"
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

#include "Common/Mesh/NMR_Mesh.h"
#include "Model/Classes/NMR_ModelMeshBeamLatticeAttributes.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CBeamLattice 
**************************************************************************************************************************/

class CBeamLattice : public virtual IBeamLattice, public virtual CBase {
private:

	/**
	* Put private members here.
	*/
	NMR::CMesh& m_mesh;
	NMR::PModelMeshBeamLatticeAttributes m_pAttributes;
	NMR::PModelMeshObject m_pMeshObject;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CBeamLattice(NMR::PModelMeshObject pMeshObject, NMR::PModelMeshBeamLatticeAttributes pAttributes);

	/**
	* Public member functions to implement.
	*/

	Lib3MF_double GetMinLength ();

	void SetMinLength (const Lib3MF_double dMinLength);

	void GetClipping (eBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nUniqueResourceID);

	void SetClipping (const eBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nUniqueResourceID);

	bool GetRepresentation(Lib3MF_uint32 & nUniqueResourceID);

	void SetRepresentation(const Lib3MF_uint32 nUniqueResourceID);

	void GetBallOptions (eBeamLatticeBallMode & eBallMode, Lib3MF_double & dBallRadius);

	void SetBallOptions (const eBeamLatticeBallMode eBallMode, const Lib3MF_double dBallRadius);

	Lib3MF_uint32 GetBeamCount ();

	sLib3MFBeam GetBeam (const Lib3MF_uint32 nIndex);

	Lib3MF_uint32 AddBeam (const sLib3MFBeam BeamInfo);

	void SetBeam (const Lib3MF_uint32 nIndex, const sLib3MFBeam BeamInfo);

	void SetBeams (const Lib3MF_uint64 nBeamInfoBufferSize, const sLib3MFBeam * pBeamInfoBuffer);

	void GetBeams (Lib3MF_uint64 nBeamInfoBufferSize, Lib3MF_uint64 * pBeamInfoNeededCount, sLib3MFBeam * pBeamInfoBuffer);

	Lib3MF_uint32 GetBallCount ();

	sLib3MFBall GetBall (const Lib3MF_uint32 nIndex);

	Lib3MF_uint32 AddBall (const sLib3MFBall BallInfo);

	void SetBall (const Lib3MF_uint32 nIndex, const sLib3MFBall BallInfo);

	void SetBalls (const Lib3MF_uint64 nBallInfoBufferSize, const sLib3MFBall * pBallInfoBuffer);

	void GetBalls (Lib3MF_uint64 nBeamInfoBufferSize, Lib3MF_uint64 * pBallInfoNeededCount, sLib3MFBall * pBallInfoBuffer);

	Lib3MF_uint32 GetBeamSetCount ();

	IBeamSet * AddBeamSet ();

	IBeamSet * GetBeamSet (const Lib3MF_uint32 nIndex);

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_BEAMLATTICE
