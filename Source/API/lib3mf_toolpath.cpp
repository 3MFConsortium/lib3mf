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

Abstract: This is a stub class definition of CToolpath

*/

#include "lib3mf_toolpath.hpp"
#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_toolpathlayerdata.hpp"
#include "lib3mf_attachment.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_customdomtree.hpp"

#include "lib3mf_writer.hpp"
#include "lib3mf_toolpathlayerreader.hpp"

#include "Common/NMR_Exception.h"

// Include custom headers here.
#include "Common/Platform/NMR_ImportStream_Shared_Memory.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include "Model/ToolpathReader/NMR_ToolpathReader.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpath
**************************************************************************************************************************/

CToolpath::CToolpath(NMR::PModelToolpath pToolpath)
	: CResource(pToolpath), m_pToolpath(pToolpath)
{

}


Lib3MF_double CToolpath::GetUnits()
{
	return m_pToolpath->getUnitFactor();
}

Lib3MF_uint32 CToolpath::GetLayerCount()
{
	return m_pToolpath->getLayerCount();
}

Lib3MF_uint32 CToolpath::GetProfileCount()
{
	return m_pToolpath->getProfileCount();
}

Lib3MF_uint32 CToolpath::GetLayerZ(const Lib3MF_uint32 nLayerIndex)
{
	auto pLayer = m_pToolpath->getLayer(nLayerIndex);
	return pLayer->getMaxZ();
}

IToolpathProfile* CToolpath::AddProfile(const std::string& sName)
{
	auto pProfile = m_pToolpath->addProfile(sName);

	return new CToolpathProfile(pProfile);
}

IToolpathProfile* CToolpath::GetProfile(const Lib3MF_uint32 nProfileIndex)
{
	auto pProfile = m_pToolpath->getProfile(nProfileIndex);

	return new CToolpathProfile(pProfile);

}

IToolpathProfile* CToolpath::GetProfileUUID(const std::string& sProfileUUID)
{
	auto pProfile = m_pToolpath->getProfileByUUID(sProfileUUID);

	return new CToolpathProfile(pProfile);
}


IToolpathLayerData* CToolpath::AddLayer(const Lib3MF_uint32 nZMax, const std::string& sPath, IWriter* pModelWriter)
{
	if (pModelWriter == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

	CWriter* pWriterInstance = dynamic_cast<CWriter*> (pModelWriter);
	if (pWriterInstance == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);

	// Retrieve 3mf writer instance
	NMR::PModelWriter pNMRModelWriterInstance = pWriterInstance->getModelWriter();
	NMR::PModelWriter_3MF pNMRModelWriter3MFInstance = std::dynamic_pointer_cast<NMR::CModelWriter_3MF> (pNMRModelWriterInstance);
	if (pNMRModelWriter3MFInstance.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDCAST);


	auto pWriterData = std::make_shared<NMR::CModelToolpathLayerWriteData>(m_pToolpath.get(), pNMRModelWriter3MFInstance, sPath, pNMRModelWriter3MFInstance->getCustomNamespaceMap ());
	std::unique_ptr<CToolpathLayerData> pToolpathData(new CToolpathLayerData(pWriterData));

	m_pToolpath->addLayer(sPath, nZMax);

	return pToolpathData.release();

}


IAttachment* CToolpath::GetLayerAttachment(const Lib3MF_uint32 nIndex)
{
	auto pLayer = m_pToolpath->getLayer(nIndex);
	auto pModel = m_pToolpath->getModel();
	auto pAttachment = pModel->findModelAttachment(pLayer->getLayerDataPath());
	if (pAttachment.get() == nullptr)
		throw NMR::CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);

	return new CAttachment(pAttachment);

}

std::string CToolpath::GetLayerPath(const Lib3MF_uint32 nIndex)
{
	auto pLayer = m_pToolpath->getLayer(nIndex);
	return pLayer->getLayerDataPath();
}

Lib3MF_uint32 CToolpath::GetLayerZMax(const Lib3MF_uint32 nIndex)
{
	auto pLayer = m_pToolpath->getLayer(nIndex);
	return pLayer->getMaxZ();
}


IToolpathLayerReader* CToolpath::ReadLayerData(const Lib3MF_uint32 nIndex)
{
	auto pLayer = m_pToolpath->getLayer(nIndex);

	auto pModel = m_pToolpath->getModel();

	NMR::PImportStream pLayerDataStream;

	std::string sLayerPathData = pLayer->getLayerDataPath();
	auto pAttachment = pModel->findModelAttachment(sLayerPathData);
	if (pAttachment.get() != nullptr) {
		pLayerDataStream = pAttachment->getStream();
	}
	else {
		pLayerDataStream = pModel->readPathFromPersistentDataSource(sLayerPathData);
		if (pLayerDataStream.get () == nullptr)
			throw NMR::CNMRException(NMR_ERROR_INVALIDMODELATTACHMENT);
	}

	auto pReader = std::make_shared<NMR::CToolpathReader>(m_pToolpath, true);
	auto pReadData = pReader->getReadData();
	for (auto iIter : m_RegisteredAttributes) {
		pReadData->registerCustomSegmentAttribute(iIter.first.first, iIter.first.second, iIter.second);
	}

	pReader->readStream(pLayerDataStream);

	return new CToolpathLayerReader(pReader->getReadData());
}

Lib3MF_uint32 CToolpath::GetCustomDataCount()
{
	return m_pToolpath->getCustomXMLDataCount();
}

ICustomDOMTree* CToolpath::GetCustomData(const Lib3MF_uint32 nIndex)
{
	return new CCustomDOMTree(m_pToolpath->getCustomXMLData (nIndex));
}

void CToolpath::GetCustomDataName(const Lib3MF_uint32 nIndex, std::string& sNameSpace, std::string& sDataName)
{
	auto pXMLData = m_pToolpath->getCustomXMLData(nIndex);
	sNameSpace = pXMLData->getNameSpace();
	sDataName = pXMLData->getRootName();
}

bool CToolpath::HasUniqueCustomData(const std::string& sNameSpace, const std::string& sDataName)
{
	uint32_t nFoundData = 0;

	uint32_t nDataCount = m_pToolpath->getCustomXMLDataCount();
	for (uint32_t nIndex = 0; nIndex < nDataCount; nIndex++) {
		auto pXMLData = m_pToolpath->getCustomXMLData(nIndex);
		if ((sNameSpace == pXMLData->getNameSpace()) && (sDataName == pXMLData->getRootName()))
			nFoundData++;
	}

	return (nFoundData == 1);

}

ICustomDOMTree* CToolpath::FindUniqueCustomData(const std::string& sNameSpace, const std::string& sDataName)
{
	NMR::PCustomXMLTree pFoundTree;

	uint32_t nDataCount = m_pToolpath->getCustomXMLDataCount();
	for (uint32_t nIndex = 0; nIndex < nDataCount; nIndex++) {
		auto pXMLData = m_pToolpath->getCustomXMLData(nIndex);
		if ((sNameSpace == pXMLData->getNameSpace()) && (sDataName == pXMLData->getRootName())) {
			if (pFoundTree.get() != nullptr)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_DUPLICATECUSTOMDATA, "duplicate custom data: " + sDataName);

			pFoundTree = pXMLData;
		}
			
	}

	if (pFoundTree.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_CUSTOMDATANOTFOUND, "custom data not found: " + sDataName);

	return new CCustomDOMTree(pFoundTree);
}

ICustomDOMTree* CToolpath::AddCustomData(const std::string& sNameSpace, const std::string& sDataName)
{
	auto pXMLData = std::make_shared<NMR::CCustomXMLTree>(sNameSpace, sDataName);
	m_pToolpath->addCustomXMLData(pXMLData);
	return new CCustomDOMTree(pXMLData);
}

Lib3MF_uint32 CToolpath::ClearCustomData()
{
	return m_pToolpath->clearCustomXMLData();
}

bool CToolpath::DeleteCustomData(ICustomDOMTree* pData)
{
	if (pData == nullptr)
		return false;

	auto pDataInstance = dynamic_cast<CCustomDOMTree*> (pData);
	if (pDataInstance == nullptr)
		return false;

	return m_pToolpath->deleteCustomXMLData (pDataInstance->getXMLTreeInstance ().get ());
}

void CToolpath::RegisterCustomIntegerAttribute(const std::string& sNameSpace, const std::string& sAttributeName)
{
	auto key = std::make_pair(sNameSpace, sAttributeName);
	auto iIter = m_RegisteredAttributes.find (key);

	if (iIter != m_RegisteredAttributes.end())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_ATTRIBUTEALREADYDEFINED, "Toolpath attribute already defined: " + sNameSpace + "/" + sAttributeName);

	m_RegisteredAttributes.insert(std::make_pair (key, NMR::eModelToolpathSegmentAttributeType::SegmentAttributeInt64));
}

void CToolpath::RegisterCustomDoubleAttribute(const std::string& sNameSpace, const std::string& sAttributeName)
{
	auto key = std::make_pair(sNameSpace, sAttributeName);
	auto iIter = m_RegisteredAttributes.find(key);

	if (iIter != m_RegisteredAttributes.end())
		throw ELib3MFInterfaceException(LIB3MF_ERROR_TOOLPATH_ATTRIBUTEALREADYDEFINED, "Toolpath attribute already defined: " + sNameSpace + "/" + sAttributeName);

	m_RegisteredAttributes.insert(std::make_pair(key, NMR::eModelToolpathSegmentAttributeType::SegmentAttributeDouble));
}

