/*++

Copyright (C) 2018 3MF Consortium

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

Abstract:

NMR_ModelToolpath.cpp implements the Model Toolpath Class.

--*/

#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelToolpath.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include "Common/NMR_UUID.h"

#include <sstream>
#include <algorithm>

namespace NMR {


	CModelToolpath::CModelToolpath(_In_ const ModelResourceID sID, _In_ CModel * pModel, double dUnitFactor, CUUID uuid, nfUint32 nBottomZ)
		: CModelResource(sID, pModel), m_dUnitFactor (dUnitFactor), m_UUID (uuid), m_nBottomZ (nBottomZ), m_eToolpathType (Lib3MF::eToolpathType::Planar),
		m_pProfileNameRegistry (std::make_shared<CModelToolpathProfileNameRegistry>()),
		m_bHasDefaultLaserIndex (false), m_nDefaultLaserIndex (0), m_bLaserReferenceChecks (false)
	{
	}

	PModelToolpath CModelToolpath::make(_In_ const ModelResourceID sID, _In_ CModel * pModel, double dUnitFactor, CUUID uuid, nfUint32 nBottomZ)
	{	
		return std::make_shared<CModelToolpath> (sID, pModel, dUnitFactor, uuid, nBottomZ);
	}

	nfUint32 CModelToolpath::getBottomZ()
	{
		return m_nBottomZ;
	}

	void CModelToolpath::setBottomZ(nfUint32 nBottomZ)
	{
		if (m_Layers.size() != 0)
			throw CNMRException(NMR_ERROR_CANNOTSETBOTTOMZIFLAYERSEXIST);

		m_nBottomZ = nBottomZ;
	}

	Lib3MF::eToolpathType CModelToolpath::getToolpathType()
	{
		return m_eToolpathType;
	}

	void CModelToolpath::setToolpathType(Lib3MF::eToolpathType eToolpathType)
	{
		switch (eToolpathType) {
			case Lib3MF::eToolpathType::Planar:
			case Lib3MF::eToolpathType::ThreeAxis:
			case Lib3MF::eToolpathType::SixAxis:
				m_eToolpathType = eToolpathType;
				break;
			default:
				throw CNMRException(NMR_ERROR_INVALIDTOOLPATHTYPE);
		}
	}

	Lib3MF::eToolpathType CModelToolpath::toolpathTypeFromString(const std::string & sToolpathType)
	{
		if (sToolpathType == XML_3MF_TOOLPATHRESOURCETYPE_PLANAR)
			return Lib3MF::eToolpathType::Planar;
		if (sToolpathType == XML_3MF_TOOLPATHRESOURCETYPE_3AXIS)
			return Lib3MF::eToolpathType::ThreeAxis;
		if (sToolpathType == XML_3MF_TOOLPATHRESOURCETYPE_6AXIS)
			return Lib3MF::eToolpathType::SixAxis;

		throw CNMRException(NMR_ERROR_INVALIDTOOLPATHTYPE);
	}

	std::string CModelToolpath::toolpathTypeToString(Lib3MF::eToolpathType eToolpathType)
	{
		switch (eToolpathType) {
			case Lib3MF::eToolpathType::Planar:
				return XML_3MF_TOOLPATHRESOURCETYPE_PLANAR;
			case Lib3MF::eToolpathType::ThreeAxis:
				return XML_3MF_TOOLPATHRESOURCETYPE_3AXIS;
			case Lib3MF::eToolpathType::SixAxis:
				return XML_3MF_TOOLPATHRESOURCETYPE_6AXIS;
			default:
				throw CNMRException(NMR_ERROR_INVALIDTOOLPATHTYPE);
		}
	}

	bool CModelToolpath::layersAreEmpty()
	{
		return (m_Layers.size() == 0);
	}


	PModelToolpathLayer CModelToolpath::addLayer(const std::string & sPath, nfUint32 nMaxZ)
	{
		uint32_t nLastZ = m_nBottomZ;
		if (m_Layers.size () > 0) 
		{ 
			nLastZ = (*m_Layers.rbegin())->getMaxZ();			
		}

		if (nMaxZ <= nLastZ)
			throw CNMRException(NMR_ERROR_TOOLPATHLAYERSARENOTINCREMENTING);

		auto pLayer = std::make_shared<CModelToolpathLayer>(sPath, nMaxZ);
		m_Layers.push_back(pLayer);
		return pLayer;
	}

	nfUint32 CModelToolpath::getLayerCount()
	{
		return (nfUint32)m_Layers.size();
	}

	PModelToolpathLayer CModelToolpath::getLayer(nfUint32 nIndex)
	{
		if (nIndex >= m_Layers.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Layers[nIndex];
	}

	double CModelToolpath::getUnitFactor()
	{
		return m_dUnitFactor;
	}

	CUUID CModelToolpath::getUUID()
	{
		return m_UUID;
	}

	void CModelToolpath::resetUUID()
	{
		CUUID newUUID;
		m_UUID = newUUID;
	}



	PModelToolpathProfile CModelToolpath::addProfile(const std::string & sName)
	{
		CUUID newUUID;
		std::string sUUID = newUUID.toString();

		auto pProfile = std::make_shared<CModelToolpathProfile>(sUUID, sName, m_pProfileNameRegistry, false);
		m_Profiles.push_back(pProfile);

		m_ProfileMap.insert(std::make_pair (sUUID, pProfile));

		return pProfile;
	
	}


	PModelToolpathProfile CModelToolpath::addExistingProfile(const std::string & sUUID, const std::string & sName)
	{
		CUUID checkUUID (sUUID.c_str());

		auto pProfile = std::make_shared<CModelToolpathProfile>(checkUUID.toString (), sName, m_pProfileNameRegistry, true);
		m_Profiles.push_back(pProfile);

		m_ProfileMap.insert(std::make_pair(sUUID, pProfile));

		return pProfile;

	}

	bool CModelToolpath::hasProfileName(const std::string & sName)
	{
		return m_pProfileNameRegistry->hasName(sName);
	}


	nfUint32 CModelToolpath::getProfileCount()
	{
		return (nfUint32)m_Profiles.size();
	}

	PModelToolpathProfile CModelToolpath::getProfile(nfUint32 nIndex)
	{
		if (nIndex >= m_Profiles.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_Profiles[nIndex];
	}

	PModelToolpathProfile CModelToolpath::getProfileByUUID(std::string sUUID)
	{
		auto iIterator = m_ProfileMap.find(sUUID);
		if (iIterator != m_ProfileMap.end()) {
			return iIterator->second;
		}

		return nullptr;
	}

	PModelToolpathLaserSource CModelToolpath::addLaserSource(nfUint32 nLaserIndex, const std::string & sName)
	{
		if (m_LaserSourceMap.find(nLaserIndex) != m_LaserSourceMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATETOOLPATHLASERSOURCEINDEX, fnUint32ToString(nLaserIndex));
		if (m_LaserSources.size() >= XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHLASERSOURCE, "too many laser sources");

		auto pLaserSource = std::make_shared<CModelToolpathLaserSource>(nLaserIndex, sName);
		m_LaserSources.push_back(pLaserSource);
		m_LaserSourceMap.insert(std::make_pair(nLaserIndex, pLaserSource));

		return pLaserSource;
	}

	nfUint32 CModelToolpath::getLaserSourceCount()
	{
		return (nfUint32)m_LaserSources.size();
	}

	PModelToolpathLaserSource CModelToolpath::getLaserSource(nfUint32 nIndex)
	{
		if (nIndex >= m_LaserSources.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_LaserSources[nIndex];
	}

	PModelToolpathLaserSource CModelToolpath::findLaserSource(nfUint32 nLaserIndex)
	{
		auto iIterator = m_LaserSourceMap.find(nLaserIndex);
		if (iIterator != m_LaserSourceMap.end())
			return iIterator->second;

		return nullptr;
	}

	void CModelToolpath::removeLaserSource(nfUint32 nLaserIndex)
	{
		auto iIterator = m_LaserSourceMap.find(nLaserIndex);
		if (iIterator == m_LaserSourceMap.end())
			throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, fnUint32ToString(nLaserIndex));

		if (m_bHasDefaultLaserIndex && (m_nDefaultLaserIndex == nLaserIndex))
			throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCEINUSE, "laser index " + fnUint32ToString(nLaserIndex) + " is the default");
		for (auto pSyncGroup : m_SyncGroups) {
			if (pSyncGroup->referencesLaserIndex(nLaserIndex))
				throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCEINUSE, "laser index " + fnUint32ToString(nLaserIndex) + " is listed in sync group " + fnUint32ToString(pSyncGroup->getSyncGroupID()));
		}

		auto pLaserSource = iIterator->second;
		m_LaserSourceMap.erase(iIterator);
		m_LaserSources.erase(std::remove(m_LaserSources.begin(), m_LaserSources.end(), pLaserSource), m_LaserSources.end());
	}

	void CModelToolpath::clearLaserSources()
	{
		m_SyncGroups.clear();
		m_SyncGroupMap.clear();
		m_LaserSources.clear();
		m_LaserSourceMap.clear();
		m_bHasDefaultLaserIndex = false;
		m_nDefaultLaserIndex = 0;
	}

	nfBool CModelToolpath::hasDefaultLaserIndex()
	{
		return m_bHasDefaultLaserIndex;
	}

	nfUint32 CModelToolpath::getDefaultLaserIndex()
	{
		if (!m_bHasDefaultLaserIndex)
			throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, "no default laser index");
		return m_nDefaultLaserIndex;
	}

	void CModelToolpath::setDefaultLaserIndex(nfUint32 nLaserIndex)
	{
		if (findLaserSource(nLaserIndex).get() == nullptr)
			throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, fnUint32ToString(nLaserIndex));

		m_nDefaultLaserIndex = nLaserIndex;
		m_bHasDefaultLaserIndex = true;
	}

	void CModelToolpath::clearDefaultLaserIndex()
	{
		m_bHasDefaultLaserIndex = false;
		m_nDefaultLaserIndex = 0;
	}

	void CModelToolpath::checkSyncGroupLaserIndices(const std::vector<nfUint32> & LaserIndices)
	{
		if (LaserIndices.empty())
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, "sync group must list at least one laser index");

		for (nfUint32 nLaserIndex : LaserIndices) {
			if (findLaserSource(nLaserIndex).get() == nullptr)
				throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, fnUint32ToString(nLaserIndex));
		}
	}

	PModelToolpathSyncGroup CModelToolpath::addSyncGroup(nfUint32 nSyncGroupID, const std::vector<nfUint32> & LaserIndices)
	{
		if (m_SyncGroupMap.find(nSyncGroupID) != m_SyncGroupMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATETOOLPATHSYNCGROUPID, fnUint32ToString(nSyncGroupID));
		if (m_SyncGroups.size() >= XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_INVALIDTOOLPATHSYNCGROUP, "too many sync groups");
		checkSyncGroupLaserIndices(LaserIndices);

		auto pSyncGroup = std::make_shared<CModelToolpathSyncGroup>(nSyncGroupID, LaserIndices);
		m_SyncGroups.push_back(pSyncGroup);
		m_SyncGroupMap.insert(std::make_pair(nSyncGroupID, pSyncGroup));

		return pSyncGroup;
	}

	nfUint32 CModelToolpath::getSyncGroupCount()
	{
		return (nfUint32)m_SyncGroups.size();
	}

	PModelToolpathSyncGroup CModelToolpath::getSyncGroup(nfUint32 nIndex)
	{
		if (nIndex >= m_SyncGroups.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		return m_SyncGroups[nIndex];
	}

	PModelToolpathSyncGroup CModelToolpath::findSyncGroup(nfUint32 nSyncGroupID)
	{
		auto iIterator = m_SyncGroupMap.find(nSyncGroupID);
		if (iIterator != m_SyncGroupMap.end())
			return iIterator->second;

		return nullptr;
	}

	void CModelToolpath::removeSyncGroup(nfUint32 nSyncGroupID)
	{
		auto iIterator = m_SyncGroupMap.find(nSyncGroupID);
		if (iIterator == m_SyncGroupMap.end())
			throw CNMRException(NMR_ERROR_TOOLPATHSYNCGROUPNOTFOUND, fnUint32ToString(nSyncGroupID));

		auto pSyncGroup = iIterator->second;
		m_SyncGroupMap.erase(iIterator);
		m_SyncGroups.erase(std::remove(m_SyncGroups.begin(), m_SyncGroups.end(), pSyncGroup), m_SyncGroups.end());
	}

	void CModelToolpath::setSyncGroupLaserIndices(nfUint32 nSyncGroupID, const std::vector<nfUint32> & LaserIndices)
	{
		auto pSyncGroup = findSyncGroup(nSyncGroupID);
		if (pSyncGroup.get() == nullptr)
			throw CNMRException(NMR_ERROR_TOOLPATHSYNCGROUPNOTFOUND, fnUint32ToString(nSyncGroupID));
		checkSyncGroupLaserIndices(LaserIndices);

		pSyncGroup->setLaserIndices(LaserIndices);
	}

	nfBool CModelToolpath::getLaserReferenceChecks()
	{
		return m_bLaserReferenceChecks;
	}

	void CModelToolpath::setLaserReferenceChecks(nfBool bActive)
	{
		m_bLaserReferenceChecks = bActive;
	}

	void CModelToolpath::checkSegmentLaserReferences(const TOOLPATHSEGMENTATTRIBUTES & standardAttributes)
	{
		if (!m_bLaserReferenceChecks)
			return;

		if ((standardAttributes.m_nFlags & TOOLPATHSEGMENTATTRIBUTE_HASLASERINDEX) && !m_LaserSources.empty()) {
			if (findLaserSource(standardAttributes.m_nLaserIndex).get() == nullptr)
				throw CNMRException(NMR_ERROR_TOOLPATHLASERSOURCENOTFOUND, "segment laserindex=" + fnUint32ToString(standardAttributes.m_nLaserIndex));
		}

		if (standardAttributes.m_nFlags & TOOLPATHSEGMENTATTRIBUTE_HASLASERSYNC) {
			if (findSyncGroup(standardAttributes.m_nLaserSync).get() == nullptr)
				throw CNMRException(NMR_ERROR_TOOLPATHSYNCGROUPNOTFOUND, "segment lasersync=" + fnUint32ToString(standardAttributes.m_nLaserSync));
		}
	}

	uint32_t CModelToolpath::getCustomXMLDataCount()
	{
		return (uint32_t)m_CustomXMLData.size();
	}

	PCustomXMLTree CModelToolpath::getCustomXMLData(uint32_t nIndex)
	{
		if (nIndex >= m_CustomXMLData.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		return m_CustomXMLData.at(nIndex);

	}

	void CModelToolpath::addCustomXMLData(PCustomXMLTree pCustomXMLTree)
	{
		if (pCustomXMLTree.get () == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (m_CustomXMLData.size () >= XML_3MF_MAXRESOURCECOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYCUSTOMXMLDATAENTRIES);

		m_CustomXMLData.push_back(pCustomXMLTree);
	}

	uint32_t CModelToolpath::clearCustomXMLData()
	{
		uint32_t nCount = (uint32_t)m_CustomXMLData.size();
		m_CustomXMLData.clear();

		return nCount;
	}

	bool CModelToolpath::deleteCustomXMLData(CCustomXMLTree* pCustomXMLTree)
	{
		for (auto iIter = m_CustomXMLData.begin(); iIter != m_CustomXMLData.end(); iIter++) {
			if ((*iIter).get() == pCustomXMLTree) {
				m_CustomXMLData.erase(iIter);
				return true;
			}
		}
		return false;

	}

}

