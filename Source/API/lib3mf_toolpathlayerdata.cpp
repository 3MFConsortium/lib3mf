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

Abstract: This is a stub class definition of CToolpathLayerData

*/

#include "lib3mf_toolpathlayerdata.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_object.hpp"
#include "lib3mf_toolpathlayerreader.hpp"
#include "lib3mf_builditem.hpp"
#include "lib3mf_customdomtree.hpp"


// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include <cmath>

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLayerData
**************************************************************************************************************************/

#define LIB3MF_MINDISCRETECOORDINATE (-1024*1024*1024)
#define LIB3MF_MAXDISCRETECOORDINATE (+1024*1024*1024)
#define LIB3MF_MAXTOOLPATHHATCHCOUNT 1024*1024*1024
#define LIB3MF_MAXTOOLPATHPOINTCOUNT 1024*1024*1024

namespace {

	// Converts point data given in model units into discrete toolpath coordinates.
	void convertModelPointsToDiscrete(double dUnits, unsigned int nPointCount, const Lib3MF::sPosition2D* pPointDataBuffer, std::vector<int32_t>& XValues, std::vector<int32_t>& YValues)
	{
		XValues.resize(nPointCount);
		YValues.resize(nPointCount);

		const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
		for (unsigned int nIndex = 0; nIndex < nPointCount; nIndex++) {
			double dX = pPointData->m_Coordinates[0] / dUnits;
			double dY = pPointData->m_Coordinates[1] / dUnits;

			if ((dX < (double)LIB3MF_MINDISCRETECOORDINATE) || (dX > (double)LIB3MF_MAXDISCRETECOORDINATE))
				throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid point coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));
			if ((dY < (double)LIB3MF_MINDISCRETECOORDINATE) || (dY > (double)LIB3MF_MAXDISCRETECOORDINATE))
				throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid point coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));

			XValues[nIndex] = (int32_t)round(dX);
			YValues[nIndex] = (int32_t)round(dY);
			pPointData++;
		}
	}

	// Copies and validates point data already given in discrete toolpath coordinates.
	void convertDiscretePoints(unsigned int nPointCount, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, std::vector<int32_t>& XValues, std::vector<int32_t>& YValues)
	{
		XValues.resize(nPointCount);
		YValues.resize(nPointCount);

		const Lib3MF::sDiscretePosition2D* pPointData = pPointDataBuffer;
		for (unsigned int nIndex = 0; nIndex < nPointCount; nIndex++) {
			int32_t nX = pPointData->m_Coordinates[0];
			int32_t nY = pPointData->m_Coordinates[1];

			if ((nX < LIB3MF_MINDISCRETECOORDINATE) || (nX > LIB3MF_MAXDISCRETECOORDINATE))
				throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid point coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));
			if ((nY < LIB3MF_MINDISCRETECOORDINATE) || (nY > LIB3MF_MAXDISCRETECOORDINATE))
				throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid point coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));

			XValues[nIndex] = nX;
			YValues[nIndex] = nY;
			pPointData++;
		}
	}

	// Normalizes empty factor/sub buffers to null and validates their cardinality against the point count.
	void preparePointFactorBuffers(unsigned int nPointCount,
		Lib3MF_uint64 & nFactorData1BufferSize, const Lib3MF_double* & pFactorData1Buffer,
		Lib3MF_uint64 & nFactorData2BufferSize, const Lib3MF_double* & pFactorData2Buffer,
		Lib3MF_uint64 & nSubInterpolationCountsBufferSize, const Lib3MF_uint32* & pSubInterpolationCountsBuffer,
		Lib3MF_uint64 & nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* & pModificationInterpolationDataBuffer)
	{
		if (nFactorData1BufferSize == 0)
			pFactorData1Buffer = nullptr;
		if (nFactorData2BufferSize == 0)
			pFactorData2Buffer = nullptr;
		if (nSubInterpolationCountsBufferSize == 0)
			pSubInterpolationCountsBuffer = nullptr;
		if (nModificationInterpolationDataBufferSize == 0)
			pModificationInterpolationDataBuffer = nullptr;

		if ((pFactorData1Buffer != nullptr) && (nFactorData1BufferSize != nPointCount))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
		if ((pFactorData2Buffer != nullptr) && (nFactorData2BufferSize != nPointCount))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
		if ((pSubInterpolationCountsBuffer != nullptr) && (nSubInterpolationCountsBufferSize != nPointCount))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

}

CToolpathLayerData::CToolpathLayerData(NMR::PModelToolpathLayerWriteData pLayerData)
	: m_pLayerData(pLayerData)
{
	if (pLayerData.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);


}


CToolpathLayerData::~CToolpathLayerData()
{

}


Lib3MF_uint32 CToolpathLayerData::RegisterProfile(IToolpathProfile* pProfile)
{
	if (pProfile == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CToolpathProfile* pProfileClass = dynamic_cast<CToolpathProfile*> (pProfile);
	if (pProfileClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	return m_pLayerData->RegisterProfile(pProfileClass->getProfileInstance());
}

Lib3MF_uint32 CToolpathLayerData::RegisterBuildItem(IBuildItem* pBuildItem)
{
	if (pBuildItem == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CBuildItem* pBuildItemClass = dynamic_cast<CBuildItem*> (pBuildItem);
	if (pBuildItemClass == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	return m_pLayerData->RegisterPart(pBuildItemClass->getBuildItemInstance());
}


void CToolpathLayerData::Finish()
{
	m_pLayerData->finishWriting();
}

ICustomDOMTree* CToolpathLayerData::AddCustomData(const std::string& sNameSpace, const std::string& sDataName)
{
	auto pXMLData = std::make_shared<NMR::CCustomXMLTree>(sNameSpace, sDataName);
	m_pLayerData->addCustomXMLData(pXMLData);
	return new CCustomDOMTree(pXMLData);
}

void CToolpathLayerData::WriteHatchDataInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer)
{
	WriteHatchDataInModelUnitsWithLinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataInModelUnitsWithConstantFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) 
{
	WriteHatchDataInModelUnitsWithLinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nFactorDataBufferSize, pFactorDataBuffer, nFactorDataBufferSize, pFactorDataBuffer);
}


void CToolpathLayerData::WriteHatchDataInModelUnitsWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer) 
{
	WriteHatchDataInModelUnitsWithNonlinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataInModelUnitsWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer) 
{
	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (pHatchDataBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto nScalingDataF1BufferSize = nFactorData1BufferSize;
	auto nScalingDataF2BufferSize = nFactorData2BufferSize;
	auto pScalingDataF1Buffer = pFactorData1Buffer;
	auto pScalingDataF2Buffer = pFactorData2Buffer;

	if (nScalingDataF1BufferSize == 0)
		pScalingDataF1Buffer = nullptr;
	if (pScalingDataF1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataF1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataF2BufferSize == 0)
		pScalingDataF2Buffer = nullptr;
	if (pScalingDataF2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataF2BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nSubInterpolationCountsBufferSize == 0)
		pSubInterpolationCountsBuffer = nullptr;
	if (pSubInterpolationCountsBuffer != nullptr) {
		if (nHatchDataBufferSize != nSubInterpolationCountsBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	uint32_t nHatchCount = (unsigned int)nHatchDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int32_t> X1Values;
	std::vector<int32_t> Y1Values;
	std::vector<int32_t> X2Values;
	std::vector<int32_t> Y2Values;
	std::vector<int32_t> TagValues;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);
	TagValues.resize(nHatchCount);

	uint32_t nTagCount = 0;

	const Lib3MF::sHatch2D* pHatchData = pHatchDataBuffer;
	for (uint32_t nIndex = 0; nIndex < nHatchCount; nIndex++) {
		double dX1 = pHatchData->m_Point1Coordinates[0] / dUnits;
		double dY1 = pHatchData->m_Point1Coordinates[1] / dUnits;
		double dX2 = pHatchData->m_Point2Coordinates[0] / dUnits;
		double dY2 = pHatchData->m_Point2Coordinates[1] / dUnits;

		if ((dX1 < (double)LIB3MF_MINDISCRETECOORDINATE) || (dX1 > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point1Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point1Coordinates[1]));
		if ((dY1 < (double)LIB3MF_MINDISCRETECOORDINATE) || (dY1 > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point1Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point1Coordinates[1]));
		if ((dX2 < (double)LIB3MF_MINDISCRETECOORDINATE) || (dX2 > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point2Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point2Coordinates[1]));
		if ((dY2 < (double)LIB3MF_MINDISCRETECOORDINATE) || (dY2 > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point2Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point2Coordinates[1]));

		X1Values[nIndex] = (int32_t)round(dX1);
		Y1Values[nIndex] = (int32_t)round(dY1);
		X2Values[nIndex] = (int32_t)round(dX2);
		Y2Values[nIndex] = (int32_t)round(dY2);

		TagValues[nIndex] = pHatchData->m_Tag;

		if (pHatchData->m_Tag != 0)
			nTagCount++;
		pHatchData++;
	}

	int32_t* pTagValues = nullptr;
	if (nTagCount > 0)
		pTagValues = TagValues.data();

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data(), pTagValues, pScalingDataF1Buffer, pScalingDataF2Buffer, pSubInterpolationCountsBuffer, nModificationInterpolationDataBufferSize, pModificationInterpolationDataBuffer);

}

void CToolpathLayerData::WriteHatchDataDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer)
{
	WriteHatchDataDiscreteWithLinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, 0, nullptr, 0, nullptr);
}


void CToolpathLayerData::WriteHatchDataDiscreteWithConstantFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) 
{
	WriteHatchDataDiscreteWithLinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nFactorDataBufferSize, pFactorDataBuffer, nFactorDataBufferSize, pFactorDataBuffer);

}

void CToolpathLayerData::WriteHatchDataDiscreteWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer)
{
	WriteHatchDataDiscreteWithNonlinearFactors(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataDiscreteWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_double* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_double* pScalingData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer) 
{

	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (pHatchDataBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	auto nScalingDataF1BufferSize = nScalingData1BufferSize;
	auto nScalingDataF2BufferSize = nScalingData2BufferSize;
	auto pScalingDataF1Buffer = pScalingData1Buffer;
	auto pScalingDataF2Buffer = pScalingData2Buffer;

	if (nScalingDataF1BufferSize == 0)
		pScalingDataF1Buffer = nullptr;
	if (pScalingDataF1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataF1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataF2BufferSize == 0)
		pScalingDataF2Buffer = nullptr;
	if (pScalingDataF2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataF2BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nModificationInterpolationDataBufferSize == 0)
		pModificationInterpolationDataBuffer = nullptr;
	if (pModificationInterpolationDataBuffer != nullptr) {
		if (nHatchDataBufferSize != nModificationInterpolationDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	uint32_t nHatchCount = (unsigned int)nHatchDataBufferSize;

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int32_t> X1Values;
	std::vector<int32_t> Y1Values;
	std::vector<int32_t> X2Values;
	std::vector<int32_t> Y2Values;
	std::vector<int32_t> TagValues;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);
	TagValues.resize(nHatchCount);

	uint32_t nTagCount = 0;

	const Lib3MF::sDiscreteHatch2D* pHatchData = pHatchDataBuffer;
	for (uint32_t nIndex = 0; nIndex < nHatchCount; nIndex++) {

		int32_t nX1 = pHatchData->m_Point1Coordinates[0];
		int32_t nY1 = pHatchData->m_Point1Coordinates[1];
		int32_t nX2 = pHatchData->m_Point2Coordinates[0];
		int32_t nY2 = pHatchData->m_Point2Coordinates[1];

		if ((nX1 < LIB3MF_MINDISCRETECOORDINATE) || (nX1 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX1) + "/" + std::to_string(nY1));
		if ((nY1 < LIB3MF_MINDISCRETECOORDINATE) || (nY1 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX1) + "/" + std::to_string(nY1));
		if ((nX2 < LIB3MF_MINDISCRETECOORDINATE) || (nX2 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX2) + "/" + std::to_string(nY2));
		if ((nY2 < LIB3MF_MINDISCRETECOORDINATE) || (nY2 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX2) + "/" + std::to_string(nY2));

		X1Values[nIndex] = nX1;
		Y1Values[nIndex] = nY1;
		X2Values[nIndex] = nX2;
		Y2Values[nIndex] = nY2;
		TagValues[nIndex] = pHatchData->m_Tag;

		if (pHatchData->m_Tag != 0)
			nTagCount++;

		pHatchData++;
	}

	int32_t* pTagValues = nullptr;
	if (nTagCount > 0)
		pTagValues = TagValues.data();

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data(), pTagValues, pScalingDataF1Buffer, pScalingDataF2Buffer, pSubInterpolationCountsBuffer, nModificationInterpolationDataBufferSize, pModificationInterpolationDataBuffer);

}





void CToolpathLayerData::WriteLoopInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	WriteLoopInModelUnitsWithFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}


void CToolpathLayerData::WriteLoopInModelUnitsWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nFactorDataBufferSize == 0)
		pFactorDataBuffer = nullptr;
	if (pFactorDataBuffer != nullptr) {
		if (nFactorDataBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	std::vector<int32_t> XValues;
	std::vector<int32_t> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {
		double dX = pPointData->m_Coordinates[0] / dUnits;
		double dY = pPointData->m_Coordinates[1] / dUnits;

		if ((dX < (double)LIB3MF_MINDISCRETECOORDINATE) || (dX > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));
		if ((dY < (double)LIB3MF_MINDISCRETECOORDINATE) || (dY > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));

		XValues[nIndex] = (int32_t)round(dX);
		YValues[nIndex] = (int32_t)round(dY);
		pPointData++;
	}

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pFactorDataBuffer, nullptr, nullptr);

}

void CToolpathLayerData::WriteLoopDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{
	WriteLoopDiscreteWithFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WriteLoopDiscreteWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nFactorDataBufferSize == 0)
		pFactorDataBuffer = nullptr;
	if (pFactorDataBuffer != nullptr) {
		if (nFactorDataBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	std::vector<int32_t> XValues;
	std::vector<int32_t> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sDiscretePosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {

		int32_t nX = pPointData->m_Coordinates[0];
		int32_t nY = pPointData->m_Coordinates[1];

		if ((nX < (double)LIB3MF_MINDISCRETECOORDINATE) || (nX > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));
		if ((nY < (double)LIB3MF_MINDISCRETECOORDINATE) || (nY > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));

		XValues[nIndex] = nX;
		YValues[nIndex] = nY;
		pPointData++;
	}

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pFactorDataBuffer, nullptr, nullptr);

}

void CToolpathLayerData::WritePolylineInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	WritePolylineInModelUnitsWithFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WritePolylineInModelUnitsWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer) 
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nFactorDataBufferSize == 0)
		pFactorDataBuffer = nullptr;
	if (pFactorDataBuffer != nullptr) {
		if (nFactorDataBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}



	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	std::vector<int32_t> XValues;
	std::vector<int32_t> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {
		double dX = pPointData->m_Coordinates[0] / dUnits;
		double dY = pPointData->m_Coordinates[1] / dUnits;

		if ((dX < (double)LIB3MF_MINDISCRETECOORDINATE) || (dX > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));
		if ((dY < (double)LIB3MF_MINDISCRETECOORDINATE) || (dY > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));

		XValues[nIndex] = (int32_t)round(dX);
		YValues[nIndex] = (int32_t)round(dY);
		pPointData++;
	}

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pFactorDataBuffer, nullptr, nullptr);
}


void CToolpathLayerData::WritePolylineDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{
	WritePolylineDiscreteWithFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WritePolylineDiscreteWithFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nFactorDataBufferSize, const Lib3MF_double* pFactorDataBuffer)
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nFactorDataBufferSize == 0)
		pFactorDataBuffer = nullptr;
	if (pFactorDataBuffer != nullptr) {
		if (nFactorDataBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}


	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;

	std::vector<int32_t> XValues;
	std::vector<int32_t> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sDiscretePosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {
		int32_t nX = pPointData->m_Coordinates[0];
		int32_t nY = pPointData->m_Coordinates[1];

		if ((nX < (double)LIB3MF_MINDISCRETECOORDINATE) || (nX > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));
		if ((nY < (double)LIB3MF_MINDISCRETECOORDINATE) || (nY > (double)LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));

		XValues[nIndex] = nX;
		YValues[nIndex] = nY;

		pPointData++;
	}

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pFactorDataBuffer, nullptr, nullptr);
}


void CToolpathLayerData::WriteLoopInModelUnitsWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer)
{
	WriteLoopInModelUnitsWithNonlinearFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, eModificationFactor, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteLoopInModelUnitsWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer)
{
	if ((nPointDataBufferSize == 0) || (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	Lib3MF_uint64 nF1Size = nFactorData1BufferSize, nF2Size = nFactorData2BufferSize, nSubSize = nSubInterpolationCountsBufferSize, nModSize = nModificationInterpolationDataBufferSize;
	const Lib3MF_double* pF1 = pFactorData1Buffer; const Lib3MF_double* pF2 = pFactorData2Buffer;
	const Lib3MF_uint32* pSub = pSubInterpolationCountsBuffer; const Lib3MF::sHatchModificationInterpolationData* pMod = pModificationInterpolationDataBuffer;
	preparePointFactorBuffers(nPointCount, nF1Size, pF1, nF2Size, pF2, nSubSize, pSub, nModSize, pMod);

	std::vector<int32_t> XValues, YValues;
	convertModelPointsToDiscrete(m_pLayerData->getUnits(), nPointCount, pPointDataBuffer, XValues, YValues);

	m_pLayerData->WritePointSegment(XML_3MF_TOOLPATHTYPE_LOOP, nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, eModificationFactor, pF1, pF2, pSub, nModSize, pMod);
}

void CToolpathLayerData::WriteLoopDiscreteWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer)
{
	WriteLoopDiscreteWithNonlinearFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, eModificationFactor, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteLoopDiscreteWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer)
{
	if ((nPointDataBufferSize == 0) || (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	Lib3MF_uint64 nF1Size = nFactorData1BufferSize, nF2Size = nFactorData2BufferSize, nSubSize = nSubInterpolationCountsBufferSize, nModSize = nModificationInterpolationDataBufferSize;
	const Lib3MF_double* pF1 = pFactorData1Buffer; const Lib3MF_double* pF2 = pFactorData2Buffer;
	const Lib3MF_uint32* pSub = pSubInterpolationCountsBuffer; const Lib3MF::sHatchModificationInterpolationData* pMod = pModificationInterpolationDataBuffer;
	preparePointFactorBuffers(nPointCount, nF1Size, pF1, nF2Size, pF2, nSubSize, pSub, nModSize, pMod);

	std::vector<int32_t> XValues, YValues;
	convertDiscretePoints(nPointCount, pPointDataBuffer, XValues, YValues);

	m_pLayerData->WritePointSegment(XML_3MF_TOOLPATHTYPE_LOOP, nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, eModificationFactor, pF1, pF2, pSub, nModSize, pMod);
}

void CToolpathLayerData::WritePolylineInModelUnitsWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer)
{
	WritePolylineInModelUnitsWithNonlinearFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, eModificationFactor, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WritePolylineInModelUnitsWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer)
{
	if ((nPointDataBufferSize == 0) || (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	Lib3MF_uint64 nF1Size = nFactorData1BufferSize, nF2Size = nFactorData2BufferSize, nSubSize = nSubInterpolationCountsBufferSize, nModSize = nModificationInterpolationDataBufferSize;
	const Lib3MF_double* pF1 = pFactorData1Buffer; const Lib3MF_double* pF2 = pFactorData2Buffer;
	const Lib3MF_uint32* pSub = pSubInterpolationCountsBuffer; const Lib3MF::sHatchModificationInterpolationData* pMod = pModificationInterpolationDataBuffer;
	preparePointFactorBuffers(nPointCount, nF1Size, pF1, nF2Size, pF2, nSubSize, pSub, nModSize, pMod);

	std::vector<int32_t> XValues, YValues;
	convertModelPointsToDiscrete(m_pLayerData->getUnits(), nPointCount, pPointDataBuffer, XValues, YValues);

	m_pLayerData->WritePointSegment(XML_3MF_TOOLPATHTYPE_POLYLINE, nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, eModificationFactor, pF1, pF2, pSub, nModSize, pMod);
}

void CToolpathLayerData::WritePolylineDiscreteWithLinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer)
{
	WritePolylineDiscreteWithNonlinearFactors(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, eModificationFactor, nFactorData1BufferSize, pFactorData1Buffer, nFactorData2BufferSize, pFactorData2Buffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WritePolylineDiscreteWithNonlinearFactors(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF::eToolpathProfileModificationFactor eModificationFactor, const Lib3MF_uint64 nFactorData1BufferSize, const Lib3MF_double* pFactorData1Buffer, const Lib3MF_uint64 nFactorData2BufferSize, const Lib3MF_double* pFactorData2Buffer, const Lib3MF_uint64 nSubInterpolationCountsBufferSize, const Lib3MF_uint32* pSubInterpolationCountsBuffer, const Lib3MF_uint64 nModificationInterpolationDataBufferSize, const Lib3MF::sHatchModificationInterpolationData* pModificationInterpolationDataBuffer)
{
	if ((nPointDataBufferSize == 0) || (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT))
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	Lib3MF_uint64 nF1Size = nFactorData1BufferSize, nF2Size = nFactorData2BufferSize, nSubSize = nSubInterpolationCountsBufferSize, nModSize = nModificationInterpolationDataBufferSize;
	const Lib3MF_double* pF1 = pFactorData1Buffer; const Lib3MF_double* pF2 = pFactorData2Buffer;
	const Lib3MF_uint32* pSub = pSubInterpolationCountsBuffer; const Lib3MF::sHatchModificationInterpolationData* pMod = pModificationInterpolationDataBuffer;
	preparePointFactorBuffers(nPointCount, nF1Size, pF1, nF2Size, pF2, nSubSize, pSub, nModSize, pMod);

	std::vector<int32_t> XValues, YValues;
	convertDiscretePoints(nPointCount, pPointDataBuffer, XValues, YValues);

	m_pLayerData->WritePointSegment(XML_3MF_TOOLPATHTYPE_POLYLINE, nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, eModificationFactor, pF1, pF2, pSub, nModSize, pMod);
}


std::string CToolpathLayerData::GetLayerDataUUID()
{
	return m_pLayerData->getUUID();
}


void CToolpathLayerData::SetSegmentAttribute(const std::string& sNameSpace, const std::string& sAttributeName, const std::string& sValue)
{
	m_pLayerData->setCustomSegmentAttribute(sNameSpace, sAttributeName, sValue);
}

void CToolpathLayerData::ClearSegmentAttributes()
{
	m_pLayerData->clearCustomSegmentAttributes();
}

void CToolpathLayerData::SetLaserIndex(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASLASERINDEX, nValue);
}

void CToolpathLayerData::ClearLaserIndex()
{
	m_pLayerData->clearStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASLASERINDEX);
}

void CToolpathLayerData::SetLaserSync(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASLASERSYNC, nValue);
}

void CToolpathLayerData::ClearLaserSync()
{
	m_pLayerData->clearStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASLASERSYNC);
}

void CToolpathLayerData::SetTimePrediction(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASTIMEPREDICTION, nValue);
}

void CToolpathLayerData::ClearTimePrediction()
{
	m_pLayerData->clearStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASTIMEPREDICTION);
}

void CToolpathLayerData::SetJumpPrediction(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASJUMPPREDICTION, nValue);
}

void CToolpathLayerData::ClearJumpPrediction()
{
	m_pLayerData->clearStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASJUMPPREDICTION);
}

void CToolpathLayerData::SetSegmentTag(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASTAG, nValue);
}

void CToolpathLayerData::ClearSegmentTag()
{
	m_pLayerData->clearStandardSegmentAttribute(TOOLPATHSEGMENTATTRIBUTE_HASTAG);
}


