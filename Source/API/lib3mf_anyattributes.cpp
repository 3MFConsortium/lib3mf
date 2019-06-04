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

Abstract: This is a stub class definition of CAnyAttributes

*/

#include "lib3mf_anyattributes.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "lib3mf_anyattribute.hpp"
// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CAnyAttributes 
**************************************************************************************************************************/

CAnyAttributes::CAnyAttributes(NMR::PModelAnyAttributes pModelAnyAttributes)
	: m_pAttributes(pModelAnyAttributes)
{

}

Lib3MF_uint32 CAnyAttributes::GetAttributeCount ()
{
	return m_pAttributes->getAttributeCount();
}

IAnyAttribute * CAnyAttributes::GetAttribute (const Lib3MF_uint32 nIndex)
{
	return new CAnyAttribute(m_pAttributes->getAttribute(nIndex));
}

IAnyAttribute * CAnyAttributes::GetAttributeByKey (const std::string & sNameSpace, const std::string & sName)
{
	for (NMR::nfUint32 i = 0; i < m_pAttributes->getAttributeCount(); i++) {
		NMR::PModelAnyAttribute pAttribute = m_pAttributes->getAttribute(i);
		if (sNameSpace.empty()) {
			if (pAttribute->getName() == sName) {
				return new CAnyAttribute(pAttribute);
			}
		}
		else {
			if (pAttribute->getKey() == sNameSpace + ":" + sName) {
				return new CAnyAttribute(pAttribute);
			}
		}
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

void CAnyAttributes::RemoveAttributeByIndex (const Lib3MF_uint32 nIndex)
{
	m_pAttributes->removeAttribute(nIndex); 
}

void CAnyAttributes::RemoveAttribute(IAnyAttribute* pAttribute)
{
	for (NMR::nfUint32 i = 0; i < m_pAttributes->getAttributeCount(); i++) {
		NMR::PModelAnyAttribute pAttr = m_pAttributes->getAttribute(i);
		if (pAttribute->GetName() == pAttr->getName()) {
			m_pAttributes->removeAttribute(i);
			return;
		}
	}
	throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

IAnyAttribute * CAnyAttributes::AddAttribute(const std::string & sNameSpace, const std::string & sName, const std::string & sValue)
{
	NMR::PModelAnyAttribute pAttribute = m_pAttributes->addAttribute(sNameSpace, sName, sValue);
	return new CAnyAttribute(pAttribute);
}

