/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ModelBaseMaterials.h defines the Model Base Material Resource Class.
A model material resource is an in memory representation of the 3MF
material resource object.

--*/

#ifndef __NMR_MODELBASEMATERIALS
#define __NMR_MODELBASEMATERIALS

#include "Model/Classes/NMR_ModelBaseMaterial.h" 
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h" 
#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelBaseMaterialResource : public CModelResource {
	private:
		std::vector<PModelBaseMaterial> m_pMaterials;
	public:
		CModelBaseMaterialResource() = delete;
		CModelBaseMaterialResource(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		nfUint32 addBaseMaterial(_In_ const std::string sName, _In_ nfColor cDisplayColor);

		nfUint32 getCount();
		PModelBaseMaterial getBaseMaterial(_In_ nfUint32 nIndex);

		void removeMaterial(_In_ nfUint32 nIndex);
		void mergeFrom(_In_ CModelBaseMaterialResource * pSourceMaterial);
	};

	typedef std::shared_ptr <CModelBaseMaterialResource> PModelBaseMaterialResource;

}

#endif // __NMR_MODELBASEMATERIAL
