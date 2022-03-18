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

NMR_ModelVector3DFieldFromConstant.h defines the Model Vector3DFieldConstant Class.
--*/

#ifndef __NMR_MODELVECTOR3DFIELDCONSTANT
#define __NMR_MODELVECTOR3DFIELDCONSTANT

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelVector3DField.h"
#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Matrix.h"

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelVector3DFieldConstant : public CModelVector3DField {
	private:
		nfDouble m_dValueX = 0.0;
		nfDouble m_dValueY = 0.0;
		nfDouble m_dValueZ = 0.0;
	public:
		CModelVector3DFieldConstant() = delete;
		CModelVector3DFieldConstant(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		nfDouble getValueX() const;
		void setValueX(nfDouble);
		nfDouble getValueY() const;
		void setValueY(nfDouble);
		nfDouble getValueZ() const;
		void setValueZ(nfDouble);
	};

	typedef std::shared_ptr<CModelVector3DFieldConstant> PModelVector3DFieldConstant;
}

#endif // __NMR_MODELVECTOR3DFIELDCONSTANT
