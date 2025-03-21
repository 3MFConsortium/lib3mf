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

Abstract: This is a stub class definition of CImplicitNode

*/

#include "lib3mf_implicitnode.hpp"
#include "lib3mf_implicitport.hpp"
#include "lib3mf_implicitportiterator.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_meshobject.hpp"
#include <Model/Classes/NMR_ModelMeshObject.h>

// Include custom headers here.

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImplicitNode
**************************************************************************************************************************/

std::string CImplicitNode::GetIdentifier()
{
    if (!m_pImplicitNode)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }
    return m_pImplicitNode->getIdentifier();
}

void CImplicitNode::SetIdentifier(const std::string & sIdentifier)
{
    m_pImplicitNode->setIdentifier(sIdentifier);
}

std::string CImplicitNode::GetDisplayName()
{
    return m_pImplicitNode->getDisplayName();
}

void CImplicitNode::SetDisplayName(const std::string& sDisplayName)
{
    m_pImplicitNode->setDisplayName(sDisplayName);
}

std::string CImplicitNode::GetTag()
{
    return m_pImplicitNode->getTag();
}

void CImplicitNode::SetTag(const std::string& sTag)
{
    m_pImplicitNode->setTag(sTag);
}

IImplicitPort* Lib3MF::Impl::CImplicitNode::FindInputOrThrow(
    const std::string& sIdentifier)
{
    auto pPort = FindInput(sIdentifier);
    if (!pPort)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }
    return pPort;
}

IImplicitPort* Lib3MF::Impl::CImplicitNode::FindOutputOrThrow(
    const std::string& sIdentifier)
{
    auto pPort = FindOutput(sIdentifier);
    if (!pPort)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }
    return pPort;
}

CImplicitNode::CImplicitNode(NMR::PModelImplicitNode pImplicitNode)
    : m_pImplicitNode{pImplicitNode}
{
}

Lib3MF::eImplicitNodeType CImplicitNode::GetNodeType()
{
    return m_pImplicitNode->getNodeType();
}

IImplicitPort * CImplicitNode::AddInput(const std::string & sIdentifier,
                                        const std::string & sDisplayName)
{
    return new CImplicitPort(m_pImplicitNode->addInput(sIdentifier, sDisplayName));
}

IImplicitPort * CImplicitNode::AddOutput(const std::string & sIdentifier,
                                         const std::string & sDisplayName)
{
    return new CImplicitPort(m_pImplicitNode->addOutput(sIdentifier, sDisplayName));
}

IImplicitPortIterator * CImplicitNode::GetOutputs()
{
    return new CImplicitPortIterator(m_pImplicitNode->getOutputs());
}

IImplicitPortIterator * CImplicitNode::GetInputs()
{
    return new CImplicitPortIterator(m_pImplicitNode->getInputs());
}

IImplicitPort * CImplicitNode::FindInput(const std::string & sIdentifier)
{
    auto pPort = m_pImplicitNode->findInput(sIdentifier);
    if (!pPort)
    {
        return nullptr;
    }
    return new CImplicitPort(pPort);
}

IImplicitPort * CImplicitNode::FindOutput(const std::string & sIdentifier)
{
    auto pPort = m_pImplicitNode->findOutput(sIdentifier);
    if (!pPort)
    {
        return nullptr;
    }
    return new CImplicitPort(pPort);
}

bool Lib3MF::Impl::CImplicitNode::AreTypesValid()
{
    return m_pImplicitNode->arePortsValid();
}

