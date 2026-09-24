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

NMR_ModelToolpathProfile.h defines the Model Toolpath Profile.

--*/

#ifndef __NMR_MODELTOOLPATHPROFILE
#define __NMR_MODELTOOLPATHPROFILE

#include "Common/NMR_Types.h" 

#define MODELTOOLPATH_MAXCOUNT (1024 * 1024)

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <list>
#include <set>

#include "lib3mf_types.hpp"

namespace NMR {

	class CModelToolpathProfileValue {
	private:
		std::string m_sNameSpace;
		std::string m_sValueName;
		std::string m_sValue;
	public:
		CModelToolpathProfileValue(const std::string & sNameSpace, const std::string & sValueName, const std::string & sValue);

		virtual ~CModelToolpathProfileValue();

		bool hasNameSpace();
		std::string getNameSpace ();
		std::string getValueName ();
		std::string getValue ();

		double getBaseDoubleValue();
	};

	typedef std::shared_ptr<CModelToolpathProfileValue> PModelToolpathProfileValue;

	class CModelToolpathProfileModifier {
	private:
		Lib3MF::eToolpathProfileModificationType m_eModificationType;
		Lib3MF::eToolpathProfileModificationFactor m_ModificationFactor;

		PModelToolpathProfileValue m_pValue;
		double m_dMinimumValue;
		double m_dMaximumValue;

	public:

		CModelToolpathProfileModifier(PModelToolpathProfileValue pValue, Lib3MF::eToolpathProfileModificationType modificationType, double dMinimumValue, double dMaximumValue, Lib3MF::eToolpathProfileModificationFactor modificationFactor);

		virtual ~CModelToolpathProfileModifier();

		PModelToolpathProfileValue getBaseValue();

		double evaluate (double dFactorF, double dFactorG, double dFactorH);

		std::string getName();

		std::string getNameSpace();

		double getMinimumValue();

		double getMaximumValue();

		Lib3MF::eToolpathProfileModificationType getModificationType ();		

		Lib3MF::eToolpathProfileModificationFactor getModificationFactor ();

		std::string getModificationTypeString();

	};

	typedef std::shared_ptr<CModelToolpathProfileModifier> PModelToolpathProfileModifier;

	// Profile names MUST be unique within a toolpath. The registry is shared between a toolpath
	// and its profiles, so that renaming a profile can be checked against its siblings.
	class CModelToolpathProfileNameRegistry {
	private:
		std::set<std::string> m_Names;
	public:
		void registerName(const std::string & sName);
		void unregisterName(const std::string & sName);
	};

	typedef std::shared_ptr<CModelToolpathProfileNameRegistry> PModelToolpathProfileNameRegistry;

	class CModelToolpathProfile {
	private:
		std::string m_sUUID;
		std::string m_sName;
		PModelToolpathProfileNameRegistry m_pNameRegistry;


		std::vector<PModelToolpathProfileValue> m_ValueList;
		std::map<std::pair<std::string, std::string>, PModelToolpathProfileValue> m_ValueMap;

		std::vector<PModelToolpathProfileModifier> m_ModifierList;
		std::map<std::pair<std::string, std::string>, PModelToolpathProfileModifier> m_ModifierMap;

		// Throws if the factor is not e/f/g/h, or if it is used by a modifier on another attribute.
		void checkModifierFactor(const std::pair<std::string, std::string> & key, Lib3MF::eToolpathProfileModificationFactor modificationFactor);

	public:
		CModelToolpathProfile() = delete;
		CModelToolpathProfile(std::string sUUID, std::string sName, PModelToolpathProfileNameRegistry pNameRegistry);

		std::string getUUID();
		std::string getName();
		void setName (const std::string & sName);

		bool hasValue (const std::string & sNameSpace, const std::string & sValueName);
		std::string getValue(const std::string& sNameSpace, const std::string& sValueName);

		// If value does already exist, and failIfValueExists is false, the value and its potential modifier will be deleted first.
		void addValue(const std::string& sNameSpace, const std::string& sValueName, const std::string & sValue, bool bFailIfValueExists);

		uint32_t getParameterCount();
		std::string getParameterName(const uint32_t nIndex);
		std::string getParameterNameSpace(const uint32_t nIndex);
		void removeParameter(const std::string& sNameSpace, const std::string& sValueName);
		PModelToolpathProfileValue findParameter (const std::string& sNameSpace, const std::string& sValueName, bool bMustExist);

		void addModifier(const std::string& sNameSpace, const std::string& sValueName, Lib3MF::eToolpathProfileModificationType modifierType, double dMinimum, double dMaximum, Lib3MF::eToolpathProfileModificationFactor modificationFactor);
		void changeModifier(const std::string& sNameSpace, const std::string& sValueName, Lib3MF::eToolpathProfileModificationType modifierType, double dMinimum, double dMaximum, Lib3MF::eToolpathProfileModificationFactor modificationFactor);

		uint32_t getModifierCount();
		std::string getModifierName(const uint32_t nIndex);
		Lib3MF::eToolpathProfileModificationType getModifierType(const uint32_t nIndex);
		std::string getModifierNameSpace(const uint32_t nIndex);
		PModelToolpathProfileModifier getModifier(const uint32_t nIndex);
		void removeModifier(const std::string& sNameSpace, const std::string& sValueName);
		PModelToolpathProfileModifier findModifier(const std::string& sNameSpace, const std::string& sValueName, bool bMustExist);

		std::vector<PModelToolpathProfileValue> & getValues ();
		std::vector<PModelToolpathProfileModifier>& getModifiers();

		double evaluate(const std::string& sNameSpace, const std::string& sValueName, double dFactorF, double dFactorG, double dFactorH);

	};	

	typedef std::shared_ptr <CModelToolpathProfile> PModelToolpathProfile;

}

#endif // __NMR_MODELTOOLPATHPROFILE

