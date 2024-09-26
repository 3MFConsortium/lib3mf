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

Abstract: This is the class declaration of CToolpathLayerData

*/


#ifndef __LIB3MF_TOOLPATHLAYERDATA
#define __LIB3MF_TOOLPATHLAYERDATA

#include "lib3mf_interfaces.hpp"

#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_XmlWriter.h"
#include "Common/Platform/NMR_ExportStream_Memory.h"

// Parent classes
#include "lib3mf_base.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Model/Classes/NMR_ModelAttachment.h"
#include "Model/Classes/NMR_ModelToolpathLayerWriteData.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CToolpathLayerData 
**************************************************************************************************************************/

class CToolpathLayerData : public virtual IToolpathLayerData, public virtual CBase {
private:


protected:

	NMR::PModelToolpathLayerWriteData m_pLayerData;

public:

	CToolpathLayerData(NMR::PModelToolpathLayerWriteData pLayerData);

	virtual ~CToolpathLayerData();

	std::string GetLayerDataUUID() override;

	void SetSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, const std::string& sValue) override;

	void ClearSegmentAttributes() override;

	void SetLaserIndex(const Lib3MF_uint32 nValue) override;

	void ClearLaserIndex() override;

	void SetFactorRange(const Lib3MF_uint32 nValue) override;

	Lib3MF_uint32 RegisterProfile(IToolpathProfile* pProfile) override;

	Lib3MF_uint32 RegisterBuildItem(IBuildItem* pBuildItem) override;

	void WriteHatchDataInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer) override;

	void WriteHatchDataInModelUnitsWithConstantOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WriteHatchDataInModelUnitsWithRampedOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_int32* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_int32* pScalingData2Buffer) override;

	void WriteHatchDataInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataF1BufferSize, const Lib3MF_int32* pScalingDataF1Buffer, const Lib3MF_uint64 nScalingDataF2BufferSize, const Lib3MF_int32* pScalingDataF2Buffer, const Lib3MF_uint64 nScalingDataG1BufferSize, const Lib3MF_int32* pScalingDataG1Buffer, const Lib3MF_uint64 nScalingDataG2BufferSize, const Lib3MF_int32* pScalingDataG2Buffer, const Lib3MF_uint64 nScalingDataH1BufferSize, const Lib3MF_int32* pScalingDataH1Buffer, const Lib3MF_uint64 nScalingDataH2BufferSize, const Lib3MF_int32* pScalingDataH2Buffer) override;

	void WriteHatchDataDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer) override;

	void WriteHatchDataDiscreteWithConstantOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WriteHatchDataDiscreteWithRampedOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_int32* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_int32* pScalingData2Buffer) override;

	void WriteHatchDataDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataF1BufferSize, const Lib3MF_int32* pScalingDataF1Buffer, const Lib3MF_uint64 nScalingDataF2BufferSize, const Lib3MF_int32* pScalingDataF2Buffer, const Lib3MF_uint64 nScalingDataG1BufferSize, const Lib3MF_int32* pScalingDataG1Buffer, const Lib3MF_uint64 nScalingDataG2BufferSize, const Lib3MF_int32* pScalingDataG2Buffer, const Lib3MF_uint64 nScalingDataH1BufferSize, const Lib3MF_int32* pScalingDataH1Buffer, const Lib3MF_uint64 nScalingDataH2BufferSize, const Lib3MF_int32* pScalingDataH2Buffer) override;

	void WriteLoopInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer) override;

	void WriteLoopInModelUnitsWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WriteLoopInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) override;

	void WriteLoopDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer) override;

	void WriteLoopDiscreteWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WriteLoopDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) override;

	void WritePolylineInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer) override;

	void WritePolylineInModelUnitsWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WritePolylineInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) override;

	void WritePolylineDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer) override;

	void WritePolylineDiscreteWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer) override;

	void WritePolylineDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) override;

	void Finish() override;

	ICustomDOMTree* AddCustomData(const std::string& sNameSpace, const std::string& sDataName) override;


};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TOOLPATHLAYERDATA
