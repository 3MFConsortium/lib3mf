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

Abstract: This is a stub class definition of CMultiPropertyGroup

*/

#include "lib3mf_multipropertygroup.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CMultiPropertyGroup 
**************************************************************************************************************************/

CMultiPropertyGroup::CMultiPropertyGroup(NMR::PModelMultiPropertyGroupResource pResource)
	: CResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{

}


NMR::CModelMultiPropertyGroupResource& CMultiPropertyGroup::multiPropertyGroup()
{
	NMR::CModelMultiPropertyGroupResource* pMultiPropertyGroup = dynamic_cast<NMR::CModelMultiPropertyGroupResource*>(resource().get());
	if (pMultiPropertyGroup == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDMULTIPROPERTYGROUP);
	return *pMultiPropertyGroup;
}

Lib3MF_uint32 CMultiPropertyGroup::GetCount ()
{
	return multiPropertyGroup().getCount();
}

void CMultiPropertyGroup::GetAllPropertyIDs (Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	Lib3MF_uint32 nMultiMaterialCount = multiPropertyGroup().getCount();

	if (pPropertyIDsNeededCount)
		*pPropertyIDsNeededCount = nMultiMaterialCount;

	if (nPropertyIDsBufferSize >= nMultiMaterialCount && pPropertyIDsBuffer) {
		if (!multiPropertyGroup().hasResourceIndexMap()) {
			multiPropertyGroup().buildResourceIndexMap();
		}
		for (Lib3MF_uint32 i = 0; i < nMultiMaterialCount; i++) {
			DWORD nPropertyID;
			if (multiPropertyGroup().mapResourceIndexToPropertyID(i, nPropertyID)) {
				*pPropertyIDsBuffer = nPropertyID;
			}
			else {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDRESOURCEINDEX);
			}
			pPropertyIDsBuffer++;
		}
	}
}

Lib3MF_uint32 CMultiPropertyGroup::AddMultiProperty (const Lib3MF_uint64 nPropertyIDsBufferSize, const Lib3MF_uint32 * pPropertyIDsBuffer)
{
	if (nPropertyIDsBufferSize > LIB3MF_MAXMULTIPROPERTIES)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	NMR::PModelMultiProperty multiProperty = std::make_shared<NMR::CModelMultiProperty>();
	multiProperty->resize((size_t)nPropertyIDsBufferSize);
	for (size_t i = 0; i < (size_t) nPropertyIDsBufferSize; i++) {
		(*multiProperty)[i] = pPropertyIDsBuffer[i];
	}
	return multiPropertyGroup().addMultiProperty(multiProperty);
}

void CMultiPropertyGroup::SetMultiProperty (const Lib3MF_uint32 nPropertyID, const Lib3MF_uint64 nPropertyIDsBufferSize, const Lib3MF_uint32 * pPropertyIDsBuffer)
{
	if (nPropertyIDsBufferSize > LIB3MF_MAXMULTIPROPERTIES)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	NMR::PModelMultiProperty multiProperty = std::make_shared<NMR::CModelMultiProperty>();
	multiProperty->resize((size_t)nPropertyIDsBufferSize);
	for (size_t i = 0; i < (size_t)nPropertyIDsBufferSize; i++) {
		(*multiProperty)[i] = pPropertyIDsBuffer[i];
	}
	multiPropertyGroup().setMultiProperty(nPropertyID, multiProperty);
}

void CMultiPropertyGroup::GetMultiProperty (const Lib3MF_uint32 nPropertyID, Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	NMR::PModelMultiProperty multiProperty = multiPropertyGroup().getMultiProperty(nPropertyID);

	if (pPropertyIDsNeededCount) {
		*pPropertyIDsNeededCount = multiProperty->size();
	}

	if (pPropertyIDsBuffer && nPropertyIDsBufferSize >= multiProperty->size()) {

		if (nPropertyIDsBufferSize > LIB3MF_MAXMULTIPROPERTIES)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

		for (size_t i = 0; i < (size_t)nPropertyIDsBufferSize; i++) {
			pPropertyIDsBuffer[i] = (*multiProperty)[i];
		}
	}
}

void CMultiPropertyGroup::RemoveMultiProperty (const Lib3MF_uint32 nPropertyID)
{
	multiPropertyGroup().removeMultiProperty(nPropertyID);
}

Lib3MF_uint32 CMultiPropertyGroup::GetLayerCount ()
{
	return multiPropertyGroup().getLayerCount();
}

Lib3MF_uint32 CMultiPropertyGroup::AddLayer (const sLib3MFMultiPropertyLayer TheLayer)
{
	return multiPropertyGroup().addLayer(NMR::MODELMULTIPROPERTYLAYER{ TheLayer.m_ResourceID, NMR::eModelBlendMethod(TheLayer.m_TheBlendMethod)});
}

sLib3MFMultiPropertyLayer CMultiPropertyGroup::GetLayer (const Lib3MF_uint32 nLayerIndex)
{
	NMR::MODELMULTIPROPERTYLAYER sLayer = multiPropertyGroup().getLayer(nLayerIndex);
	sLib3MFMultiPropertyLayer outLayer;
	outLayer.m_ResourceID = sLayer.m_nUniqueResourceID;
	outLayer.m_TheBlendMethod = (eLib3MFBlendMethod)sLayer.m_nMethod;
	return outLayer;
}

void CMultiPropertyGroup::RemoveLayer (const Lib3MF_uint32 nLayerIndex)
{
	multiPropertyGroup().removeLayer(nLayerIndex);
}

