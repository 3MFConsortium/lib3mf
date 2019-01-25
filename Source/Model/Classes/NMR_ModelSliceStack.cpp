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

	CModelSliceStack::CModelSliceStack(_In_ const ModelResourceID sID, _In_ CModel * pModel, PSliceStackGeometry pSliceStackGeometry)
		: CModelResource(sID, pModel) 
	{
		m_pSliceStackGeometry = pSliceStackGeometry;
		m_nNumSliceRefsToMe = 0;
		m_dZBottom = 0.0;
	}

	CModelSliceStack::CModelSliceStack(_In_ const ModelResourceID sID, _In_ CModel * pModel, nfDouble dZBottom)
		: CModelResource(sID, pModel)
	{
		m_pSliceStackGeometry = std::make_shared<CSliceStackGeometry>();
		m_nNumSliceRefsToMe = 0;
		m_dZBottom = dZBottom;
	}

	CModelSliceStack::~CModelSliceStack()
	{

	}

	//PSlice CModelSliceStack::AddSliceRef()
	//{
	//	if (!AllowsReferences()) {
	//		throw CNMRException(NMR_ERROR_SLICES_MIXING_SLICES_WITH_SLICEREFS);
	//	}

	//}

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

	nfUint32 CModelSliceStack::getSliceCount()
	{
		return nfUint32(m_pSlices.size());
	}

	bool CModelSliceStack::AllowsGeometry() const
	{
		return m_pSliceRefs.empty();
	}

	nfUint32 CModelSliceStack::getSliceRefCount()
	{
		return nfUint32(m_pSliceRefs.size());
	}

	bool CModelSliceStack::AllowsReferences() const
	{
		return m_pSlices.empty();
	}

	_Ret_notnull_ PSliceStackGeometry CModelSliceStack::Geometry()
	{
		return m_pSliceStackGeometry;
	}

	nfUint32& CModelSliceStack::NumSliceRefsToMe()
	{
		return m_nNumSliceRefsToMe;
	}

	std::string CModelSliceStack::sliceRefPath()
	{
		return Geometry()->usesSliceRef() ? "/2D/2dmodel_" + std::to_string(getResourceID()->getUniqueID()) + ".model" : "";
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





	CSliceStackGeometry::CSliceStackGeometry()
	{
		m_BottomZ = 0.0;
		m_bUsesSliceRef = false;
	}

	CSliceStackGeometry::~CSliceStackGeometry()
	{
	}

	_Ret_notnull_ PSlice CSliceStackGeometry::getSlice(nfUint32 nIndex)
	{
		return m_Slices[nIndex];
	}

	nfUint32 CSliceStackGeometry::addSlice(PSlice pSlice)
	{
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

	void CSliceStackGeometry::mergeSliceStackGeometry(PSliceStackGeometry pSliceStackGeometry)
	{
		for (int i = 0; i < (int)pSliceStackGeometry->getSliceCount(); i++)
		{
			this->addSlice(pSliceStackGeometry->getSlice(i));
		}
	}

	nfUint32 CSliceStackGeometry::getSliceCount()
	{
		return (nfUint32)m_Slices.size();
	}

	nfFloat CSliceStackGeometry::getBottomZ()
	{
		return m_BottomZ;
	}

	void CSliceStackGeometry::setBottomZ(nfFloat nBottomZ)
	{
		m_BottomZ = nBottomZ;
	}

	void CSliceStackGeometry::setUsesSliceRef(nfBool bUsesSliceRef)
	{
		m_bUsesSliceRef = bUsesSliceRef;
	}

	nfBool CSliceStackGeometry::usesSliceRef()
	{
		return m_bUsesSliceRef;
	}

	bool CSliceStackGeometry::areAllPolygonsClosed()
	{
		for (auto pSlice : m_Slices) {
			if (!pSlice->allPolygonsAreClosed())
				return false;
		}
		return true;
	}

	
}

