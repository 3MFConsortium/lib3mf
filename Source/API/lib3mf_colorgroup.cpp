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

Abstract: This is a stub class definition of CLib3MFColorGroup

*/

#include "lib3mf_colorgroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFColorGroup 
**************************************************************************************************************************/

CLib3MFColorGroup::CLib3MFColorGroup(NMR::PModelColorGroupResource pResource)
	: CLib3MFResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{

}


NMR::CModelColorGroupResource& CLib3MFColorGroup::colorGroup()
{
	NMR::CModelColorGroupResource* pColorGroup = dynamic_cast<NMR::CModelColorGroupResource*>(resource().get());
	if (pColorGroup == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOLORGROUP);
	return *pColorGroup;
}


Lib3MF_uint32 CLib3MFColorGroup::GetCount ()
{
	return colorGroup().getCount();
}

void CLib3MFColorGroup::GetAllPropertyIDs (Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	Lib3MF_uint32 nCount = colorGroup().getCount();

	if (pPropertyIDsNeededCount)
		*pPropertyIDsNeededCount = nCount;

	if (nPropertyIDsBufferSize >= nCount && pPropertyIDsBuffer) {
		if (!colorGroup().hasResourceIndexMap()) {
			colorGroup().buildResourceIndexMap();
		}
		for (Lib3MF_uint32 i = 0; i < nCount; i++) {
			DWORD nPropertyID;
			if (colorGroup().mapResourceIndexToPropertyID(i, nPropertyID)) {
				*pPropertyIDsBuffer = nPropertyID;
			}
			else {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDRESOURCEINDEX);
			}
			pPropertyIDsBuffer++;
		}
	}
}

Lib3MF_uint32 CLib3MFColorGroup::AddColor (const sLib3MFColor TheColor)
{
	NMR::nfColor cColor = TheColor.m_Red | (TheColor.m_Green << 8) | (TheColor.m_Blue << 16) | (TheColor.m_Alpha << 24);

	return colorGroup().addColor(cColor);
}

void CLib3MFColorGroup::SetColor (const Lib3MF_uint32 nPropertyID, const sLib3MFColor TheColor)
{
	NMR::nfColor cColor = TheColor.m_Red | (TheColor.m_Green << 8) | (TheColor.m_Blue << 16) | (TheColor.m_Alpha << 24);
	colorGroup().getColor(nPropertyID);
}

sLib3MFColor CLib3MFColorGroup::GetColor (const Lib3MF_uint32 nPropertyID)
{
	NMR::nfColor cColor = colorGroup().getColor(nPropertyID);
	sLib3MFColor c;
	c.m_Red = (cColor) & 0xff;
	c.m_Green = (cColor >> 8) & 0xff;
	c.m_Blue = (cColor >> 16) & 0xff;
	c.m_Alpha = (cColor >> 24) & 0xff;
	return c;
}

void CLib3MFColorGroup::RemoveColor(const Lib3MF_uint32 nPropertyID)
{
	colorGroup().removeColor(nPropertyID);
}
