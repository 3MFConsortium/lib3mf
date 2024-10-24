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


	CModelToolpathProfileValue::CModelToolpathProfileValue(const std::string& sNameSpace, const std::string& sValueName, const std::string& sValue)
		: m_sNameSpace (sNameSpace),
		m_sValueName (sValueName), 
		m_sValue (sValue)
	{

	}

	CModelToolpathProfileValue::~CModelToolpathProfileValue()
	{
								
	}

	bool CModelToolpathProfileValue::hasNameSpace()
	{
		return !m_sNameSpace.empty();
	}

	std::string CModelToolpathProfileValue::getNameSpace()
	{
		return m_sNameSpace;
	}

	std::string CModelToolpathProfileValue::getValueName()
	{
		return m_sValueName;
	}

	std::string CModelToolpathProfileValue::getValue()
	{
		return m_sValue;
	}

	double CModelToolpathProfileValue::getBaseDoubleValue()
	{
		return fnStringToDouble(m_sValue.c_str());
	}



	CModelToolpathProfileModifier::CModelToolpathProfileModifier(PModelToolpathProfileValue pValue, double dDeltaValue, eModelToolpathProfileOverrideFactor overrideFactor)
		: m_pValue (pValue), m_dDeltaValue (dDeltaValue), m_OverrideFactor (overrideFactor)
	{
		if (pValue.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_dBaseValue = m_pValue->getBaseDoubleValue();
		
	}

	CModelToolpathProfileModifier::~CModelToolpathProfileModifier()
	{

	}

	double CModelToolpathProfileModifier::getBaseValue()
	{
		return m_dBaseValue;
	}

	double CModelToolpathProfileModifier::evaluate(double dFactorF, double dFactorG, double dFactorH)
	{
		switch (m_OverrideFactor) {
			case eModelToolpathProfileOverrideFactor::pfFactorF:
				return m_dBaseValue + m_dDeltaValue * dFactorF;
			case eModelToolpathProfileOverrideFactor::pfFactorG:
				return m_dBaseValue + m_dDeltaValue * dFactorG;
			case eModelToolpathProfileOverrideFactor::pfFactorH:
				return m_dBaseValue + m_dDeltaValue * dFactorH;

			default:
				return m_dBaseValue;
		}
	}


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
		return (uint32_t)m_ValueList.size();
	}

	std::string CModelToolpathProfile::getParameterName(const uint32_t nIndex)
	{
		if (nIndex >= m_ValueList.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAMETERINDEX);

		return m_ValueList.at(nIndex)->getValueName ();
	}

	std::string CModelToolpathProfile::getParameterNameSpace(const uint32_t nIndex)
	{
		if (nIndex >= m_ValueList.size())
			throw CNMRException(NMR_ERROR_INVALIDPARAMETERINDEX);

		return m_ValueList.at(nIndex)->getNameSpace ();
	}

	bool CModelToolpathProfile::hasValue(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto iter = m_ValueMap.find(std::make_pair (sNameSpace, sValueName));
		return iter != m_ValueMap.end();
	}

	std::string CModelToolpathProfile::getValue(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto iter = m_ValueMap.find(std::make_pair(sNameSpace, sValueName));
		if (iter == m_ValueMap.end())
			throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

		return iter->second->getValue ();
	}

	void CModelToolpathProfile::addValue(const std::string& sNameSpace, const std::string& sValueName, const std::string& sValue)
	{
		if (m_ValueList.size () >= MODELTOOLPATH_MAXCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYPROFILEVALUES);

		auto key = std::make_pair(sNameSpace, sValueName);
		auto iIter = m_ValueMap.find(key);
		if (iIter != m_ValueMap.end ())
			throw CNMRException(NMR_ERROR_DUPLICATEPROFILEVALUE);

		auto pValue = std::make_shared<CModelToolpathProfileValue>(sNameSpace, sValueName, sValue);
		m_ValueMap.insert(std::make_pair (key, pValue));
		m_ValueList.push_back (pValue);
	}

	void CModelToolpathProfile::addModifier(const std::string& sNameSpace, const std::string& sValueName, double dDelta, eModelToolpathProfileOverrideFactor overrideFactor)
	{
		auto key = std::make_pair(sNameSpace, sValueName);
		auto iValueIter = m_ValueMap.find(key);
		if (iValueIter == m_ValueMap.end())
			throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

		auto pValue = iValueIter->second;

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEPROFILEMODIFIER);

		auto pModifier = std::make_shared<CModelToolpathProfileModifier>(pValue, dDelta, overrideFactor);

		m_ModifierMap.insert (std::make_pair (key, pModifier));

	}


	std::vector<PModelToolpathProfileValue>& CModelToolpathProfile::getValues()
	{
		return m_ValueList;
	}

	double CModelToolpathProfile::evaluate(const std::string& sNameSpace, const std::string& sValueName, double dFactorF, double dFactorG, double dFactorH)
	{

		auto key = std::make_pair(sNameSpace, sValueName);
		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end()) {
			return iModifierIter->second->evaluate (dFactorF, dFactorG, dFactorH);

		}
		else {
			auto iValueIter = m_ValueMap.find(key);
			if (iValueIter == m_ValueMap.end())
				throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

			return iValueIter->second->getBaseDoubleValue();
		}


	}

}