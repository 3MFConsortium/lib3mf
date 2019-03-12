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

NMR_ModelCompositeMaterials.h defines the Model Composite Materials Resource Class.
A model composite materials resource is an in memory representation of the 3MF
composite materials resource object.

--*/

#ifndef __NMR_MODELCOMPOSITEMATERIALS
#define __NMR_MODELCOMPOSITEMATERIALS

#include "Model/Classes/NMR_ModelBaseMaterials.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_Model.h"
#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr<CModel> PModel;

	class CModelComposite: public std::vector<MODELCOMPOSITECONSTITUENT>
	{
	public:
		nfDouble GetMixingRatio(ModelPropertyID nPropertyID)
		{
			for (MODELCOMPOSITECONSTITUENT constituent : (*this)) {
				if (constituent.m_nPropertyID == nPropertyID) {
					return constituent.m_dMixingRatio;
				}
			}
			return 0.0;
		}
	};
	typedef std::shared_ptr<CModelComposite> PModelComposite;

	class CModelCompositeMaterialsResource : public CModelResource {
	private:
		std::map<ModelPropertyID, PModelComposite> m_pComposites;
		ModelPropertyID m_nNextPropertyID;

		PModelBaseMaterialResource m_pBaseMaterialResource;
	public:
		CModelCompositeMaterialsResource() = delete;
		CModelCompositeMaterialsResource(_In_ const ModelResourceID sID, _In_ CModel * pModel,
			_In_ PModelBaseMaterialResource pBaseMaterialResource);

		nfUint32 addComposite(_In_ PModelComposite pComposite);

		nfUint32 getCount();
		PModelComposite getComposite(_In_ ModelPropertyID nPropertyID);
		void setComposite(_In_ ModelPropertyID nPropertyID, _In_ PModelComposite pComposite);

		void removeComposite(_In_ ModelPropertyID nPropertyID);
		void mergeFrom(_In_ CModelCompositeMaterialsResource * pSourceCompositesMaterials);
		void buildResourceIndexMap();

		PModelBaseMaterialResource getBaseMaterialResource();
	};

	typedef std::shared_ptr<CModelCompositeMaterialsResource> PModelCompositeMaterialsResource;
}

#endif // __NMR_MODELCOMPOSITEMATERIALS
