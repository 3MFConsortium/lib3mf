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

NMR_VolumeDataProperty.h defines the class CVolumeDataProperty.

--*/

#ifndef __NMR_VOLUMEDATAPROPERTY
#define __NMR_VOLUMEDATAPROPERTY

#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/NMR_FieldReference.h"

namespace NMR {
	class CModelScalarField;
	typedef std::shared_ptr<CModelScalarField> PModelScalarField;

	class CModelVector3DField;
	typedef std::shared_ptr<CModelVector3DField> PModelVector3DField;

	class CVolumeDataProperty : public CFieldReference {
	private:
		std::string m_sName;
		bool m_bIsRequired;
	public:
		CVolumeDataProperty() = delete;
		CVolumeDataProperty(PModelScalarField pScalarField, std::string sName);
		CVolumeDataProperty(PModelVector3DField pVectorField, std::string sName);

		std::string getName();
		void setName(std::string sPropertyName);

		bool isRequired();
		void setIsRequired(bool bIsRequired);
	};

	typedef std::shared_ptr<CVolumeDataProperty> PVolumeDataProperty;

}

#endif // __NMR_VOLUMEDATAPROPERTY
