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

NMR_ModelAnyAttribute.cpp implements the Model Any Attribute Class.

--*/

#include "Model/Classes/NMR_ModelAnyAttribute.h" 

namespace NMR {

	CModelAnyAttribute::CModelAnyAttribute(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue)
	{
		m_sNameSpace = sNameSpace;
		m_sName = sName;
		m_sValue = sValue;
	}

	std::string CModelAnyAttribute::getName()
	{
		return m_sName;
	}

	std::string CModelAnyAttribute::getNameSpace()
	{
		return m_sNameSpace;
	}

	std::string CModelAnyAttribute::getKey()
	{
		return calculateKey(m_sNameSpace, m_sName);
	}

	std::string CModelAnyAttribute::getValue()
	{
		return m_sValue;
	}

	void CModelAnyAttribute::setName(std::string sName)
	{
		m_sName = sName;
	}

	void CModelAnyAttribute::setNameSpace(std::string sNameSpace)
	{
		m_sNameSpace = sNameSpace;
	}

	void CModelAnyAttribute::setValue(std::string sValue)
	{
		m_sValue = sValue;
	}

	std::string CModelAnyAttribute::calculateKey(const std::string &sNameSpace, const std::string &sName)
	{
		if (sNameSpace.empty())
			return sName;
		else
			return sNameSpace + ":" + sName;
	}
}
