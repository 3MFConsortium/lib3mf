/*++

Copyright (C) 2026 3MF Consortium (Original Author)

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

Abstract: This is the class definition of CToolpathLaserSource and CToolpathSyncGroup

*/

#include "lib3mf_toolpathlasersource.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLaserSource
**************************************************************************************************************************/

CToolpathLaserSource::CToolpathLaserSource(NMR::PModelToolpathLaserSource pLaserSource)
	: m_pLaserSource(pLaserSource)
{
	if (pLaserSource.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

Lib3MF_uint32 CToolpathLaserSource::GetLaserIndex()
{
	return m_pLaserSource->getLaserIndex();
}

std::string CToolpathLaserSource::GetName()
{
	return m_pLaserSource->getName();
}

void CToolpathLaserSource::SetName(const std::string & sName)
{
	m_pLaserSource->setName(sName);
}

bool CToolpathLaserSource::HasBounds()
{
	return m_pLaserSource->hasBounds();
}

void CToolpathLaserSource::GetBounds(Lib3MF_int32 & nMinX, Lib3MF_int32 & nMaxX, Lib3MF_int32 & nMinY, Lib3MF_int32 & nMaxY)
{
	m_pLaserSource->getBounds(nMinX, nMaxX, nMinY, nMaxY);
}

void CToolpathLaserSource::SetBounds(const Lib3MF_int32 nMinX, const Lib3MF_int32 nMaxX, const Lib3MF_int32 nMinY, const Lib3MF_int32 nMaxY)
{
	m_pLaserSource->setBounds(nMinX, nMaxX, nMinY, nMaxY);
}

void CToolpathLaserSource::ClearBounds()
{
	m_pLaserSource->clearBounds();
}

bool CToolpathLaserSource::HasCenter()
{
	return m_pLaserSource->hasCenter();
}

void CToolpathLaserSource::GetCenter(Lib3MF_int32 & nCenterX, Lib3MF_int32 & nCenterY)
{
	m_pLaserSource->getCenter(nCenterX, nCenterY);
}

void CToolpathLaserSource::SetCenter(const Lib3MF_int32 nCenterX, const Lib3MF_int32 nCenterY)
{
	m_pLaserSource->setCenter(nCenterX, nCenterY);
}

void CToolpathLaserSource::ClearCenter()
{
	m_pLaserSource->clearCenter();
}

bool CToolpathLaserSource::HasCornerRadius()
{
	return m_pLaserSource->hasCornerRadius();
}

Lib3MF_uint32 CToolpathLaserSource::GetCornerRadius()
{
	return m_pLaserSource->getCornerRadius();
}

void CToolpathLaserSource::SetCornerRadius(const Lib3MF_uint32 nCornerRadius)
{
	m_pLaserSource->setCornerRadius(nCornerRadius);
}

void CToolpathLaserSource::ClearCornerRadius()
{
	m_pLaserSource->clearCornerRadius();
}


/*************************************************************************************************************************
 Class definition of CToolpathSyncGroup
**************************************************************************************************************************/

CToolpathSyncGroup::CToolpathSyncGroup(NMR::PModelToolpath pToolpath, NMR::PModelToolpathSyncGroup pSyncGroup)
	: m_pToolpath(pToolpath), m_pSyncGroup(pSyncGroup)
{
	if ((pToolpath.get() == nullptr) || (pSyncGroup.get() == nullptr))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

Lib3MF_uint32 CToolpathSyncGroup::GetSyncGroupID()
{
	return m_pSyncGroup->getSyncGroupID();
}

void CToolpathSyncGroup::GetLaserIndices(Lib3MF_uint64 nLaserIndicesBufferSize, Lib3MF_uint64 * pLaserIndicesNeededCount, Lib3MF_uint32 * pLaserIndicesBuffer)
{
	const std::vector<NMR::nfUint32> & LaserIndices = m_pSyncGroup->getLaserIndices();
	Lib3MF_uint64 nCount = LaserIndices.size();
	if (pLaserIndicesNeededCount)
		*pLaserIndicesNeededCount = nCount;

	if ((nLaserIndicesBufferSize >= nCount) && (pLaserIndicesBuffer != nullptr)) {
		for (Lib3MF_uint64 nIndex = 0; nIndex < nCount; nIndex++)
			pLaserIndicesBuffer[nIndex] = LaserIndices[(size_t)nIndex];
	}
}

void CToolpathSyncGroup::SetLaserIndices(const Lib3MF_uint64 nLaserIndicesBufferSize, const Lib3MF_uint32 * pLaserIndicesBuffer)
{
	if ((nLaserIndicesBufferSize > 0) && (pLaserIndicesBuffer == nullptr))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	NMR::nfUint32 nSyncGroupID = m_pSyncGroup->getSyncGroupID();
	if (m_pToolpath->findSyncGroup(nSyncGroupID) != m_pSyncGroup)
		throw NMR::CNMRException(NMR_ERROR_TOOLPATHSYNCGROUPNOTFOUND, "sync group " + NMR::fnUint32ToString(nSyncGroupID) + " has been removed from the toolpath");

	std::vector<NMR::nfUint32> LaserIndices(pLaserIndicesBuffer, pLaserIndicesBuffer + nLaserIndicesBufferSize);
	m_pToolpath->setSyncGroupLaserIndices(nSyncGroupID, LaserIndices);
}
