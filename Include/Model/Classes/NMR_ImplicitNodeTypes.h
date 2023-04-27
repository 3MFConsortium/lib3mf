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

#pragma once

#include <lib3mf_types.hpp>

#include <string>
#include <map>
#include <vector>

namespace NMR
{
    class CModelImplicitNode;
    namespace implicit
    {
        using PortIdentifier = std::string;
        using ExpectedPorts = std::vector<PortIdentifier>;
        
        class NodeType
        {
          public:
            NodeType(std::string const & name,
                     ExpectedPorts const & inputs,
                     ExpectedPorts const & outputs);
                     
            NodeType() = delete;

            std::string const & getName() const;
            ExpectedPorts const & getInputs() const;
            ExpectedPorts const & getOutputs() const;
            Lib3MF::eImplicitNodeType getType() const;

          private:
            std::string m_name;
            ExpectedPorts m_inputs;
            ExpectedPorts m_outputs;
        };

        using NodeTypesMap = std::map<Lib3MF::eImplicitNodeType, NodeType>;
        
        class NodeTypes
        {
          public:
            NodeTypes();
            NodeTypes(NodeTypes const & other) = delete;
            NodeTypes(NodeTypes && other) = delete;
            NodeTypes & operator=(NodeTypes const & other) = delete;
            NodeTypes & operator=(NodeTypes && other) = delete;

            NodeType const & getNodeType(Lib3MF::eImplicitNodeType type) const;

            NodeTypesMap const& getTypes() const;

            void addExpectedPortsToNode(NMR::CModelImplicitNode & node) const;
            
          private:
            const NodeTypesMap m_nodeTypes;
        };

        

        NodeTypes const & getNodeTypes();
    }
}