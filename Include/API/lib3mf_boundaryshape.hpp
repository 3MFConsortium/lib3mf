/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CBoundaryShape

*/


#ifndef __LIB3MF_BOUNDARYSHAPE
#define __LIB3MF_BOUNDARYSHAPE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_object.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelBoundaryShapeObject.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CBoundaryShape 
**************************************************************************************************************************/

class CBoundaryShape : public virtual IBoundaryShape, public virtual CObject {
private:

	/**
	* Put private members here.
	*/
	NMR::PModelBoundaryShapeObject boundaryShapeObject();
protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/

	CBoundaryShape() = delete;

	CBoundaryShape(NMR::PModelResource pResource);


	/**
	* Public member functions to implement.
	*/

	/**
	* IBoundaryShape::GetFunction - Returns the function that is used as boundary shape.
	* @return the function to use as boundary shape
	*/
	IFunction * GetFunction() override;

	/**
	* IBoundaryShape::SetFunction - Sets the function to use as boundary shape.
	* @param[in] pTheFunction - the function to use as boundary shape
	*/
	void SetFunction(IFunction* pTheFunction) override;

	/**
	* IBoundaryShape::GetTransform - Returns the transformation matrix into the coordinate system of the referenced Function.
	* @return the transformation matrix
	*/
	Lib3MF::sTransform GetTransform() override;

	/**
	* IBoundaryShape::SetTransform - Sets the transformation matrix into the coordinate system of the referenced Function.
	* @param[in] Transform - new transformation matrix
	*/
	void SetTransform(const Lib3MF::sTransform Transform) override;

	/**
	* IBoundaryShape::GetChannelName - Returns the name of the function output channel to use.
	* @return the name of the function output channel
	*/
	std::string GetChannelName() override;

	/**
	* IBoundaryShape::SetChannelName - Sets the name of the function output channel to use.
	* @param[in] sChannelName - new name of the function output channel
	*/
	void SetChannelName(const std::string & sChannelName) override;

	/**
	* IBoundaryShape::SetMinFeatureSize - Sets the minimal feature size as a hint for the function evaluator
	* @param[in] dMinFeatureSize - minimal feature size
	*/
	void SetMinFeatureSize(const Lib3MF_double dMinFeatureSize) override;

	/**
	* IBoundaryShape::GetMinFeatureSize - Returns the minimal feature size as a hint for the function evaluator
	* @return minimal feature size
	*/
	Lib3MF_double GetMinFeatureSize() override;

	/**
	* IBoundaryShape::SetFallBackValue - Sets the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).
	* @param[in] dFallBackValue - fallback value
	*/
	void SetFallBackValue(const Lib3MF_double dFallBackValue) override;

	/**
	* IBoundaryShape::GetFallBackValue - Returns the fallback value to use if the function evaluation fails (e.g. evaluates to NaN or Inf).
	* @return fallback value
	*/
	Lib3MF_double GetFallBackValue() override;

	/**
	* IBoundaryShape::SetMeshBBoxOnly - If set only the bounding box of the mesh is intersected with the boundary
	* @param[in] bMeshBBoxOnly - If set only the bounding box of the mesh is intersected with the boundary
	*/
	void SetMeshBBoxOnly(const bool bMeshBBoxOnly) override;

	/**
	* IBoundaryShape::GetMeshBBoxOnly - If set only the bounding box of the mesh is intersected with the boundary
	* @return If set only the bounding box of the mesh is intersected with the boundary
	*/
	bool GetMeshBBoxOnly() override;

	/**
	* IBoundaryShape::SetMesh - Sets the mesh to use as evaluation domain
	* @param[in] pTheMesh - The mesh
	*/
	void SetMesh(IMeshObject* pTheMesh) override;

	/**
	* IBoundaryShape::GetMesh - Returns the mesh that is used as evaluation domain
	* @return The mesh
	*/
	IMeshObject * GetMesh() override;


	/**
	* IBoundaryShape::GetVolumeData - Retrieves the VolumeData this MeshObject.
	* @return the VolumeData of this MeshObject
	*/
	IVolumeData * GetVolumeData() override;

	/**
	* IBoundaryShape::SetVolumeData - Sets the VolumeData of this BoundaryShape.
	* @param[in] pTheVolumeData - the VolumeData of this MeshObject
	*/
	void SetVolumeData(IVolumeData* pTheVolumeData) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_BOUNDARYSHAPE
