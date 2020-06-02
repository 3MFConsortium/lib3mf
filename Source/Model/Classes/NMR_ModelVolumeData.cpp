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

NMR_ModelVolumeData.cpp implements the class CModelVolumeData.

--*/

#include "Model/Classes/NMR_ModelVolumeData.h" 

namespace NMR {

	CModelVolumeData::CModelVolumeData()
		:m_pLevelset(nullptr), m_pComposite(nullptr), m_pColor(nullptr), m_mapProperties()
	{

	}

	void CModelVolumeData::clear()
	{

	}

	bool CModelVolumeData::HasLevelset()
	{
		return m_pLevelset.get() != nullptr;
	}

	void CModelVolumeData::SetLevelset(PVolumeLevelset pLevelset)
	{
		if (!pLevelset)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pLevelset = pLevelset;
	}

	PVolumeLevelset CModelVolumeData::CreateLevelset(PModelVolumetricStack pVolumetricStack)
	{
		m_pLevelset = std::make_shared<CVolumeLevelset>(pVolumetricStack);
		return m_pLevelset;
	}

	PVolumeLevelset CModelVolumeData::GetLevelset()
	{
		return m_pLevelset;
	}


	nfBool CModelVolumeData::hasProperty(_In_ std::string sName)
	{
		std::map<std::string, PVolumeProperty>::iterator iIterator = m_mapProperties.find(sName);
		return iIterator != m_mapProperties.end();
	}

	nfUint32 CModelVolumeData::GetPropertyCount()
	{
		return (nfUint32)m_mapProperties.size();
	}

	PVolumeProperty CModelVolumeData::GetProperty(nfUint32 nIndex)
	{
		auto iIterator = m_mapProperties.begin();
		if (nIndex >= m_mapProperties.size())
			throw CNMRException(NMR_ERROR_INVALIDINDEX);
		
		while (nIndex > 0) {
			nIndex--;
			iIterator++;
		}
		return iIterator->second;
	}

	PVolumeProperty CModelVolumeData::FindProperty(std::string sName)
	{
		std::map<std::string, PVolumeProperty>::iterator iIterator = m_mapProperties.find(sName);
		if (iIterator != m_mapProperties.end()) {
			return iIterator->second;
		}
		return nullptr;
	}

	PVolumeProperty CModelVolumeData::CreateProperty(std::string sName, PModelVolumetricStack pVolumetricStack)
	{
		if (hasProperty(sName)) {
			throw CNMRException(NMR_ERROR_DUPLICATEVOLUMEDATAPROPERTY);
		}

		PVolumeProperty pVolumeProperty = std::make_shared<CVolumeProperty>(sName, pVolumetricStack);
		m_mapProperties.insert(std::make_pair(pVolumeProperty->GetName(), pVolumeProperty));
		return pVolumeProperty;
	}

	void CModelVolumeData::RemoveProperty(std::string sName)
	{
		m_mapProperties.erase(sName);
	}

}
