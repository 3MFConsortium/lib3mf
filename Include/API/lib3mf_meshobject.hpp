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

Abstract: This is the class declaration of CMeshObject

*/


#ifndef __LIB3MF_MESHOBJECT
#define __LIB3MF_MESHOBJECT

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_object.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CMeshObject 
**************************************************************************************************************************/

class CMeshObject : public virtual IMeshObject, public virtual CObject {
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

	Lib3MF_uint32 GetVertexCount() override;

	Lib3MF_uint32 GetTriangleCount() override;

	Lib3MF::sPosition GetVertex(const Lib3MF_uint32 nIndex) override;

	void SetVertex(const Lib3MF_uint32 nIndex, const Lib3MF::sPosition Coordinates) override;

	Lib3MF_uint32 AddVertex(const Lib3MF::sPosition Coordinates) override;

	void GetVertices(Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, Lib3MF::sPosition * pVerticesBuffer) override;

	Lib3MF::sTriangle GetTriangle(const Lib3MF_uint32 nIndex) override;

	void SetTriangle(const Lib3MF_uint32 nIndex, const Lib3MF::sTriangle Indices) override;

	Lib3MF_uint32 AddTriangle(const Lib3MF::sTriangle Indices) override;

	void GetTriangleIndices(Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, Lib3MF::sTriangle * pIndicesBuffer) override;

	void SetObjectLevelProperty(const Lib3MF_uint32 nUniqueResourceID, const Lib3MF_uint32 nPropertyID) override;

	bool GetObjectLevelProperty(Lib3MF_uint32 & nUniqueResourceID, Lib3MF_uint32 & nPropertyID) override;

	void SetTriangleProperties(const Lib3MF_uint32 nIndex, const Lib3MF::sTriangleProperties Properties) override;

	void GetTriangleProperties(const Lib3MF_uint32 nIndex, Lib3MF::sTriangleProperties & sProperty) override;

	void SetAllTriangleProperties(const Lib3MF_uint64 nPropertiesArrayBufferSize, const Lib3MF::sTriangleProperties * pPropertiesArrayBuffer) override;

	void GetAllTriangleProperties(Lib3MF_uint64 nPropertiesArrayBufferSize, Lib3MF_uint64* pPropertiesArrayNeededCount, Lib3MF::sTriangleProperties * pPropertiesArrayBuffer) override;

	void ClearAllProperties() override;

	void SetGeometry(const Lib3MF_uint64 nVerticesBufferSize, const Lib3MF::sPosition * pVerticesBuffer, const Lib3MF_uint64 nIndicesBufferSize, const Lib3MF::sTriangle * pIndicesBuffer) override;

	bool IsManifoldAndOriented() override;

	IBeamLattice * BeamLattice() override;

	IVolumeData * GetVolumeData() override;

	void SetVolumeData(IVolumeData* pTheVolumeData) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_MESHOBJECT
