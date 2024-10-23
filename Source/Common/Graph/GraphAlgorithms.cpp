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

#include "Common/Graph/GraphAlgorithms.h"
#include "Common/Graph/IDirectedGraph.h"

#include <list>
#include <queue>
#include <sstream>
#include <stack>

namespace NMR::common::graph
{
    bool isCyclicUtil(const IDirectedGraph & graph, Identifier node, std::vector<bool> & visited, std::vector<bool> & recStack)
    {
        if (!visited[node])
        {
            visited[node] = true;
            recStack[node] = true;

            for (Identifier dep = 0u; dep < static_cast<Identifier>(graph.getSize()); ++dep)
            {
                if (graph.isDirectlyDependingOn(node, dep))
                {
                    if (!visited[dep] && isCyclicUtil(graph, dep, visited, recStack))
                    {
                        return true;
                    }
                    else if (recStack[dep])
                    {
                        return true;
                    }
                }
            }
        }
        recStack[node] = false;
        return false;
    }

    bool isCyclic(const IDirectedGraph & graph)
    {
        std::vector<bool> visited(graph.getSize(), false);
        std::vector<bool> recStack(graph.getSize(), false);

        for (Identifier id = 0u; id < static_cast<Identifier>(graph.getSize()); ++id)
        {
            if (isCyclicUtil(graph, id, visited, recStack))
            {
                return true;
            }
        }
        return false;
    }

    VertexList topologicalSort(const IDirectedGraph & graph)
    {
        if (isCyclic(graph))
        {
            return {};
        }

        // tsort based on DFS
        enum class NodeType
        {
            CHILD,
            PARENT
        };
        std::stack<std::pair<NodeType, Identifier>> nodesToVisit;
        std::vector<bool> visited(graph.getSize(), false);
        VertexList topologicalOrder;

        for (Identifier id = 0u; id < static_cast<Identifier>(graph.getSize()); ++id)
        {
            if (!visited[id])
            {
                nodesToVisit.push({NodeType::CHILD, id});

                while (!nodesToVisit.empty())
                {
                    auto [nodeType, nodeId] = nodesToVisit.top();
                    nodesToVisit.pop();

                    if (nodeType == NodeType::PARENT)
                    {
                        topologicalOrder.push_back(nodeId);
                        continue;
                    }

                    visited[nodeId] = true;
                    nodesToVisit.push({NodeType::PARENT, nodeId});

                    for (Identifier dep = 0u; dep < static_cast<Identifier>(graph.getSize()); ++dep)
                    {
                        if (!visited[dep])
                        {
                            if (graph.isDirectlyDependingOn(nodeId, dep))
                            {
                                nodesToVisit.push({NodeType::CHILD, dep});
                            }
                        }
                    }
                    
                }
            }
        }
        return topologicalOrder;
    }
} // namespace gladius::nodes::graph
