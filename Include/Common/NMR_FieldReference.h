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

NMR_FieldReference.h defines the class CFieldReference.

--*/

#ifndef __NMR_FIELDREFERENCE
#define __NMR_FIELDREFERENCE

#include "Common/NMR_Types.h"
#include "Common/Math/NMR_Geometry.h"
#include "Model/Classes/NMR_ModelTypes.h"
#include "Model/Classes/NMR_PackageResourceID.h"
#include "Model/Classes/NMR_ModelScalarField.h"
#include "Model/Classes/NMR_ModelVector3DField.h"

namespace NMR {

	class CFieldReference {
	private:
		UniqueResourceID m_FieldUniqueResourceID;
		NMR::NMATRIX3 m_transform = NMR::fnMATRIX3_identity();
	public:
		CFieldReference() = delete;
		CFieldReference(UniqueResourceID FieldResourceId);

		NMR::NMATRIX3 getTransform() const;
		void setTransform(NMR::NMATRIX3 transform);

		UniqueResourceID getFieldReferenceID() const;
		virtual void setFieldReferenceID(UniqueResourceID FieldResourceId);
	};

	typedef std::shared_ptr<CFieldReference> PFieldReference;

	class CScalarFieldReference: public CFieldReference {
	public:
		CScalarFieldReference() = delete;
		CScalarFieldReference(PModelScalarField pScalar3DField);
	};

	typedef std::shared_ptr<CScalarFieldReference> PScalarFieldReference;

	class CVector3DFieldReference: public CFieldReference {
	public:
		CVector3DFieldReference() = delete;
		CVector3DFieldReference(PModelVector3DField pVector3DField);
	};

	typedef std::shared_ptr<CVector3DFieldReference> PVetor3DFieldReference;
}

#endif // __NMR_FIELDREFERENCE
