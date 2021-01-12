/*++

Copyright (C) 2019 3MF Consortium

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

NMR_PackageResourceID.cpp implements the UniqueResourceID Class.

--*/

#include "Model/Classes/NMR_PackageResourceID.h"
#include "Common/NMR_StringUtils.h" 
#include "Common/NMR_Exception.h" 

#include <algorithm>

namespace NMR {

	CPackageModelPath::CPackageModelPath(CResourceHandler* pResourceHandler, std::string sPath)
	  : m_pResourceHandler(pResourceHandler), m_sPath(sPath)
	{
		if (pResourceHandler == nullptr) {
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		}
	}

	std::string CPackageModelPath::getPath()
	{
		return m_sPath;
	}

	void CPackageModelPath::setPath(std::string sPath)
	{
		if (m_sPath != sPath) {
			PPackageModelPath pPath = m_pResourceHandler->findPackageModelPath(sPath);
			if (pPath) {
				throw CNMRException(NMR_ERROR_DUPLICATEPACKAGEPATH);
			}
			m_sPath = sPath;
		}
	}

	CPackageResourceID::CPackageResourceID(CResourceHandler* pResourceHandler, PPackageModelPath pModelPath, ModelResourceID nID)
	  : m_pResourceHandler(pResourceHandler), m_pModelPath(pModelPath), m_id(nID)
	{
		if (pResourceHandler == nullptr || pModelPath == nullptr) {
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		}
	}

	PPackageModelPath CPackageResourceID::getPackageModelPath() {
		return m_pModelPath;
	}
	std::string CPackageResourceID::getPath() {
		return m_pModelPath->getPath();
	}

	ModelResourceID CPackageResourceID::getModelResourceID() {
		return m_id;
	}

	void CPackageResourceID::setModelPath(std::shared_ptr<CPackageResourceID> pPackageResourceID, PPackageModelPath pPath)
	{
		pPackageResourceID->m_pResourceHandler->updateModelPath(pPackageResourceID, pPath);
	}

	CResourceHandler * CPackageResourceID::getResourceHandler() {
		return m_pResourceHandler;
	}

	void CPackageResourceID::setUniqueID(UniqueResourceID id) {
		m_uniqueID = id;
	}
	UniqueResourceID CPackageResourceID::getUniqueID() {
		return m_uniqueID;
	}

	PPackageModelPath CResourceHandler::makePackageModelPath(std::string sPath)
	{
		if (findPackageModelPath(sPath)) {
			throw CNMRException(NMR_ERROR_DUPLICATEPACKAGEPATH);
		}
		PPackageModelPath pModelPath = std::make_shared<CPackageModelPath>(this, sPath);
		m_PathToModelPath.insert(std::make_pair(sPath, pModelPath));
		return pModelPath;
	}

	std::vector<PPackageModelPath> CResourceHandler::retrieveAllModelPaths()
	{
		std::vector<PPackageModelPath> vctPModelPaths;
		vctPModelPaths.reserve(m_PathToModelPath.size());
		for (auto pModelPathPair : m_PathToModelPath) {
			vctPModelPaths.push_back(pModelPathPair.second);
		}
		return vctPModelPaths;
	}

	PPackageModelPath CResourceHandler::findPackageModelPath(std::string sPath)
	{
		auto it = m_PathToModelPath.find(sPath);
		if (it != m_PathToModelPath.end())
		{
			return it->second;
		}
		return nullptr;
	}

	// this is supposed to be the only way to generate a CPackageResourceID
	PPackageResourceID CResourceHandler::makePackageResourceID(std::string path, ModelResourceID id)
	{
		PPackageModelPath pModelPath = findPackageModelPath(path);
		if (!pModelPath) {
			pModelPath = makePackageModelPath(path);
		}
		// This can be optimized
		if (findResourceIDByPair(pModelPath->getPath(), id))
			throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);

		PPackageResourceID pPackageResourceID = std::make_shared<CPackageResourceID>(this, pModelPath, id);
		UniqueIDPackageIdMap::const_iterator biggestId = std::max_element(m_resourceIDs.begin(), m_resourceIDs.end(), [](const UniqueIdPackageIdPair & v1, const UniqueIdPackageIdPair v2) {
			return v1.first < v2.first;
		});
		if (biggestId != m_resourceIDs.end()) {
			pPackageResourceID->setUniqueID(int(biggestId->first) + 1);
		} else {
			pPackageResourceID->setUniqueID(1);
		}


		m_resourceIDs.insert(std::make_pair(pPackageResourceID->getUniqueID(), pPackageResourceID));
		m_IdAndPathToPackageResourceIDs.insert(std::make_pair(std::make_pair(id, pModelPath), pPackageResourceID));
		return pPackageResourceID;
	}

	PPackageResourceID CResourceHandler::findResourceIDByUniqueID(UniqueResourceID id)
	{
		auto it = m_resourceIDs.find(id);
		if (it != m_resourceIDs.end())
		{
			return it->second;
		}
		return nullptr;
	}
	
	PPackageResourceID CResourceHandler::findResourceIDByPair(std::string path, ModelResourceID id)
	{
		PPackageModelPath pModelPath = findPackageModelPath(path);
		if (pModelPath) {
			auto it = m_IdAndPathToPackageResourceIDs.find(std::make_pair(id, pModelPath));
			if (it != m_IdAndPathToPackageResourceIDs.end())
			{
				return it->second;
			}
		}
		return nullptr;
	}

	void CResourceHandler::updateModelPath(PPackageResourceID pPackageResourceID, PPackageModelPath pNewPath)
	{
		if (pPackageResourceID == nullptr || pNewPath == nullptr) {
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		}

		PPackageModelPath pOldPath = pPackageResourceID->m_pModelPath;

		if (pNewPath == pOldPath) {
			return;
		}

		auto itOld = m_IdAndPathToPackageResourceIDs.find(std::make_pair(pPackageResourceID->m_id, pOldPath));
		if (itOld == m_IdAndPathToPackageResourceIDs.end())
		{
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}

		auto itNew = m_IdAndPathToPackageResourceIDs.find(std::make_pair(pPackageResourceID->m_id, pNewPath));
		if (itNew != m_IdAndPathToPackageResourceIDs.end())
		{
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}

		m_IdAndPathToPackageResourceIDs.erase(itOld); 
		//what if idOld was the last one standing pointint to PackageModelPath?
		
		pPackageResourceID->m_pModelPath = pNewPath;
		m_IdAndPathToPackageResourceIDs.insert(std::make_pair(std::make_pair(pPackageResourceID->m_id, pNewPath), pPackageResourceID));
	}

	void CResourceHandler::removePackageResourceID(PPackageResourceID pPackageResourceID)
	{
		auto it = m_IdAndPathToPackageResourceIDs.find(std::make_pair(pPackageResourceID->m_id, pPackageResourceID->getPackageModelPath()));
		if (it == m_IdAndPathToPackageResourceIDs.end())
		{
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}

		auto itID = m_resourceIDs.find(pPackageResourceID->m_uniqueID);
		if (itID == m_resourceIDs.end())
		{
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		}
		m_IdAndPathToPackageResourceIDs.erase(it);
		m_resourceIDs.erase(itID);
	}

	void CResourceHandler::clear() {
		m_resourceIDs.clear();
		m_IdAndPathToPackageResourceIDs.clear();
	}

}
