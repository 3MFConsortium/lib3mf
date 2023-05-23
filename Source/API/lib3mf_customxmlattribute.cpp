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

Abstract: This is a stub class definition of CCustomXMLAttribute

*/

#include "lib3mf_customxmlattribute.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "Common/NMR_StringUtils.h"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CCustomXMLAttribute 
**************************************************************************************************************************/

CCustomXMLAttribute::CCustomXMLAttribute(NMR::PCustomXMLNode pXMLNode, NMR::PCustomXMLAttribute pXMLAttribute)
	: m_pXMLNode (pXMLNode), m_pXMLAttribute (pXMLAttribute)
{
	if (pXMLNode.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	if (pXMLAttribute.get() == nullptr)
		throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
}

CCustomXMLAttribute::~CCustomXMLAttribute()
{

}


std::string CCustomXMLAttribute::GetName()
{
	return m_pXMLAttribute->getName();
}

std::string CCustomXMLAttribute::GetValue()
{
	return m_pXMLAttribute->getValue();
}

bool CCustomXMLAttribute::IsValidInteger(const Lib3MF_int64 nMinValue, const Lib3MF_int64 nMaxValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_int64 CCustomXMLAttribute::GetIntegerValue(const Lib3MF_int64 nMinValue, const Lib3MF_int64 nMaxValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CCustomXMLAttribute::IsValidDouble(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

Lib3MF_double CCustomXMLAttribute::GetDoubleValue(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CCustomXMLAttribute::IsValidBool()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

bool CCustomXMLAttribute::GetBoolValue(const Lib3MF_double dMinValue, const Lib3MF_double dMaxValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CCustomXMLAttribute::SetValue(const std::string & sValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CCustomXMLAttribute::SetIntegerValue(const Lib3MF_int64 nValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CCustomXMLAttribute::SetDoubleValue(const Lib3MF_double dValue)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CCustomXMLAttribute::SetBoolValue(const bool bValue)
{
	if (bValue)
		m_pXMLAttribute->setValue("true");
	else
		m_pXMLAttribute->setValue("false");
}

void CCustomXMLAttribute::Remove()
{
	m_pXMLNode->removeAttribute(m_pXMLAttribute->getName());
}

