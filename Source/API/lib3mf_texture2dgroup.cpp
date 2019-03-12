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

Abstract: This is a stub class definition of CLib3MFTexture2DGroup

*/

#include "lib3mf_texture2dgroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_texture2d.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFTexture2DGroup 
**************************************************************************************************************************/

NMR::CModelTexture2DGroupResource& CLib3MFTexture2DGroup::texture2DGroup()
{
	NMR::CModelTexture2DGroupResource* pTexture2dGroup = dynamic_cast<NMR::CModelTexture2DGroupResource*>(resource().get());
	if (pTexture2dGroup == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDTEXTURE2DGROUP);
	return *pTexture2dGroup;
}

CLib3MFTexture2DGroup::CLib3MFTexture2DGroup(NMR::PModelTexture2DGroupResource pResource)
	: CLib3MFResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{

}

ILib3MFTexture2D * CLib3MFTexture2DGroup::GetTexture2D()
{
	return new CLib3MFTexture2D(texture2DGroup().getTexture2D());
}

Lib3MF_uint32 CLib3MFTexture2DGroup::GetCount ()
{
	return texture2DGroup().getCount();
}

void CLib3MFTexture2DGroup::GetAllPropertyIDs (Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	Lib3MF_uint32 nCount = texture2DGroup().getCount();

	if (pPropertyIDsNeededCount)
		*pPropertyIDsNeededCount = nCount;

	if (nPropertyIDsBufferSize >= nCount && pPropertyIDsBuffer) {
		if (!texture2DGroup().hasResourceIndexMap()) {
			texture2DGroup().buildResourceIndexMap();
		}
		for (Lib3MF_uint32 i = 0; i < nCount; i++) {
			DWORD nPropertyID;
			if (texture2DGroup().mapResourceIndexToPropertyID(i, nPropertyID)) {
				*pPropertyIDsBuffer = nPropertyID;
			}
			else {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDRESOURCEINDEX);
			}
			pPropertyIDsBuffer++;
		}
	}
}

Lib3MF_uint32 CLib3MFTexture2DGroup::AddTex2Coord (const sLib3MFTex2Coord UVCoordinate)
{
	return texture2DGroup().addUVCoordinate(NMR::MODELTEXTURE2DCOORDINATE({ UVCoordinate.m_U, UVCoordinate.m_V }));
}

sLib3MFTex2Coord CLib3MFTexture2DGroup::GetTex2Coord (const Lib3MF_uint32 nPropertyID)
{
	NMR::MODELTEXTURE2DCOORDINATE coord = texture2DGroup().getUVCoordinate(nPropertyID);
	return sLib3MFTex2Coord({ coord.m_dU, coord.m_dV});
}

void CLib3MFTexture2DGroup::RemoveTex2Coord(const Lib3MF_uint32 nPropertyID)
{
	texture2DGroup().removePropertyID(nPropertyID);
}

