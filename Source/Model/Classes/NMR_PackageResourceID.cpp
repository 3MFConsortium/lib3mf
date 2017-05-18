/*++

Copyright (C) 2017 Autodesk Inc.

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

NMR_PackageResourceID.cpp implements the PackageResourceID Class.

--*/

#include "Model/Classes/NMR_PackageResourceID.h" 
#include "Common/NMR_StringUtils.h" 
#include "Common/NMR_Exception.h" 

namespace NMR {

	void CPackageResourceID::set(std::string p, ModelResourceID id) {
		m_path = p;
		m_id = id;
	}
	void CPackageResourceID::get(std::string& p) {
		p = m_path;
	}
	void CPackageResourceID::get(ModelResourceID& id) {
		id = m_id;
	}

	void CPackageResourceID::setUniqueID(PackageResourceID id) {
		m_uniqueID = id;
	}
	PackageResourceID CPackageResourceID::getUniqueID() {
		return m_uniqueID;
	}

	PPackageResourceID CResourceHandler::getNewRessourceID(std::wstring path, ModelResourceID id)
	{
		return getNewRessourceID(fnUTF16toUTF8(path), id);
	}

	PPackageResourceID CResourceHandler::getNewRessourceID(std::string path, ModelResourceID id)	// this is supposed to be the only way to generate a CPackageResourceID
	{
		PPackageResourceID p = std::make_shared<CPackageResourceID>();
		if (findRessourceID(path, id))
			throw CNMRException(NMR_ERROR_DUPLICATERESOURCEID);
		p->set(path, id);
		p->setUniqueID(int(m_resourceIDs.size())+1);
		m_resourceIDs.insert(std::make_pair(p->getUniqueID(), p));
		return p;
	}
	PPackageResourceID CResourceHandler::findRessourceID(PackageResourceID id)
	{
		auto it = m_resourceIDs.find(id);
		if (it != m_resourceIDs.end())
		{
			return it->second;
		}
		return nullptr;
	}
	PPackageResourceID CResourceHandler::findRessourceID(std::wstring path, ModelResourceID id)	// 
	{
		return findRessourceID(fnUTF16toUTF8(path), id);
	}
	PPackageResourceID CResourceHandler::findRessourceID(std::string path, ModelResourceID id)	// 
	{
		for (auto it : m_resourceIDs) {
			std::string tPath;
			ModelResourceID tID;
			it.second.get()->get(tID);
			if (tID == id) {
				it.second.get()->get(tPath);
				if (tPath == path) {
					return it.second;
				}
			}
		}
		return nullptr;
	}

	void CResourceHandler::FlattenIDs() {

	}

	void CResourceHandler::clear() {
		m_resourceIDs.clear();
	}

}
