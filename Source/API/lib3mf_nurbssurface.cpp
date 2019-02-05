/*++

Copyright (C) 2018 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CLib3MFNurbsSurface

*/

#include "lib3mf_nurbssurface.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "Model/Classes/NMR_ModelNurbsSurface.h"
#include "Model/Classes/NMR_ModelResource.h" 
#include "Model/Classes/NMR_Model.h"  

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CLib3MFNurbsSurface 
**************************************************************************************************************************/

CLib3MFNurbsSurface::CLib3MFNurbsSurface(NMR::PModelNurbsSurface pNurbsSurface)
	: CLib3MFResource(pNurbsSurface)
{
	m_pNurbsSurface = pNurbsSurface.get();
	if (m_pNurbsSurface == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}

CLib3MFNurbsSurface::~CLib3MFNurbsSurface()
{

}


Lib3MF_double CLib3MFNurbsSurface::GetDegreeU ()
{
	return m_pNurbsSurface->getDegreeU();
}

Lib3MF_double CLib3MFNurbsSurface::GetDegreeV ()
{
	return m_pNurbsSurface->getDegreeV();
}

bool CLib3MFNurbsSurface::CheckValidity ()
{
	return m_pNurbsSurface->CheckValidity();
}

void CLib3MFNurbsSurface::GetKnotsU (Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer)
{
	Lib3MF_uint64 nKnotCount = m_pNurbsSurface->getKnotCountU();

	if (pKnotsNeededCount != nullptr) {
		*pKnotsNeededCount = nKnotCount;
	}

	if (pKnotsBuffer != nullptr) {
		if (nKnotsBufferSize < nKnotCount)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		sLib3MFNURBSKnot * pKnot = pKnotsBuffer;
		Lib3MF_uint64 nIndex;

		for (nIndex = 0; nIndex < nKnotCount; nIndex++) {
			double dValue = 0.0;
			NMR::nfUint32 nMultiplicity = 0;
			m_pNurbsSurface->getKnotU((NMR::nfUint32) nIndex, nMultiplicity, dValue);

			pKnot->m_Multiplicity = nMultiplicity;
			pKnot->m_Value = dValue;

			pKnot++;
		}
	}
}

void CLib3MFNurbsSurface::SetKnotsU (const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer)
{
	if (pKnotsBuffer == nullptr) 
		throw ELib3MFInterfaceException (LIB3MF_ERROR_INVALIDPARAM);

	const sLib3MFNURBSKnot * pKnot = pKnotsBuffer;

	m_pNurbsSurface->clearKnotsU ();
	for (Lib3MF_uint64 index = 0; index < nKnotsBufferSize; index++) {
		m_pNurbsSurface->addKnotU(pKnot->m_Multiplicity, pKnot->m_Value);
		pKnot++;
	}
}

void CLib3MFNurbsSurface::AddKnotU (const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue)
{
	m_pNurbsSurface->addKnotU(nMultiplicity, dValue);
}

void CLib3MFNurbsSurface::GetKnotsV(Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer)
{
	Lib3MF_uint64 nKnotCount = m_pNurbsSurface->getKnotCountV();

	if (pKnotsNeededCount != nullptr) {
		*pKnotsNeededCount = nKnotCount;
	}

	if (pKnotsBuffer != nullptr) {
		if (nKnotsBufferSize < nKnotCount)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		sLib3MFNURBSKnot * pKnot = pKnotsBuffer;
		Lib3MF_uint64 nIndex;

		for (nIndex = 0; nIndex < nKnotCount; nIndex++) {
			double dValue = 0.0;
			NMR::nfUint32 nMultiplicity = 0;
			m_pNurbsSurface->getKnotV((NMR::nfUint32) nIndex, nMultiplicity, dValue);

			pKnot->m_Multiplicity = nMultiplicity;
			pKnot->m_Value = dValue;

			pKnot++;
		}
	}
}

void CLib3MFNurbsSurface::SetKnotsV(const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer)
{
	if (pKnotsBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	const sLib3MFNURBSKnot * pKnot = pKnotsBuffer;

	m_pNurbsSurface->clearKnotsV();
	for (Lib3MF_uint64 index = 0; index < nKnotsBufferSize; index++) {
		m_pNurbsSurface->addKnotV(pKnot->m_Multiplicity, pKnot->m_Value);
		pKnot++;
	}
}

void CLib3MFNurbsSurface::AddKnotV(const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue)
{
	m_pNurbsSurface->addKnotV(nMultiplicity, dValue);
}

sLib3MFNURBSUVCoordinate CLib3MFNurbsSurface::GetUVCoordinate(const Lib3MF_uint32 nId)
{
	sLib3MFNURBSUVCoordinate sUVCoordinate;

	double dU, dV;
	if (m_pNurbsSurface->getUVCoordinate(nId, dU, dV)) {
		sUVCoordinate.m_Id = nId;
		sUVCoordinate.m_U = dU;
		sUVCoordinate.m_V = dV;
	}
	else {
		sUVCoordinate.m_Id = 0;
		sUVCoordinate.m_U = 0.0;
		sUVCoordinate.m_V = 0.0;
	}

	return sUVCoordinate;
}

void CLib3MFNurbsSurface::GetUVCoordinates(Lib3MF_uint64 nUVCoordinatesBufferSize, Lib3MF_uint64* pUVCoordinatesNeededCount, sLib3MFNURBSUVCoordinate * pUVCoordinatesBuffer)
{
	uint32_t nCoordinateCount = m_pNurbsSurface->getUVCoordinateCount();
	if (pUVCoordinatesNeededCount != nullptr) {
		*pUVCoordinatesNeededCount = nCoordinateCount;
	}

	if (pUVCoordinatesBuffer != nullptr) {
		if (nCoordinateCount > nUVCoordinatesBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		sLib3MFNURBSUVCoordinate * pUVCoordinate = pUVCoordinatesBuffer;
		auto Walker = [&pUVCoordinate](uint32_t nID, double dU, double dV) {
			pUVCoordinate->m_Id = nID;
			pUVCoordinate->m_U = dU;
			pUVCoordinate->m_V = dV;
			pUVCoordinate++;
		};

		m_pNurbsSurface->walkUVCoordinates(Walker);
	}
	
}

Lib3MF_uint32 CLib3MFNurbsSurface::AddUVCoordinate(const Lib3MF_double dU, const Lib3MF_double dV)
{
	return m_pNurbsSurface->addUVCoordinate(dU, dV);
}

void CLib3MFNurbsSurface::RemoveUVCoordinate(const Lib3MF_uint32 nId)
{
	m_pNurbsSurface->removeUVCoordinate(nId);
}

bool CLib3MFNurbsSurface::HasUVCoordinate(const Lib3MF_uint32 nId)
{
	double dU, dV;
	return m_pNurbsSurface->getUVCoordinate (nId, dU, dV);
}


void CLib3MFNurbsSurface::SetControlPoint(const Lib3MF_uint32 nIndexU, const Lib3MF_uint32 nIndexV, const Lib3MF_double dX, const Lib3MF_double dY, const Lib3MF_double dZ, const Lib3MF_double dW)
{
	m_pNurbsSurface->setControlPoint(nIndexU, nIndexV, dX, dY, dZ, dW);
}

void CLib3MFNurbsSurface::GetControlPoint(const Lib3MF_uint32 nIndexU, const Lib3MF_uint32 nIndexV, Lib3MF_double & dX, Lib3MF_double & dY, Lib3MF_double & dZ, Lib3MF_double & dW)
{
	m_pNurbsSurface->getControlPoint(nIndexU, nIndexV, dX, dY, dZ, dW);
}


sLib3MFNURBSEdgeMapping CLib3MFNurbsSurface::GetEdgeMapping(const Lib3MF_uint32 nId)
{
	sLib3MFNURBSEdgeMapping sEdgeMapping;

	double dT1, dT2;
	NMR::ModelResourceID nCurveID;
	if (m_pNurbsSurface->getEdgeMapping(nId, nCurveID, dT1, dT2)) {
		sEdgeMapping.m_Id = nId;
		sEdgeMapping.m_CurveId = nCurveID;
		sEdgeMapping.m_T1 = dT1;
		sEdgeMapping.m_T2 = dT2;
	}
	else {
		sEdgeMapping.m_Id = 0;
		sEdgeMapping.m_CurveId = 0;
		sEdgeMapping.m_T1 = 0;
		sEdgeMapping.m_T2 = 0;
	}

	return sEdgeMapping;
}

void CLib3MFNurbsSurface::GetEdgeMappings(Lib3MF_uint64 nEdgeMappingsBufferSize, Lib3MF_uint64* pEdgeMappingsNeededCount, sLib3MFNURBSEdgeMapping * pEdgeMappingsBuffer)
{
	uint32_t nEdgeMappingCount = m_pNurbsSurface->getEdgeMappingCount();
	if (pEdgeMappingsNeededCount != nullptr) {
		*pEdgeMappingsNeededCount = nEdgeMappingCount;
	}

	if (pEdgeMappingsBuffer != nullptr) {
		if (nEdgeMappingCount > nEdgeMappingsBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

		sLib3MFNURBSEdgeMapping * pEdgeMapping = pEdgeMappingsBuffer;
		auto Walker = [&pEdgeMapping](uint32_t nID, uint32_t nCurveID, double dT1, double dT2) {
			pEdgeMapping->m_Id = nID;
			pEdgeMapping->m_CurveId = nCurveID;
			pEdgeMapping->m_T1 = dT1;
			pEdgeMapping->m_T2 = dT2;
			pEdgeMapping++;
		};

		m_pNurbsSurface->walkEdgeMappings(Walker);
	}


}

Lib3MF_uint32 CLib3MFNurbsSurface::AddEdgeMapping(const Lib3MF_uint32 nCurveID, const Lib3MF_double dT1, const Lib3MF_double dT2)
{
	return m_pNurbsSurface->addEdgeMapping(nCurveID, dT1, dT2);
}

void CLib3MFNurbsSurface::RemoveEdgeMapping(const Lib3MF_uint32 nId)
{
	m_pNurbsSurface->removeEdgeMapping(nId);
}

bool CLib3MFNurbsSurface::HasEdgeMapping(const Lib3MF_uint32 nId)
{
	double dT1, dT2;
	NMR::ModelResourceID nCurveID;

	return m_pNurbsSurface->getEdgeMapping(nId, nCurveID, dT1, dT2);
}

void CLib3MFNurbsSurface::AddEdgeMappingUVCoordinate(const Lib3MF_uint32 nId, const Lib3MF_double dU, const Lib3MF_double dV, const Lib3MF_double dT)
{
	m_pNurbsSurface->addEdgeMappingUVCoordinate(nId, dU, dV, dT);
}

void CLib3MFNurbsSurface::ClearEdgeMappingUVCoordinates(const Lib3MF_uint32 nId)
{
	m_pNurbsSurface->clearEdgeMappingUVCoordinate(nId);
}


void CLib3MFNurbsSurface::GetEdgeMappingUVCoordinate(const Lib3MF_uint32 nId, const Lib3MF_uint32 nIndex, Lib3MF_double & dU, Lib3MF_double & dV, Lib3MF_double & dT)
{
	m_pNurbsSurface->getEdgeMappingUVCoordinate(nId, nIndex, dU, dV, dT);
}

Lib3MF_uint32 CLib3MFNurbsSurface::GetEdgeMappingUVCoordinateCount(const Lib3MF_uint32 nId)
{
	return m_pNurbsSurface->getEdgeMappingUVCoordinateCount(nId);
}

