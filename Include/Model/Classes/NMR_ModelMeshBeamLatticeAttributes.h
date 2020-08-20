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

NMR_ModelMeshBeamLatticeAttributes.h defines the Model Mesh BeamLattice Attributes 
class and is part of the BeamLattice extension to 3MF

--*/

#ifndef __NMR_MODELMESHBEAMLATTICEATTRIBUTES
#define __NMR_MODELMESHBEAMLATTICEATTRIBUTES

#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_Model.h"
#include "Common/NMR_Types.h"
#include <memory>
#include <algorithm>

namespace NMR {
	class CModelMeshBeamLatticeAttributes {
	private:
	public:
		eModelBeamLatticeClipMode m_eClipMode;
		nfBool m_bHasClippingMeshID;
		PPackageResourceID m_pClippingMeshUniqueID;
		nfBool m_bHasRepresentationMeshID;
		PPackageResourceID m_pRepresentationUniqueID;
		eModelBeamLatticeBallMode m_eBallMode;
		
		CModelMeshBeamLatticeAttributes();
		virtual ~CModelMeshBeamLatticeAttributes();
	};
	typedef std::shared_ptr<CModelMeshBeamLatticeAttributes> PModelMeshBeamLatticeAttributes;
}

#endif // __NMR_MODELMESHBEAMLATTICEATTRIBUTES

