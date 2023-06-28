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

Abstract: This is the class declaration of CToolpathLayerReader

*/


#ifndef __LIB3MF_TOOLPATHLAYERREADER
#define __LIB3MF_TOOLPATHLAYERREADER

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif


// Include custom headers here.
#include "Model/Classes/NMR_ModelToolpathLayerReadData.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CToolpathLayerReader 
**************************************************************************************************************************/

class CToolpathLayerReader : public virtual IToolpathLayerReader, public virtual CBase {
private:

protected:

	NMR::PModelToolpathLayerReadData m_pReadData;

public:

	CToolpathLayerReader(NMR::PModelToolpathLayerReadData pReadData);

	virtual ~CToolpathLayerReader();

	std::string GetLayerDataUUID() override;

	Lib3MF_uint32 GetSegmentCount() override;

	void GetSegmentInfo(const Lib3MF_uint32 nIndex, Lib3MF::eToolpathSegmentType & eType, Lib3MF_uint32 & nPointCount) override;

	IToolpathProfile * GetSegmentProfile(const Lib3MF_uint32 nIndex) override;

	std::string GetSegmentProfileUUID(const Lib3MF_uint32 nIndex) override;

	IBuildItem * GetSegmentPart(const Lib3MF_uint32 nIndex) override;

	std::string GetSegmentPartUUID(const Lib3MF_uint32 nIndex) override;

	void GetSegmentPointData(const Lib3MF_uint32 nIndex, Lib3MF_uint64 nPointDataBufferSize, Lib3MF_uint64* pPointDataNeededCount, Lib3MF::sPosition2D * pPointDataBuffer) override;

	Lib3MF_uint32 GetCustomDataCount() override;

	ICustomDOMTree* GetCustomData(const Lib3MF_uint32 nIndex) override;

	void GetCustomDataName(const Lib3MF_uint32 nIndex, std::string& sNameSpace, std::string& sDataName) override;

	void FindAttributeInfoByName(const std::string& sNameSpace, const std::string& sAttributeName, Lib3MF_uint32& nID, Lib3MF::eToolpathAttributeType& eAttributeType) override;

	Lib3MF_uint32 FindAttributeIDByName(const std::string& sNameSpace, const std::string& sAttributeName) override;

	Lib3MF::eToolpathAttributeType FindAttributeValueByName(const std::string& sNameSpace, const std::string& sAttributeName) override;

	Lib3MF_int64 GetSegmentIntegerAttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID) override;

	Lib3MF_int64 GetSegmentIntegerAttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName) override;

	Lib3MF_double GetSegmentDoubleAttributeByID(const Lib3MF_uint32 nIndex, const Lib3MF_uint32 nID) override;

	Lib3MF_double GetSegmentDoubleAttributeByName(const Lib3MF_uint32 nIndex, const std::string& sNameSpace, const std::string& sAttributeName) override;


};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TOOLPATHLAYERREADER
