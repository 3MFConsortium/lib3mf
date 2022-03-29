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

Abstract: This is a stub class definition of CVector3DFieldFromImage3D

*/

#include "lib3mf_vector3Dfieldfromimage3d.hpp"
#include "lib3mf_image3d.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include <memory>

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CVector3DFieldFromImage3D 
**************************************************************************************************************************/

CVector3DFieldFromImage3D::CVector3DFieldFromImage3D(NMR::PModelVector3DFieldFromImage3D pVector3DFieldFromImage3D)
	:CVector3DField(pVector3DFieldFromImage3D), CResource(pVector3DFieldFromImage3D)
{

}

NMR::CModelVector3DFieldFromImage3D* CVector3DFieldFromImage3D::vector3DFieldFromImage3D()
{
	NMR::CModelVector3DFieldFromImage3D* pFieldFromImage3D = dynamic_cast<NMR::CModelVector3DFieldFromImage3D*>(resource().get());
	if (pFieldFromImage3D == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pFieldFromImage3D;
}

IImage3D * CVector3DFieldFromImage3D::GetImage()
{
	NMR::PModelResource pResource = model()->findResource(vector3DFieldFromImage3D()->getImage3DResourceID());
	return new CImage3D(pResource);
}

void CVector3DFieldFromImage3D::SetImage(IImage3D* pImage3D)
{
	vector3DFieldFromImage3D()->setImage3DResourceID(model()->findPackageResourceID(pImage3D->GetUniqueResourceID()));
}

void CVector3DFieldFromImage3D::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	vector3DFieldFromImage3D()->setFilter(NMR::eModelTextureFilter(eFilter));
}

Lib3MF::eTextureFilter CVector3DFieldFromImage3D::GetFilter()
{
	return eTextureFilter(vector3DFieldFromImage3D()->getFilter());
}

void CVector3DFieldFromImage3D::SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV, const Lib3MF::eTextureTileStyle eTileStyleW)
{
	vector3DFieldFromImage3D()->setTileStyleU(NMR::eModelTextureTileStyle(eTileStyleU));
	vector3DFieldFromImage3D()->setTileStyleV(NMR::eModelTextureTileStyle(eTileStyleV));
	vector3DFieldFromImage3D()->setTileStyleW(NMR::eModelTextureTileStyle(eTileStyleW));
}

void CVector3DFieldFromImage3D::GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV, Lib3MF::eTextureTileStyle & eTileStyleW)
{
	eTileStyleU = eTextureTileStyle(vector3DFieldFromImage3D()->getTileStyleU());
	eTileStyleV = eTextureTileStyle(vector3DFieldFromImage3D()->getTileStyleV());
	eTileStyleW = eTextureTileStyle(vector3DFieldFromImage3D()->getTileStyleW());

}

Lib3MF_double CVector3DFieldFromImage3D::GetOffset()
{
	return vector3DFieldFromImage3D()->getOffset();
}

void CVector3DFieldFromImage3D::SetOffset(const Lib3MF_double dOffset)
{
	vector3DFieldFromImage3D()->setOffset(dOffset);
}

Lib3MF_double CVector3DFieldFromImage3D::GetScale()
{
	return vector3DFieldFromImage3D()->getScale();
}

void CVector3DFieldFromImage3D::SetScale(const Lib3MF_double dScale)
{
	vector3DFieldFromImage3D()->setScale(dScale);
}

