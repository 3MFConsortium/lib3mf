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


void CToolpathLayerData::AddCustomLineAttributes(const std::string& sNameSpace, const std::string& sAttributeName, const Lib3MF_uint64 nValuesBufferSize, const Lib3MF_int32* pValuesBuffer)
{

}

void CToolpathLayerData::ClearCustomLineAttributes()
{

}

void CToolpathLayerData::WriteHatchDataInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sHatch2D* pHatchDataBuffer) 
{
	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	unsigned int nHatchCount = (unsigned int)nHatchDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int32_t> X1Values;
	std::vector<int32_t> Y1Values;
	std::vector<int32_t> X2Values;
	std::vector<int32_t> Y2Values;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);

	unsigned nIndex;
	const Lib3MF::sHatch2D* pHatchData = pHatchDataBuffer;
	for (nIndex = 0; nIndex < nHatchCount; nIndex++) {
		double dX1 = pHatchData->m_Point1Coordinates[0] / dUnits;
		double dY1 = pHatchData->m_Point1Coordinates[1] / dUnits;
		double dX2 = pHatchData->m_Point2Coordinates[0] / dUnits;
		double dY2 = pHatchData->m_Point2Coordinates[1] / dUnits;

		if ((dX1 < LIB3MF_MINDISCRETECOORDINATE) || (dX1 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string (pHatchData->m_Point1Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point1Coordinates[1]));
		if ((dY1 < LIB3MF_MINDISCRETECOORDINATE) || (dY1 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point1Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point1Coordinates[1]));
		if ((dX2 < LIB3MF_MINDISCRETECOORDINATE) || (dX2 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point2Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point2Coordinates[1]));
		if ((dY2 < LIB3MF_MINDISCRETECOORDINATE) || (dY2 > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOORDINATE, "invalid hatch coordinate: " + std::to_string(pHatchData->m_Point2Coordinates[0]) + "/" + std::to_string(pHatchData->m_Point2Coordinates[1]));

		X1Values[nIndex] = (int32_t)round (dX1);
		Y1Values[nIndex] = (int32_t)round (dY1);
		X2Values[nIndex] = (int32_t)round (dX2);
		Y2Values[nIndex] = (int32_t)round (dY2);
		pHatchData++;
	}

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data());

}

void CToolpathLayerData::WriteHatchDataDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nHatchDataBufferSize, const Lib3MF::sDiscreteHatch2D* pHatchDataBuffer)
{
	if (nHatchDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	if (nHatchDataBufferSize > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDHATCHCOUNT);

	unsigned int nHatchCount = (unsigned int)nHatchDataBufferSize;

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int32_t> X1Values;
	std::vector<int32_t> Y1Values;
	std::vector<int32_t> X2Values;
	std::vector<int32_t> Y2Values;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);

	unsigned nIndex;
	const Lib3MF::sDiscreteHatch2D* pHatchData = pHatchDataBuffer;
	for (nIndex = 0; nIndex < nHatchCount; nIndex++) {

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
		pHatchData++;
	}

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data());

}

void CToolpathLayerData::WriteLoopInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

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

		if ((dX < LIB3MF_MINDISCRETECOORDINATE) || (dX > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));
		if ((dY < LIB3MF_MINDISCRETECOORDINATE) || (dY > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));

		XValues[nIndex] = (int32_t)round(dX);
		YValues[nIndex] = (int32_t)round(dY);
		pPointData++;
	}

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());

}

void CToolpathLayerData::WriteLoopDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

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

		if ((nX < LIB3MF_MINDISCRETECOORDINATE) || (nX > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));
		if ((nY < LIB3MF_MINDISCRETECOORDINATE) || (nY > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));

		XValues[nIndex] = nX;
		YValues[nIndex] = nY;
		pPointData++;
	}

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());

}

void CToolpathLayerData::WritePolylineInModelUnits(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

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

		if ((dX < LIB3MF_MINDISCRETECOORDINATE) || (dX > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));
		if ((dY < LIB3MF_MINDISCRETECOORDINATE) || (dY > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(pPointData->m_Coordinates[0]) + "/" + std::to_string(pPointData->m_Coordinates[1]));

		XValues[nIndex] = (int32_t)round(dX);
		YValues[nIndex] = (int32_t)round(dY);
		pPointData++;
	}

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());
}


void CToolpathLayerData::WritePolylineDiscrete(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const bool bWriteCustomLineAttributes, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sDiscretePosition2D* pPointDataBuffer)
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

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

		if ((nX < LIB3MF_MINDISCRETECOORDINATE) || (nX > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));
		if ((nY < LIB3MF_MINDISCRETECOORDINATE) || (nY > LIB3MF_MAXDISCRETECOORDINATE))
			throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOORDINATE, "invalid hatch coordinate: " + std::to_string(nX) + "/" + std::to_string(nY));

		XValues[nIndex] = nX;
		YValues[nIndex] = nY;

		pPointData++;
	}

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());
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