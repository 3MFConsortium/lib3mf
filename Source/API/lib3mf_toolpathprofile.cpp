/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CToolpathProfile

*/

#include "lib3mf_toolpathprofile.hpp"
#include "lib3mf_interfaceexception.hpp"
// Include custom headers here.
#include "Common/NMR_StringUtils.h"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CToolpathProfile
**************************************************************************************************************************/


CToolpathProfile::CToolpathProfile(NMR::PModelToolpathProfile pToolpathProfile)
	: m_pToolpathProfile(pToolpathProfile)
{
	if (pToolpathProfile.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}


CToolpathProfile::~CToolpathProfile()
{

}

std::string CToolpathProfile::GetUUID()
{
	return m_pToolpathProfile->getUUID();
}

std::string CToolpathProfile::GetName()
{
	return m_pToolpathProfile->getName();
}

Lib3MF_uint32 CToolpathProfile::GetParameterCount()
{
	return m_pToolpathProfile->getParameterCount();
}

std::string CToolpathProfile::GetParameterName(const Lib3MF_uint32 nIndex)
{
	return m_pToolpathProfile->getParameterName(nIndex);

}

std::string CToolpathProfile::GetParameterNameSpace(const Lib3MF_uint32 nIndex)
{
	return m_pToolpathProfile->getParameterNameSpace(nIndex);
}


void CToolpathProfile::SetName(const std::string& sName)
{
	m_pToolpathProfile->setName(sName);
}


NMR::PModelToolpathProfile CToolpathProfile::getProfileInstance()
{
	return m_pToolpathProfile;
}

bool CToolpathProfile::HasParameterValue(const std::string& sNameSpaceName, const std::string& sValueName)
{
	return m_pToolpathProfile->hasValue(sNameSpaceName, sValueName);
}


std::string CToolpathProfile::GetParameterValue(const std::string& sNameSpaceName, const std::string& sValueName)
{
	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return sValue;
}

std::string CToolpathProfile::GetParameterValueDef(const std::string& sNameSpaceName, const std::string& sValueName, const std::string& sDefaultValue)
{
	if (!m_pToolpathProfile->hasValue(sNameSpaceName, sValueName))
		return sDefaultValue;

	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return sValue;
}

Lib3MF_double CToolpathProfile::GetParameterDoubleValue(const std::string& sNameSpaceName, const std::string& sValueName)
{
	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return std::stod(sValue);
}

Lib3MF_double CToolpathProfile::GetParameterDoubleValueDef(const std::string& sNameSpaceName, const std::string& sValueName, const Lib3MF_double dDefaultValue)
{
	if (!m_pToolpathProfile->hasValue(sNameSpaceName, sValueName))
		return dDefaultValue;

	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return std::stod(sValue);
}


Lib3MF_int64 CToolpathProfile::GetParameterIntegerValue(const std::string& sNameSpaceName, const std::string& sValueName)
{
	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return std::stoll(sValue);
}

Lib3MF_int64 CToolpathProfile::GetParameterIntegerValueDef(const std::string& sNameSpaceName, const std::string& sValueName, const Lib3MF_int64 nDefaultValue)
{
	if (!m_pToolpathProfile->hasValue(sNameSpaceName, sValueName))
		return nDefaultValue;

	std::string sValue = m_pToolpathProfile->getValue(sNameSpaceName, sValueName);
	return std::stoll(sValue);

}

bool CToolpathProfile::GetParameterBoolValue(const std::string& sNameSpaceName, const std::string& sValueName)
{
	std::string sValue = NMR::fnTrimString (NMR::fnStringToLower (m_pToolpathProfile->getValue(sNameSpaceName, sValueName)));
	if (sValue == "true")
		return true;

	if (sValue == "false")
		return true;

	return (std::stoll(sValue) != 0);
}

bool CToolpathProfile::GetParameterBoolValueDef(const std::string& sNameSpaceName, const std::string& sValueName, const bool bDefaultValue)
{
	if (!m_pToolpathProfile->hasValue(sNameSpaceName, sValueName))
		return bDefaultValue;

	std::string sValue = NMR::fnTrimString(NMR::fnStringToLower(m_pToolpathProfile->getValue(sNameSpaceName, sValueName)));
	if (sValue == "true")
		return true;

	if (sValue == "false")
		return true;

	return (std::stoll(sValue) != 0);
}

void CToolpathProfile::SetParameterValue(const std::string& sNameSpaceName, const std::string& sValueName, const std::string& sValue)
{
	m_pToolpathProfile->addValue(sNameSpaceName, sValueName, sValue, true);
}


void CToolpathProfile::SetParameterDoubleValue(const std::string& sNameSpaceName, const std::string& sValueName, const Lib3MF_double dValue)
{
	m_pToolpathProfile->addValue(sNameSpaceName, sValueName, std::to_string(dValue), true);
}


void CToolpathProfile::SetParameterIntegerValue(const std::string& sNameSpaceName, const std::string& sValueName, const Lib3MF_int64 nValue)
{
	m_pToolpathProfile->addValue(sNameSpaceName, sValueName, std::to_string(nValue), true);

}

void CToolpathProfile::SetParameterBoolValue(const std::string& sNameSpaceName, const std::string& sValueName, const bool bValue)
{
	if (bValue)
		m_pToolpathProfile->addValue(sNameSpaceName, sValueName, "true", true);
	else
		m_pToolpathProfile->addValue(sNameSpaceName, sValueName, "false", true);
}

void CToolpathProfile::RemoveParameter(const std::string& sNameSpaceName, const std::string& sValueName)
{
	m_pToolpathProfile->removeParameter(sNameSpaceName, sNameSpaceName);
}

Lib3MF_uint32 CToolpathProfile::GetModifierCount()
{
	return m_pToolpathProfile->getModifierCount();
}

std::string CToolpathProfile::GetModifierNameByIndex(const Lib3MF_uint32 nIndex)
{
	return m_pToolpathProfile->getModifierName(nIndex);
}

std::string CToolpathProfile::GetModifierNameSpaceByIndex(const Lib3MF_uint32 nIndex) 
{
	return m_pToolpathProfile->getModifierNameSpace(nIndex);
}


bool CToolpathProfile::HasModifier(const std::string& sNameSpaceName, const std::string& sValueName)
{
	auto pModifier = m_pToolpathProfile->findModifier(sNameSpaceName, sValueName, false);
	return pModifier.get() != nullptr;
}

void CToolpathProfile::GetModifierInformationByIndex(const Lib3MF_uint32 nIndex, std::string& sNameSpaceName, std::string& sValueName, Lib3MF::eToolpathProfileOverrideFactor& eOverrideFactor, Lib3MF_double& dDeltaValue)
{
	auto pModifier = m_pToolpathProfile->getModifier(nIndex);
	sNameSpaceName = pModifier->getNameSpace();
	sValueName = pModifier->getName();

	switch (pModifier->getOverrideFactor())
	{
		case NMR::eModelToolpathProfileOverrideFactor::pfFactorF:
			eOverrideFactor = eToolpathProfileOverrideFactor::FactorF;
			break;
		case NMR::eModelToolpathProfileOverrideFactor::pfFactorG:
			eOverrideFactor = eToolpathProfileOverrideFactor::FactorG;
			break;
		case NMR::eModelToolpathProfileOverrideFactor::pfFactorH:
			eOverrideFactor = eToolpathProfileOverrideFactor::FactorH;
			break;
		default:
			eOverrideFactor = eToolpathProfileOverrideFactor::Unknown;
	}

	dDeltaValue = pModifier->getDeltaValue();


}

void CToolpathProfile::GetModifierInformationByName(const std::string& sNameSpaceName, const std::string& sValueName, Lib3MF::eToolpathProfileOverrideFactor& eOverrideFactor, Lib3MF_double& dDeltaValue)
{
	auto pModifier = m_pToolpathProfile->findModifier(sNameSpaceName, sValueName, true);

	switch (pModifier->getOverrideFactor())
	{
	case NMR::eModelToolpathProfileOverrideFactor::pfFactorF:
		eOverrideFactor = eToolpathProfileOverrideFactor::FactorF;
		break;
	case NMR::eModelToolpathProfileOverrideFactor::pfFactorG:
		eOverrideFactor = eToolpathProfileOverrideFactor::FactorG;
		break;
	case NMR::eModelToolpathProfileOverrideFactor::pfFactorH:
		eOverrideFactor = eToolpathProfileOverrideFactor::FactorH;
		break;
	default:
		eOverrideFactor = eToolpathProfileOverrideFactor::Unknown;
	}

	dDeltaValue = pModifier->getDeltaValue();

}

void CToolpathProfile::SetModifier(const std::string& sNameSpaceName, const std::string& sValueName, Lib3MF::eToolpathProfileOverrideFactor eOverrideFactor, Lib3MF_double dDeltaValue) 
{
	NMR::eModelToolpathProfileOverrideFactor internalOverrideFactor;
	switch (eOverrideFactor) {
		case eToolpathProfileOverrideFactor::FactorF:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorF;
			break;
		case eToolpathProfileOverrideFactor::FactorG:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorG;
			break;
		case eToolpathProfileOverrideFactor::FactorH:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfFactorH;
			break;
		default:
			internalOverrideFactor = NMR::eModelToolpathProfileOverrideFactor::pfNone;

	}

	m_pToolpathProfile->addModifier(sNameSpaceName, sValueName, dDeltaValue, internalOverrideFactor);
}

void CToolpathProfile::RemoveModifier(const std::string& sNameSpaceName, const std::string& sValueName)
{
	m_pToolpathProfile->removeModifier(sNameSpaceName, sValueName);
}

Lib3MF_double CToolpathProfile::EvaluateDoubleValue(const std::string& sNameSpaceName, const std::string& sValueName, const Lib3MF_double dFactorF, const Lib3MF_double dFactorG, const Lib3MF_double dFactorH)
{
	return m_pToolpathProfile->evaluate(sNameSpaceName, sValueName, dFactorF, dFactorG, dFactorH);
}

 