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

NMR_ModelTexture2D.cpp implements the Model Texture Class.

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelNurbsSurface.h"
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelNurbsSurface::CModelNurbsSurface(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nDegreeU, _In_ nfUint32 nDegreeV, _In_ nfUint32 nControlPointCountU, _In_ nfUint32 nControlPointCountV)
		: CModelResource(sID, pModel)
	{
		setDegree(nDegreeU, nDegreeV);
		setControlPointCount(nControlPointCountU, nControlPointCountV);

		m_nNextUVID = 1;
	}


	nfUint32 CModelNurbsSurface::getDegreeU()
	{
		return m_nDegreeU;
	}

	nfUint32 CModelNurbsSurface::getDegreeV()
	{
		return m_nDegreeV;
	}

	void CModelNurbsSurface::setDegree(_In_ nfUint32 nDegreeU, _In_ nfUint32 nDegreeV)
	{
		m_nDegreeU = nDegreeU;
		m_nDegreeV = nDegreeV;
	}

	void CModelNurbsSurface::setControlPointCount(_In_ nfUint32 ControlPointCountU, _In_ nfUint32 ControlPointCountV)
	{
		if (ControlPointCountU > MAXNURBSCONTROLPOINTS)
			throw CNMRException(NMR_ERROR_INVALIDCONTROLPOINTCOUNT);
		if (ControlPointCountV > MAXNURBSCONTROLPOINTS)
			throw CNMRException(NMR_ERROR_INVALIDCONTROLPOINTCOUNT);

		m_ControlPointCountU = ControlPointCountU;
		m_ControlPointCountV = ControlPointCountV;

		m_ControlPoints.resize(m_ControlPointCountU * m_ControlPointCountV);
	}


	// copy all parameters from source
	void CModelNurbsSurface::copyFrom(_In_ CModelNurbsSurface * pSourceNurbs)
	{
		if (pSourceNurbs == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		setDegree(pSourceNurbs->getDegreeU(), pSourceNurbs->getDegreeV());
		setControlPointCount(pSourceNurbs->getControlPointCountU(), pSourceNurbs->getControlPointCountV());

		clearKnotsU();
		clearKnotsV();

		nfUint32 nIndex;
		nfUint32 nKnotCountU = pSourceNurbs->getKnotCountU();
		for (nIndex = 0; nIndex < nKnotCountU; nIndex++) {
			nfUint32 nMultiplicity = 0;
			nfDouble dValue = 0.0;

			pSourceNurbs->getKnotU(nIndex, nMultiplicity, dValue);
			addKnotU(nMultiplicity, dValue);
		}

		nfUint32 nKnotCountV = pSourceNurbs->getKnotCountV();
		for (nIndex = 0; nIndex < nKnotCountV; nIndex++) {
			nfUint32 nMultiplicity = 0;
			nfDouble dValue = 0.0;

			pSourceNurbs->getKnotV(nIndex, nMultiplicity, dValue);
			addKnotV(nMultiplicity, dValue);
		}

		nfUint32 nIndexU, nIndexV;
		nfUint32 nControlPointCountU = getControlPointCountU ();
		nfUint32 nControlPointCountV = getControlPointCountV();

		for (nIndexU = 0; nIndexU < nControlPointCountU; nIndexU++) {
			for (nIndexV = 0; nIndexV < nControlPointCountV; nIndexV++) {
				nfDouble dX, dY, dZ, dW;
				pSourceNurbs->getControlPoint(nIndexU, nIndexV, dX, dY, dZ, dW);
				setControlPoint(nIndexU, nIndexV, dX, dY, dZ, dW);
			}
		}

	}

	nfBool CModelNurbsSurface::CheckValidity()
	{
		nfUint32 nKnotCountU = getKnotCountU ();
		nfUint32 nKnotMultiplicitySumU = 0;
		nfDouble nCurrentValueU = 0.0;
		for (nfUint32 nKnotIndexU = 0; nKnotIndexU < nKnotCountU; nKnotIndexU ++) {
			nfUint32 nMultiplicity;
			nfDouble dValue;

			getKnotU(nKnotCountU, nMultiplicity, dValue);

			nKnotMultiplicitySumU += nMultiplicity;

			// Check Value increment of Knots
			if (nKnotIndexU != 0) {
				if (dValue <= nCurrentValueU)
					return false;
			}

			nCurrentValueU = dValue;
		}

		if (nKnotMultiplicitySumU != (getControlPointCountU() + getDegreeU() + 1))
			return false;

		nfUint32 nKnotCountV = getKnotCountV();
		nfUint32 nKnotMultiplicitySumV = 0;
		nfDouble nCurrentValueV = 0.0;
		for (nfUint32 nKnotIndexV = 0; nKnotIndexV < nKnotCountV; nKnotIndexV++) {
			nfUint32 nMultiplicity;
			nfDouble dValue;

			getKnotV(nKnotCountV, nMultiplicity, dValue);

			nKnotMultiplicitySumV += nMultiplicity;

			// Check Value increment of Knots
			if (nKnotIndexV != 0) {
				if (dValue <= nCurrentValueV)
					return false;
			}

			nCurrentValueV = dValue;

		}

		if (nKnotMultiplicitySumV != (getControlPointCountV() + getDegreeV() + 1))
			return false;

		return true;
	}


	void CModelNurbsSurface::addKnotU(const nfUint32 nMultiplicity, const nfDouble dValue)
	{
		if (m_KnotsU.size() >= MAXNURBSKNOTS)
			throw CNMRException(NMR_ERROR_BUFFERISFULL);

		sModelNurbsSurfaceKnot Knot;
		Knot.m_Multiplicity = nMultiplicity;
		Knot.m_Value = dValue;

		m_KnotsU.push_back(Knot);
	}

	void CModelNurbsSurface::addKnotV(const nfUint32 nMultiplicity, const nfDouble dValue)
	{
		if (m_KnotsV.size() >= MAXNURBSKNOTS)
			throw CNMRException(NMR_ERROR_BUFFERISFULL);

		sModelNurbsSurfaceKnot Knot;
		Knot.m_Multiplicity = nMultiplicity;
		Knot.m_Value = dValue;

		m_KnotsV.push_back(Knot);
	}

	nfUint32 CModelNurbsSurface::getKnotCountU()
	{
		return (nfUint32)m_KnotsU.size();
	}

	nfUint32 CModelNurbsSurface::getKnotCountV()
	{
		return (nfUint32)m_KnotsV.size();
	}

	void CModelNurbsSurface::getKnotU(nfUint32 nIndex, nfUint32 & nMultiplicity, nfDouble & dValue)
	{
		if (nIndex >= m_KnotsU.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pKnot = &m_KnotsU[nIndex];
		nMultiplicity = pKnot->m_Multiplicity;
		dValue = pKnot->m_Value;
	}

	void CModelNurbsSurface::getKnotV(nfUint32 nIndex, nfUint32 & nMultiplicity, nfDouble & dValue)
	{
		if (nIndex >= m_KnotsV.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pKnot = &m_KnotsV[nIndex];
		nMultiplicity = pKnot->m_Multiplicity;
		dValue = pKnot->m_Value;
	}

	void CModelNurbsSurface::clearKnotsU()
	{
		m_KnotsU.clear ();
	}

	void CModelNurbsSurface::clearKnotsV()
	{
		m_KnotsV.clear();
	}

	void CModelNurbsSurface::addKnotsU(const std::vector<sModelNurbsSurfaceKnot> & Knots)
	{
		auto iIterator = Knots.begin();
		while (iIterator != Knots.end()) {
			m_KnotsU.push_back(*iIterator);

			iIterator++;
		}

	}

	void CModelNurbsSurface::addKnotsV(const std::vector<sModelNurbsSurfaceKnot> & Knots)
	{
		auto iIterator = Knots.begin();
		while (iIterator != Knots.end()) {
			m_KnotsV.push_back(*iIterator);

			iIterator++;
		}

	}

	void CModelNurbsSurface::setControlPoint(_In_ const nfUint32 nIndexU, _In_ const nfUint32 nIndexV, _In_ const nfDouble dX, _In_ const nfDouble dY, _In_ const nfDouble dZ, _In_ const nfDouble dW)
	{
		if (nIndexU >= m_ControlPointCountU)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		if (nIndexV >= m_ControlPointCountV)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pControlPoint = &m_ControlPoints[nIndexU + nIndexV * m_ControlPointCountU];
		pControlPoint->m_Position[0] = dX;
		pControlPoint->m_Position[1] = dY;
		pControlPoint->m_Position[2] = dZ;
		pControlPoint->m_Weight = dW;
	}

	nfUint32 CModelNurbsSurface::getControlPointCountU()
	{
		return m_ControlPointCountU;
	}

	nfUint32 CModelNurbsSurface::getControlPointCountV()
	{
		return m_ControlPointCountV;
	}

	void CModelNurbsSurface::getControlPoint(_In_ const nfUint32 nIndexU, _In_ const nfUint32 nIndexV, _Out_ nfDouble & dX, _Out_ nfDouble & dY, _Out_ nfDouble & dZ, _Out_ nfDouble & dW)
	{
		if (nIndexU >= m_ControlPointCountU)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		if (nIndexV >= m_ControlPointCountV)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pControlPoint = &m_ControlPoints[nIndexU + nIndexV * m_ControlPointCountU];
		dX = pControlPoint->m_Position[0];
		dY = pControlPoint->m_Position[1];
		dZ = pControlPoint->m_Position[2];
		dW = pControlPoint->m_Weight;

	}

	nfUint32 CModelNurbsSurface::addUVCoordinate(_In_ nfDouble fU, _In_ nfDouble fV)
	{
		nfUint32 nID = m_nNextUVID;

		sModelNurbsUVCoord UVCoord;
		UVCoord.m_U = fU;
		UVCoord.m_V = fV;

		m_UVCoords.insert(std::make_pair (nID, UVCoord));
		m_nNextUVID++;

		return nID;
	}

	nfBool CModelNurbsSurface::getUVCoordinate(_In_ nfUint32 nID, _Out_ nfDouble & fU, _Out_ nfDouble & fV)
	{
		auto iIterator = m_UVCoords.find(nID);
		if (iIterator != m_UVCoords.end()) {
			fU = iIterator->second.m_U;
			fV = iIterator->second.m_V;

			return true;
		}
		else
			return false;
	}

	void CModelNurbsSurface::setUVBounds(nfDouble dMinU, nfDouble dMinV, nfDouble dMaxU, nfDouble dMaxV)
	{
		m_dMinU = dMinU;
		m_dMinV = dMinV;
		m_dMaxU = dMaxU;
		m_dMaxV = dMaxV;
	}

	void CModelNurbsSurface::getUVBounds(nfDouble & dMinU, nfDouble & dMinV, nfDouble & dMaxU, nfDouble & dMaxV)
	{
		dMinU = m_dMinU;
		dMinV = m_dMinV;
		dMaxU = m_dMaxU;
		dMaxV = m_dMaxV;
	}

	nfUint32 CModelNurbsSurface::getUVCoordinateCount()
	{
		return (nfUint32)m_UVCoords.size();
	}

	void CModelNurbsSurface::walkUVCoordinates(NurbsSurfaceUVWalker Walker)
	{
		auto iIterator = m_UVCoords.begin();
		while (iIterator != m_UVCoords.end()) {
			Walker(iIterator->first, iIterator->second.m_U, iIterator->second.m_V);

			iIterator++;
		}
	}

}
