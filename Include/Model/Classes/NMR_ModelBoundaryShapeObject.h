/*++

Copyright (C) 2024 3MF Consortium

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

Abstract:

NMR_ModelBoundaryShapeObject.cpp implements the Model BoundaryShape Object Class.
A model boundaryshape object is an in memory representation of the 3MF
boundaryshape object.

--*/

#ifndef __NMR_ModelBoundaryShapeObject
#define __NMR_ModelBoundaryShapeObject

#include "Model/Classes/NMR_ModelObject.h" 

#include <memory>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

    class CModelVolumeData;
	typedef std::shared_ptr <CModelVolumeData> PModelVolumeData;

	class CModelMeshObject;
	typedef std::shared_ptr <CModelMeshObject> PModelMeshObject;

	class CModelFunction;
	typedef std::shared_ptr <CModelFunction> PModelFunction;

    class CModelBoundaryShapeObject : public CModelObject {
	private:
		PModelMeshObject m_pMesh; 
		PModelFunction m_pFunction;
		PModelVolumeData m_pVolumeData;

		std::string m_outputPortName;
		NMR::NMATRIX3 m_transform = NMR::fnMATRIX3_identity();
		bool m_bHasTransform = false;
		double m_minFeatureSize = 0.0;
		double m_fallBackValue = 0.0;
		bool m_meshBBoxOnly = false;
	public:
		CModelBoundaryShapeObject() = delete;
		CModelBoundaryShapeObject(const ModelResourceID sID, CModel * pModel);
		CModelBoundaryShapeObject(const ModelResourceID sID, CModel * pModel, PModelMeshObject pMesh);
		~CModelBoundaryShapeObject();
 
		CMesh * getMesh ();
		void setMesh (PModelMeshObject pMesh);

		nfBool hasSlices(nfBool bRecursive) override;
		nfBool isValidForSlices(const NMATRIX3& totalParentMatrix) override;

		PModelFunction getFunction();
		void setFunction(PModelFunction pFunction);

		PModelVolumeData getVolumeData();
		void setVolumeData(_In_ PModelVolumeData pVolumeData);

		void setChannelName(std::string outputPortName);
		std::string const& getChannelName() const;

		void setTransform(NMR::NMATRIX3 transform);
		NMR::NMATRIX3 getTransform() const;
		bool hasTransform() const;

		void setMinFeatureSize(double minFeatureSize);
		double getMinFeatureSize() const;

		void setFallBackValue(double fallBackValue);
		double getFallBackValue() const;

		void setMeshBBoxOnly(bool bMeshBBoxOnly);
		bool getMeshBBoxOnly() const;
	};

	typedef std::shared_ptr <CModelBoundaryShapeObject> PModelBoundaryShapeObject;

}

#endif // __NMR_ModelBoundaryShapeObject
