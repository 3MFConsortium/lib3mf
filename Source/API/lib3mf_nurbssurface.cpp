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

void CLib3MFNurbsSurface::GetUVCoordinate(const Lib3MF_uint32 nId, sLib3MFNURBSUVCoordinate & sUVCoordinate)
{
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

	}
	
}

void CLib3MFNurbsSurface::AddUVCoordinate(const sLib3MFNURBSUVCoordinate UVCoordinate, Lib3MF_uint32 & nId)
{
	nId = m_pNurbsSurface->addUVCoordinate(UVCoordinate.m_U, UVCoordinate.m_V);
}

