/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ModelColorGroup.h defines the Model Color Group Resource Class.
A model color group resource is an in memory representation of the 3MF
color group resource object.

--*/

#ifndef __NMR_MODELCOLORGROUP
#define __NMR_MODELCOLORGROUP

#include "Model/Classes/NMR_ModelBaseMaterial.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_Model.h"
#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelColorGroupResource : public CModelResource {
	private:
		std::map<ModelPropertyID, nfColor> m_pColors;
		ModelPropertyID m_nNextPropertyID;

	public:
		CModelColorGroupResource() = delete;
		CModelColorGroupResource(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		nfUint32 addColor(_In_ nfColor cColor);

		nfUint32 getCount();
		nfColor getColor(_In_ ModelPropertyID nPropertyID);
		void setColor(_In_ ModelPropertyID nPropertyID, _In_ nfColor cColor);

		void removeColor(_In_ ModelPropertyID nPropertyID);
		void mergeFrom(_In_ CModelColorGroupResource * pSourceMaterial);
		void buildResourceIndexMap();
	};

	typedef std::shared_ptr <CModelColorGroupResource> PModelColorGroupResource;

}

#endif // __NMR_MODELCOLORGROUP
