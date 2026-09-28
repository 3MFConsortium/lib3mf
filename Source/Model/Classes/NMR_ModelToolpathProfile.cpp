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
#include <cmath>

namespace NMR {

	namespace {

		enum class eProfileValueType { Number, PositiveNumber, NonNegativeNumber, NonNegativeInteger };

		bool findStandardValueType(const std::string & sValueName, eProfileValueType & eType)
		{
			static const std::map<std::string, eProfileValueType> standardValueTypes = {
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERPOWER, eProfileValueType::NonNegativeNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERSPEED, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_JUMPSPEED, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERFOCUS, eProfileValueType::Number },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_SPOTRADIUS, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_LASERINDEX, eProfileValueType::NonNegativeInteger },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_DEPOSITIONSPEED, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_BEADWIDTH, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_BEADHEIGHT, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_PREWAITTIME, eProfileValueType::PositiveNumber },
				{ XML_3MF_ATTRIBUTE_TOOLPATHPROFILE_POSTWAITTIME, eProfileValueType::PositiveNumber },
			};

			auto iIter = standardValueTypes.find(sValueName);
			if (iIter == standardValueTypes.end())
				return false;

			eType = iIter->second;
			return true;
		}

		bool isDigit(char c)
		{
			return (c >= '0') && (c <= '9');
		}

		// Lexical space of ST_Number: [-+]?((([0-9]+(\.[0-9]*)?)|(\.[0-9]+))([eE][-+]?[0-9]+)?)
		bool matchesNumberPattern(const std::string & sValue)
		{
			size_t nIndex = 0;
			size_t nLength = sValue.length();

			if ((nIndex < nLength) && ((sValue[nIndex] == '+') || (sValue[nIndex] == '-')))
				nIndex++;

			size_t nMantissaDigits = 0;
			while ((nIndex < nLength) && isDigit(sValue[nIndex])) {
				nIndex++;
				nMantissaDigits++;
			}
			if ((nIndex < nLength) && (sValue[nIndex] == '.')) {
				nIndex++;
				while ((nIndex < nLength) && isDigit(sValue[nIndex])) {
					nIndex++;
					nMantissaDigits++;
				}
			}
			if (nMantissaDigits == 0)
				return false;

			if ((nIndex < nLength) && ((sValue[nIndex] == 'e') || (sValue[nIndex] == 'E'))) {
				nIndex++;
				if ((nIndex < nLength) && ((sValue[nIndex] == '+') || (sValue[nIndex] == '-')))
					nIndex++;
				size_t nExponentDigits = 0;
				while ((nIndex < nLength) && isDigit(sValue[nIndex])) {
					nIndex++;
					nExponentDigits++;
				}
				if (nExponentDigits == 0)
					return false;
			}

			return nIndex == nLength;
		}

		// ST_NonNegativeInteger: xs:nonNegativeInteger below 2^31.
		bool isValidNonNegativeInteger(const std::string & sValue)
		{
			size_t nIndex = 0;
			if ((nIndex < sValue.length()) && (sValue[nIndex] == '+'))
				nIndex++;
			if (nIndex == sValue.length())
				return false;

			nfUint64 nValue = 0;
			for (; nIndex < sValue.length(); nIndex++) {
				if (!isDigit(sValue[nIndex]))
					return false;
				nValue = nValue * 10 + (nfUint64)(sValue[nIndex] - '0');
				if (nValue > 2147483647ULL)
					return false;
			}
			return true;
		}

	}

	bool CModelToolpathProfile::isValidStandardValue(const std::string & sValueName, const std::string & sValue)
	{
		eProfileValueType eType;
		if (!findStandardValueType(sValueName, eType))
			return true;

		// Schema number types collapse surrounding whitespace.
		size_t nFirst = sValue.find_first_not_of(" \t\r\n");
		if (nFirst == std::string::npos)
			return false;
		size_t nLast = sValue.find_last_not_of(" \t\r\n");
		std::string sTrimmed = sValue.substr(nFirst, nLast - nFirst + 1);

		if (eType == eProfileValueType::NonNegativeInteger)
			return isValidNonNegativeInteger(sTrimmed);

		if (!matchesNumberPattern(sTrimmed))
			return false;

		double dValue;
		try {
			dValue = fnStringToDouble(sTrimmed.c_str());
		}
		catch (CNMRException &) {
			return false;
		}
		if (!std::isfinite(dValue))
			return false;

		switch (eType) {
			case eProfileValueType::PositiveNumber: return dValue > 0.0;
			case eProfileValueType::NonNegativeNumber: return dValue >= 0.0;
			default: return true;
		}
	}


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



	bool CModelToolpathProfileNameRegistry::hasName(const std::string & sName)
	{
		return m_Names.find(sName) != m_Names.end();
	}

	void CModelToolpathProfileNameRegistry::registerName(const std::string & sName, bool bAllowDuplicate)
	{
		auto iIter = m_Names.find(sName);
		if (iIter == m_Names.end()) {
			m_Names.insert(std::make_pair(sName, 1));
			return;
		}

		if (!bAllowDuplicate)
			throw CNMRException(NMR_ERROR_DUPLICATETOOLPATHPROFILENAME);

		iIter->second++;
	}

	void CModelToolpathProfileNameRegistry::unregisterName(const std::string & sName)
	{
		auto iIter = m_Names.find(sName);
		if (iIter == m_Names.end())
			return;

		if (iIter->second > 1)
			iIter->second--;
		else
			m_Names.erase(iIter);
	}


	CModelToolpathProfile::CModelToolpathProfile(std::string sUUID, std::string sName, PModelToolpathProfileNameRegistry pNameRegistry, bool bAllowDuplicateName)
		: m_sUUID (sUUID), m_sName (sName), m_pNameRegistry (pNameRegistry)
	{
		if (pNameRegistry.get() == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pNameRegistry->registerName(m_sName, bAllowDuplicateName);
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
		if (sName == m_sName)
			return;

		m_pNameRegistry->registerName(sName, false);
		m_pNameRegistry->unregisterName(m_sName);
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

	void CModelToolpathProfile::checkModifierFactor(const std::pair<std::string, std::string> & key, Lib3MF::eToolpathProfileModificationFactor modificationFactor)
	{
		switch (modificationFactor) {
			case Lib3MF::eToolpathProfileModificationFactor::FactorE:
			case Lib3MF::eToolpathProfileModificationFactor::FactorF:
			case Lib3MF::eToolpathProfileModificationFactor::FactorG:
			case Lib3MF::eToolpathProfileModificationFactor::FactorH:
				break;
			default:
				throw CNMRException(NMR_ERROR_MISSINGPROFILEMODIFIERFACTOR);
		}

		// With four distinct factors available, this also limits a profile to four modifiers.
		for (auto & modifierEntry : m_ModifierMap) {
			if ((modifierEntry.first != key) && (modifierEntry.second->getModificationFactor() == modificationFactor))
				throw CNMRException(NMR_ERROR_PROFILEMODIFIERFACTORINUSE);
		}
	}

	void CModelToolpathProfile::changeModifier(const std::string& sNameSpace, const std::string& sValueName, Lib3MF::eToolpathProfileModificationType modifierType, double dMinimum, double dMaximum, Lib3MF::eToolpathProfileModificationFactor modificationFactor)
	{
		auto key = std::make_pair(sNameSpace, sValueName);
		if (m_ValueMap.find(key) == m_ValueMap.end())
			throw CNMRException(NMR_ERROR_PROFILEVALUENOTFOUND);
		checkModifierFactor(key, modificationFactor);

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

		checkModifierFactor(key, modificationFactor);

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
