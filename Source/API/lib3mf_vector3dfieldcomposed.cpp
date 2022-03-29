/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CVector3DFieldComposed

*/

#include "lib3mf_vector3dfieldcomposed.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_vector3dfieldreference.hpp"
#include "lib3mf_scalarfieldreference.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVector3DFieldComposed
**************************************************************************************************************************/
CVector3DFieldComposed::CVector3DFieldComposed(NMR::PModelVector3DFieldComposed pVector3DFieldComposed)
	:CVector3DField(pVector3DFieldComposed), CResource(pVector3DFieldComposed)
{

}

NMR::CModelVector3DFieldComposed* CVector3DFieldComposed::vector3DFieldComposed()
{
	NMR::CModelVector3DFieldComposed* pFieldComposed = dynamic_cast<NMR::CModelVector3DFieldComposed*>(resource().get());
	if (pFieldComposed == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pFieldComposed;
}

void CVector3DFieldComposed::SetMethod(const Lib3MF::eCompositionMethod eTheMethod)
{
	vector3DFieldComposed()->setMethod(NMR::eModelCompositionMethod(eTheMethod));
}

Lib3MF::eCompositionMethod CVector3DFieldComposed::GetMethod()
{
	return Lib3MF::eCompositionMethod(vector3DFieldComposed()->getMethod());
}

Lib3MF_double CVector3DFieldComposed::GetFactor1()
{
	return vector3DFieldComposed()->getFactor1();
}

void CVector3DFieldComposed::SetFactor1(const Lib3MF_double dFactor1)
{
	vector3DFieldComposed()->setFactor1(dFactor1);
}

Lib3MF_double CVector3DFieldComposed::GetFactor2()
{
	return vector3DFieldComposed()->getFactor2();
}

void CVector3DFieldComposed::SetFactor2(const Lib3MF_double dFactor2)
{
	vector3DFieldComposed()->setFactor2(dFactor2);
}

IVector3DFieldReference* CVector3DFieldComposed::Vector3DFieldReference1()
{
	auto pVector3DFieldReference1 = vector3DFieldComposed()->Vector3DFieldReference1();
	if (!pVector3DFieldReference1) {
		return nullptr;
	}
	return new CVector3DFieldReference(pVector3DFieldReference1);
}

IVector3DFieldReference* CVector3DFieldComposed::Vector3DFieldReference2()
{
	auto pVector3DFieldReference2 = vector3DFieldComposed()->Vector3DFieldReference2();
	if (!pVector3DFieldReference2) {
		return nullptr;
	}
	return new CVector3DFieldReference(pVector3DFieldReference2);
}

IScalarFieldReference* CVector3DFieldComposed::ScalarFieldReferenceMask()
{
	auto pScalarFieldReferenceMask = vector3DFieldComposed()->ScalarFieldReferenceMask();
	if (!pScalarFieldReferenceMask) {
		return nullptr;
	}
	return new CScalarFieldReference(pScalarFieldReferenceMask);
}

