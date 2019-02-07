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
#include "Model/Classes/NMR_ModelNurbsCurve.h"
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {



	CModelNurbsCurve::CModelNurbsCurve(_In_ const ModelResourceID sID, _In_ CModel * pModel, _In_ nfUint32 nDegree, _In_ nfUint32 nControlPointCount)
		: CModelResource(sID, pModel)
	{
		setDegree(nDegree);
		setControlPointCount(nControlPointCount);
	}


	nfUint32 CModelNurbsCurve::getDegree()
	{
		return m_nDegree;
	}

	void CModelNurbsCurve::setDegree(_In_ nfUint32 nDegree)
	{
		m_nDegree = nDegree;
	}

	void CModelNurbsCurve::setControlPointCount(_In_ nfUint32 ControlPointCount)
	{
		if (ControlPointCount > MAXNURBSCONTROLPOINTS)
			throw CNMRException(NMR_ERROR_INVALIDCONTROLPOINTCOUNT);
		
		m_ControlPoints.resize(ControlPointCount);
	}


	/*// copy all parameters from source
	void CModelNurbsCurve::copyFrom(_In_ CModelNurbsCurve * pSourceCurve)
	{
		if (pSourceCurve == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		setDegree(pSourceCurve->getDegree());
		setControlPointCount(pSourceCurve->getControlPointCount());

		clearKnots();

		nfUint32 nIndex;
		nfUint32 nKnotCount = pSourceCurve->getKnotCount();
		for (nIndex = 0; nIndex < nKnotCount; nIndex++) {
			nfUint32 nMultiplicity = 0;
			nfDouble dValue = 0.0;

			pSourceCurve->getKnot(nIndex, nMultiplicity, dValue);
			addKnot(nMultiplicity, dValue);
		}


		nfUint32 nControlPointCount = getControlPointCount ();

		for (nIndex = 0; nIndex < nControlPointCount; nIndex++) {
			nfDouble dX, dY, dZ, dW;
			pSourceCurve->getControlPoint(nIndex, dX, dY, dZ, dW);
			setControlPoint(nIndex, dX, dY, dZ, dW);			
		}

	} */

	nfBool CModelNurbsCurve::CheckValidity()
	{
		nfUint32 nKnotCount = getKnotCount ();
		nfUint32 nKnotMultiplicitySum = 0;
		nfDouble nCurrentValue = 0.0;
		for (nfUint32 nKnotIndex = 0; nKnotIndex < nKnotCount; nKnotIndex ++) {
			nfUint32 nMultiplicity;
			nfDouble dValue;

			getKnot(nKnotCount, nMultiplicity, dValue);

			nKnotMultiplicitySum += nMultiplicity;

			// Check Value increment of Knots
			if (nKnotIndex != 0) {
				if (dValue <= nCurrentValue)
					return false;
			}

			nCurrentValue = dValue;
		}

		if (nKnotMultiplicitySum != (getControlPointCount() + getDegree() + 1))
			return false;

		return true;
	}


	void CModelNurbsCurve::addKnot(const nfUint32 nMultiplicity, const nfDouble dValue)
	{
		if (m_Knots.size() >= MAXNURBSKNOTS)
			throw CNMRException(NMR_ERROR_BUFFERISFULL);

		sModelNurbsSurfaceKnot Knot;
		Knot.m_Multiplicity = nMultiplicity;
		Knot.m_Value = dValue;

		m_Knots.push_back(Knot);
	}

	nfUint32 CModelNurbsCurve::getKnotCount()
	{
		return (nfUint32)m_Knots.size();
	}

	void CModelNurbsCurve::getKnot(nfUint32 nIndex, nfUint32 & nMultiplicity, nfDouble & dValue)
	{
		if (nIndex >= m_Knots.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pKnot = &m_Knots[nIndex];
		nMultiplicity = pKnot->m_Multiplicity;
		dValue = pKnot->m_Value;
	}


	void CModelNurbsCurve::clearKnots()
	{
		m_Knots.clear ();
	}

	
	void CModelNurbsCurve::addKnots(const std::vector<sModelNurbsSurfaceKnot> & Knots)
	{
		auto iIterator = Knots.begin();
		while (iIterator != Knots.end()) {
			m_Knots.push_back(*iIterator);

			iIterator++;
		}

	}


	void CModelNurbsCurve::setControlPoint(_In_ const nfUint32 nIndex, _In_ const nfDouble dX, _In_ const nfDouble dY, _In_ const nfDouble dZ, _In_ const nfDouble dW)
	{
		if (nIndex >= m_ControlPoints.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pControlPoint = &m_ControlPoints[nIndex];
		pControlPoint->m_Position[0] = dX;
		pControlPoint->m_Position[1] = dY;
		pControlPoint->m_Position[2] = dZ;
		pControlPoint->m_Weight = dW;
	}

	nfUint32 CModelNurbsCurve::getControlPointCount()
	{
		return (nfUint32)m_ControlPoints.size();
	}



	void CModelNurbsCurve::getControlPoint(_In_ const nfUint32 nIndex, _Out_ nfDouble & dX, _Out_ nfDouble & dY, _Out_ nfDouble & dZ, _Out_ nfDouble & dW)
	{
		if (nIndex >= m_ControlPoints.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto pControlPoint = &m_ControlPoints[nIndex];
		dX = pControlPoint->m_Position[0];
		dY = pControlPoint->m_Position[1];
		dZ = pControlPoint->m_Position[2];
		dW = pControlPoint->m_Weight;

	}


	void CModelNurbsCurve::setTBounds(nfDouble dMinT, nfDouble dMaxT)
	{
		m_dMinT = dMinT;
		m_dMaxT = dMaxT;
	}

	void CModelNurbsCurve::getTBounds(nfDouble & dMinT, nfDouble & dMaxT)
	{
		dMinT = m_dMinT;
		dMaxT = m_dMaxT;
	}


}
