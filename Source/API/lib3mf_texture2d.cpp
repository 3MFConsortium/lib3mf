/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CTexture2D

*/

#include "lib3mf_texture2d.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CTexture2D 
**************************************************************************************************************************/

IAttachment * CTexture2D::GetAttachment()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CTexture2D::SetAttachment(IAttachment* pAttachment)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::eTextureType CTexture2D::GetContentType()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CTexture2D::SetContentType(const Lib3MF::eTextureType eContentType)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CTexture2D::GetTileStyleUV(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CTexture2D::SetTileStyleUV(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF::eTextureFilter CTexture2D::GetFilter()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CTexture2D::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

