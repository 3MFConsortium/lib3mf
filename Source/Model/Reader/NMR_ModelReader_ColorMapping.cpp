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
NMR_ModelReader_ColorMapping.cpp implements the Model Reader Color Mapping Class.

--*/

#include "Model/Reader/NMR_ModelReader_ColorMapping.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 


namespace NMR {

	bool operator< (_In_ const READERCOLORMAPPINGTREEENTRY & entry1, _In_ const READERCOLORMAPPINGTREEENTRY & entry2)
	{
		if (entry1.m_nResourceID < entry2.m_nResourceID)
			return true;
		if (entry1.m_nResourceID > entry2.m_nResourceID)
			return false;
		if (entry1.m_nResourceIndex < entry2.m_nResourceIndex)
			return true;
		return false;
	}

	CModelReader_ColorMapping::CModelReader_ColorMapping()
	{
	}

	void CModelReader_ColorMapping::registerColor(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _In_ nfColor cColor)
	{
		READERCOLORMAPPINGTREEENTRY MappingEntry;
		MappingEntry.m_nResourceID = nResourceID;
		MappingEntry.m_nResourceIndex = nResourceIndex;

		m_ColorEntries.insert(std::make_pair(MappingEntry, cColor));
		m_ResourceIDs.insert(std::make_pair(nResourceID, 1));
	}

	nfBool CModelReader_ColorMapping::findColor(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _Out_ nfColor & cColor)
	{
		READERCOLORMAPPINGTREEENTRY MappingEntry;
		MappingEntry.m_nResourceID = nResourceID;
		MappingEntry.m_nResourceIndex = nResourceIndex;
		auto iIterator = m_ColorEntries.find(MappingEntry);

		if (iIterator != m_ColorEntries.end()) {
			cColor = iIterator->second;
			return true;
		}
		else {
			cColor = 0xffffffff;
			return false;
		}
	}

	nfBool CModelReader_ColorMapping::hasResource(_In_ ModelResourceID nResourceID)
	{
		auto iIterator = m_ResourceIDs.find(nResourceID);
		return (iIterator != m_ResourceIDs.end());
	}

	void CModelReader_ColorMapping::registerTextureReference(_In_ ModelResourceID nResourceID, _In_ ModelResourceID nTextureID)
	{
		m_TextureMappings.insert(std::make_pair(nResourceID, nTextureID));
	}

	nfBool CModelReader_ColorMapping::hasTextureReference(_In_ ModelResourceID nResourceID)
	{
		auto iIterator = m_TextureMappings.find(nResourceID);
		return (iIterator != m_TextureMappings.end());
	}

	ModelResourceID CModelReader_ColorMapping::getTextureReference(_In_ ModelResourceID nResourceID)
	{
		auto iIterator = m_TextureMappings.find(nResourceID);
		if (iIterator != m_TextureMappings.end()) {
			return iIterator->second;
		}
		return 0;
	}

	void CModelReader_ColorMapping::registerMaterialReference(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nMaterialIndex)
	{
		m_MaterialMappings.insert(std::make_pair(nResourceID, nMaterialIndex));
	}

	nfBool CModelReader_ColorMapping::hasMaterialReference(_In_ ModelResourceID nResourceID)
	{
		auto iIterator = m_MaterialMappings.find(nResourceID);
		return (iIterator != m_MaterialMappings.end());
	}

	nfBool CModelReader_ColorMapping::getMaterialReference(_In_ ModelResourceID nResourceID, _Out_ ModelResourceIndex & nMaterialIndex)
	{
		auto iIterator = m_MaterialMappings.find(nResourceID);
		if (iIterator != m_MaterialMappings.end()) {
			nMaterialIndex = iIterator->second;
			return true;
		}
		nMaterialIndex = 0;
		return false;
	}

	nfBool CModelReader_ColorMapping::retrieveARessource(_Out_ ModelResourceID& nResourceID)
	{
		std::map <ModelResourceID, nfInt32>::iterator it;
		for (it = m_ResourceIDs.begin(); it != m_ResourceIDs.end(); it++)
		{
			nResourceID = it->first;
			return true;
		}
		return false;
	}
}
