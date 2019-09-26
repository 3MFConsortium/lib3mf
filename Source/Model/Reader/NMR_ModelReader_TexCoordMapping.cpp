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
NMR_ModelReader_TexCoordMapping.cpp implements the Model Reader TexCoord Mapping Class.

--*/

#include "Model/Reader/NMR_ModelReader_TexCoordMapping.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 


namespace NMR {

	bool operator< (_In_ const READERTEXCOORDMAPPINGTREEENTRY & entry1, _In_ const READERTEXCOORDMAPPINGTREEENTRY & entry2)
	{
		if (entry1.m_nResourceID < entry2.m_nResourceID)
			return true;
		if (entry1.m_nResourceID > entry2.m_nResourceID)
			return false;
		if (entry1.m_nResourceIndex < entry2.m_nResourceIndex)
			return true;
		return false;
	}


	CModelReader_TexCoordMapping::CModelReader_TexCoordMapping()
	{
	}

	void CModelReader_TexCoordMapping::registerTexCoords(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _In_ ModelResourceID nTextureID, _In_ nfFloat fU, _In_ nfFloat fV)
	{
		READERTEXCOORDMAPPINGTREEENTRY MappingEntry;
		MappingEntry.m_nResourceID = nResourceID;
		MappingEntry.m_nResourceIndex = nResourceIndex;

		READERTEXCOORDMAPPINGVALUE ValueEntry;
		ValueEntry.m_nTextureID = nTextureID;
		ValueEntry.m_fU = fU;
		ValueEntry.m_fV = fV;

		m_TexCoordEntries.insert(std::make_pair(MappingEntry, ValueEntry));
		m_ResourceIDs.insert(std::make_pair(nResourceID, 1));
	}

	nfBool CModelReader_TexCoordMapping::findTexCoords(_In_ ModelResourceID nResourceID, _In_ ModelResourceIndex nResourceIndex, _Out_ ModelResourceID & nTextureID, _Out_ nfFloat & fU, _Out_ nfFloat & fV)
	{
		READERTEXCOORDMAPPINGTREEENTRY MappingEntry;
		MappingEntry.m_nResourceID = nResourceID;
		MappingEntry.m_nResourceIndex = nResourceIndex;
		auto iIterator = m_TexCoordEntries.find(MappingEntry);

		if (iIterator != m_TexCoordEntries.end()) {
			nTextureID = iIterator->second.m_nTextureID;
			fU = iIterator->second.m_fU;
			fV = iIterator->second.m_fV;
			return true;
		}
		else {
			nTextureID = 0;
			fU = 0.0f;
			fV = 0.0f;
			return false;
		}
	}

	nfBool CModelReader_TexCoordMapping::hasResource(_In_ ModelResourceID nResourceID)
	{
		auto iIterator = m_ResourceIDs.find(nResourceID);
		return (iIterator != m_ResourceIDs.end());
	}

	nfBool CModelReader_TexCoordMapping::retrieveAResource(_Out_ ModelResourceID& nResourceID)
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

