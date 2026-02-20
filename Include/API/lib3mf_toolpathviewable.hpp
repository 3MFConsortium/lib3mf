/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CToolpathViewable

*/


#ifndef __LIB3MF_TOOLPATHVIEWABLE
#define __LIB3MF_TOOLPATHVIEWABLE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream.h"


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CToolpathViewable 
**************************************************************************************************************************/

class CToolpathViewable : public virtual IToolpathViewable, public virtual CBase {
private:

	NMR::PImportStream m_pLayerDataStream;
	Lib3MF_uint32 m_nLayerIndex;
	std::string m_sLayerPath;
	Lib3MF_uint32 m_nLayerZMin;
	Lib3MF_uint32 m_nLayerZMax;
	Lib3MF_uint32 m_nLayerThickness;
	mutable bool m_bJSONCached;
	mutable std::string m_sCachedJSON;

protected:

public:

	CToolpathViewable(
		NMR::PImportStream pLayerDataStream,
		Lib3MF_uint32 nLayerIndex,
		const std::string & sLayerPath,
		Lib3MF_uint32 nLayerZMin,
		Lib3MF_uint32 nLayerZMax,
		Lib3MF_uint32 nLayerThickness);

	virtual ~CToolpathViewable();

	void cacheJSONIfNeeded() const;

	Lib3MF_uint32 GetLayerIndex() override;

	std::string GetLayerPath() override;

	Lib3MF_uint32 GetLayerZMin() override;

	Lib3MF_uint32 GetLayerZMax() override;

	Lib3MF_uint32 GetLayerThickness() override;

	std::string GetJSONString() override;

	void GetJSONBuffer(Lib3MF_uint64 nJSONBufferBufferSize, Lib3MF_uint64* pJSONBufferNeededCount, Lib3MF_uint8 * pJSONBufferBuffer) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TOOLPATHVIEWABLE
