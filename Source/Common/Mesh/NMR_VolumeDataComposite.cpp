/*++

Copyright (C) 2019 3MF Consortium

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

Abstract:

NMR_VolumeComposite.cpp implements the class CVolumeDataComposite.

--*/

#include "Common/Mesh/NMR_VolumeDataComposite.h"

namespace NMR {

	CVolumeDataComposite::CVolumeDataComposite()
	{

	}

	NMR::PModelBaseMaterialResource CVolumeDataComposite::getBaseMaterialGroup()
	{
		return m_pBaseMaterialGroup;
	}

	void CVolumeDataComposite::setBaseMaterialGroup(PModelBaseMaterialResource pBaseMaterialGroup)
	{
		m_pBaseMaterialGroup = pBaseMaterialGroup;
	}

	nfUint32 CVolumeDataComposite::materialMappingCount() const
	{
		return (nfUint32)m_vctFunctionReferences.size();
	}

	PFunctionReference CVolumeDataComposite::getMaterialMapping(nfUint32 nIndex)
	{
		if (nIndex >= materialMappingCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_vctFunctionReferences[nIndex];
	}

	PFunctionReference CVolumeDataComposite::addMaterialMapping(PModelFunction pFunction)
	{
		PFunctionReference pFunctionReference = std::make_shared<CFunctionReference>(pFunction);
		m_vctFunctionReferences.push_back(pFunctionReference);
		return pFunctionReference;
	}

	void CVolumeDataComposite::removeMaterialMapping(nfUint32)
	{}

}
