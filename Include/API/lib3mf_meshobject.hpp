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
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Common/MeshInformation/NMR_MeshInformation_Properties.h"

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
	NMR::PModelMeshObject meshObject();
	NMR::CMesh* mesh();

	NMR::CMeshInformation_Properties* getMeshInformationProperties();


protected:

	/**
	* Put protected members here.
	*/

public:
	static IMeshObject* fnCreateMeshObjectFromModelResource(NMR::PModelResource pResource, bool bFailIfUnkownClass);

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CMeshObject(NMR::PModelResource pResource);

	/**
	* Public member functions to implement.
	*/

	Lib3MF_uint32 GetVertexCount () override;

	Lib3MF_uint32 GetTriangleCount () override;

	void SetVertex (const Lib3MF_uint32 nIndex, const sLib3MFPosition Coordinates) override;

	sLib3MFPosition GetVertex(const Lib3MF_uint32 nIndex) override;

	Lib3MF_uint32 AddVertex (const sLib3MFPosition Coordinates) override;

	virtual void GetVertices(Lib3MF_uint64 nVerticesBufferSize, Lib3MF_uint64* pVerticesNeededCount, sLib3MFPosition * pVerticesBuffer) override;

	sLib3MFTriangle GetTriangle (const Lib3MF_uint32 nIndex) override;

	void SetTriangle (const Lib3MF_uint32 nIndex, const sLib3MFTriangle Indices) override;

	Lib3MF_uint32 AddTriangle (const sLib3MFTriangle Indices) override;

	void GetTriangleIndices (Lib3MF_uint64 nIndicesBufferSize, Lib3MF_uint64* pIndicesNeededCount, sLib3MFTriangle * pIndicesBuffer) override;

	void SetGeometry(const Lib3MF_uint64 nVerticesBufferSize, const sLib3MFPosition * pVerticesBuffer, const Lib3MF_uint64 nIndicesBufferSize, const sLib3MFTriangle * pIndicesBuffer) override;

	bool IsManifoldAndOriented() override;

	bool IsMeshObject() override;

	bool IsComponentsObject() override;

    bool IsLevelSetObject() override;

    bool IsValid();

	virtual IBeamLattice * BeamLattice() override;

	void SetObjectLevelProperty(const Lib3MF_uint32 nUniqueResourceID, const Lib3MF_uint32 nPropertyID) override;

	bool GetObjectLevelProperty(Lib3MF_uint32 & nUniqueResourceID, Lib3MF_uint32 & nPropertyID) override;

	void SetTriangleProperties(const Lib3MF_uint32 nIndex, const sLib3MFTriangleProperties Properties) override;

	void GetTriangleProperties(const Lib3MF_uint32 nIndex, sLib3MFTriangleProperties & sProperty) override;

	void SetAllTriangleProperties(const Lib3MF_uint64 nPropertiesArrayBufferSize, const sLib3MFTriangleProperties * pPropertiesArrayBuffer) override;

	void GetAllTriangleProperties(Lib3MF_uint64 nPropertiesArrayBufferSize, Lib3MF_uint64* pPropertiesArrayNeededCount, sLib3MFTriangleProperties * pPropertiesArrayBuffer) override;

	void ClearAllProperties() override;

	IVolumeData * GetVolumeData() override;

	void SetVolumeData(IVolumeData* pTheVolumeData) override;

	ITriangleSet* AddTriangleSet(const std::string& sIdentifier, const std::string& sName) override;

	bool HasTriangleSet(const std::string& sIdentifier) override;

	ITriangleSet* FindTriangleSet(const std::string& sIdentifier) override;

	Lib3MF_uint32 GetTriangleSetCount() override;

	ITriangleSet* GetTriangleSet(const Lib3MF_uint32 nIndex) override;
};

}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_MESHOBJECT
