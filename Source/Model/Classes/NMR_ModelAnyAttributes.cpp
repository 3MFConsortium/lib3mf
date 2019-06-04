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

NMR_ModelAnyAttributes.cpp implements the Model AnyAttribute Class.

--*/

#include "Model/Classes/NMR_ModelAnyAttributes.h"
#include "Common/NMR_Exception.h"

namespace NMR {

	CModelAnyAttributes::CModelAnyAttributes()
	{
	}

	void CModelAnyAttributes::clear()
	{
		m_Attributes.clear();
		m_AttributesMap.clear();
	}


	PModelAnyAttribute CModelAnyAttributes::addAttribute(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue)
	{
		if (hasAttribute(CModelAnyAttribute::calculateKey(sNameSpace, sName))) {
			throw CNMRException(NMR_ERROR_DUPLICATEANYATTRIBUTE);
		}

		PModelAnyAttribute pAnyAttribute = std::make_shared<CModelAnyAttribute>(sNameSpace, sName, sValue);
		m_Attributes.push_back(pAnyAttribute);
		m_AttributesMap.insert(std::make_pair(pAnyAttribute->getKey(), pAnyAttribute));
		return pAnyAttribute;
	}

	nfUint32 CModelAnyAttributes::getAttributeCount()
	{
		return (nfUint32)m_Attributes.size();
	}

	PModelAnyAttribute CModelAnyAttributes::getAttribute(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttributeCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelAnyAttribute pAnyAttribute = m_Attributes[nIndex];
		__NMRASSERT(pAnyAttribute.get() != nullptr);

		return pAnyAttribute;
	}

	void CModelAnyAttributes::removeAttribute(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getAttributeCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto iIterator = m_Attributes.begin();
		if (nIndex > 0)
			iIterator += nIndex;
		m_Attributes.erase(iIterator);
	}

	nfBool CModelAnyAttributes::hasAttribute(_In_ std::string sKey)
	{
		std::map<std::string, PModelAnyAttribute>::iterator iIterator = m_AttributesMap.find(sKey);
		return iIterator != m_AttributesMap.end();
	}

	void CModelAnyAttributes::mergeAttribute(_In_ CModelAnyAttributes * pSourceAnyAttributes)
	{
		if (pSourceAnyAttributes == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceAnyAttributes->getAttributeCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			std::string sName;
			std::string sValue;
			PModelAnyAttribute pAnyAttribute = pSourceAnyAttributes->getAttribute(nIndex);
			addAttribute(pAnyAttribute->getNameSpace(), pAnyAttribute->getName(), pAnyAttribute->getValue());
		}
	}
}
