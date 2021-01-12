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

NMR_ModelMetaData.cpp implements the Model MetaData Class.
A metadata item is an in memory representation of the 3MF
metadata, and can be attached to any 3MF model node.

--*/

#include "Model/Classes/NMR_ModelMetaData.h" 
#include "Model/Classes/NMR_ModelConstants.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {

	CModelMetaData::CModelMetaData(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue,
		_In_ std::string sType, _In_ nfBool bPreserve)
	{
		if (!isValidNamespaceAndName(sNameSpace, sName))
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);
		
		// TODO: this check SHOULD be more clever
		// https://www.ibm.com/support/knowledgecenter/en/SS7J8H/com.ibm.odm.dserver.rules.designer.author/designing_bom_topics/tpc_bom_builtin_simple_types.html
		if (sType.empty()) 
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);

		m_sNameSpace = sNameSpace;
		m_sName = sName;
		m_sValue = sValue;
		m_sType = sType;
		m_bPreserve = bPreserve;
	}

	std::string CModelMetaData::getName()
	{
		return m_sName;
	}

	std::string CModelMetaData::getNameSpace()
	{
		return m_sNameSpace;
	}

	std::string CModelMetaData::getKey()
	{
		return composeNamespaceAndNameIntoKey(m_sNameSpace, m_sName);
	}

	std::string CModelMetaData::getValue()
	{
		return m_sValue;
	}

	std::string CModelMetaData::getType()
	{
		return m_sType;
	}

	nfBool CModelMetaData::getPreserve() {
		return m_bPreserve;
	}


	void CModelMetaData::setName(std::string sName)
	{
		if (!isValidNamespaceAndName(m_sNameSpace, sName)) {
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);
		}
		m_sName = sName;
	}

	void CModelMetaData::setNameSpace(std::string sNameSpace)
	{
		if (!isValidNamespaceAndName(sNameSpace, m_sName)) {
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);
		}
		m_sNameSpace = sNameSpace;
	}

	void CModelMetaData::setValue(std::string sValue)
	{
		m_sValue = sValue;
	}

	void CModelMetaData::setType(std::string sType)
	{
		if (sType.empty())
			throw CNMRException(NMR_ERROR_INVALIDMETADATA);
		m_sType = sType;
	}

	void CModelMetaData::setPreserve(nfBool bPreserve)
	{
		m_bPreserve = bPreserve;
	}

	bool CModelMetaData::isValidNamespaceAndName(std::string sNameSpace, std::string sName)
	{
		if (sNameSpace.empty()) {
			return ((sName == XML_3MF_METADATA_VALUE_1) ||
				(sName == XML_3MF_METADATA_VALUE_2) ||
				(sName == XML_3MF_METADATA_VALUE_3) ||
				(sName == XML_3MF_METADATA_VALUE_4) ||
				(sName == XML_3MF_METADATA_VALUE_5) ||
				(sName == XML_3MF_METADATA_VALUE_6) ||
				(sName == XML_3MF_METADATA_VALUE_7) ||
				(sName == XML_3MF_METADATA_VALUE_8) ||
				(sName == XML_3MF_METADATA_VALUE_9));
		}
		else {
			return !sName.empty();
		}
	}

}
