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

#pragma once
#include "IDirectedGraph.h"

namespace NMR::common::graph
{
    class DirectedGraph : public IDirectedGraph
    {
       public:
        explicit DirectedGraph(std::size_t size);
        void addDependency(Identifier id, Identifier idOfDependency) override;
        void removeDependency(Identifier id,
                              Identifier idOfDependency) override;
        [[nodiscard]] auto isDirectlyDependingOn(
            Identifier id,
            Identifier dependencyInQuestion) const -> bool override;

        [[nodiscard]] auto getSize() const -> std::size_t override;

        void removeVertex(Identifier id) override;

        [[nodiscard]] auto getVertices() const -> const DependencySet& override;
        void addVertex(Identifier id) override;

       private:
        std::size_t m_size;
        std::vector<bool> m_graphData;

        DependencySet m_vertices;  // Possible performance improvement: We could
                                   // try out a std::set

        using PredecessorList = std::vector<std::size_t>;
        std::vector<PredecessorList> m_predecessors;

        // Inherited via IDirectedGraph
        [[nodiscard]] auto hasPredecessors(Identifier id) const
            -> bool override;
    };
}  // namespace NMR::common::graph
