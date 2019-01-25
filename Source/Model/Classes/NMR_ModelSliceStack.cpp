/*++

Copyright (C) 2018 3MF Consortium

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

namespace NMR {

	CModelSliceStack::CModelSliceStack(_In_ const ModelResourceID sID, _In_ CModel * pModel, PSliceStack pSliceStack) : CModelResource(sID, pModel) 
	{
		m_pSliceStack = pSliceStack;
		m_nNumSliceRefsToMe = 0;
	}

	CModelSliceStack::~CModelSliceStack() {

	}

	void CModelSliceStack::setSliceStack(PSliceStack pSliceStack) {
		m_pSliceStack = pSliceStack;
	}

	_Ret_notnull_ PSliceStack CModelSliceStack::getSliceStack() {
		return m_pSliceStack;
	}

	nfUint32& CModelSliceStack::NumSliceRefsToMe()
	{
		return m_nNumSliceRefsToMe;
	}

	std::string CModelSliceStack::sliceRefPath() {
		return getSliceStack()->usesSliceRef() ? "/2D/2dmodel_" + std::to_string(getResourceID()->getUniqueID()) + ".model" : "";
	}

	CSliceStack::CSliceStack() {
		m_BottomZ = 0.0;
		m_bUsesSliceRef = false;
	}

	CSliceStack::~CSliceStack() {
	}

	_Ret_notnull_ PSlice CSliceStack::getSlice(nfUint32 nIndex) {
		return m_Slices[nIndex];
	}

	nfUint32 CSliceStack::addSlice(PSlice pSlice) {
		if (pSlice->getTopZ() < m_BottomZ)
			throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
		if (!m_Slices.empty()) {
			if (pSlice->getTopZ() < m_Slices.rbegin()->get()->getTopZ() ) {
				throw CNMRException(NMR_ERROR_SLICES_Z_NOTINCREASING);
			}
		}
		m_Slices.push_back(pSlice);
		return (nfUint32)m_Slices.size() - 1;
	}

	void CSliceStack::mergeSliceStack(PSliceStack pSliceStack)
	{
		for (int i = 0; i < (int)pSliceStack->getSliceCount(); i++)
		{
			this->addSlice(pSliceStack->getSlice(i));
		}
	}

	nfUint32 CSliceStack::getSliceCount() {
		return (nfUint32)m_Slices.size();
	}

	nfFloat CSliceStack::getBottomZ() {
		return m_BottomZ;
	}

	void CSliceStack::setBottomZ(nfFloat nBottomZ) {
		m_BottomZ = nBottomZ;
	}

	void CSliceStack::setUsesSliceRef(nfBool bUsesSliceRef) {
		m_bUsesSliceRef = bUsesSliceRef;
	}

	nfBool CSliceStack::usesSliceRef() {
		return m_bUsesSliceRef;
	}

	bool CSliceStack::areAllPolygonsClosed() {
		for (auto pSlice : m_Slices) {
			if (!pSlice->allPolygonsAreClosed())
				return false;
		}
		return true;
	}
}

