/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFBaseMaterial

*/

#include "lib3mf_basematerial.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFBaseMaterial 
**************************************************************************************************************************/

CLib3MFBaseMaterial::CLib3MFBaseMaterial(NMR::PModelBaseMaterialResource pResource)
	: CLib3MFResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{
}

NMR::CModelBaseMaterialResource& CLib3MFBaseMaterial::baseMaterial()
{
	NMR::CModelBaseMaterialResource* pBaseMaterial = dynamic_cast<NMR::CModelBaseMaterialResource*>(resource().get());
	if (pBaseMaterial == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return *pBaseMaterial;
}

Lib3MF_uint32 CLib3MFBaseMaterial::GetCount ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFBaseMaterial::GetResourceID ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_uint32 CLib3MFBaseMaterial::AddMaterial(const std::string & sName, const sLib3MFColor DisplayColor)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFBaseMaterial::RemoveMaterial (const Lib3MF_uint32 nResourceIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

std::string CLib3MFBaseMaterial::GetName (const Lib3MF_uint32 nResourceIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFBaseMaterial::SetName (const Lib3MF_uint32 nResourceIndex, const std::string & sName)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFBaseMaterial::SetDisplayColor(const Lib3MF_uint32 nResourceIndex, const sLib3MFColor TheColor)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

sLib3MFColor CLib3MFBaseMaterial::GetDisplayColor(const Lib3MF_uint32 nResourceIndex)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}
