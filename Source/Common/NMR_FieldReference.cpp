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

NMR_FieldReference.cpp implements the class CFieldReference.

--*/

#include "Common/NMR_FieldReference.h" 

namespace NMR {

	CFieldReference::CFieldReference(PPackageResourceID pFieldResourceId)
		: m_pFieldPackageResourceID(pFieldResourceId), m_transform(fnMATRIX3_identity())
	{
	}

	NMR::NMATRIX3 CFieldReference::getTransform() const
	{
		return m_transform;
	}

	void CFieldReference::setTransform(NMR::NMATRIX3 transform)
	{
		m_transform = transform;
	}

	PPackageResourceID CFieldReference::getFieldReferenceID() const
	{
		return m_pFieldPackageResourceID;
	}

	void CFieldReference::setFieldReferenceID(PPackageResourceID pFieldResourceId)
	{
		// TODO: check if this points to a Scalar or Vector3DField
		m_pFieldPackageResourceID = pFieldResourceId;
	}


	CScalarFieldReference::CScalarFieldReference(PModelScalarField PModelScalarField)
		:CFieldReference(PModelScalarField->getPackageResourceID())
	{

	}

	void CScalarFieldReference::setFieldReferenceID(PPackageResourceID pFieldResourceId)
	{
		// TODO: check if this points to a ScalarField
		CFieldReference::setFieldReferenceID(pFieldResourceId);
	}

	CVector3DFieldReference::CVector3DFieldReference(PModelVector3DField pVector3DField)
		:CFieldReference(pVector3DField->getPackageResourceID())
	{

	}

	void CVector3DFieldReference::setFieldReferenceID(PPackageResourceID pFieldResourceId)
	{
		// TODO: check if this points to a Vector3DField
		CFieldReference::setFieldReferenceID(pFieldResourceId);
	}
}
