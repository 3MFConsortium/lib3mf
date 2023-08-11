/*++

Copyright (C) 2023 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,276529
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
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_ModelImplicitPort.h"

namespace NMR
{
    const implicit::NodeTypes CModelImplicitFunction::m_nodeTypes;

    CModelImplicitNode *
    CModelImplicitFunction::findNode(const ImplicitIdentifier & sIdentifier) const
    {
        for (auto & node : *m_nodes)
        {
            if (node->getIdentifier() == sIdentifier)
            {
                return node.get();
            }
        }
        return nullptr;
    }

    CModelImplicitFunction::CModelImplicitFunction(const ModelResourceID sID, CModel * pModel)
        : CModelFunction(sID, pModel)
    {
        m_nodes = std::make_shared<ImplicitNodes>();
    }

    std::string const & CModelImplicitFunction::getIdentifier() const
    {
        return m_identifier;
    }

    void CModelImplicitFunction::setIdentifier(std::string const & identifier)
    {
        m_identifier = identifier;
    }

    PModelImplicitNode CModelImplicitFunction::addNode(const Lib3MF::eImplicitNodeType eNodeType,
                                                       const std::string & sIdentifier,
                                                       const std::string & sDisplayName,
                                                       const std::string & sTag)
    {
        auto node = std::make_shared<CModelImplicitNode>(eNodeType, sIdentifier, sDisplayName, sTag, this);

        m_nodeTypes.addExpectedPortsToNode(*node);
        m_nodes->push_back(node);
        return node;
    }

    PModelImplicitNode CModelImplicitFunction::addNode(const Lib3MF::eImplicitNodeType eNodeType)
    {
        return addNode(eNodeType, "", "", "");
    }

    PImplicitNodes NMR::CModelImplicitFunction::getNodes() const
    {
        return m_nodes;
    }

    void NMR::CModelImplicitFunction::addLink(
        const ImplicitIdentifier& sSourceNodeIdentifier,
        const ImplicitIdentifier& sTargetNodeIdentifier)

    {
        auto const sourceNodeName = extractNodeName(sSourceNodeIdentifier);

        PModelImplicitPort sourcePort;
        if(sourceNodeName == "inputs")
        {
            auto const sourcePortName = extractPortName(sSourceNodeIdentifier);
            sourcePort = findInput(sourcePortName);
        }
        else
        {
            auto const sourceNode = findNode(sourceNodeName);
            if(sourceNode == nullptr)
            {
                throw CNMRException(
                    NMR_ERROR_IMPLICIT_FUNCTION_INVALID_SOURCE_NODE);
            }
            auto const sourcePortName = extractPortName(sSourceNodeIdentifier);
            if(sourcePortName.empty())
            {
                throw CNMRException(
                    NMR_ERROR_IMPLICIT_FUNCTION_INVALID_SOURCE_PORT);
            }
            sourcePort = sourceNode->findOutput(sourcePortName);
        }
        if(!sourcePort)
        {
            throw CNMRException(
                NMR_ERROR_IMPLICIT_FUNCTION_INVALID_SOURCE_PORT);
        }

        auto const targetNodeName = extractNodeName(sTargetNodeIdentifier);
        auto const targetPortName = extractPortName(sTargetNodeIdentifier);
        NMR::PModelImplicitPort targetPort;
        if(targetNodeName == "outputs")
        {
            targetPort = findOutput(targetPortName);
        }
        else
        {
            auto const targetNode = findNode(targetNodeName);
            if(targetNode == nullptr)
            {
                throw CNMRException(
                    NMR_ERROR_IMPLICIT_FUNCTION_INVALID_TARGET_NODE);
            }

            if(targetPortName.empty())
            {
                throw CNMRException(
                    NMR_ERROR_IMPLICIT_FUNCTION_INVALID_TARGET_PORT);
            }

            targetPort = targetNode->findInput(targetPortName);
        }

        if(!targetPort)
        {
            throw CNMRException(
                NMR_ERROR_IMPLICIT_FUNCTION_INVALID_TARGET_PORT);
        }

        if(sourcePort->getType() != targetPort->getType())
        {
            throw CNMRException(NMR_ERROR_IMPLICIT_FUNCTION_INVALID_PORT_TYPE);
        }

        targetPort->setReference(sSourceNodeIdentifier);
        if(sourcePort)
        {
            sourcePort->setReference(
                sTargetNodeIdentifier);  // won't be visible in the xml
        }
    }

    void CModelImplicitFunction::addLink(CModelImplicitPort const & pSourcePort,
                                         CModelImplicitPort & pTargetPort)
    {
        pTargetPort.setReference(makeReferenceIdentifier(pSourcePort.getParent()->getIdentifier(),
                                                         pSourcePort.getIdentifier()));
    }

    std::string extractNodeName(const ImplicitIdentifier & sIdentifier)
    {
        auto pos = sIdentifier.find_last_of(".");
        if (pos == std::string::npos)
        {
            return "";
        }
        return sIdentifier.substr(0, pos);
    }

    std::string extractPortName(const ImplicitIdentifier & sIdentifier)
    {
        auto pos = sIdentifier.find_last_of(".");
        if (pos == std::string::npos)
        {
            return sIdentifier;
        }
        return sIdentifier.substr(pos + 1);
    }

    ImplicitIdentifier makeReferenceIdentifier(const ImplicitIdentifier & sNodeIdentifier,
                                               const ImplicitIdentifier & sPortIdentifier)
    {
        return sNodeIdentifier + "." + sPortIdentifier;
    }
}
