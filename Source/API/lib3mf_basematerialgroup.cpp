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

Abstract: This is a stub class definition of CLib3MFBaseMaterialGroup

*/

#include "lib3mf_basematerialgroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFBaseMaterialGroup 
**************************************************************************************************************************/

CLib3MFBaseMaterialGroup::CLib3MFBaseMaterialGroup(NMR::PModelBaseMaterialResource pResource)
	: CLib3MFResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{
}

NMR::CModelBaseMaterialResource& CLib3MFBaseMaterialGroup::baseMaterialGroup()
{
	NMR::CModelBaseMaterialResource* pBaseMaterialGroup = dynamic_cast<NMR::CModelBaseMaterialResource*>(resource().get());
	if (pBaseMaterialGroup == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDBASEMATERIALGROUP);
	return *pBaseMaterialGroup;
}

Lib3MF_uint32 CLib3MFBaseMaterialGroup::GetCount ()
{
	return baseMaterialGroup().getCount();
}

Lib3MF_uint32 CLib3MFBaseMaterialGroup::AddMaterial(const std::string & sName, const sLib3MFColor DisplayColor)
{
	NMR::nfColor cColor = DisplayColor.m_Red | (DisplayColor.m_Green << 8) | (DisplayColor.m_Blue << 16) | (DisplayColor.m_Alpha << 24);

	return baseMaterialGroup().addBaseMaterial(sName, cColor);
}

void CLib3MFBaseMaterialGroup::RemoveMaterial (const Lib3MF_uint32 nPropertyID)
{
	baseMaterialGroup().removeMaterial(nPropertyID);
}

std::string CLib3MFBaseMaterialGroup::GetName (const Lib3MF_uint32 nPropertyID)
{
	return baseMaterialGroup().getBaseMaterial(nPropertyID)->getName();
}

void CLib3MFBaseMaterialGroup::SetName (const Lib3MF_uint32 nPropertyID, const std::string & sName)
{
	baseMaterialGroup().getBaseMaterial(nPropertyID)->setName(sName);
}

void CLib3MFBaseMaterialGroup::SetDisplayColor(const Lib3MF_uint32 nPropertyID, const sLib3MFColor TheColor)
{
	NMR::nfColor cColor = TheColor.m_Red | (TheColor.m_Green << 8) | (TheColor.m_Blue << 16) | (TheColor.m_Alpha << 24);
	baseMaterialGroup().getBaseMaterial(nPropertyID)->setColor(cColor);
}

sLib3MFColor CLib3MFBaseMaterialGroup::GetDisplayColor(const Lib3MF_uint32 nPropertyID)
{
	NMR::nfColor cColor = baseMaterialGroup().getBaseMaterial(nPropertyID)->getDisplayColor();
	sLib3MFColor c;
	c.m_Red = (cColor) & 0xff;
	c.m_Green = (cColor >> 8) & 0xff;
	c.m_Blue = (cColor >> 16) & 0xff;
	c.m_Alpha = (cColor >> 24) & 0xff;
	return c;
}

void CLib3MFBaseMaterialGroup::GetAllPropertyIDs(Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	Lib3MF_uint32 nMaterialCount = baseMaterialGroup().getCount();

	if (pPropertyIDsNeededCount)
		*pPropertyIDsNeededCount = nMaterialCount;

	if (nPropertyIDsBufferSize >= nMaterialCount && pPropertyIDsBuffer) {
		if (!baseMaterialGroup().hasResourceIndexMap()) {
			baseMaterialGroup().buildResourceIndexMap();
		}
		for (Lib3MF_uint32 i = 0; i < nMaterialCount; i++) {
			DWORD nPropertyID;
			if (baseMaterialGroup().mapResourceIndexToPropertyID(i, nPropertyID)) {
				*pPropertyIDsBuffer = nPropertyID;
			}
			else {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDRESOURCEINDEX);
			}
			pPropertyIDsBuffer++;
		}
	}

}


