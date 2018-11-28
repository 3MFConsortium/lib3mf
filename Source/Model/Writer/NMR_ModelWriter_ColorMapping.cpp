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

#include "Model/Writer/NMR_ModelWriter_ColorMapping.h" 
#include "Common/NMR_Exception.h" 
#include "Common/NMR_Exception_Windows.h" 
#include "Common/NMR_Types.h" 

namespace NMR {

	CModelWriter_ColorMapping::CModelWriter_ColorMapping(_In_ ModelResourceID ResourceID)
	{
		m_nCurrentIndex = 0;
		m_ResourceID = ResourceID;
	}

	void CModelWriter_ColorMapping::registerColor(_In_ nfColor cColor)
	{
		auto iIterator = m_IndexMap.find(cColor);
		if (iIterator == m_IndexMap.end()) {
			m_IndexMap.insert(std::make_pair(cColor, m_nCurrentIndex));
			m_nCurrentIndex++;
			m_ColorVector.push_back(cColor);
		}

	}

	nfBool CModelWriter_ColorMapping::findColor(_In_ nfColor cColor, _Out_ ModelResourceIndex & nResourceIndex)
	{
		auto iIterator = m_IndexMap.find(cColor);
		if (iIterator != m_IndexMap.end()) {
			nResourceIndex = iIterator->second;
			return true;
		}
		else {
			nResourceIndex = 0;
			return false;
		}

	}

	nfUint32 CModelWriter_ColorMapping::getCount()
	{
		return (nfUint32)m_ColorVector.size();

	}
	nfColor CModelWriter_ColorMapping::getColor(_In_ nfUint32 nIndex)
	{
		return m_ColorVector[nIndex];
	}

	ModelResourceID CModelWriter_ColorMapping::getResourceID()
	{
		return m_ResourceID;
	}

}
