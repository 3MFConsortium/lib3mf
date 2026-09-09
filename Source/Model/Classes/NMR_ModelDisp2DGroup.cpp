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

#include "Model/Classes/NMR_ModelDisp2DGroup.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include <cmath>

namespace NMR {

	CModelDisp2DGroupResource::CModelDisp2DGroupResource(ModelResourceID nID, CModel * pModel,
		PModelDisplacement2DResource pDisplacement2D, PModelNormVectorGroupResource pNormVectorGroup,
		nfDouble dHeight, nfDouble dOffset)
		: CModelResource(nID, pModel), m_pDisplacement2D(pDisplacement2D),
		m_pNormVectorGroup(pNormVectorGroup), m_dHeight(dHeight), m_dOffset(dOffset)
	{
		if (!pDisplacement2D || !pNormVectorGroup)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (pDisplacement2D->getModel() != pModel || pNormVectorGroup->getModel() != pModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		setHeight(dHeight);
		setOffset(dOffset);
	}

	void CModelDisp2DGroupResource::validateCoordinate(const MODELDISPLACEMENT2DCOORDINATE & sCoordinate) const
	{
		if (!std::isfinite(sCoordinate.m_dU) || !std::isfinite(sCoordinate.m_dV) ||
			!std::isfinite(sCoordinate.m_dDisplacementFactor) || sCoordinate.m_dDisplacementFactor < 0.0)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (sCoordinate.m_nNormalVectorIndex >= m_pNormVectorGroup->getCount())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
	}

	PModelDisplacement2DResource CModelDisp2DGroupResource::getDisplacement2D() const { return m_pDisplacement2D; }
	PModelNormVectorGroupResource CModelDisp2DGroupResource::getNormVectorGroup() const { return m_pNormVectorGroup; }
	nfDouble CModelDisp2DGroupResource::getHeight() const { return m_dHeight; }
	void CModelDisp2DGroupResource::setHeight(nfDouble dHeight) { if (!std::isfinite(dHeight)) throw CNMRException(NMR_ERROR_INVALIDPARAM); m_dHeight = dHeight; }
	nfDouble CModelDisp2DGroupResource::getOffset() const { return m_dOffset; }
	void CModelDisp2DGroupResource::setOffset(nfDouble dOffset) { if (!std::isfinite(dOffset)) throw CNMRException(NMR_ERROR_INVALIDPARAM); m_dOffset = dOffset; }
	nfUint32 CModelDisp2DGroupResource::getCount() const { return (nfUint32)m_Coordinates.size(); }

	nfUint32 CModelDisp2DGroupResource::addCoordinate(const MODELDISPLACEMENT2DCOORDINATE & sCoordinate)
	{
		if (m_Coordinates.size() >= XML_3MF_MAXRESOURCEINDEX)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		validateCoordinate(sCoordinate);
		m_Coordinates.push_back(sCoordinate);
		return (nfUint32)m_Coordinates.size() - 1;
	}

	MODELDISPLACEMENT2DCOORDINATE CModelDisp2DGroupResource::getCoordinate(nfUint32 nIndex) const
	{
		if (nIndex >= m_Coordinates.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Coordinates[nIndex];
	}

	void CModelDisp2DGroupResource::setCoordinate(nfUint32 nIndex, const MODELDISPLACEMENT2DCOORDINATE & sCoordinate)
	{
		if (nIndex >= m_Coordinates.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		validateCoordinate(sCoordinate);
		m_Coordinates[nIndex] = sCoordinate;
	}

	ResourceDependencies CModelDisp2DGroupResource::getDependencies()
	{
		return { m_pDisplacement2D->getPackageResourceID(), m_pNormVectorGroup->getPackageResourceID() };
	}
}
