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

NMR_ModelSliceStackResource.h: implements the resource object for a slice stack

--*/

#include "Model/Classes/NMR_ModelSliceStack.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Common/NMR_Exception.h"

#include <cmath>

namespace NMR {

	CModelSliceStack::CModelSliceStack(_In_ const ModelResourceID sID, _In_ CModel * pModel, nfDouble dZBottom)
		: CModelResource(sID, pModel)
	{
		m_dZBottom = dZBottom;
	}

	CModelSliceStack::~CModelSliceStack()
	{

	}

	PSlice CModelSliceStack::AddSlice(const nfDouble dZTop)
	{
		if (!AllowsGeometry()) {
			throw CNMRException(NMR_ERROR_SLICES_MIXING_SLICES_WITH_SLICEREFS);
		}

		if (m_pSlices.size() > 0)
		{
			if (m_pSlices.back()->getTopZ() >= dZTop)
				throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
		}
		else {
			if (m_dZBottom >= dZTop)
				throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
		}
		PSlice pSlice = std::make_shared<CSlice>(dZTop);
		m_pSlices.push_back(pSlice);
		return pSlice;
	}

	void CModelSliceStack::AddSliceRef(PModelSliceStack pOtherStack)
	{
		if (!AllowsReferences()) {
			throw CNMRException(NMR_ERROR_SLICES_MIXING_SLICES_WITH_SLICEREFS);
		}
		if (this == pOtherStack.get()) {
			throw CNMRException(NMR_ERROR_SLICES_SLICEREF_CIRCULAR);
		}
		if (pOtherStack->getSliceRefCount() > 0) {
			throw CNMRException(NMR_ERROR_SLICES_REFS_LEVELTOODEEP);
		}
		if (pOtherStack->getZBottom() < getHighestZ()) {
			throw CNMRException(NMR_ERROR_SLICES_REFS_Z_NOTINCREASING);
		}

		// TODO: check in model, whether "this" is used as sliceref anywhere in the model
		m_pSliceRefs.push_back(pOtherStack);
	}

	nfUint32 CModelSliceStack::getSliceCount()
	{
		return nfUint32(m_pSlices.size());
	}

	PSlice CModelSliceStack::getSlice(nfUint32 nIndex)
	{
		if (nIndex >= m_pSlices.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_pSlices[nIndex];
	}

	bool CModelSliceStack::AllowsGeometry() const
	{
		return m_pSliceRefs.empty();
	}

	nfUint32 CModelSliceStack::getSliceRefCount()
	{
		return nfUint32(m_pSliceRefs.size());
	}

	PModelSliceStack CModelSliceStack::getSliceRef(nfUint32 nIndex)
	{
		if (nIndex >= m_pSliceRefs.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_pSliceRefs[nIndex];
	}

	bool CModelSliceStack::AllowsReferences() const
	{
		return m_pSlices.empty();
	}


	void CModelSliceStack::CollapseSliceReferences()
	{
		m_pSlices.clear();
		nfUint64 nSlices = 0;
		for (auto pStack : m_pSliceRefs) {
			nSlices += pStack->getSliceCount();
		}
		m_pSlices.reserve(nSlices);
		for (auto pStack : m_pSliceRefs) {
			for (auto pOldSlice : pStack->m_pSlices) {
				m_pSlices.push_back(std::make_shared<CSlice>(*pOldSlice.get()));
			}
		}
	}

	std::string CModelSliceStack::OwnPath()
	{
		std::string sPath = getPackageResourceID()->getPath();
		if (sPath.empty()) {
			throw CNMRException(NMR_ERROR_INVALID_SLICEPATH);
		}
		return sPath;
	}

	void CModelSliceStack::SetOwnPath(std::string sOwnPath)
	{
		PPackageModelPath pModelPath = getModel()->findOrCreateModelPath(sOwnPath);
		CPackageResourceID::setModelPath(getPackageResourceID(), pModelPath);
	}

	nfDouble CModelSliceStack::getZBottom()
	{
		return m_dZBottom;
	}

	void CModelSliceStack::setZBottom(nfDouble dZBottom)
	{
		if (!m_pSlices.empty()) {
			if (m_pSlices.front()->getTopZ() <= dZBottom)
				throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
		}
		if (!m_pSliceRefs.empty()) {
			if (m_pSliceRefs.front()->getZBottom() < dZBottom)
				throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
		}
		m_dZBottom = dZBottom;
	}

	nfDouble CModelSliceStack::getHighestZ() const
	{
		nfDouble dHighestZ = m_dZBottom;
		if (!m_pSlices.empty()) {
			dHighestZ = std::max(dHighestZ, m_pSlices.back()->getTopZ());
		}
		if (!m_pSliceRefs.empty()) {
			dHighestZ = std::max(dHighestZ, m_pSliceRefs.back()->getHighestZ());
		}
		return dHighestZ;
	}

	bool CModelSliceStack::areAllPolygonsClosed()
	{
		for (auto pSlice : m_pSlices) {
			if (!pSlice->allPolygonsAreClosed())
				return false;
		}

		for (auto pSliceRef : m_pSliceRefs) {
			if (!pSliceRef->areAllPolygonsClosed())
				return false;
		}

		return true;
	}

}

