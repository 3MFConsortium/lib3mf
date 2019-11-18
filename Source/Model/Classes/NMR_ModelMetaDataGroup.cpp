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

NMR_ModelMetaDataGroup.cpp implements the Model MetaData Group Class.

--*/

#include "Model/Classes/NMR_ModelMetaDataGroup.h"
#include "Common/NMR_Exception.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelMetaDataGroup::CModelMetaDataGroup()
	{
	}

	void CModelMetaDataGroup::clear()
	{
		m_MetaData.clear();
		m_MetaDataMap.clear();
	}


	PModelMetaData CModelMetaDataGroup::addMetaData(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue, _In_ std::string sType, _In_ nfBool bPreserve)
	{
		if (hasMetaData(composeNamespaceAndNameIntoKey(sNameSpace, sName))) {
			throw CNMRException(NMR_ERROR_DUPLICATEMETADATA);
		}
		
		if (!CModelMetaData::isValidNamespaceAndName(sNameSpace, sName)) {
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);
		}

		if (m_MetaData.size() >= XML_3MF_MAXMETADATACOUNT)
			throw CNMRException(NMR_ERROR_INVALIDMETADATACOUNT);

		PModelMetaData pMetaData = std::make_shared<CModelMetaData>(sNameSpace, sName, sValue, sType, bPreserve);
		m_MetaData.push_back(pMetaData);
		m_MetaDataMap.insert(std::make_pair(pMetaData->getKey(), pMetaData));
		return pMetaData;
	}

	nfUint32 CModelMetaDataGroup::getMetaDataCount()
	{
		return (nfUint32)m_MetaData.size();
	}

	PModelMetaData CModelMetaDataGroup::getMetaData(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getMetaDataCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		PModelMetaData pMetaData = m_MetaData[nIndex];
		__NMRASSERT(pMetaData.get() != nullptr);

		return pMetaData;
	}

	void CModelMetaDataGroup::removeMetaData(_In_ nfUint32 nIndex)
	{
		nfUint32 nCount = getMetaDataCount();
		if (nIndex >= nCount)
			throw CNMRException(NMR_ERROR_INVALIDINDEX);

		auto iIterator = m_MetaData.begin();
		if (nIndex > 0)
			iIterator += nIndex;
		m_MetaData.erase(iIterator);
	}

	nfBool CModelMetaDataGroup::hasMetaData(_In_ std::string sKey)
	{
		std::map<std::string, PModelMetaData>::iterator iIterator = m_MetaDataMap.find(sKey);
		return iIterator != m_MetaDataMap.end();
	}

	void CModelMetaDataGroup::mergeMetaData(_In_ CModelMetaDataGroup * pSourceMetaDataGroup)
	{
		if (pSourceMetaDataGroup == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		nfUint32 nCount = pSourceMetaDataGroup->getMetaDataCount();
		nfUint32 nIndex;

		for (nIndex = 0; nIndex < nCount; nIndex++) {
			std::string sName;
			std::string sValue;
			PModelMetaData metaData = pSourceMetaDataGroup->getMetaData(nIndex);
			addMetaData(metaData->getNameSpace(), metaData->getName(), metaData->getValue(), metaData->getType(), metaData->getPreserve());
		}
	}
}
