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

	enum class eModelToolpathProfileOverrideFactor {
		pfNone = 0,
		pfFactorF = 1,
		pfFactorG = 2,
		pfFactorH = 3
	};

	class CModelToolpathProfileModifier {
	private:
		PModelToolpathProfileValue m_pValue;
		double m_dDeltaValue;
		eModelToolpathProfileOverrideFactor m_OverrideFactor;

	public:

		CModelToolpathProfileModifier(PModelToolpathProfileValue pValue, double dDeltaValue, eModelToolpathProfileOverrideFactor overrideFactor);

		virtual ~CModelToolpathProfileModifier();

		PModelToolpathProfileValue getBaseValue();

		double evaluate (double dFactorF, double dFactorG, double dFactorH);

		std::string getName();

		std::string getNameSpace();

		double getDeltaValue();

		eModelToolpathProfileOverrideFactor getOverrideFactor ();
	};

	typedef std::shared_ptr<CModelToolpathProfileModifier> PModelToolpathProfileModifier;

	class CModelToolpathProfile {
	private:
		std::string m_sUUID;
		std::string m_sName;

		std::vector<PModelToolpathProfileValue> m_ValueList;
		std::map<std::pair<std::string, std::string>, PModelToolpathProfileValue> m_ValueMap;

		std::vector<PModelToolpathProfileModifier> m_ModifierList;
		std::map<std::pair<std::string, std::string>, PModelToolpathProfileModifier> m_ModifierMap;

	public:
		CModelToolpathProfile() = delete;
		CModelToolpathProfile(std::string sUUID, std::string sName);

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

		void addModifier(const std::string& sNameSpace, const std::string& sValueName, double dDelta, eModelToolpathProfileOverrideFactor overrideFactor);

		uint32_t getModifierCount();
		std::string getModifierName(const uint32_t nIndex);
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

