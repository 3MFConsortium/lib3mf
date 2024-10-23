/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CFunctionFromImage3D

*/

#include "lib3mf_functionfromimage3d.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_image3d.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CFunctionFromImage3D
**************************************************************************************************************************/

IImage3D * CFunctionFromImage3D::GetImage3D()
{
    auto id = functionfromimage3d()->getImage3DUniqueResourceID();
    auto image = model()->findImage3D(id);

    
    if(!image)
        throw ELib3MFInterfaceException(NMR_ERROR_INVALIDMODELRESOURCE);

    return new CImage3D(image);
}

NMR::CModelFunctionFromImage3D * Lib3MF::Impl::CFunctionFromImage3D::functionfromimage3d()
{
    auto pFunctionFromImage3D = std::dynamic_pointer_cast<NMR::CModelFunctionFromImage3D>(resource());
    
    if(!pFunctionFromImage3D)
    { 
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT, "Resource is not a FunctionFromImage3D Object");
    }

    return pFunctionFromImage3D.get();
}

Lib3MF::Impl::CFunctionFromImage3D::CFunctionFromImage3D(NMR::PModelResource pResource)
    : CResource(pResource)
    , CFunction(pResource)
{
}

void CFunctionFromImage3D::SetImage3D(IImage3D * pImage3D)
{
    if (pImage3D == nullptr)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }
	functionfromimage3d()->setImage3DUniqueResourceID(pImage3D->GetUniqueResourceID());
}

void CFunctionFromImage3D::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	functionfromimage3d()->setFilter((NMR::eModelTextureFilter) eFilter);
}

Lib3MF::eTextureFilter CFunctionFromImage3D::GetFilter()
{
	return (Lib3MF::eTextureFilter) functionfromimage3d()->getFilter();
}

void CFunctionFromImage3D::SetTileStyles(const Lib3MF::eTextureTileStyle eTileStyleU,
                                         const Lib3MF::eTextureTileStyle eTileStyleV,
                                         const Lib3MF::eTextureTileStyle eTileStyleW)
{
    functionfromimage3d()->setTileStyleU((NMR::eModelTextureTileStyle) eTileStyleU);
	functionfromimage3d()->setTileStyleV((NMR::eModelTextureTileStyle) eTileStyleV);
	functionfromimage3d()->setTileStyleW((NMR::eModelTextureTileStyle) eTileStyleW);
}

void CFunctionFromImage3D::GetTileStyles(Lib3MF::eTextureTileStyle & eTileStyleU,
                                         Lib3MF::eTextureTileStyle & eTileStyleV,
                                         Lib3MF::eTextureTileStyle & eTileStyleW)
{
    eTileStyleU = (Lib3MF::eTextureTileStyle) functionfromimage3d()->getTileStyleU();
	eTileStyleV = (Lib3MF::eTextureTileStyle) functionfromimage3d()->getTileStyleV();
	eTileStyleW = (Lib3MF::eTextureTileStyle) functionfromimage3d()->getTileStyleW();
}

Lib3MF_double CFunctionFromImage3D::GetOffset()
{
    return functionfromimage3d()->getOffset();
}

void CFunctionFromImage3D::SetOffset(const Lib3MF_double dOffset)
{
    functionfromimage3d()->setOffset(dOffset);
}

Lib3MF_double CFunctionFromImage3D::GetScale()
{
    return functionfromimage3d()->getScale();
}

void CFunctionFromImage3D::SetScale(const Lib3MF_double dScale)
{
    functionfromimage3d()->setScale(dScale);
}
