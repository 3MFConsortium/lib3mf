/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CImplicitPort

*/

#include "lib3mf_implicitport.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_types.hpp"

// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImplicitPort
**************************************************************************************************************************/

Lib3MF::Impl::CImplicitPort::CImplicitPort(NMR::PModelImplicitPort pImplicitPort)
    : m_pImplicitPort(pImplicitPort)
{
}

std::string CImplicitPort::GetIdentifier()
{
    return m_pImplicitPort->getIdentifier();
}

void CImplicitPort::SetIdentifier(const std::string & sIdentifier)
{
    m_pImplicitPort->setIdentifier(sIdentifier);
}

std::string CImplicitPort::GetDisplayName()
{
    return m_pImplicitPort->getDisplayName();
}

void CImplicitPort::SetDisplayName(const std::string & sDisplayName)
{
    m_pImplicitPort->setDisplayName(sDisplayName);
}

void CImplicitPort::SetType(const Lib3MF::eImplicitPortType eType)
{
    m_pImplicitPort->setType(eType);
}

Lib3MF::eImplicitPortType CImplicitPort::GetType()
{
    return m_pImplicitPort->getType();
}

std::string CImplicitPort::GetReference()
{
    return m_pImplicitPort->getReference();
}

void CImplicitPort::SetReference(const std::string & sReference)
{
    m_pImplicitPort->setReference(sReference);
}