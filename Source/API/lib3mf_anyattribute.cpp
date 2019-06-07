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

Abstract: This is a stub class definition of CAnyAttribute

*/

#include "lib3mf_anyattribute.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CMetaData 
**************************************************************************************************************************/

CAnyAttribute::CAnyAttribute(NMR::PModelAnyAttribute pAttribute)
	:m_pAttribute(pAttribute)
{
}


std::string CAnyAttribute::GetNameSpace ()
{
	return m_pAttribute->getNameSpace();
}

void CAnyAttribute::SetNameSpace (const std::string & sNameSpace)
{
	m_pAttribute->setNameSpace(sNameSpace);
}

std::string CAnyAttribute::GetName()
{
	return m_pAttribute->getName();
}

std::string CAnyAttribute::GetKey()
{
	return m_pAttribute->getKey();
}

void CAnyAttribute::SetName (const std::string & sName)
{
	m_pAttribute->setName(sName);
}

std::string CAnyAttribute::GetValue ()
{
	return m_pAttribute->getValue();
}

void CAnyAttribute::SetValue (const std::string & sValue)
{
	m_pAttribute->setValue(sValue);
}

