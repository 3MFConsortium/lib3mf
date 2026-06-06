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
#include "Model/Classes/NMR_ModelConstants.h"
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



	CModelToolpathProfileModifier::CModelToolpathProfileModifier(PModelToolpathProfileValue pValue, Lib3MF::eToolpathProfileModificationType modificationType, double dMinimumValue, double dMaximumValue, Lib3MF::eToolpathProfileModificationFactor modificationFactor)
		: m_pValue (pValue), m_dMinimumValue (dMinimumValue), m_dMaximumValue(dMaximumValue), m_ModificationFactor(modificationFactor), m_eModificationType (modificationType)
	{
		if (pValue.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		
	}

	CModelToolpathProfileModifier::~CModelToolpathProfileModifier()
	{

	}

	PModelToolpathProfileValue CModelToolpathProfileModifier::getBaseValue()
	{
		return m_pValue;
	}

	double CModelToolpathProfileModifier::evaluate(double dFactorF, double dFactorG, double dFactorH)
	{
		double dBaseValue = m_pValue->getBaseDoubleValue();

		switch (m_ModificationFactor) {
			case Lib3MF::eToolpathProfileModificationFactor::FactorF:
				if (dFactorF <= 0.0)
					dFactorF = 0.0;
				if (dFactorF >= 1.0)
					dFactorF = 1.0;
				return (m_dMinimumValue * (1.0 - dFactorF)) + m_dMaximumValue * dFactorF;
			
			case Lib3MF::eToolpathProfileModificationFactor::FactorG:
				if (dFactorG <= 0.0)
					dFactorG = 0.0;
				if (dFactorG >= 1.0)
					dFactorG = 1.0;
				return (m_dMinimumValue * (1.0 - dFactorG)) + m_dMaximumValue * dFactorG;

			case Lib3MF::eToolpathProfileModificationFactor::FactorH:
				if (dFactorH <= 0.0)
					dFactorH = 0.0;
				if (dFactorH >= 1.0)
					dFactorH = 1.0;
				return (m_dMinimumValue * (1.0 - dFactorH)) + m_dMaximumValue * dFactorH;

			default:
				return dBaseValue;
		}
	}

	std::string CModelToolpathProfileModifier::getName()
	{
		return m_pValue->getValueName();
	}

	std::string CModelToolpathProfileModifier::getNameSpace()
	{
		return m_pValue->getNameSpace();
	}

	double CModelToolpathProfileModifier::getMinimumValue()
	{
		return m_dMinimumValue;
	}

	double CModelToolpathProfileModifier::getMaximumValue()
	{
		return m_dMaximumValue;
	}

	Lib3MF::eToolpathProfileModificationType CModelToolpathProfileModifier::getModificationType()
	{
		return m_eModificationType;
	}

	std::string CModelToolpathProfileModifier::getModificationTypeString()
	{
		switch (m_eModificationType) {
			case Lib3MF::eToolpathProfileModificationType::ConstantModification:
				return XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_TYPE_CONSTANT;
			case Lib3MF::eToolpathProfileModificationType::LinearModification:
				return XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_TYPE_LINEAR;
			case Lib3MF::eToolpathProfileModificationType::NonlinearModification:
				return XML_3MF_ATTRIBUTE_TOOLPATHMODIFIER_TYPE_NONLINEAR;

			default:
				throw CNMRException(NMR_ERROR_TOOLPATH_INVALIDPROFILEMODIFIERTYPE);
		}
	}

	Lib3MF::eToolpathProfileModificationFactor CModelToolpathProfileModifier::getModificationFactor()
	{
		return m_ModificationFactor;
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

	uint32_t CModelToolpathProfile::getModifierCount()
	{
		return (uint32_t)m_ModifierList.size();
	}

	std::string CModelToolpathProfile::getModifierName(const uint32_t nIndex)
	{
		if (nIndex >= m_ModifierList.size())
			throw CNMRException(NMR_ERROR_INVALIDMODIFIERINDEX);

		return m_ModifierList.at(nIndex)->getName();
	}

	Lib3MF::eToolpathProfileModificationType CModelToolpathProfile::getModifierType(const uint32_t nIndex)
	{
		if (nIndex >= m_ModifierList.size())
			throw CNMRException(NMR_ERROR_INVALIDMODIFIERINDEX);

		return m_ModifierList.at(nIndex)->getModificationType();
	}


	std::string CModelToolpathProfile::getModifierNameSpace(const uint32_t nIndex)
	{
		if (nIndex >= m_ModifierList.size())
			throw CNMRException(NMR_ERROR_INVALIDMODIFIERINDEX);

		return m_ModifierList.at(nIndex)->getNameSpace();

	}

	PModelToolpathProfileModifier CModelToolpathProfile::getModifier(const uint32_t nIndex)
	{
		if (nIndex >= m_ModifierList.size())
			throw CNMRException(NMR_ERROR_INVALIDMODIFIERINDEX);

		return m_ModifierList.at(nIndex);
	}

	void CModelToolpathProfile::removeModifier(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto key = std::make_pair(sNameSpace, sValueName);

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end()) {
			auto pOldModifier = iModifierIter->second;
			m_ModifierMap.erase(iModifierIter);
			m_ModifierList.erase(std::remove(m_ModifierList.begin(), m_ModifierList.end(), pOldModifier));
		}

	}

	PModelToolpathProfileModifier CModelToolpathProfile::findModifier(const std::string& sNameSpace, const std::string& sValueName, bool bMustExist)
	{
		auto key = std::make_pair(sNameSpace, sValueName);

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter == m_ModifierMap.end()) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_PROFILEMODIFIERNOTFOUND);

			return nullptr;
		}

		return iModifierIter->second;

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

	void CModelToolpathProfile::addValue(const std::string& sNameSpace, const std::string& sValueName, const std::string& sValue, bool bFailIfValueExists)
	{
		if (m_ValueList.size () >= MODELTOOLPATH_MAXCOUNT)
			throw CNMRException(NMR_ERROR_TOOMANYPROFILEVALUES);

		auto key = std::make_pair(sNameSpace, sValueName);
		auto iIter = m_ValueMap.find(key);
		if (iIter != m_ValueMap.end()) {
			if (bFailIfValueExists)
				throw CNMRException(NMR_ERROR_DUPLICATEPROFILEVALUE);

			auto pOldValue = iIter->second;
			m_ValueMap.erase(iIter);
			m_ValueList.erase(std::remove(m_ValueList.begin(), m_ValueList.end(), pOldValue));
		}

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end()) {
			auto pOldModifier = iModifierIter->second;
			m_ModifierMap.erase(iModifierIter);
			m_ModifierList.erase(std::remove(m_ModifierList.begin(), m_ModifierList.end(), pOldModifier));
		}

		auto pValue = std::make_shared<CModelToolpathProfileValue>(sNameSpace, sValueName, sValue);
		m_ValueMap.insert(std::make_pair (key, pValue));
		m_ValueList.push_back (pValue);
	}

	void CModelToolpathProfile::removeParameter(const std::string& sNameSpace, const std::string& sValueName)
	{
		auto key = std::make_pair(sNameSpace, sValueName);

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end()) {
			auto pOldModifier = iModifierIter->second;
			m_ModifierMap.erase(iModifierIter);
			m_ModifierList.erase(std::remove(m_ModifierList.begin(), m_ModifierList.end(), pOldModifier));
		}

		auto iIter = m_ValueMap.find(key);
		if (iIter != m_ValueMap.end()) {
			auto pOldValue = iIter->second;
			m_ValueMap.erase(iIter);
			m_ValueList.erase(std::remove(m_ValueList.begin(), m_ValueList.end(), pOldValue));
		}

	}

	PModelToolpathProfileValue CModelToolpathProfile::findParameter(const std::string& sNameSpace, const std::string& sValueName, bool bMustExist)
	{
		auto key = std::make_pair(sNameSpace, sValueName);
		auto iIter = m_ValueMap.find(key);
		if (iIter == m_ValueMap.end()) {
			if (bMustExist)
				throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

			return nullptr;
		}

		return iIter->second;
	}

	void CModelToolpathProfile::changeModifier(const std::string& sNameSpace, const std::string& sValueName, Lib3MF::eToolpathProfileModificationType modifierType, double dMinimum, double dMaximum, Lib3MF::eToolpathProfileModificationFactor modificationFactor)
	{
		removeModifier(sNameSpace, sValueName);
		addModifier(sNameSpace, sValueName, modifierType, dMinimum, dMaximum, modificationFactor);
	}

	void CModelToolpathProfile::addModifier(const std::string& sNameSpace, const std::string& sValueName, Lib3MF::eToolpathProfileModificationType modifierType, double dMinimum, double dMaximum, Lib3MF::eToolpathProfileModificationFactor modificationFactor)
	{
		auto key = std::make_pair(sNameSpace, sValueName);
		auto iValueIter = m_ValueMap.find(key);
		if (iValueIter == m_ValueMap.end())
			throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);

		auto pValue = iValueIter->second;

		auto iModifierIter = m_ModifierMap.find(key);
		if (iModifierIter != m_ModifierMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATEPROFILEMODIFIER);

		auto pModifier = std::make_shared<CModelToolpathProfileModifier>(pValue, modifierType, dMinimum, dMaximum, modificationFactor);

		m_ModifierMap.insert (std::make_pair (key, pModifier));
		m_ModifierList.push_back (pModifier);

	}


	std::vector<PModelToolpathProfileValue>& CModelToolpathProfile::getValues()
	{
		return m_ValueList;
	}

	std::vector<PModelToolpathProfileModifier>& CModelToolpathProfile::getModifiers()
	{
		return m_ModifierList;
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
