/*++

Copyright (C) 2026 3MF Consortium

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

In-memory representation and serialization support for the 3MF Displacement extension.

--*/

#include "Model/Classes/NMR_ModelNormVectorGroup.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <cmath>

namespace NMR {

	CModelNormVectorGroupResource::CModelNormVectorGroupResource(ModelResourceID nID, CModel * pModel)
		: CModelResource(nID, pModel)
	{
	}

	MODELDISPLACEMENTVECTOR CModelNormVectorGroupResource::normalize(const MODELDISPLACEMENTVECTOR & vVector)
	{
		const nfDouble dLength = std::sqrt(vVector[0] * vVector[0] + vVector[1] * vVector[1] + vVector[2] * vVector[2]);
		if (!std::isfinite(dLength) || dLength <= 0.0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		return { vVector[0] / dLength, vVector[1] / dLength, vVector[2] / dLength };
	}

	nfUint32 CModelNormVectorGroupResource::getCount() const { return (nfUint32)m_Vectors.size(); }

	nfUint32 CModelNormVectorGroupResource::addVector(const MODELDISPLACEMENTVECTOR & vVector)
	{
		if (m_Vectors.size() >= XML_3MF_MAXRESOURCEINDEX)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		m_Vectors.push_back(normalize(vVector));
		return (nfUint32)m_Vectors.size() - 1;
	}

	MODELDISPLACEMENTVECTOR CModelNormVectorGroupResource::getVector(nfUint32 nIndex) const
	{
		if (nIndex >= m_Vectors.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Vectors[nIndex];
	}

	void CModelNormVectorGroupResource::setVector(nfUint32 nIndex, const MODELDISPLACEMENTVECTOR & vVector)
	{
		if (nIndex >= m_Vectors.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		m_Vectors[nIndex] = normalize(vVector);
	}
}
