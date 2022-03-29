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

Abstract: This is a stub class definition of CVector3DField

*/

#include "lib3mf_vector3Dfield.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Model/Classes/NMR_ModelVector3DFieldFromImage3D.h"
#include "Model/Classes/NMR_ModelVector3DFieldComposed.h"
#include "Model/Classes/NMR_ModelVector3DFieldConstant.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVector3DField 
**************************************************************************************************************************/

CVector3DField::CVector3DField(NMR::PModelVector3DField pResource)
	:CResource(pResource)
{

}

NMR::CModelVector3DField* CVector3DField::vector3DField()
{
	NMR::CModelVector3DField* pVector3DField = dynamic_cast<NMR::CModelVector3DField*> (resource().get());
	if (pVector3DField == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_RESOURCENOTFOUND);

	return pVector3DField;
}

std::string CVector3DField::GetName()
{
	return vector3DField()->getName();
}

void CVector3DField::SetName(const std::string & sName)
{
	vector3DField()->setName(sName);
}

bool CVector3DField::IsFromImage3D()
{
	return (dynamic_cast<NMR::CModelVector3DFieldFromImage3D*>(vector3DField()) != nullptr);
}

bool CVector3DField::IsConstant()
{
	return (dynamic_cast<NMR::CModelVector3DFieldConstant*>(vector3DField()) != nullptr);
}

bool CVector3DField::IsComposed()
{
	return (dynamic_cast<NMR::CModelVector3DFieldComposed*>(vector3DField()) != nullptr);
}

