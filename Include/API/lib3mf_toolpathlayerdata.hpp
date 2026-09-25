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

	void SetLaserSync(const Lib3MF_uint32 nValue) override;

	void ClearLaserSync() override;

	void SetTimePrediction(const Lib3MF_uint32 nValue) override;

	void ClearTimePrediction() override;

	void SetJumpPrediction(const Lib3MF_uint32 nValue) override;

	void ClearJumpPrediction() override;

	void SetSegmentTag(const Lib3MF_uint32 nValue) override;

	void ClearSegmentTag() override;

	Lib3MF_uint32 RegisterProfile(IToolpathProfile* pProfile) override;

	Lib3MF_uint32 RegisterBuildItem(IBuildItem* pBuildItem) override;

	void WriteHatchDataInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer) override;

	void WriteHatchDataInModelUnitsWithConstantFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void WriteHatchDataInModelUnitsWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer) override;

	void WriteHatchDataInModelUnitsWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer) override;

	void WriteHatchDataDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer) override;

	void WriteHatchDataDiscreteWithConstantFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void WriteHatchDataDiscreteWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer) override;

	void WriteHatchDataDiscreteWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_double* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_double* pScalingData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer) override;

	void WriteLoopInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer) override;

	void WriteLoopDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer) override;

	void WriteLoopInModelUnitsWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void WriteLoopDiscreteWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void WritePolylineInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer) override;

	void WritePolylineInModelUnitsWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void WritePolylineDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer) override;

	void WritePolylineDiscreteWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) override;

	void Finish() override;

	ICustomDOMTree* AddCustomData(const std::string& sNameSpace, const std::string& sDataName) override;


};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_TOOLPATHLAYERDATA
