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

Abstract: This is a stub class definition of CScalarFieldComposed

*/

#include "lib3mf_scalarfieldcomposed.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_scalarfieldreference.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CScalarFieldComposed 
**************************************************************************************************************************/
CScalarFieldComposed::CScalarFieldComposed(NMR::PModelScalarFieldComposed pScalarFieldComposed)
	:CScalarField(pScalarFieldComposed), CResource(pScalarFieldComposed)
{

}

NMR::CModelScalarFieldComposed* CScalarFieldComposed::scalarFieldComposed()
{
	NMR::CModelScalarFieldComposed* pFieldComposed = dynamic_cast<NMR::CModelScalarFieldComposed*>(resource().get());
	if (pFieldComposed == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pFieldComposed;
}

void CScalarFieldComposed::SetMethod(const Lib3MF::eCompositionMethod eTheMethod)
{
	scalarFieldComposed()->setMethod(NMR::eModelCompositionMethod(eTheMethod));
}

Lib3MF::eCompositionMethod CScalarFieldComposed::GetMethod()
{
	return Lib3MF::eCompositionMethod(scalarFieldComposed()->getMethod());
}

Lib3MF_double CScalarFieldComposed::GetFactor1()
{
	return scalarFieldComposed()->getFactor1();
}

void CScalarFieldComposed::SetFactor1(const Lib3MF_double dFactor1)
{
	scalarFieldComposed()->setFactor1(dFactor1);
}

Lib3MF_double CScalarFieldComposed::GetFactor2()
{
	return scalarFieldComposed()->getFactor2();
}

void CScalarFieldComposed::SetFactor2(const Lib3MF_double dFactor2)
{
	scalarFieldComposed()->setFactor2(dFactor2);
}

IScalarFieldReference * CScalarFieldComposed::ScalarFieldReference1()
{
	auto pScalarFieldReference1 = scalarFieldComposed()->ScalarFieldReference1();
	if (!pScalarFieldReference1) {
		return nullptr;
	}
	return new CScalarFieldReference(pScalarFieldReference1);
}

IScalarFieldReference * CScalarFieldComposed::ScalarFieldReference2()
{
	auto pScalarFieldReference2 = scalarFieldComposed()->ScalarFieldReference2();
	if (!pScalarFieldReference2) {
		return nullptr;
	}
	return new CScalarFieldReference(pScalarFieldReference2);
}

IScalarFieldReference * CScalarFieldComposed::ScalarFieldReferenceMask()
{
	auto pScalarFieldReferenceMask = scalarFieldComposed()->ScalarFieldReferenceMask();
	if (!pScalarFieldReferenceMask) {
		return nullptr;
	}
	return new CScalarFieldReference(pScalarFieldReferenceMask);
}

