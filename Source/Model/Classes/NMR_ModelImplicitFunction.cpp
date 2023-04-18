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

#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Common/NMR_Exception.h"

namespace NMR
{
    CModelImplicitFunction::CModelImplicitFunction(const ModelResourceID sID, CModel * pModel) : CModelResource(sID, pModel)
    {
        m_nodes = std::make_shared<ImplicitNodes>();
    }


    std::string const & CModelImplicitFunction::getIdentifier() const
    {
        return m_identifier;
    }

    std::string const & CModelImplicitFunction::getDisplayName() const
    {
        return m_displayname;
    }

    void CModelImplicitFunction::setIdentifier(std::string const & identifier)
    {
        m_identifier = identifier;
    }

    void CModelImplicitFunction::setDisplayName(std::string const & displayname)
    {
        m_displayname = displayname;
    }

    PModelImplicitNode CModelImplicitFunction::addNode(const Lib3MF::eImplicitNodeType eNodeType,
                                                       const std::string & sIdentifier,
                                                       const std::string & sDisplayName)
    {
        auto node = std::make_shared<CModelImplicitNode>(eNodeType, sIdentifier, sDisplayName);
        m_nodes->push_back(node);
        return node;
    }

    PModelImplicitNode CModelImplicitFunction::addNode(const Lib3MF::eImplicitNodeType eNodeType)
    {
        auto node = std::make_shared<CModelImplicitNode>(eNodeType);
        m_nodes->push_back(node);
        return node;
    }

    PImplicitNodes NMR::CModelImplicitFunction::getNodes() const
    {
        return m_nodes;
    }
}
