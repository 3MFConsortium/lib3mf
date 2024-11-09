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

#include <set>
#include <vector>

namespace NMR::common::graph
{
    using Identifier = std::size_t;
    using DependencySet = std::set<Identifier>;
    using VertexList = std::vector<Identifier>;

    class IDirectedGraph
    {
       public:
        explicit IDirectedGraph(std::size_t const /*unused*/) {};
        virtual ~IDirectedGraph() = default;
        virtual void addDependency(Identifier id,
                                   Identifier idOfDependency) = 0;
        virtual void removeDependency(Identifier id,
                                      Identifier idOfDependency) = 0;
        [[nodiscard]] virtual auto isDirectlyDependingOn(
            Identifier id, Identifier dependencyInQuestion) const -> bool = 0;

        [[nodiscard]] virtual auto getSize() const -> std::size_t = 0;
        virtual void removeVertex(Identifier id) = 0;
        virtual void addVertex(Identifier id) = 0;
        [[nodiscard]] virtual auto hasPredecessors(Identifier id) const
            -> bool = 0;

        [[nodiscard]] virtual auto getVertices() const
            -> const DependencySet& = 0;
    };
}  // namespace NMR::common::graph
