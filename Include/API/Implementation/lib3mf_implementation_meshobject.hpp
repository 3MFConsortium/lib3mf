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

Abstract: This is the class declaration of CInternalLib3MFMeshObject
Interface version: 1.2.2

*/


#ifndef __LIB3MF_LIB3MFMESHOBJECT
#define __LIB3MF_LIB3MFMESHOBJECT

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_implementation_object.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "lib3mf_implementation_model.hpp"

#include "Model/Classes/NMR_ModelMeshObject.h" 

namespace Lib3MF {


/*************************************************************************************************************************
 Class declaration of CInternalLib3MFMeshObject 
**************************************************************************************************************************/

class CInternalLib3MFMeshObject : public virtual IInternalLib3MFMeshObject, public virtual CInternalLib3MFObject {
private:

	NMR::CMesh& getMesh();

protected:

	/**
	* Put protected members here.
	*/

public:

	CInternalLib3MFMeshObject(CInternalLib3MFModel & model);

	NMR::CModelMeshObject& getModelMeshObject();

	/**
	* Public member functions to implement.
	*/

	unsigned int GetVertexCount ();

	unsigned int GetTriangleCount ();

	void SetVertex (const unsigned int nIndex, const sLib3MFPosition Coordinates);

	unsigned int AddVertex (const sLib3MFPosition Coordinates);

	sLib3MFTriangle GetTriangle (const unsigned int nIndex);

	void SetTriangle (const unsigned int nIndex, const sLib3MFTriangle Indices);

	unsigned int AddTriangle (const sLib3MFTriangle Indices);

	void GetTriangleIndices (unsigned int nIndicesBufferSize, unsigned int * pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer);

	void SetTriangleIndices (const unsigned int nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer);

	double GetBeamLattice_MinLength ();

	void SetBeamLattice_MinLength (const double dMinLength);

	double GetBeamLattice_Radius ();

	void SetBeamLattice_Radius (const double dRadius);

	eLib3MFBeamLatticeCapMode GetBeamLattice_CapMode ();

	void SetBeamLattice_CapMode (const eLib3MFBeamLatticeCapMode eCapMode);

	void GetBeamLattice_Clipping (eLib3MFBeamLatticeClipMode & eClipMode, unsigned int & nResourceID);

	void SetBeamLattice_Clipping (const eLib3MFBeamLatticeClipMode eClipMode, const unsigned int nResourceID);

	void GetBeamLattice_Representation (bool & bHasRepresentation, unsigned int & nResourceID);

	void SetBeamLattice_Representation (const unsigned int nResourceID);

	unsigned int GetBeamCount ();

	unsigned int AddBeam (const sLib3MFBeam BeamInfo);

	void SetBeam (const unsigned int nIndex, const sLib3MFBeam BeamInfo);

	unsigned int GetBeamSetCount ();

	IInternalLib3MFBeamSet * AddBeamSet ();

	IInternalLib3MFBeamSet * GetBeamSet (const unsigned int nIndex);

	bool IsManifoldAndOriented ();

};

}

#pragma warning( pop )
#endif // __LIB3MF_LIB3MFMESHOBJECT
