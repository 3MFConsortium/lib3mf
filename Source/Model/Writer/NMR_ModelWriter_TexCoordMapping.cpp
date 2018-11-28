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

NMR_ModelWriter_ColorMapping.cpp implements the Model Writer Color Mapping Class.

--*/

#include "Model/Writer/NMR_ModelWriter_TexCoordMapping.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/NMR_Types.h" 

namespace NMR {

	bool operator< (_In_ const WRITERTEXCOORDMAPPINGTREEENTRY & entry1, _In_ const WRITERTEXCOORDMAPPINGTREEENTRY & entry2)
	{
		if (entry1.m_fU < entry2.m_fU)
			return true;
		if (entry1.m_fU > entry2.m_fU)
			return false;
		if (entry1.m_fV < entry2.m_fV)
			return true;
		return false;
	}

	CModelWriter_TexCoordMapping::CModelWriter_TexCoordMapping(_In_ ModelResourceID ResourceID, _In_ ModelResourceID TextureID)
	{
		m_nCurrentIndex = 0;
		m_ResourceID = ResourceID;
		m_TextureID = TextureID;
	}

	void CModelWriter_TexCoordMapping::registerTexCoords(_In_ nfFloat fU, _In_ nfFloat fV)
	{
		WRITERTEXCOORDMAPPINGTREEENTRY Entry;
		Entry.m_fU = fU;
		Entry.m_fV = fV;

		auto iIterator = m_IndexMap.find(Entry);
		if (iIterator == m_IndexMap.end()) {
			m_IndexMap.insert(std::make_pair(Entry, m_nCurrentIndex));
			m_TexCoordVector.push_back(Entry);
			m_nCurrentIndex++;
		}

	}

	nfBool CModelWriter_TexCoordMapping::findTexCoords(_In_ nfFloat fU, _In_ nfFloat fV, _Out_ ModelResourceIndex & nResourceIndex)
	{
		WRITERTEXCOORDMAPPINGTREEENTRY Entry;
		Entry.m_fU = fU;
		Entry.m_fV = fV;

		auto iIterator = m_IndexMap.find(Entry);
		if (iIterator != m_IndexMap.end()) {
			nResourceIndex = iIterator->second;
			return true;
		}
		else {
			nResourceIndex = 0;
			return false;
		}

	}

	ModelResourceID CModelWriter_TexCoordMapping::getResourceID()
	{
		return m_ResourceID;
	}

	ModelResourceID CModelWriter_TexCoordMapping::getTextureID()
	{
		return m_TextureID;
	}

	nfUint32 CModelWriter_TexCoordMapping::getCount()
	{
		return (nfUint32) m_TexCoordVector.size();

	}

	void CModelWriter_TexCoordMapping::getTexCoords(_In_ nfUint32 nIndex, _Out_ nfFloat & fU, _Out_ nfFloat & fV)
	{
		WRITERTEXCOORDMAPPINGTREEENTRY Entry = m_TexCoordVector[nIndex];
		fU = Entry.m_fU;
		fV = Entry.m_fV;
	}


	
}
