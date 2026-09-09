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

Abstract: This is a stub class definition of CDisplacement2D

*/

#include "lib3mf_displacement2d.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_attachment.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
Class definition of CDisplacement2D
**************************************************************************************************************************/

CDisplacement2D::CDisplacement2D(NMR::PModelDisplacement2DResource pResource)
	: CResource(pResource)
{
}

NMR::PModelDisplacement2DResource CDisplacement2D::displacement2D()
{
	auto pResource = std::dynamic_pointer_cast<NMR::CModelDisplacement2DResource>(resource());
	if (!pResource)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);
	return pResource;
}

IAttachment * CDisplacement2D::GetAttachment()
{
	return new CAttachment(displacement2D()->getAttachment());
}

void CDisplacement2D::SetAttachment(IAttachment* pAttachment)
{
	if (!pAttachment)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	auto pModelAttachment = displacement2D()->getModel()->findModelAttachment(pAttachment->GetPath());
	displacement2D()->setAttachment(pModelAttachment);
}

Lib3MF::eChannelName CDisplacement2D::GetChannel()
{
	return static_cast<Lib3MF::eChannelName>(displacement2D()->getChannel());
}

void CDisplacement2D::SetChannel(const Lib3MF::eChannelName eChannel)
{
	displacement2D()->setChannel(static_cast<NMR::eModelColorChannel>(eChannel));
}

void CDisplacement2D::GetTileStyleUV(Lib3MF::eTextureTileStyle & eTileStyleU, Lib3MF::eTextureTileStyle & eTileStyleV)
{
	eTileStyleU = static_cast<Lib3MF::eTextureTileStyle>(displacement2D()->getTileStyleU());
	eTileStyleV = static_cast<Lib3MF::eTextureTileStyle>(displacement2D()->getTileStyleV());
}

void CDisplacement2D::SetTileStyleUV(const Lib3MF::eTextureTileStyle eTileStyleU, const Lib3MF::eTextureTileStyle eTileStyleV)
{
	displacement2D()->setTileStyleU(static_cast<NMR::eModelTextureTileStyle>(eTileStyleU));
	displacement2D()->setTileStyleV(static_cast<NMR::eModelTextureTileStyle>(eTileStyleV));
}

Lib3MF::eTextureFilter CDisplacement2D::GetFilter()
{
	return static_cast<Lib3MF::eTextureFilter>(displacement2D()->getFilter());
}

void CDisplacement2D::SetFilter(const Lib3MF::eTextureFilter eFilter)
{
	displacement2D()->setFilter(static_cast<NMR::eModelTextureFilter>(eFilter));
}
