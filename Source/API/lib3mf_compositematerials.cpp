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

Abstract: This is a stub class definition of CCompositeMaterials

*/

#include "lib3mf_compositematerials.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_basematerialgroup.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CCompositeMaterials 
**************************************************************************************************************************/


CCompositeMaterials::CCompositeMaterials(NMR::PModelCompositeMaterialsResource pResource)
	: CResource(std::static_pointer_cast<NMR::CModelResource>(pResource))
{

}


NMR::CModelCompositeMaterialsResource& CCompositeMaterials::compositeMaterials()
{
	NMR::CModelCompositeMaterialsResource* pCompositeMaterials = dynamic_cast<NMR::CModelCompositeMaterialsResource*>(resource().get());
	if (pCompositeMaterials == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCOMPOSITEMATERIALS);
	return *pCompositeMaterials;
}

Lib3MF_uint32 CCompositeMaterials::GetCount ()
{
	return compositeMaterials().getCount();
}

void CCompositeMaterials::GetAllPropertyIDs (Lib3MF_uint64 nPropertyIDsBufferSize, Lib3MF_uint64* pPropertyIDsNeededCount, Lib3MF_uint32 * pPropertyIDsBuffer)
{
	Lib3MF_uint32 nCompositesCount = compositeMaterials().getCount();

	if (pPropertyIDsNeededCount)
		*pPropertyIDsNeededCount = nCompositesCount;

	if (nPropertyIDsBufferSize >= nCompositesCount && pPropertyIDsBuffer) {
		if (!compositeMaterials().hasResourceIndexMap()) {
			compositeMaterials().buildResourceIndexMap();
		}
		for (Lib3MF_uint32 i = 0; i < nCompositesCount; i++) {
			DWORD nPropertyID;
			if (compositeMaterials().mapResourceIndexToPropertyID(i, nPropertyID)) {
				*pPropertyIDsBuffer = nPropertyID;
			}
			else {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDRESOURCEINDEX);
			}
			pPropertyIDsBuffer++;
		}
	}
}

IBaseMaterialGroup * CCompositeMaterials::GetBaseMaterialGroup ()
{
	return new CBaseMaterialGroup(compositeMaterials().getBaseMaterialResource());
}

Lib3MF_uint32 CCompositeMaterials::AddComposite(const Lib3MF_uint64 nCompositeBufferSize, const sLib3MFCompositeConstituent * pCompositeBuffer)
{
	if (nCompositeBufferSize > LIB3MF_MAXCOMPOSITEMATERIALS)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	NMR::PModelComposite constituents = std::make_shared<NMR::CModelComposite>();
	constituents->resize((size_t)nCompositeBufferSize);
	for (size_t i = 0; i < (size_t)nCompositeBufferSize; i++) {
		(*constituents)[i].m_dMixingRatio = pCompositeBuffer[i].m_MixingRatio;
		(*constituents)[i].m_nPropertyID = pCompositeBuffer[i].m_PropertyID;
	}
	return compositeMaterials().addComposite(constituents);
}

void CCompositeMaterials::RemoveComposite (const Lib3MF_uint32 nPropertyID)
{
	compositeMaterials().removeComposite(nPropertyID);
}

void CCompositeMaterials::GetComposite(const Lib3MF_uint32 nPropertyID, Lib3MF_uint64 nCompositeBufferSize, Lib3MF_uint64* pCompositeNeededCount, sLib3MFCompositeConstituent * pCompositeBuffer)
{

	if (nCompositeBufferSize > LIB3MF_MAXCOMPOSITEMATERIALS)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	NMR::PModelComposite constituents = compositeMaterials().getComposite(nPropertyID);
	
	if (pCompositeNeededCount) {
		*pCompositeNeededCount = constituents->size();
	}

	if (pCompositeBuffer && nCompositeBufferSize >= constituents->size()) {
		for (size_t i = 0; i < (size_t) nCompositeBufferSize; i++) {
			pCompositeBuffer[i].m_MixingRatio = (*constituents)[i].m_dMixingRatio;
			pCompositeBuffer[i].m_PropertyID = (*constituents)[i].m_nPropertyID;
		}
	}

}


