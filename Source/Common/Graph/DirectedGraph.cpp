/*++

Copyright (C) 2023 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification,276529 are permitted provided that the following conditions are
met:

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

#include "Common/Graph/DirectedGraph.h"

#include <algorithm>

namespace NMR::common::graph
{
    DirectedGraph::DirectedGraph(size_t const size)
        : IDirectedGraph(size),
          m_size(size),
          m_graphData((m_size + 1) * (m_size + 1), false)
    {
    }

    void DirectedGraph::addDependency(Identifier id, Identifier idOfDependency)
    {
        if(id == idOfDependency)
        {
            return;
        }
        auto const index = id * m_size + idOfDependency;
        m_graphData[index] = true;
    }

    void DirectedGraph::removeDependency(Identifier id,
                                         Identifier idOfDependency)
    {
        auto const index = id * m_size + idOfDependency;
        m_graphData[index] = false;
    }

    bool DirectedGraph::isDirectlyDependingOn(
        Identifier id, Identifier dependencyInQuestion) const
    {
        auto const index = id * m_size + dependencyInQuestion;
        return m_graphData[index];
    }

    size_t DirectedGraph::getSize() const
    {
        return m_size;
    }

    void DirectedGraph::removeVertex(Identifier id)
    {
        auto const iterElemToRemove =
            std::find(std::begin(m_vertices), std::end(m_vertices), id);
        if(iterElemToRemove == std::end(m_vertices))
        {
            return;
        }
        m_vertices.erase(iterElemToRemove);

        for(auto vertex : m_vertices)
        {
            removeDependency(id, vertex);
            removeDependency(vertex, id);
        }
    }

    auto DirectedGraph::getVertices() const -> const DependencySet&
    {
        return m_vertices;
    }

    void DirectedGraph::addVertex(Identifier id)
    {
        m_vertices.insert(id);
    }

    auto DirectedGraph::hasPredecessors(Identifier id) const -> bool
    {
        return !m_predecessors[id].empty();
    }
}  // namespace NMR::common::graph
