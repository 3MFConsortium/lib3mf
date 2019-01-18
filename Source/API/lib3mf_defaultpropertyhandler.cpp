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

Abstract: This is a stub class definition of CLib3MFDefaultPropertyHandler

*/

#include "lib3mf_defaultpropertyhandler.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFDefaultPropertyHandler 
**************************************************************************************************************************/

void CLib3MFDefaultPropertyHandler::RemoveProperty ()
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFPropertyType CLib3MFDefaultPropertyHandler::GetPropertyType (const Lib3MF_uint32 nIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::GetBaseMaterial (Lib3MF_uint32 & nMaterialGroupId, Lib3MF_uint32 & nMaterialIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetBaseMaterial (const Lib3MF_uint32 nMaterialGroupId, const Lib3MF_uint32 nMaterialIndex)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetColorRGB (const Lib3MF_uint8 nRed, const Lib3MF_uint8 nGreen, const Lib3MF_uint8 nBlue)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetColorRGBA (const Lib3MF_uint8 nRed, const Lib3MF_uint8 nGreen, const Lib3MF_uint8 nBlue, const Lib3MF_uint8 nAlpha)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetFloatColorRGB (const float fRed, const float fGreen, const float fBlue)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetFloatColorRGBA (const float fRed, const float fGreen, const float fBlue, const float fAlpha)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::GetTexture (Lib3MF_uint32 & nTextureID, float & fU, float & fV)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CLib3MFDefaultPropertyHandler::SetTexture (Lib3MF_uint32 & nTextureID, float & fU, float & fV)
{
	throw ELib3MFInterfaceException (LIB3MF_ERROR_NOTIMPLEMENTED);
}

