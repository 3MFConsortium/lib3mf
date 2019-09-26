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

NMR_ModelComponentsObject.h defines the Model Component Object Class.
A model component object is an in memory representation of the 3MF
component object.

--*/

#ifndef __NMR_MODELCOMPONENTSOBJECT
#define __NMR_MODELCOMPONENTSOBJECT

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelComponent.h" 
#include "Model/Classes/NMR_Model.h" 
#include "Common/Mesh/NMR_Mesh.h" 

#include <list>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelComponentsObject : public CModelObject {
	private:
		std::vector<PModelComponent> m_Components;
	public:
		CModelComponentsObject() = delete;
		CModelComponentsObject(_In_ const ModelResourceID sID, _In_ CModel * pModel);
		~CModelComponentsObject();

		void addComponent(_In_ PModelComponent pComponent);
		nfUint32 getComponentCount();
		PModelComponent getComponent(_In_ nfUint32 nIdx);

		void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix) override;

		// check, if the object is a valid object description
		nfBool isValid() override;

		nfBool hasSlices(nfBool bRecursive) override;
		nfBool isValidForSlices(const NMATRIX3& totalParentMatrix) override;

		void calculateComponentDepthLevel(nfUint32 nLevel) override;

		void extendOutbox(_Out_ NOUTBOX3& vOutBox, _In_ const NMATRIX3 mAccumulatedMatrix) override;
	};

	typedef std::shared_ptr <CModelComponentsObject> PModelComponentsObject;

}

#endif // __NMR_MODELCOMPONENTSOBJECT
