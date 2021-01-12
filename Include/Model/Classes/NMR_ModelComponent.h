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

NMR_ModelComponent.h defines the Model Component Class.
A model component is an in memory representation of the 3MF component.

--*/

#ifndef __NMR_MODELCOMPONENT
#define __NMR_MODELCOMPONENT

#include "Model/Classes/NMR_ModelObject.h" 
#include "Model/Classes/NMR_ModelMetaData.h" 
#include "Common/NMR_Types.h" 
#include "Common/Math/NMR_Matrix.h" 

namespace NMR {

	class CModelComponent {
	private:
		CModelObject * m_pObject;
		NMATRIX3 m_mTransform;

		PUUID m_UUID;
	public:
		CModelComponent() = delete;
		CModelComponent(_In_ CModelObject * pObject);
		CModelComponent(_In_ CModelObject * pObject, _In_ const NMATRIX3 mTransform);
		~CModelComponent();
		
		CModelObject * getObject();
		CModel * getModel();

		NMATRIX3 getTransform ();
		void setTransform(_In_ const NMATRIX3 mTransform);
		
		nfBool hasTransform();
		std::string getTransformString();

		// Production extension UUID
		PUUID uuid();
		void setUUID(PUUID uuid);

		void mergeToMesh(_In_ CMesh * pMesh, _In_ const NMATRIX3 mMatrix);
	};

	typedef std::shared_ptr <CModelComponent> PModelComponent;

}

#endif // __NMR_MODELCOMPONENT
