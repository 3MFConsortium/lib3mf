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

NMR_ModelScalarField.h defines the Model ScalarField Class.
A model scalar field is the base class of different scalar field specializations.
--*/

#ifndef __NMR_MODELSCALARFIELD
#define __NMR_MODELSCALARFIELD

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Matrix.h"

#include <vector>

namespace NMR {

	class CModel;
	typedef std::shared_ptr <CModel> PModel;

	class CModelScalarField : public CModelResource { 
	private:
		std::string m_sName;
	public:
		CModelScalarField() = delete;
		CModelScalarField(_In_ const ModelResourceID sID, _In_ CModel * pModel);

		std::string getName();
		void setName(_In_ std::string sName);
	};

	typedef std::shared_ptr <CModelScalarField> PModelScalarField;
}

#endif // __NMR_MODELSCALARFIELD
