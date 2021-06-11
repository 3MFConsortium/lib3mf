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

Abstract: This is a stub class definition of CMeshObject

*/
#include "lib3mf_triangleset.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelTriangleSet.h"
#include "Model/Classes/NMR_ModelMeshObject.h"

#include <cmath>

using namespace Lib3MF::Impl;

CTriangleSet::CTriangleSet(NMR::PModelTriangleSet pTriangleSet, NMR::PModelMeshObject pMeshObject)
	: CBase (), m_pTriangleSet (pTriangleSet), m_pMeshObject (pMeshObject)
{
	if (pTriangleSet.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pMeshObject.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

}

CTriangleSet::~CTriangleSet()
{

}

void CTriangleSet::SetName(const std::string& sName)
{
	m_pTriangleSet->setName(sName);
}

std::string CTriangleSet::GetName()
{
	return m_pTriangleSet->getName();
}

void CTriangleSet::SetIdentifier(const std::string& sIdentifier)
{
	m_pTriangleSet->setIdentifier(sIdentifier);
}

std::string CTriangleSet::GetIdentifier()
{
	return m_pTriangleSet->getIdentifier();
}

void CTriangleSet::AddTriangle(const Lib3MF_uint32 nTriangleIndex)
{
	m_pTriangleSet->addTriangle(nTriangleIndex);
}

void CTriangleSet::RemoveTriangle(const Lib3MF_uint32 nTriangleIndex)
{
	m_pTriangleSet->removeTriangle(nTriangleIndex);
}

void CTriangleSet::Clear()
{
	m_pTriangleSet->clear();
}

void CTriangleSet::SetTriangleList(const Lib3MF_uint64 nTriangleIndicesBufferSize, const Lib3MF_uint32* pTriangleIndicesBuffer)
{
	if (pTriangleIndicesBuffer) {
		m_pTriangleSet->clear();
		const uint32_t* pPtr = pTriangleIndicesBuffer;
		for (uint64_t nIndex = 0; nIndex < nTriangleIndicesBufferSize; nIndex++) {
			m_pTriangleSet->addTriangle(*pPtr);
			pPtr++;
		}
	}
}

void CTriangleSet::AddTriangleList(const Lib3MF_uint64 nTriangleIndicesBufferSize, const Lib3MF_uint32* pTriangleIndicesBuffer)
{
	if (pTriangleIndicesBuffer) {
		const uint32_t* pPtr = pTriangleIndicesBuffer;
		for (uint64_t nIndex = 0; nIndex < nTriangleIndicesBufferSize; nIndex++) {
			m_pTriangleSet->addTriangle(*pPtr);
			pPtr++;
		}
	}

}

void CTriangleSet::Merge(ITriangleSet* pOtherTriangleSet, const bool bDeleteOther)
{
	if (pOtherTriangleSet == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto pOtherTriangleSetInstance = dynamic_cast <CTriangleSet *> (pOtherTriangleSet);
	if (pOtherTriangleSetInstance == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	m_pTriangleSet->merge (pOtherTriangleSetInstance->triangleSet());

	if (bDeleteOther)
		pOtherTriangleSet->DeleteSet();
}

void CTriangleSet::DeleteSet()
{
	m_pMeshObject->deleteTriangleSet (m_pTriangleSet.get ());
}

ITriangleSet* CTriangleSet::Duplicate(const std::string& sIdentifier)
{
	return new CTriangleSet(m_pTriangleSet->duplicate (sIdentifier), m_pMeshObject);
}

NMR::PModelTriangleSet CTriangleSet::triangleSet()
{
	return m_pTriangleSet;
}