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

NMR_ModelToolpathProfile.cpp defines the Model Toolpath Profile.

--*/

#include "Model/Classes/NMR_ModelToolpathProfile.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_StringUtils.h"
#include <sstream>
#include <algorithm>

namespace NMR {

	CModelToolpathProfile::CModelToolpathProfile(std::string sUUID, std::string sName)
		: m_sUUID (sUUID), m_sName (sName)
	{
		
	}

	std::string CModelToolpathProfile::getUUID()
	{
		return m_sUUID;
	}

	std::string CModelToolpathProfile::getName()
	{
		return m_sName;
	}

	void CModelToolpathProfile::setName(const std::string & sName)
	{
		m_sName = sName;
	}

	uint32_t CModelToolpathProfile::getParameterCount()
	{
		return (uint32_t)m_Parameters.size();
	}

	std::string CModelToolpathProfile::getParameterName(const uint32_t nIndex)
	{
		if (nIndex >= m_Parameters.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAMETERINDEX);

		return m_Parameters.at(nIndex).second;
	}

	std::string CModelToolpathProfile::getParameterNameSpace(const uint32_t nIndex)
	{
		if (nIndex >= m_Parameters.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAMETERINDEX);

		return m_Parameters.at(nIndex).first;
	}

	bool CModelToolpathProfile::hasValue(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto iter = m_Values.find(std::make_pair (sNameSpace, sValueName));
		return iter != m_Values.end();
	}

	std::string CModelToolpathProfile::getValue(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto iter = m_Values.find(std::make_pair(sNameSpace, sValueName));
		if (iter == m_Values.end())
			throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

		return iter->second;
	}

	void CModelToolpathProfile::addValue(const std::string& sNameSpace, const std::string& sValueName, const std::string& sValue)
	{
		if (m_Values.size () >= MODELTOOLPATH_MAXCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYPROFILEVALUES);
		m_Values.insert(std::make_pair (std::make_pair (sNameSpace, sValueName), sValue));
		m_Parameters.push_back(std::make_pair(sNameSpace, sValueName));
	}

	std::list<sModelToolpathProfileValue> CModelToolpathProfile::listValues()
	{
		std::list<sModelToolpathProfileValue> profileValueList;
		for (auto profileValue : m_Values) {
			profileValueList.push_back(sModelToolpathProfileValue { profileValue.first.first, profileValue.first.second, profileValue.second });
		}

		return profileValueList;

	}


}
