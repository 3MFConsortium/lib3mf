/*++

Copyright (C) 2023 3MF Consortium

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

--*/

#include "Model/Classes/NMR_ModelImplicitPort.h"
#include "Model/Classes/NMR_ModelImplicitNode.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Common/NMR_Exception.h"
#include "lib3mf_interfaceexception.hpp"

namespace NMR
{
    CModelImplicitPort::CModelImplicitPort(CModelImplicitNode * parent,
                                           ImplicitIdentifier const & identifier,
                                           std::string const & displayname)
        : m_parent(parent)
        , m_identifier(identifier)
        , m_displayname(displayname)
    {
    }

    CModelImplicitPort::CModelImplicitPort(ImplicitIdentifier const & identifier,
                                           std::string const & displayname,
                                           Lib3MF::eImplicitPortType type)
        : m_identifier(identifier)
        , m_displayname(displayname)
        , m_type(type)
    {
    }

    ImplicitIdentifier const & CModelImplicitPort::getIdentifier() const
    {
        return m_identifier;
    }

    std::string const & CModelImplicitPort::getDisplayName() const
    {
        return m_displayname;
    }

    void CModelImplicitPort::setIdentifier(std::string const & identifier)
    {
        m_identifier = identifier;
    }

    void CModelImplicitPort::setDisplayName(std::string const & displayname)
    {
        m_displayname = displayname;
    }

    Lib3MF::eImplicitPortType CModelImplicitPort::getType() const
    {
        return m_type;
    }

    void CModelImplicitPort::setType(Lib3MF::eImplicitPortType type)
    {
        m_type = type;
    }

    ImplicitIdentifier CModelImplicitPort::getReference() const
    {
        if (!m_referencedPort)
        {
            updateReference();
        }
        if (!m_referencedPort)
        {
            return m_reference;
        }

        auto sourceParentNode = m_referencedPort->getParent();

        if (!sourceParentNode)  // Function input ports don't have a node as parent
        {
            return "inputs." + m_referencedPort->getIdentifier();
        }
        
        return makeReferenceIdentifier(sourceParentNode->getIdentifier(), m_referencedPort->getIdentifier());
    }

    void CModelImplicitPort::setReference(ImplicitIdentifier const & reference)
    {
        m_referencedPort.reset();
        m_reference = reference;
        updateReference();
    }

    CModelImplicitNode * CModelImplicitPort::getParent() const
    {
        return m_parent;
    }

    std::shared_ptr<CModelImplicitPort> CModelImplicitPort::getReferencedPort() const
    {
        if (!m_referencedPort)
        {
            updateReference();
        }
        if (!m_referencedPort)
        { 
            std::string nodeName;
            if (m_parent)
            {
                nodeName = m_parent->getIdentifier();
            }
            else
            {
                nodeName = "inputs";
            }
            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM, "Referenced port of port " + nodeName + "." + m_identifier + " is not set.");
        }
        return m_referencedPort;
    }

    void CModelImplicitPort::setReferencedPort(
      std::shared_ptr<CModelImplicitPort> const & referencedPort)
    {
        m_referencedPort = referencedPort;
    }

    void CModelImplicitPort::updateReference() const
    {
        if (!m_parent)
        {
            return;
        }

        auto * function = m_parent->getParent();
        if (!function)
        {
            return;
        }

        m_referencedPort = function->findPort(m_reference);
    }
} //namespace NMR
