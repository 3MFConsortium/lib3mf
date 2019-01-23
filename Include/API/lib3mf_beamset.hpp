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

Abstract: This is the class declaration of CLib3MFBeamSet

*/


#ifndef __LIB3MF_LIB3MFBEAMSET
#define __LIB3MF_LIB3MFBEAMSET

#include "lib3mf_interfaces.hpp"

#include "Common/Mesh/NMR_MeshTypes.h" 
#include "Common/Mesh/NMR_Mesh.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CLib3MFBeamSet 
**************************************************************************************************************************/

class CLib3MFBeamSet : public virtual ILib3MFBeamSet {
private:

	/**
	* Put private members here.
	*/
	NMR::PBEAMSET m_pBeamSet;
	NMR::CMesh& m_mesh;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CLib3MFBeamSet(NMR::PBEAMSET pBeamSet, NMR::PModelMeshObject pMeshObject);

	/**
	* Public member functions to implement.
	*/

	virtual void SetName(const std::string & sName);

	virtual std::string GetName();

	virtual void SetIdentifier(const std::string & sIdentifier);

	virtual std::string GetIdentifier();

	virtual Lib3MF_uint32 GetReferenceCount();

	virtual void SetReferences(const Lib3MF_uint64 nReferencesBufferSize, const Lib3MF_uint32 * pReferencesBuffer);

	virtual void GetReferences(Lib3MF_uint64 nReferencesBufferSize, Lib3MF_uint64* pReferencesNeededCount, Lib3MF_uint32 * pReferencesBuffer);

};

}
}

#endif // __LIB3MF_LIB3MFBEAMSET
