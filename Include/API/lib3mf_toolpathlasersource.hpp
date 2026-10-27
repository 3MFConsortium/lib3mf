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

Abstract: This is the class declaration of CToolpathLaserSource and CToolpathSyncGroup

*/


#ifndef __LIB3MF_TOOLPATHLASERSOURCE
#define __LIB3MF_TOOLPATHLASERSOURCE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelToolpath.h"
#include "Model/Classes/NMR_ModelToolpathLaserSource.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CToolpathLaserSource
**************************************************************************************************************************/

class CToolpathLaserSource : public virtual IToolpathLaserSource, public virtual CBase {
protected:

	NMR::PModelToolpathLaserSource m_pLaserSource;

public:

	CToolpathLaserSource(NMR::PModelToolpathLaserSource pLaserSource);

	Lib3MF_uint32 GetLaserIndex() override;

	std::string GetName() override;

	void SetName(const std::string & sName) override;

	bool HasBounds() override;

	void GetBounds(Lib3MF_int32 & nMinX, Lib3MF_int32 & nMaxX, Lib3MF_int32 & nMinY, Lib3MF_int32 & nMaxY) override;

	void SetBounds(const Lib3MF_int32 nMinX, const Lib3MF_int32 nMaxX, const Lib3MF_int32 nMinY, const Lib3MF_int32 nMaxY) override;

	void ClearBounds() override;

	bool HasCenter() override;

	void GetCenter(Lib3MF_int32 & nCenterX, Lib3MF_int32 & nCenterY) override;

	void SetCenter(const Lib3MF_int32 nCenterX, const Lib3MF_int32 nCenterY) override;

	void ClearCenter() override;

	bool HasCornerRadius() override;

	Lib3MF_uint32 GetCornerRadius() override;

	void SetCornerRadius(const Lib3MF_uint32 nCornerRadius) override;

	void ClearCornerRadius() override;

};


/*************************************************************************************************************************
 Class declaration of CToolpathSyncGroup
**************************************************************************************************************************/

class CToolpathSyncGroup : public virtual IToolpathSyncGroup, public virtual CBase {
protected:

	// The toolpath validates laser indices against its laser sources.
	NMR::PModelToolpath m_pToolpath;
	NMR::PModelToolpathSyncGroup m_pSyncGroup;

public:

	CToolpathSyncGroup(NMR::PModelToolpath pToolpath, NMR::PModelToolpathSyncGroup pSyncGroup);

	Lib3MF_uint32 GetSyncGroupID() override;

	void GetLaserIndices(Lib3MF_uint64 nLaserIndicesBufferSize, Lib3MF_uint64 * pLaserIndicesNeededCount, Lib3MF_uint32 * pLaserIndicesBuffer) override;

	void SetLaserIndices(const Lib3MF_uint64 nLaserIndicesBufferSize, const Lib3MF_uint32 * pLaserIndicesBuffer) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TOOLPATHLASERSOURCE
