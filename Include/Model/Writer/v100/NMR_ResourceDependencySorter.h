/*++

Copyright (C) 2024 3MF Consortium

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

Abstract:

NMR_ResourcDependencySorter resolves the dependencies between resources by
sorting them topologically.
--*/

#pragma once

#include <unordered_map>

#include "Common/Graph/DirectedGraph.h"
#include "Common/Graph/GraphAlgorithms.h"
#include "Model/Classes/NMR_Model.h"

namespace NMR
{
    using TopologicalSortResult = std::vector<PPackageResourceID>;

    class CResourceDependencySorter
    {
       private:
        common::graph::DirectedGraph buildGraph();
        void buildIndexMaps();
        
       [[nodiscard]] PPackageResourceID indexToModelResourceID(size_t index) const;
        [[nodiscard]] size_t modelResourceIDToIndex(PPackageResourceID resourceID) const;

        CModel * m_pModel;

        std::unordered_map<size_t, PPackageResourceID> m_indexToResourceID;
        std::unordered_map<PPackageResourceID, size_t> m_resourceIDToIndex;

       public:
        explicit CResourceDependencySorter(CModel * pModel);
        TopologicalSortResult sort();

    };

}  // namespace NMR