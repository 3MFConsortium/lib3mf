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

#include "Model/Writer/v100/NMR_ResourceDependencySorter.h"

#include "Common/Graph/DirectedGraph.h"
#include "Common/Graph/GraphAlgorithms.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelResource.h"

namespace NMR
{

    CResourceDependencySorter::CResourceDependencySorter(CModel* pModel)
        : m_pModel(pModel)
    {
        buildIndexMaps();
    }

    TopologicalSortResult CResourceDependencySorter::sort()
    {
        // gather list of resources that are standalone
        TopologicalSortResult sortedResources;

        for(nfUint32 i = 0; i < m_pModel->getResourceCount(); i++)
        {
            auto pResource = m_pModel->getResource(i);
            if(pResource->getDependencies().empty())
            {
                sortedResources.push_back(
                    pResource->getPackageResourceID());
            }
        }

        auto graph = buildGraph();
        if (common::graph::isCyclic(graph))
        {
            throw CNMRException(NMR_ERROR_INVALIDPARAM, "Cyclic dependency between resources detected");
        }
        auto sortedIndices = common::graph::topologicalSort(
            buildGraph());  // only contains resources that have or are a
                            // dependency

        for (auto index : sortedIndices)
        {
            
            auto modelResourceID = indexToModelResourceID(index);
            if (std::find(sortedResources.begin(), sortedResources.end(), modelResourceID) == sortedResources.end()) {
                sortedResources.push_back(modelResourceID);
            }
        }
        return sortedResources;
    }

    common::graph::DirectedGraph CResourceDependencySorter::buildGraph()
    {
        common::graph::DirectedGraph graph(m_pModel->getResourceCount());

        for(nfUint32 i = 0; i < m_pModel->getResourceCount(); i++)
        {
            auto pResource = m_pModel->getResource(i);
            auto dependencies = pResource->getDependencies();
            for(auto dependency : dependencies)
            {
                auto dependencyIndex = modelResourceIDToIndex(dependency);
                graph.addDependency(i, dependencyIndex);
            }
        }

        return graph;
    }

    void CResourceDependencySorter::buildIndexMaps()
    {
        for(nfUint32 i = 0; i < m_pModel->getResourceCount(); i++)
        {
            auto pResource = m_pModel->getResource(i);
            m_indexToResourceID[i] =
                pResource->getPackageResourceID();
            m_resourceIDToIndex[pResource->getPackageResourceID()] = i;
        }
    }

    PPackageResourceID CResourceDependencySorter::indexToModelResourceID(
        size_t index) const
    {
        return m_indexToResourceID.at(index);
    }

    size_t CResourceDependencySorter::modelResourceIDToIndex(
        PPackageResourceID resourceID) const
    {
        return m_resourceIDToIndex.at(resourceID);
    }

}  // namespace NMR