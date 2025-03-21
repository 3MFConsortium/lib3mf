/*++

Copyright (C) 2019 3MF Consortium

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

NMR_ModelMeshObject.h defines the Model Mesh Object Class.
A model mesh object is an in memory representation of the 3MF
mesh object.

--*/

#ifndef __NMR_MODELMESHOBJECT
#define __NMR_MODELMESHOBJECT

#include "Common/Mesh/NMR_Mesh.h"
#include "Model/Classes/NMR_ModelVolumeData.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelTriangleSet.h"
#include "Model/Classes/NMR_ModelMeshBeamLatticeAttributes.h"

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelObject;
	typedef std::shared_ptr <CModelObject> PModelObject;

	class CModelMeshObject : public CModelObject {
	private:
		PMesh m_pMesh; 

		PModelVolumeData m_pVolumeData;
		PModelMeshBeamLatticeAttributes m_pBeamLatticeAttributes;

		std::map<std::string, PModelTriangleSet> m_TriangleSetMap;
		std::vector<PModelTriangleSet> m_TriangleSets;

	public:
		CModelMeshObject() = delete;
		CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel);
		CModelMeshObject(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ PMesh pMesh);
		~CModelMeshObject();
		
		_Ret_notnull_ CMesh * getMesh ();
		void setMesh (_In_ PMesh pMesh);

		void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix) override;

		void setObjectType(_In_ eModelObjectType ObjectType) override;

		// check, if the object is a valid object description
		nfBool isValid() override;

		nfBool hasSlices(nfBool bRecursive) override;
		nfBool isValidForSlices(const NMATRIX3& totalParentMatrix) override;

		virtual nfBool isValidForBeamLattices();

		// check, if the mesh is manifold and oriented
		virtual nfBool isManifoldAndOriented();

		_Ret_notnull_ PModelMeshBeamLatticeAttributes getBeamLatticeAttributes();
		void setBeamLatticeAttributes(_In_ PModelMeshBeamLatticeAttributes pBeamLatticeAttributes);

		void extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix) override;

		_Ret_notnull_ PModelVolumeData getVolumeData();
		void setVolumeData(_In_ PModelVolumeData pVolumeData);

		ResourceDependencies getDependencies() override;
		void deleteTriangleSet (_In_ CModelTriangleSet * pTriangleSet);
		PModelTriangleSet addTriangleSet(const std::string& sIdentifier, const std::string& sName);
		PModelTriangleSet addTriangleSet(PModelTriangleSet pModelTriangleSet);
		PModelTriangleSet findTriangleSet (const std::string & sIdentifier);
		uint32_t getTriangleSetCount();
		PModelTriangleSet getTriangleSet(const uint32_t nIndex);
		

	};

	typedef std::shared_ptr <CModelMeshObject> PModelMeshObject;

}

#endif // __NMR_MODELMESHOBJECT
