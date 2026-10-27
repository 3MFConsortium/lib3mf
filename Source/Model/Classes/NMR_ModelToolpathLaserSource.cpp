/*++

Copyright (C) 2026 3MF Consortium

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

NMR_ModelToolpathLaserSource.cpp implements the laser sources and sync groups of a toolpath (Toolpath Extension, 4.5).

--*/

#include "Model/Classes/NMR_ModelToolpathLaserSource.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"

#include <algorithm>

namespace NMR {

	CModelToolpathLaserSource::CModelToolpathLaserSource(nfUint32 nLaserIndex, const std::string & sName)
		: m_nLaserIndex(nLaserIndex), m_sName(sName),
		m_bHasBounds(false), m_nMinX(0), m_nMaxX(0), m_nMinY(0), m_nMaxY(0),
		m_bHasCenter(false), m_nCenterX(0), m_nCenterY(0),
		m_nCornerRadius(0)
	{
		if (nLaserIndex > (nfUint32)XML_3MF_MAXTOOLPATHLASERINDEX)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, "laser index out of range");
	}

	nfUint32 CModelToolpathLaserSource::getLaserIndex()
	{
		return m_nLaserIndex;
	}

	std::string CModelToolpathLaserSource::getName()
	{
		return m_sName;
	}

	void CModelToolpathLaserSource::setName(const std::string & sName)
	{
		m_sName = sName;
	}

	nfBool CModelToolpathLaserSource::hasBounds()
	{
		return m_bHasBounds;
	}

	void CModelToolpathLaserSource::getBounds(nfInt32 & nMinX, nfInt32 & nMaxX, nfInt32 & nMinY, nfInt32 & nMaxY)
	{
		if (!m_bHasBounds)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCEBOUNDS, "laser source has no bounds");

		nMinX = m_nMinX;
		nMaxX = m_nMaxX;
		nMinY = m_nMinY;
		nMaxY = m_nMaxY;
	}

	void CModelToolpathLaserSource::setBounds(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY)
	{
		if (!boundsAreValid(nMinX, nMaxX, nMinY, nMaxY))
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCEBOUNDS, "maxx must be >= minx and maxy must be >= miny");

		if ((m_nCornerRadius != 0) && !cornerRadiusIsValid(nMinX, nMaxX, nMinY, nMaxY, m_nCornerRadius))
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, "corner radius exceeds half of the shorter side of the new bounds");

		m_nMinX = nMinX;
		m_nMaxX = nMaxX;
		m_nMinY = nMinY;
		m_nMaxY = nMaxY;
		m_bHasBounds = true;
	}

	void CModelToolpathLaserSource::clearBounds()
	{
		m_bHasBounds = false;
		m_nMinX = 0;
		m_nMaxX = 0;
		m_nMinY = 0;
		m_nMaxY = 0;
		m_nCornerRadius = 0;
	}

	nfBool CModelToolpathLaserSource::hasCenter()
	{
		return m_bHasCenter;
	}

	void CModelToolpathLaserSource::getCenter(nfInt32 & nCenterX, nfInt32 & nCenterY)
	{
		if (!m_bHasCenter)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECENTER, "laser source has no center");

		nCenterX = m_nCenterX;
		nCenterY = m_nCenterY;
	}

	void CModelToolpathLaserSource::setCenter(nfInt32 nCenterX, nfInt32 nCenterY)
	{
		m_nCenterX = nCenterX;
		m_nCenterY = nCenterY;
		m_bHasCenter = true;
	}

	void CModelToolpathLaserSource::clearCenter()
	{
		m_bHasCenter = false;
		m_nCenterX = 0;
		m_nCenterY = 0;
	}

	nfBool CModelToolpathLaserSource::hasCornerRadius()
	{
		return m_nCornerRadius != 0;
	}

	nfUint32 CModelToolpathLaserSource::getCornerRadius()
	{
		if (m_nCornerRadius == 0)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, "laser source has no corner radius");

		return m_nCornerRadius;
	}

	void CModelToolpathLaserSource::setCornerRadius(nfUint32 nCornerRadius)
	{
		if (!m_bHasBounds)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, "corner radius requires the scan-field bounds");
		if (!cornerRadiusIsValid(m_nMinX, m_nMaxX, m_nMinY, m_nMaxY, nCornerRadius))
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCECORNERRADIUS, "corner radius must be > 0 and <= half of the shorter field side");

		m_nCornerRadius = nCornerRadius;
	}

	void CModelToolpathLaserSource::clearCornerRadius()
	{
		m_nCornerRadius = 0;
	}

	nfBool CModelToolpathLaserSource::boundsAreValid(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY)
	{
		return (nMaxX >= nMinX) && (nMaxY >= nMinY);
	}

	nfBool CModelToolpathLaserSource::cornerRadiusIsValid(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY, nfInt64 nCornerRadius)
	{
		if ((nCornerRadius <= 0) || (nCornerRadius > (nfInt64)XML_3MF_MAXTOOLPATHLASERINDEX))
			return false;

		nfInt64 nShorterSide = std::min((nfInt64)nMaxX - (nfInt64)nMinX, (nfInt64)nMaxY - (nfInt64)nMinY);
		return (2 * nCornerRadius) <= nShorterSide;
	}


	CModelToolpathSyncGroup::CModelToolpathSyncGroup(nfUint32 nSyncGroupID, const std::vector<nfUint32> & LaserIndices)
		: m_nSyncGroupID(nSyncGroupID), m_LaserIndices(LaserIndices)
	{
		if ((nSyncGroupID == 0) || (nSyncGroupID > (nfUint32)XML_3MF_MAXTOOLPATHLASERINDEX))
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, "sync group id must be a positive integer");
	}

	nfUint32 CModelToolpathSyncGroup::getSyncGroupID()
	{
		return m_nSyncGroupID;
	}

	const std::vector<nfUint32> & CModelToolpathSyncGroup::getLaserIndices()
	{
		return m_LaserIndices;
	}

	void CModelToolpathSyncGroup::setLaserIndices(const std::vector<nfUint32> & LaserIndices)
	{
		m_LaserIndices = LaserIndices;
	}

	nfBool CModelToolpathSyncGroup::referencesLaserIndex(nfUint32 nLaserIndex)
	{
		return std::find(m_LaserIndices.begin(), m_LaserIndices.end(), nLaserIndex) != m_LaserIndices.end();
	}

}
