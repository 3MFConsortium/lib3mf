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

Abstract: This is a stub class definition of CNurbsCurve

*/

#include "lib3mf_nurbscurve.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CNurbsCurve 
**************************************************************************************************************************/

CNurbsCurve::CNurbsCurve(NMR::PModelNurbsCurve pNurbsCurve)
	: CResource(pNurbsCurve)
{
	m_pNurbsCurve = pNurbsCurve;
	if (m_pNurbsCurve == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}

CNurbsCurve::~CNurbsCurve()
{

}


Lib3MF_uint32 CNurbsCurve::GetDegree ()
{
	return m_pNurbsCurve->getDegree();
}

bool CNurbsCurve::CheckValidity ()
{
	return m_pNurbsCurve->CheckValidity();
}

void CNurbsCurve::GetKnots (Lib3MF_uint64 nKnotsBufferSize, Lib3MF_uint64* pKnotsNeededCount, sLib3MFNURBSKnot * pKnotsBuffer)
{
	Lib3MF_uint64 nKnotCount = m_pNurbsCurve->getKnotCount();

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
			m_pNurbsCurve->getKnot((NMR::nfUint32) nIndex, nMultiplicity, dValue);

			pKnot->m_Multiplicity = nMultiplicity;
			pKnot->m_Value = dValue;

			pKnot++;
		}
	}
}

void CNurbsCurve::SetKnots (const Lib3MF_uint64 nKnotsBufferSize, const sLib3MFNURBSKnot * pKnotsBuffer)
{
	if (pKnotsBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	const sLib3MFNURBSKnot * pKnot = pKnotsBuffer;

	m_pNurbsCurve->clearKnots();
	for (Lib3MF_uint64 index = 0; index < nKnotsBufferSize; index++) {
		m_pNurbsCurve->addKnot(pKnot->m_Multiplicity, pKnot->m_Value);
		pKnot++;
	}
}

void CNurbsCurve::AddKnot (const Lib3MF_uint32 nMultiplicity, const Lib3MF_double dValue)
{
	m_pNurbsCurve->addKnot(nMultiplicity, dValue);
}

void CNurbsCurve::SetControlPoint (const Lib3MF_uint32 nIndex, const Lib3MF_double dX, const Lib3MF_double dY, const Lib3MF_double dZ, const Lib3MF_double dW)
{
	m_pNurbsCurve->setControlPoint(nIndex, dX, dY, dZ, dW);
}

void CNurbsCurve::GetControlPoint (const Lib3MF_uint32 nIndex, Lib3MF_double & dX, Lib3MF_double & dY, Lib3MF_double & dZ, Lib3MF_double & dW)
{
	m_pNurbsCurve->getControlPoint(nIndex, dX, dY, dZ, dW);
}

