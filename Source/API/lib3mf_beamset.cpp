/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CBeamSet

*/

#include "lib3mf_beamset.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CBeamSet 
**************************************************************************************************************************/

CBeamSet::CBeamSet(NMR::PBEAMSET pBeamSet, NMR::PModelMeshObject pMeshObject):
	m_pBeamSet(pBeamSet), m_mesh(*pMeshObject->getMesh())
{

}

void CBeamSet::SetName(const std::string & sName)
{
	m_pBeamSet->m_sName = sName;
}

std::string CBeamSet::GetName()
{
	return m_pBeamSet->m_sName;
}

void CBeamSet::SetIdentifier(const std::string & sIdentifier)
{
	// Ensure identifier is unique
	if (sIdentifier.compare("") != 0) {
		Lib3MF_uint32 nBeamSets = m_mesh.getBeamSetCount();
		for (Lib3MF_uint32 iBeamSet = 0; iBeamSet < nBeamSets; iBeamSet++)
			if (sIdentifier.compare(m_mesh.getBeamSet(iBeamSet)->m_sIdentifier) == 0)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	}

	m_pBeamSet->m_sIdentifier = sIdentifier;
}

std::string CBeamSet::GetIdentifier()
{
	return m_pBeamSet->m_sIdentifier;
}

Lib3MF_uint32 CBeamSet::GetReferenceCount()
{
	return (Lib3MF_uint32)m_pBeamSet->m_Refs.size();
}

void CBeamSet::SetReferences(const Lib3MF_uint64 nReferencesBufferSize, const Lib3MF_uint32 * pReferencesBuffer)
{
	if (nReferencesBufferSize > LIB3MF_MAXBEAMCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	m_pBeamSet->m_Refs.resize((size_t) nReferencesBufferSize);
	const Lib3MF_uint32 beamCount = m_mesh.getBeamCount();
	for (size_t i = 0; i < (size_t)nReferencesBufferSize; i++) {
		if (beamCount <= pReferencesBuffer[i])
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		m_pBeamSet->m_Refs[i] = Lib3MF_uint32(pReferencesBuffer[i]);
	}

}

void CBeamSet::GetReferences(Lib3MF_uint64 nReferencesBufferSize, Lib3MF_uint64* pReferencesNeededCount, Lib3MF_uint32 * pReferencesBuffer)
{
	Lib3MF_uint32 referenceCount = (Lib3MF_uint32)m_pBeamSet->m_Refs.size();
	if (pReferencesNeededCount)
		*pReferencesNeededCount = referenceCount;

	if (nReferencesBufferSize >= referenceCount && pReferencesBuffer)
	{
		Lib3MF_uint32* pRef = pReferencesBuffer;
		for (Lib3MF_uint32 i = 0; i < referenceCount; i++)
		{
			*pRef = m_pBeamSet->m_Refs[i];
			pRef++;
		}
	}
}

Lib3MF_uint32 CBeamSet::GetBallReferenceCount()
{
	return (Lib3MF_uint32)m_pBeamSet->m_BallRefs.size();
}

void CBeamSet::SetBallReferences(const Lib3MF_uint64 nBallReferencesBufferSize, const Lib3MF_uint32* pBallReferencesBuffer)
{
	if (nBallReferencesBufferSize > LIB3MF_MAXBEAMCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_ELEMENTCOUNTEXCEEDSLIMIT);

	m_pBeamSet->m_BallRefs.resize((size_t)nBallReferencesBufferSize);
	const Lib3MF_uint32 ballCount = m_mesh.getBallCount();
	for (size_t i = 0; i < (size_t)nBallReferencesBufferSize; i++) {
		if (ballCount <= pBallReferencesBuffer[i])
			throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		m_pBeamSet->m_BallRefs[i] = Lib3MF_uint32(pBallReferencesBuffer[i]);
	}
}

void CBeamSet::GetBallReferences(Lib3MF_uint64 nBallReferencesBufferSize, Lib3MF_uint64* pBallReferencesNeededCount, Lib3MF_uint32* pBallReferencesBuffer)
{
	Lib3MF_uint32 ballReferenceCount = GetBallReferenceCount();
	if (pBallReferencesNeededCount)
		*pBallReferencesNeededCount = ballReferenceCount;

	if (nBallReferencesBufferSize >= ballReferenceCount && pBallReferencesBuffer) {
		Lib3MF_uint32* pBallRef = pBallReferencesBuffer;
		for (Lib3MF_uint32 i = 0; i < ballReferenceCount; i++) {
			*pBallRef = m_pBeamSet->m_BallRefs[i];
			pBallRef++;
		}
	}
}
