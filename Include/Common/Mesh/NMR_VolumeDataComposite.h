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

NMR_VolumeComposite.h defines the class CVolumeDataComposite.

--*/

#ifndef __NMR_VOLUMEDATACOMPOSITE
#define __NMR_VOLUMEDATACOMPOSITE

#include <memory>
#include "Common/NMR_Types.h"
#include "Common/NMR_FieldReference.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_ModelBaseMaterials.h"

namespace NMR {

	// does not derive from the CVolumeBase, as CVolumeDataComposite contains
	//  multiple ScalarFieldReferences, but is not a ScalarFieldReference itself.
	class CVolumeDataComposite {
	private:
		std::vector<PScalarFieldReference> m_vctFieldReferences;
		NMR::PModelBaseMaterialResource m_pBaseMaterialGroup;
	public:
		CVolumeDataComposite();

		NMR::PModelBaseMaterialResource getBaseMaterialGroup();
		void setBaseMaterialGroup(PModelBaseMaterialResource );

		nfUint32 materialMappingCount() const;
		PScalarFieldReference getMaterialMapping(nfUint32);
		PScalarFieldReference addMaterialMapping(PModelScalarField pScalarField);
		void removeMaterialMapping(nfUint32);
	};

	typedef std::shared_ptr<CVolumeDataComposite> PVolumeDataComposite;

}

#endif // __NMR_VOLUMEDATACOMPOSITE
