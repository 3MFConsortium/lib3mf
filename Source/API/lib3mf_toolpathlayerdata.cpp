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
	WriteHatchDataInModelUnitsWithRampedOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, 0, nullptr, 0, nullptr);
}



void CToolpathLayerData::WriteHatchDataInModelUnitsWithConstantOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WriteHatchDataInModelUnitsWithRampedOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, nScalingDataBufferSize, pScalingDataBuffer);
}

void CToolpathLayerData::WriteHatchDataInModelUnitsWithRampedOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_int32* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_int32* pScalingData2Buffer)
{
	WriteHatchDataInModelUnitsWithMultipleOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nScalingData1BufferSize, pScalingData1Buffer, nScalingData2BufferSize, pScalingData2Buffer, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataF1BufferSize, const Lib3MF_int32* pScalingDataF1Buffer, const Lib3MF_uint64 nScalingDataF2BufferSize, const Lib3MF_int32* pScalingDataF2Buffer, const Lib3MF_uint64 nScalingDataG1BufferSize, const Lib3MF_int32* pScalingDataG1Buffer, const Lib3MF_uint64 nScalingDataG2BufferSize, const Lib3MF_int32* pScalingDataG2Buffer, const Lib3MF_uint64 nScalingDataH1BufferSize, const Lib3MF_int32* pScalingDataH1Buffer, const Lib3MF_uint64 nScalingDataH2BufferSize, const Lib3MF_int32* pScalingDataH2Buffer)

{
	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (pHatchDataBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

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

	if (nScalingDataG1BufferSize == 0)
		pScalingDataG1Buffer = nullptr;
	if (pScalingDataG1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataG1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataG2BufferSize == 0)
		pScalingDataG2Buffer = nullptr;
	if (pScalingDataG2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataG2BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataH1BufferSize == 0)
		pScalingDataH1Buffer = nullptr;
	if (pScalingDataH1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataH1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataH2BufferSize == 0)
		pScalingDataH2Buffer = nullptr;
	if (pScalingDataH2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataH2BufferSize)
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
	std::vector<int32_t> ProfileIDValues;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);
	TagValues.resize(nHatchCount);
	ProfileIDValues.resize(nHatchCount);

	uint32_t nTagCount = 0;
	uint32_t nProfileIDCount = 0;

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

		ProfileIDValues[nIndex] = (int32_t)pHatchData->m_ProfileOverrideID;
		TagValues[nIndex] = pHatchData->m_Tag;

		if (pHatchData->m_Tag != 0)
			nTagCount++;
		if (pHatchData->m_ProfileOverrideID != 0)
			nProfileIDCount++;
		pHatchData++;
	}

	int32_t* pTagValues = nullptr;
	if (nTagCount > 0)
		pTagValues = TagValues.data();
	int32_t* pProfileIDValues = nullptr;
	if (nProfileIDCount > 0)
		pProfileIDValues = ProfileIDValues.data();

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data(), pTagValues, pProfileIDValues, pScalingDataF1Buffer, pScalingDataF2Buffer, pScalingDataG1Buffer, pScalingDataG2Buffer, pScalingDataH1Buffer, pScalingDataH2Buffer);

}

void CToolpathLayerData::WriteHatchDataDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer)
{
	WriteHatchDataDiscreteWithRampedOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataDiscreteWithConstantOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WriteHatchDataDiscreteWithRampedOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, nScalingDataBufferSize, pScalingDataBuffer);
}

void CToolpathLayerData::WriteHatchDataDiscreteWithRampedOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingData1BufferSize, const Lib3MF_int32* pScalingData1Buffer, const Lib3MF_uint64 nScalingData2BufferSize, const Lib3MF_int32* pScalingData2Buffer)
{
	WriteHatchDataDiscreteWithMultipleOverrides(nProfileID, nPartID, nHatchDataBufferSize, pHatchDataBuffer, nScalingData1BufferSize, pScalingData1Buffer, nScalingData2BufferSize, pScalingData2Buffer, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteHatchDataDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer, const Lib3MF_uint64 nScalingDataF1BufferSize, const Lib3MF_int32* pScalingDataF1Buffer, const Lib3MF_uint64 nScalingDataF2BufferSize, const Lib3MF_int32* pScalingDataF2Buffer, const Lib3MF_uint64 nScalingDataG1BufferSize, const Lib3MF_int32* pScalingDataG1Buffer, const Lib3MF_uint64 nScalingDataG2BufferSize, const Lib3MF_int32* pScalingDataG2Buffer, const Lib3MF_uint64 nScalingDataH1BufferSize, const Lib3MF_int32* pScalingDataH1Buffer, const Lib3MF_uint64 nScalingDataH2BufferSize, const Lib3MF_int32* pScalingDataH2Buffer)
{
	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (pHatchDataBuffer == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

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
	
	if (nScalingDataG1BufferSize == 0)
		pScalingDataG1Buffer = nullptr;
	if (pScalingDataG1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataG1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataG2BufferSize == 0)
		pScalingDataG2Buffer = nullptr;
	if (pScalingDataG2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataG2BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataH1BufferSize == 0)
		pScalingDataH1Buffer = nullptr;
	if (pScalingDataH1Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataH1BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	if (nScalingDataH2BufferSize == 0)
		pScalingDataH2Buffer = nullptr;
	if (pScalingDataH2Buffer != nullptr) {
		if (nHatchDataBufferSize != nScalingDataH2BufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHHATCHDATA);
	}

	uint32_t nHatchCount = (unsigned int)nHatchDataBufferSize;

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int32_t> X1Values;
	std::vector<int32_t> Y1Values;
	std::vector<int32_t> X2Values;
	std::vector<int32_t> Y2Values;
	std::vector<int32_t> TagValues;
	std::vector<int32_t> ProfileIDValues;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);
	TagValues.resize(nHatchCount);
	ProfileIDValues.resize(nHatchCount);

	uint32_t nTagCount = 0;
	uint32_t nProfileIDCount = 0;

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
		ProfileIDValues[nIndex] = (int32_t)pHatchData->m_ProfileOverrideID;
		TagValues[nIndex] = pHatchData->m_Tag;

		if (pHatchData->m_Tag != 0)
			nTagCount++;
		if (pHatchData->m_ProfileOverrideID != 0)
			nProfileIDCount++;

		pHatchData++;
	}

	int32_t* pTagValues = nullptr;
	if (nTagCount > 0)
		pTagValues = TagValues.data();
	int32_t* pProfileIDValues = nullptr;
	if (nProfileIDCount > 0)
		pProfileIDValues = ProfileIDValues.data();

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data(), pTagValues, pProfileIDValues, pScalingDataF1Buffer, pScalingDataF2Buffer, pScalingDataG1Buffer, pScalingDataG2Buffer, pScalingDataH1Buffer, pScalingDataH2Buffer);

}

void CToolpathLayerData::WriteLoopInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	WriteLoopInModelUnitsWithOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}


void CToolpathLayerData::WriteLoopInModelUnitsWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WriteLoopInModelUnitsWithMultipleOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteLoopInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nScalingDataFBufferSize == 0)
		pScalingDataFBuffer = nullptr;
	if (pScalingDataFBuffer != nullptr) {
		if (nScalingDataFBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataGBufferSize == 0)
		pScalingDataGBuffer = nullptr;
	if (pScalingDataGBuffer != nullptr) {
		if (nScalingDataGBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataHBufferSize == 0)
		pScalingDataHBuffer = nullptr;
	if (pScalingDataHBuffer != nullptr) {
		if (nScalingDataHBufferSize != nPointDataBufferSize)
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

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pScalingDataFBuffer, pScalingDataGBuffer, pScalingDataHBuffer);

}

void CToolpathLayerData::WriteLoopDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{
	WriteLoopDiscreteWithOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WriteLoopDiscreteWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WriteLoopDiscreteWithMultipleOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WriteLoopDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nScalingDataFBufferSize == 0)
		pScalingDataFBuffer = nullptr;
	if (pScalingDataFBuffer != nullptr) {
		if (nScalingDataFBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataGBufferSize == 0)
		pScalingDataGBuffer = nullptr;
	if (pScalingDataGBuffer != nullptr) {
		if (nScalingDataGBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataHBufferSize == 0)
		pScalingDataHBuffer = nullptr;
	if (pScalingDataHBuffer != nullptr) {
		if (nScalingDataHBufferSize != nPointDataBufferSize)
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

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pScalingDataFBuffer, pScalingDataGBuffer, pScalingDataHBuffer);

}

void CToolpathLayerData::WritePolylineInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	WritePolylineInModelUnitsWithOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WritePolylineInModelUnitsWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WritePolylineInModelUnitsWithMultipleOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WritePolylineInModelUnitsWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) 
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nScalingDataFBufferSize == 0)
		pScalingDataFBuffer = nullptr;
	if (pScalingDataFBuffer != nullptr) {
		if (nScalingDataFBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataGBufferSize == 0)
		pScalingDataGBuffer = nullptr;
	if (pScalingDataGBuffer != nullptr) {
		if (nScalingDataGBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataHBufferSize == 0)
		pScalingDataHBuffer = nullptr;
	if (pScalingDataHBuffer != nullptr) {
		if (nScalingDataHBufferSize != nPointDataBufferSize)
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

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pScalingDataFBuffer, pScalingDataGBuffer, pScalingDataHBuffer);
}


void CToolpathLayerData::WritePolylineDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{
	WritePolylineDiscreteWithOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, 0, nullptr);
}

void CToolpathLayerData::WritePolylineDiscreteWithOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataBufferSize, const Lib3MF_int32* pScalingDataBuffer)
{
	WritePolylineDiscreteWithMultipleOverrides(nProfileID, nPartID, nPointDataBufferSize, pPointDataBuffer, nScalingDataBufferSize, pScalingDataBuffer, 0, nullptr, 0, nullptr);
}

void CToolpathLayerData::WritePolylineDiscreteWithMultipleOverrides(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer, const Lib3MF_uint64 nScalingDataFBufferSize, const Lib3MF_int32* pScalingDataFBuffer, const Lib3MF_uint64 nScalingDataGBufferSize, const Lib3MF_int32* pScalingDataGBuffer, const Lib3MF_uint64 nScalingDataHBufferSize, const Lib3MF_int32* pScalingDataHBuffer) 
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	if (nScalingDataFBufferSize == 0)
		pScalingDataFBuffer = nullptr;
	if (pScalingDataFBuffer != nullptr) {
		if (nScalingDataFBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataGBufferSize == 0)
		pScalingDataGBuffer = nullptr;
	if (pScalingDataGBuffer != nullptr) {
		if (nScalingDataGBufferSize != nPointDataBufferSize)
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_SCALINGDATANEEDSTOMATCHPOINTDATA);
	}

	if (nScalingDataHBufferSize == 0)
		pScalingDataHBuffer = nullptr;
	if (pScalingDataHBuffer != nullptr) {
		if (nScalingDataHBufferSize != nPointDataBufferSize)
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

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data(), nullptr, pScalingDataFBuffer, pScalingDataGBuffer, pScalingDataHBuffer);
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
	m_pLayerData->setCurrentLaserIndex(nValue);
}

void CToolpathLayerData::ClearLaserIndex()
{
	m_pLayerData->setCurrentLaserIndex(0);
}

void CToolpathLayerData::SetFactorRange(const Lib3MF_uint32 nValue)
{
	m_pLayerData->setFactorRange(nValue);
}
