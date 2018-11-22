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

Abstract: This is the class declaration of CLib3MFMeshObject

*/


#ifndef __LIB3MF_LIB3MFMESHOBJECT
#define __LIB3MF_LIB3MFMESHOBJECT

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_object.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.


namespace Lib3MF {


/*************************************************************************************************************************
 Class declaration of CLib3MFMeshObject 
**************************************************************************************************************************/

class CLib3MFMeshObject : public virtual ILib3MFMeshObject, public virtual CLib3MFObject {
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

	Lib3MF_uint32 GetVertexCount ();

	Lib3MF_uint32 GetTriangleCount ();

	void SetVertex (const Lib3MF_uint32 nIndex, const sLib3MFPosition Coordinates);

	Lib3MF_uint32 AddVertex (const sLib3MFPosition Coordinates);

	sLib3MFTriangle GetTriangle (const Lib3MF_uint32 nIndex);

	void SetTriangle (const Lib3MF_uint32 nIndex, const sLib3MFTriangle Indices);

	Lib3MF_uint32 AddTriangle (const sLib3MFTriangle Indices);

	void GetTriangleIndices (Lib3MF_uint32 nIndicesBufferSize, Lib3MF_uint32* pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer);

	void SetTriangleIndices (const Lib3MF_uint32 nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer);

	Lib3MF_double GetBeamLattice_MinLength ();

	void SetBeamLattice_MinLength (const Lib3MF_double dMinLength);

	Lib3MF_double GetBeamLattice_Radius ();

	void SetBeamLattice_Radius (const Lib3MF_double dRadius);

	eLib3MFBeamLatticeCapMode GetBeamLattice_CapMode ();

	void SetBeamLattice_CapMode (const eLib3MFBeamLatticeCapMode eCapMode);

	void GetBeamLattice_Clipping (eLib3MFBeamLatticeClipMode & eClipMode, Lib3MF_uint32 & nResourceID);

	void SetBeamLattice_Clipping (const eLib3MFBeamLatticeClipMode eClipMode, const Lib3MF_uint32 nResourceID);

	void GetBeamLattice_Representation (bool & bHasRepresentation, Lib3MF_uint32 & nResourceID);

	void SetBeamLattice_Representation (const Lib3MF_uint32 nResourceID);

	Lib3MF_uint32 GetBeamCount ();

	Lib3MF_uint32 AddBeam (const sLib3MFBeam BeamInfo);

	void SetBeam (const Lib3MF_uint32 nIndex, const sLib3MFBeam BeamInfo);

	Lib3MF_uint32 GetBeamSetCount ();

	ILib3MFBeamSet * AddBeamSet ();

	ILib3MFBeamSet * GetBeamSet (const Lib3MF_uint32 nIndex);

	bool IsManifoldAndOriented ();

};

}

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFMESHOBJECT
