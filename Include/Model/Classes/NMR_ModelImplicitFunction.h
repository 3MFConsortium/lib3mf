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

#include "Model/Classes/NMR_ModelFunction.h"
#include <Common/Platform/NMR_SAL.h>
#include <Model/Classes/NMR_ModelImplicitNode.h>

#include <memory>
#include <string>
#include <vector>

namespace NMR
{
    using ImplicitNodes = std::vector<PModelImplicitNode>;
    using PImplicitNodes = std::shared_ptr<ImplicitNodes>;

    namespace implicit
    {
        class NodeTypes;
    }

    // Extract node name (before the first dot) from a node identifier
    std::string extractNodeName(const ImplicitIdentifier & sIdentifier);

    // Extract port name (after the first dot) from a node identifier
    std::string extractPortName(const ImplicitIdentifier & sIdentifier);

    // Concatenate node name and port name to a reference identifier
    ImplicitIdentifier makeReferenceIdentifier(const ImplicitIdentifier & sNodeIdentifier,
                                               const ImplicitIdentifier & sPortIdentifier);

    class CModelImplicitFunction : public CModelFunction
    {
      private:
        ImplicitIdentifier m_identifier;
        PImplicitNodes m_nodes;
        static const implicit::NodeTypes m_nodeTypes;

        CModelImplicitNode * findNode(const ImplicitIdentifier & sIdentifier) const;

      public:
        CModelImplicitFunction(_In_ const ModelResourceID sID, _In_ CModel * pModel);

        ImplicitIdentifier const & getIdentifier() const;
        void setIdentifier(ImplicitIdentifier const & identifier);

        PModelImplicitNode addNode(const Lib3MF::eImplicitNodeType eNodeType,
                                   const std::string & sIdentifier,
                                   const std::string & sDisplayName,
                                   const std::string & sTag);

        PModelImplicitNode addNode(const Lib3MF::eImplicitNodeType eNodeType);

        PImplicitNodes getNodes() const;

        void addLink(const ImplicitIdentifier & sSourceNodeIdentifier,
                     const ImplicitIdentifier & sTargetNodeIdentifier);

        void addLink(CModelImplicitPort const & pSourcePort, CModelImplicitPort & pTargetPort);

        implicit::NodeTypes const & getNodeTypes() const;
    };

    using PModelImplicitFunction = std::shared_ptr<CModelImplicitFunction>;
}
