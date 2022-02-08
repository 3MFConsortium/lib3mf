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

Abstract: This is a stub class definition of CScalarFieldFromImage3D

*/

#include "lib3mf_scalarfieldfromimage3d.hpp"
#include "lib3mf_image3d.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include <memory>

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CScalarFieldFromImage3D 
**************************************************************************************************************************/

CScalarFieldFromImage3D::CScalarFieldFromImage3D(NMR::PModelScalarFieldFromImage3D pScalarFieldFromImage3D)
	:CScalarField(pScalarFieldFromImage3D), CResource(pScalarFieldFromImage3D)
{

}

NMR::CModelScalarFieldFromImage3D* CScalarFieldFromImage3D::scalarFieldFromImage3D()
{
	NMR::CModelScalarFieldFromImage3D* pFieldFromImage3D = dynamic_cast<NMR::CModelScalarFieldFromImage3D*>(resource().get());
	if (pFieldFromImage3D == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pFieldFromImage3D;
}

IImage3D * CScalarFieldFromImage3D::GetImage()
{
	NMR::PModelResource pResource = model()->findResource(scalarFieldFromImage3D()->getImage3DResourceID());
	return new CImage3D(pResource);
}

void CScalarFieldFromImage3D::SetImage(IImage3D* pImage3D)
{
	scalarFieldFromImage3D()->setImage3DResourceID(model()->findPackageResourceID(pImage3D->GetUniqueResourceID()));
}

void CScalarFieldFromImage3D::SetChannel(const Lib3MF::eChannelName eName)
{
	scalarFieldFromImage3D()->setChannel(NMR::eModelColorChannel(eName));
}

Lib3MF::eChannelName CScalarFieldFromImage3D::GetChannel()
{
	return eChannelName(scalarFieldFromImage3D()->getChannel());
}

void CScalarFieldFromImage3D::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	scalarFieldFromImage3D()->setFilter(NMR::eModelTextureFilter(eFilter));
}

Lib3MF::eTextureFilter CScalarFieldFromImage3D::GetFilter()
{
	return eTextureFilter(scalarFieldFromImage3D()->getFilter());
}

void CScalarFieldFromImage3D::SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV, const Lib3MF::eTextureTileStyle eTileStyleW)
{
	scalarFieldFromImage3D()->setTileStyleU(NMR::eModelTextureTileStyle(eTileStyleU));
	scalarFieldFromImage3D()->setTileStyleV(NMR::eModelTextureTileStyle(eTileStyleV));
	scalarFieldFromImage3D()->setTileStyleW(NMR::eModelTextureTileStyle(eTileStyleW));
}

void CScalarFieldFromImage3D::GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV, Lib3MF::eTextureTileStyle & eTileStyleW)
{
	eTileStyleU = eTextureTileStyle(scalarFieldFromImage3D()->getTileStyleU());
	eTileStyleV = eTextureTileStyle(scalarFieldFromImage3D()->getTileStyleV());
	eTileStyleW = eTextureTileStyle(scalarFieldFromImage3D()->getTileStyleW());

}

Lib3MF_double CScalarFieldFromImage3D::GetOffset()
{
	return scalarFieldFromImage3D()->getOffset();
}

void CScalarFieldFromImage3D::SetOffset(const Lib3MF_double dOffset)
{
	scalarFieldFromImage3D()->setOffset(dOffset);
}

Lib3MF_double CScalarFieldFromImage3D::GetScale()
{
	return scalarFieldFromImage3D()->getScale();
}

void CScalarFieldFromImage3D::SetScale(const Lib3MF_double dScale)
{
	scalarFieldFromImage3D()->setScale(dScale);
}

