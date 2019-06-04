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

NMR_ModelCustomAttributes.cpp implements the Model CustomAttribute Class.

--*/

#include "Model/Classes/NMR_ModelCustomAttributes.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelCustomAttributes::CModelCustomAttributes()
	{
	}

	void CModelCustomAttributes::clear()
	{
		m_Attributes.clear();
		m_AttributesMap.clear();
	}


	PModelCustomAttribute CModelCustomAttributes::addAttribute(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue)
	{
		if (hasAttribute(CModelCustomAttribute::calculateKey(sNameSpace, sName))) {
			throw CNMRException(NMR_ERROR_DUPLICATECUSTOMATTRIBUTE);
		}

		PModelCustomAttribute pAttribute = std::make_shared<CModelCustomAttribute>(sNameSpace, sName, sValue);
		m_Attributes.push_back(pAttribute);
		m_AttributesMap.insert(std::make_pair(pAttribute->getKey(), pAttribute));
		return pAttribute;
	}

	nfUint32 CModelCustomAttributes::getAttributeCount()
	{
		return (nfUint32)m_Attributes.size();
	}

	PModelCustomAttribute CModelCustomAttributes::getAttribute(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttributeCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelCustomAttribute pAttribute = m_Attributes[nIndex];
		__NMRASSERT(pAttribute.get() != nullptr);

		return pAttribute;
	}

	void CModelCustomAttributes::removeAttribute(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttributeCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto iIterator = m_Attributes.begin();
		if (nIndex > 0)
			iIterator += nIndex;
		m_Attributes.erase(iIterator);
	}

	nfBool CModelCustomAttributes::hasAttribute(_In_ std::string sKey)
	{
		std::map<std::string, PModelCustomAttribute>::iterator iIterator = m_AttributesMap.find(sKey);
		return iIterator != m_AttributesMap.end();
	}

	void CModelCustomAttributes::mergeAttribute(_In_ CModelCustomAttributes * pSourceCustomAttributes)
	{
		if (pSourceCustomAttributes == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceCustomAttributes->getAttributeCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			std::string sName;
			std::string sValue;
			PModelCustomAttribute pAttribute = pSourceCustomAttributes->getAttribute(nIndex);
			addAttribute(pAttribute->getNameSpace(), pAttribute->getName(), pAttribute->getValue());
		}
	}
}
