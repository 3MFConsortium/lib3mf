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

NMR_ModelScalarFieldComposed.h defines the Model ScalarField Composed Class.
A model scalar field composed encodes a composition operation of two (or three) scalar fields.
--*/

#ifndef __NMR_MODELSCALARFIELDCOMPOSED
#define __NMR_MODELSCALARFIELDCOMPOSED

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelScalarField.h"
#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Matrix.h"
#include "Common/NMR_FieldReference.h"

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr<CModel> PModel;

	class CModelScalarFieldComposed : public CModelScalarField {
	private:
		nfDouble m_dFactor1 = 1.;
		nfDouble m_dFactor2 = 1.;

		eModelCompositionMethod m_eMethod = eModelCompositionMethod::MODELCOMPOSITIONMETHOD_MIN;
		
		PScalarFieldReference m_pScalarFieldReference1;
		PScalarFieldReference m_pScalarFieldReference2;
		PScalarFieldReference m_pScalarFieldReferenceMask;
	public:
		CModelScalarFieldComposed() = delete;
		CModelScalarFieldComposed(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		eModelCompositionMethod getMethod() const;
		void setMethod(eModelCompositionMethod eCompositionMethod);

		nfDouble getFactor1() const;
		void setFactor1(nfDouble);
		nfDouble getFactor2() const;
		void setFactor2(nfDouble);

		PScalarFieldReference ScalarFieldReference1();
		PScalarFieldReference ScalarFieldReference2();
		PScalarFieldReference ScalarFieldReferenceMask();

		static  std::string methodToString(_In_ eModelCompositionMethod eMethod);

		static eModelCompositionMethod methodFromString(_In_ std::string sValue);
	};

	typedef std::shared_ptr <CModelScalarFieldComposed> PModelScalarFieldComposed;
}

#endif // __NMR_MODELSCALARFIELDCOMPOSED
