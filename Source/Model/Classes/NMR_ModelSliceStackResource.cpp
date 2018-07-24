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

#include "Model/Classes/NMR_ModelSliceResource.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Common/NMR_Exception.h"

namespace NMR {
	CSlice::CSlice() {
		m_topZ = 0.0;
	}

	CSlice::~CSlice() {

	}

	nfUint32 CSlice::beginPolygon() {
		m_Polygons.push_back(std::vector<nfUint32>());
		return (nfUint32)m_Polygons.size() - 1;
	}

	nfUint32 CSlice::addVertex(nfFloat x, nfFloat y) {
		SLICENODE cNode;

		cNode.m_position.m_fields[0] = x;
		cNode.m_position.m_fields[1] = y;

		cNode.m_index = (nfUint32)m_Vertices.size();

		m_Vertices.push_back(cNode);

		return cNode.m_index;
	}

	void CSlice::getVertex(nfUint32 nIndex, nfFloat *x, nfFloat *y) {
		if (nIndex >= m_Vertices.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		*x = m_Vertices[nIndex].m_position.m_values.x;
		*y = m_Vertices[nIndex].m_position.m_values.y;
	}

	void CSlice::addPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndex) {
		if (nPolygonIndex >= m_Polygons.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		if (nIndex >= m_Vertices.size())
			throw CNMRException(NMR_ERROR_INVALID_SLICESEGMENT_VERTEXINDEX);

		if (m_Polygons[nPolygonIndex].size() > 0) {
			if (m_Polygons[nPolygonIndex][m_Polygons[nPolygonIndex].size()-1] == nIndex)
				throw CNMRException(NMR_ERROR_INVALID_SLICESEGMENT_VERTEXINDEX);
		}
		m_Polygons[nPolygonIndex].push_back(nIndex);
	}

	nfUint32 CSlice::getPolygonCount() {
		return (nfUint32)m_Polygons.size();
	}

	nfFloat CSlice::getTopZ() {
		return m_topZ;
	}

	void CSlice::setTopZ(nfFloat nTopZ) {
		m_topZ = nTopZ;
	}

	_Ret_notnull_ SLICENODE *CSlice::getNode(nfUint32 nIndex) {
		return &m_Vertices[nIndex];
	}

	nfUint32 CSlice::getVertexCount() {
		return (nfUint32)m_Vertices.size();
	}

	bool CSlice::allPolygonsAreClosed() {
		for (auto polygon : m_Polygons) {
			if (polygon.size() > 1) {
				if (*polygon.begin() != *polygon.rbegin()) {
					return false;
				}
			}
		}
		return true;
	}

	bool CSlice::isPolygonValid(nfUint32 nPolygonIndex)
	{
		if (nPolygonIndex >= m_Polygons.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		if (m_Polygons[nPolygonIndex].size() > 2)
			return true;
		if (m_Polygons[nPolygonIndex].size() <= 1)
			return false;
		// closed polygon must have 3 points or more.
		return m_Polygons[nPolygonIndex][0] != m_Polygons[nPolygonIndex][1];
	}

	nfUint32 CSlice::getNumberOfPolygons() {
		return (nfUint32)m_Polygons.size();
	}

	nfUint32 CSlice::getPolygonIndex(nfUint32 nPolygonIndex, nfUint32 nIndexOfIndex) {
		if (nPolygonIndex >= m_Polygons.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		if (nIndexOfIndex >= m_Polygons[nPolygonIndex].size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_Polygons[nPolygonIndex][nIndexOfIndex];
	}

	nfUint32 CSlice::getPolygonIndexCount(nfUint32 nPolygonIndex) {
		if (nPolygonIndex >= m_Polygons.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return (nfUint32)m_Polygons[nPolygonIndex].size();
	}


	CModelSliceStackResource::CModelSliceStackResource(_In_ const ModelResourceID sID, _In_ CModel * pModel, PSliceStack pSliceStack) : CModelResource(sID, pModel) 
	{
		m_pSliceStack = pSliceStack;
		m_nNumSliceRefsToMe = 0;
	}

	CModelSliceStackResource::~CModelSliceStackResource() {

	}

	void CModelSliceStackResource::setSliceStack(PSliceStack pSliceStack) {
		m_pSliceStack = pSliceStack;
	}

	_Ret_notnull_ PSliceStack CModelSliceStackResource::getSliceStack() {
		return m_pSliceStack;
	}

	nfUint32& CModelSliceStackResource::NumSliceRefsToMe()
	{
		return m_nNumSliceRefsToMe;
	}

	std::string CModelSliceStackResource::sliceRefPath() {
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

