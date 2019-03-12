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

NMR_ModelWriter_ColorMappingContainer.cpp implements the Model Writer Color Mapping Container Class.

--*/

#include "Model/Writer/NMR_ModelWriter_TexCoordMappingContainer.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/NMR_Types.h" 

namespace NMR {


	CModelWriter_TexCoordMappingContainer::CModelWriter_TexCoordMappingContainer()
	{

	}

	CModelWriter_TexCoordMappingContainer::~CModelWriter_TexCoordMappingContainer()
	{

	}

	nfBool CModelWriter_TexCoordMappingContainer::hasTexture(_In_ ModelResourceID nTextureID)
	{
		auto iIterator = m_TextureMappings.find(nTextureID);
		return (iIterator != m_TextureMappings.end());
	}

	PModelWriter_TexCoordMapping CModelWriter_TexCoordMappingContainer::findTexture(_In_ ModelResourceID nTextureID)
	{
		auto iIterator = m_TextureMappings.find(nTextureID);
		if (iIterator != m_TextureMappings.end()) {
			return iIterator->second;
		}
		else {
			return nullptr;
		}
	}

	PModelWriter_TexCoordMapping CModelWriter_TexCoordMappingContainer::addTexture(_In_ ModelResourceID nTextureID, _In_ ModelResourceID nResourceID)
	{
		if (hasTexture(nTextureID))
			throw CNMRException(NMR_ERROR_DUPLICATETEXTURE);

		PModelWriter_TexCoordMapping pTexture = std::make_shared<CModelWriter_TexCoordMapping>(nResourceID, nTextureID);
		m_TextureMappings.insert(std::make_pair(nTextureID, pTexture));
		m_TextureMappingVector.push_back(pTexture);

		return pTexture;
	}

	nfUint32 CModelWriter_TexCoordMappingContainer::getCount()
	{
		return (nfUint32)m_TextureMappingVector.size();
	}

	PModelWriter_TexCoordMapping CModelWriter_TexCoordMappingContainer::getMapping(_In_ nfUint32 nIndex)
	{
		return m_TextureMappingVector[nIndex];
	}

}

