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
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_types.hpp"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Model/Classes/NMR_ModelImplicitPort.h"
#include "Common/Graph/GraphAlgorithms.h"
#include "Common/Graph/DirectedGraph.h"

#include <algorithm>
#include <queue>

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

    CModelImplicitFunction::CModelImplicitFunction(const ModelResourceID sID,
                                                   CModel* pModel)
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
                                                       const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                                                       const std::string & sIdentifier,
                                                       const std::string & sDisplayName,
                                                       const std::string & sTag)
    {
        auto node = std::make_shared<CModelImplicitNode>(eNodeType, sIdentifier, sDisplayName, sTag, this);

        m_nodeTypes.addExpectedPortsToNode(*node, eConfiguration);
        m_nodes->push_back(node);
        return node;
    }

    PModelImplicitNode CModelImplicitFunction::addNode(const Lib3MF::eImplicitNodeType eNodeType, const Lib3MF::eImplicitNodeConfiguration eConfiguration)
    {
        return addNode(eNodeType, eConfiguration, "", "", "");
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
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDPARAM,
                    "Source node " + sourceNodeName + " does not exist.");
            }
            auto const sourcePortName = extractPortName(sSourceNodeIdentifier);
            if(sourcePortName.empty())
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDPARAM,
                    "Source port " + sourcePortName + " of node " +
                        sourceNodeName + " does not exist.");
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
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDPARAM,
                    "Target node " + targetNodeName + " does not exist.");
            }

            if(targetPortName.empty())
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDPARAM,
                    "Target port " + targetPortName + " of node " +
                        targetNodeName + " does not exist.");
            }

            targetPort = targetNode->findInput(targetPortName);
        }

        if(!targetPort)
        {
            throw ELib3MFInterfaceException(
                LIB3MF_ERROR_INVALIDPARAM,
                "Target port " + targetPortName + " of node " +
                    targetNodeName + " does not exist.");
        }

        if(sourcePort->getType() != targetPort->getType())
        {
            std::string sourceNodeIdentifier = "function inputs";
            std::string targetNodeIdentifier = "function outputs";

            if(sourcePort->getParent())
            {
                sourceNodeIdentifier = sourcePort->getParent()->getIdentifier();
            }

            if(targetPort->getParent())
            {
                targetNodeIdentifier = targetPort->getParent()->getIdentifier();
            }

            throw ELib3MFInterfaceException(
                LIB3MF_ERROR_INCOMPATIBLEPORTTYPES,
                "Output " + sourcePort->getIdentifier() + " of node " +
                    sourceNodeIdentifier + " has type " +
                    std::to_string(static_cast<int>(sourcePort->getType())) +
                    " while target input " + targetPort->getIdentifier() +
                    " of node " + targetNodeIdentifier + " has type " +
                    std::to_string(static_cast<int>(targetPort->getType())) +
                    ".");
            
        }
        
        targetPort->setReferencedPort(sourcePort);
    }

    void CModelImplicitFunction::addLink(PModelImplicitPort pSourcePort,
                                         PModelImplicitPort pTargetPort)
    {
        if (!pSourcePort)
        {
            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                            "Source port is nullptr.");
        }

        if (!pTargetPort)
        {
            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                            "Target port is nullptr.");
        }
        
        if(pSourcePort->getType() != pTargetPort->getType())
        {
            std::string sourceNodeIdentifier = "function inputs";
            std::string targetNodeIdentifier = "function outputs";

            if(pSourcePort->getParent())
            {
                sourceNodeIdentifier = pSourcePort->getParent()->getIdentifier();
            }

            if(pTargetPort->getParent())
            {
                targetNodeIdentifier = pTargetPort->getParent()->getIdentifier();
            }

            throw ELib3MFInterfaceException(
                LIB3MF_ERROR_INCOMPATIBLEPORTTYPES,
                "Output " + pSourcePort->getIdentifier() + " of node " +
                    sourceNodeIdentifier + " has type " +
                    std::to_string(static_cast<int>(pSourcePort->getType())) +
                    " while target input " + pTargetPort->getIdentifier() +
                    " of node " + targetNodeIdentifier + " has type " +
                    std::to_string(static_cast<int>(pTargetPort->getType())) +
                    ".");
        }
        
        pTargetPort->setReferencedPort(pSourcePort);
    }

    implicit::NodeTypes const& CModelImplicitFunction::getNodeTypes() const
    {
        return m_nodeTypes;
    }

    void CModelImplicitFunction::removeNode(
        ImplicitIdentifier const& sIdentifier)
    {
        auto node = findNode(sIdentifier);
        if(node == nullptr)
        {
            throw CNMRException(NMR_ERROR_INVALIDPOINTER);
        }
        m_nodes->erase(std::remove_if(m_nodes->begin(), m_nodes->end(),
                                      [node](PModelImplicitNode const& n)
                                      { return n.get() == node; }),
                       m_nodes->end());
    }

    void CModelImplicitFunction::clear() 
    {
        CModelFunction::clear();
        m_nodes->clear();
    }

    void CModelImplicitFunction::replaceResourceID(const ModelResourceID sOldID,
                                                  const ModelResourceID sNewID)
    {
        for (auto & node : *m_nodes)
        {
            if (node->getModelResourceID() == sOldID)
            {
                node->setModelResourceID(sNewID);
            }
        }
    }


    std::string extractNodeName(const ImplicitIdentifier& sIdentifier)
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
        if (sNodeIdentifier.empty())
        {
            return "inputs." + sPortIdentifier;
        }
        return sNodeIdentifier + "." + sPortIdentifier;
    }

    NMR::common::graph::DirectedGraph directedGraphFromNodes(const PImplicitNodes & nodes)
    {
        if (!nodes)
        {
            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                            "Nodes must not be nullptr.");
        }

        using namespace NMR::common;
        graph::DirectedGraph graph(nodes->size()+1);

        std::unordered_map<ImplicitIdentifier, graph::Identifier> identifierToIndex;
        graph::Identifier InputsIndex = 0;

        graph::Identifier index = 1;
        for (auto const& node : *nodes)
        {
            if (!node)
            {
                throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                                "Node must not be nullptr.");
            }

            auto const& identifier = node->getIdentifier();
            identifierToIndex[identifier] = index;
            ++index;
        }

        for (auto const& node : *nodes)
        {
            auto const& inputs = node->getInputs();

            if (!inputs)
            {
                throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                                "Inputs must not be nullptr.");
            }

            for (auto const& input : *inputs)
            {
                auto const& sourcePort = input->getReferencedPort();
                if (sourcePort)
                {
                    auto const& sourceNode = sourcePort->getParent();
                    if (sourceNode)
                    {
                        auto const& sourceNodeIdentifier = sourceNode->getIdentifier();
                        auto const& sourcePortIdentifier = sourcePort->getIdentifier();
                        graph::Identifier const sourceIndex = identifierToIndex[sourceNodeIdentifier];
                        graph::Identifier const targetIndex = identifierToIndex[node->getIdentifier()];
                        if (sourceIndex >= graph.getSize())
                        {
                            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                                            "Source index " + std::to_string(sourceIndex) + " is out of range.");
                        }

                        if (targetIndex >= graph.getSize())
                        {
                            throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                                            "Target index " + std::to_string(targetIndex) + " is out of range.");
                        }
                        graph.addDependency(sourceIndex, targetIndex);
                    }
                    else
                    {
                        graph.addDependency(InputsIndex, identifierToIndex[node->getIdentifier()]);
                    }
                }

            }
        }
        return graph;
    }

    void CModelImplicitFunction::sortNodesTopologically()
    {
        using namespace NMR::common;
        auto graph = directedGraphFromNodes(m_nodes);
        auto const sortedIndices = NMR::common::graph::topologicalSort(graph);
        if (sortedIndices.empty())
        {
            throw ELib3MFInterfaceException(LIB3MF_ERROR_GRAPHISCYCLIC);
        }
       
        PImplicitNodes sortedNodes = std::make_shared<ImplicitNodes>();
        sortedNodes->reserve(sortedIndices.size());

        for (auto const & index : sortedIndices)
        {
            if (index == 0)
            {
                continue;
            }
            
            auto nodeIndex = index - 1;
            
            if (nodeIndex >= m_nodes->size())
            {
                throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM,
                                                "Invalid node index " + std::to_string(nodeIndex) + ". Node index must be between 0 and " + std::to_string(m_nodes->size()) + ".");
            }

            sortedNodes->push_back(m_nodes->at(nodeIndex));
        }
        std::swap(m_nodes, sortedNodes);
    }

    PModelImplicitPort
    NMR::CModelImplicitFunction::findPort(const ImplicitIdentifier & sIdentifier) const
    {
        auto const nodeName = extractNodeName(sIdentifier);
        auto const portName = extractPortName(sIdentifier);
        if (nodeName == "inputs")
        {
            return findInput(portName);
        }
        auto const node = findNode(nodeName);
        if (node == nullptr)
        {
            return {};
        }
        if (portName.empty())
        {
            return {};
        }
        return node->findOutput(portName);
    }
}


