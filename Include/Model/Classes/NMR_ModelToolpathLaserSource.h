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

NMR_ModelToolpathLaserSource.h defines the laser sources and sync groups of a toolpath (Toolpath Extension, 4.5).

--*/

#ifndef __NMR_MODELTOOLPATHLASERSOURCE
#define __NMR_MODELTOOLPATHLASERSOURCE

#include "Common/NMR_Types.h"

#include <vector>
#include <memory>
#include <string>

namespace NMR {

	// The laser index is immutable, so that it stays unique within the owning toolpath.
	class CModelToolpathLaserSource {
	private:
		nfUint32 m_nLaserIndex;
		std::string m_sName;

		// Scan-field bounds are an all-or-nothing group.
		nfBool m_bHasBounds;
		nfInt32 m_nMinX;
		nfInt32 m_nMaxX;
		nfInt32 m_nMinY;
		nfInt32 m_nMaxY;

		// The optical center is an all-or-nothing pair.
		nfBool m_bHasCenter;
		nfInt32 m_nCenterX;
		nfInt32 m_nCenterY;

		// 0 if absent. Only valid together with the bounds.
		nfUint32 m_nCornerRadius;

	public:
		CModelToolpathLaserSource() = delete;
		CModelToolpathLaserSource(nfUint32 nLaserIndex, const std::string & sName);

		nfUint32 getLaserIndex();

		std::string getName();
		void setName(const std::string & sName);

		nfBool hasBounds();
		void getBounds(nfInt32 & nMinX, nfInt32 & nMaxX, nfInt32 & nMinY, nfInt32 & nMaxY);
		// Fails if max < min, or if an existing corner radius does not fit the new bounds.
		void setBounds(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY);
		// Also removes the corner radius, which requires the bounds.
		void clearBounds();

		nfBool hasCenter();
		void getCenter(nfInt32 & nCenterX, nfInt32 & nCenterY);
		void setCenter(nfInt32 nCenterX, nfInt32 nCenterY);
		void clearCenter();

		nfBool hasCornerRadius();
		nfUint32 getCornerRadius();
		// Fails if no bounds are set, if the radius is 0, or if it exceeds half of the shorter field side.
		void setCornerRadius(nfUint32 nCornerRadius);
		void clearCornerRadius();

		static nfBool boundsAreValid(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY);
		static nfBool cornerRadiusIsValid(nfInt32 nMinX, nfInt32 nMaxX, nfInt32 nMinY, nfInt32 nMaxY, nfInt64 nCornerRadius);
	};

	typedef std::shared_ptr <CModelToolpathLaserSource> PModelToolpathLaserSource;


	// The id is immutable, so that it stays unique within the owning toolpath.
	// The laser indices are validated by the owning toolpath.
	class CModelToolpathSyncGroup {
	private:
		nfUint32 m_nSyncGroupID;
		std::vector<nfUint32> m_LaserIndices;

	public:
		CModelToolpathSyncGroup() = delete;
		CModelToolpathSyncGroup(nfUint32 nSyncGroupID, const std::vector<nfUint32> & LaserIndices);

		nfUint32 getSyncGroupID();

		const std::vector<nfUint32> & getLaserIndices();
		void setLaserIndices(const std::vector<nfUint32> & LaserIndices);

		nfBool referencesLaserIndex(nfUint32 nLaserIndex);
	};

	typedef std::shared_ptr <CModelToolpathSyncGroup> PModelToolpathSyncGroup;

}

#endif // __NMR_MODELTOOLPATHLASERSOURCE
