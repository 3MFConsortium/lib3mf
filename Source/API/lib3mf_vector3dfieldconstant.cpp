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

Abstract: This is a stub class definition of CVector3DFieldConstant

*/

#include "lib3mf_vector3dfieldconstant.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVector3DFieldConstant
**************************************************************************************************************************/

CVector3DFieldConstant::CVector3DFieldConstant(NMR::PModelVector3DFieldConstant pVector3DFieldConstant)
	:CVector3DField(pVector3DFieldConstant), CResource(pVector3DFieldConstant)
{

}

NMR::CModelVector3DFieldConstant* CVector3DFieldConstant::vector3dFieldConstant()
{
	NMR::CModelVector3DFieldConstant* pFieldConstant = dynamic_cast<NMR::CModelVector3DFieldConstant*>(resource().get());
	if (pFieldConstant == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pFieldConstant;
}

Lib3MF_double CVector3DFieldConstant::GetValueX()
{
	return vector3dFieldConstant()->getValueX();
}

void CVector3DFieldConstant::SetValueX(const Lib3MF_double dValue)
{
	vector3dFieldConstant()->setValueX(dValue);
}

Lib3MF_double CVector3DFieldConstant::GetValueY()
{
	return vector3dFieldConstant()->getValueY();
}

void CVector3DFieldConstant::SetValueY(const Lib3MF_double dValue)
{
	vector3dFieldConstant()->setValueY(dValue);
}

Lib3MF_double CVector3DFieldConstant::GetValueZ()
{
	return vector3dFieldConstant()->getValueZ();
}

void CVector3DFieldConstant::SetValueZ(const Lib3MF_double dValue)
{
	vector3dFieldConstant()->setValueZ(dValue);
}

