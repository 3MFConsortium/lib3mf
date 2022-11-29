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


// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/Platform/NMR_XmlWriter_Native.h"
#include "Model/Classes/NMR_ModelConstants.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathLayerData
**************************************************************************************************************************/

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



void CToolpathLayerData::WriteHatchData(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize % 2 != 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize / 2 > LIB3MF_MAXTOOLPATHHATCHCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nHatchCount = (unsigned int)nPointDataBufferSize / 2;
	double dUnits = m_pLayerData->getUnits();

	// If we have a streamwriter, write all points into a binary stream
	std::vector<int> X1Values;
	std::vector<int> Y1Values;
	std::vector<int> X2Values;
	std::vector<int> Y2Values;

	X1Values.resize(nHatchCount);
	Y1Values.resize(nHatchCount);
	X2Values.resize(nHatchCount);
	Y2Values.resize(nHatchCount);

	unsigned nIndex;
	const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nHatchCount; nIndex++) {
		X1Values[nIndex] = (int)(pPointData->m_Coordinates[0] / dUnits);
		Y1Values[nIndex] = (int)(pPointData->m_Coordinates[1] / dUnits);
		pPointData++;
		X2Values[nIndex] = (int)(pPointData->m_Coordinates[0] / dUnits);
		Y2Values[nIndex] = (int)(pPointData->m_Coordinates[1] / dUnits);
		pPointData++;
	}

	m_pLayerData->WriteHatchData(nProfileID, nPartID, nHatchCount, X1Values.data(), Y1Values.data(), X2Values.data(), Y2Values.data());

}

void CToolpathLayerData::WriteLoop(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{
	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	std::vector<int> XValues;
	std::vector<int> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {
		XValues[nIndex] = (int)(pPointData->m_Coordinates[0] / dUnits);
		YValues[nIndex] = (int)(pPointData->m_Coordinates[1] / dUnits);
		pPointData++;
	}

	m_pLayerData->WriteLoop(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());

}

void CToolpathLayerData::WritePolyline(const Lib3MF_uint32 nProfileID, const Lib3MF_uint32 nPartID, const Lib3MF_uint64 nPointDataBufferSize, const Lib3MF::sPosition2D* pPointDataBuffer)
{

	if (nPointDataBufferSize == 0)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);
	if (nPointDataBufferSize > LIB3MF_MAXTOOLPATHPOINTCOUNT)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_INVALIDPOINTCOUNT);

	unsigned int nPointCount = (unsigned int)nPointDataBufferSize;
	double dUnits = m_pLayerData->getUnits();

	std::vector<int> XValues;
	std::vector<int> YValues;

	XValues.resize(nPointCount);
	YValues.resize(nPointCount);

	unsigned nIndex;
	const Lib3MF::sPosition2D* pPointData = pPointDataBuffer;
	for (nIndex = 0; nIndex < nPointCount; nIndex++) {
		XValues[nIndex] = (int)(pPointData->m_Coordinates[0] / dUnits);
		YValues[nIndex] = (int)(pPointData->m_Coordinates[1] / dUnits);
		pPointData++;
	}

	m_pLayerData->WritePolyline(nProfileID, nPartID, nPointCount, XValues.data(), YValues.data());
}



std::string CToolpathLayerData::GetLayerDataUUID()
{
	return m_pLayerData->getUUID();
}


