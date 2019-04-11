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

Abstract: This is a stub class definition of CTexture2D

*/

#include "lib3mf_texture2d.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_attachment.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CTexture2D 
**************************************************************************************************************************/


CTexture2D::CTexture2D(NMR::PModelTexture2DResource pResource)
	:CResource(pResource)
{
}

NMR::PModelTexture2DResource CTexture2D::texture()
{
	NMR::PModelTexture2DResource pTexture = std::dynamic_pointer_cast<NMR::CModelTexture2DResource>(resource());
	if (!pTexture)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDTEXTURERESOURCE);
	return pTexture;
}


IAttachment * CTexture2D::GetAttachment ()
{
	return new CAttachment(texture()->getAttachment());
}

void CTexture2D::SetAttachment (IAttachment* pAttachment)
{
	NMR::PModelAttachment attachment = texture()->getModel()->findModelAttachment(pAttachment->GetPath());
	texture()->setAttachment(attachment);
}

eLib3MFTextureType CTexture2D::GetContentType ()
{
	return (eLib3MFTextureType)(texture()->getContentType());
}

void CTexture2D::SetContentType (const eLib3MFTextureType eContentType)
{
	texture()->setContentType(NMR::eModelTexture2DType(eContentType));
}

void CTexture2D::GetTileStyleUV (eLib3MFTextureTileStyle & eTileStyleU, eLib3MFTextureTileStyle & eTileStyleV)
{
	eTileStyleU = eLib3MFTextureTileStyle(texture()->getTileStyleU());
	eTileStyleV = eLib3MFTextureTileStyle(texture()->getTileStyleV());
}

void CTexture2D::SetTileStyleUV (const eLib3MFTextureTileStyle eTileStyleU, const eLib3MFTextureTileStyle eTileStyleV)
{
	texture()->setTileStyleU(NMR::eModelTextureTileStyle(eTileStyleU));
	texture()->setTileStyleV(NMR::eModelTextureTileStyle(eTileStyleV));
}

eLib3MFTextureFilter CTexture2D::GetFilter ()
{
	return eLib3MFTextureFilter(texture()->getFilter());
}

void CTexture2D::SetFilter (const eLib3MFTextureFilter eFilter)
{
	texture()->setFilter(NMR::eModelTextureFilter(eFilter));
}

