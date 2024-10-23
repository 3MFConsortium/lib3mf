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
Writer node for LevelSet resources

--*/
#include "Model/Writer/NMR_ModelWriterNode_LevelSet.h"

#include <sstream>

#include "Common/NMR_Exception.h"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_ModelLevelSetObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "lib3mf_types.hpp"

namespace NMR
{
    CModelWriterNode_LevelSet::CModelWriterNode_LevelSet(
        CModel* pModel, 
        CModelLevelSetObject * pLevelSet,
        CXmlWriter* pXMLWriter,
        PProgressMonitor pProgressMonitor)
        : CModelWriterNode_ModelBase(pModel, pXMLWriter, pProgressMonitor), m_pLevelSet(pLevelSet)
    {
    }

    void CModelWriterNode_LevelSet::writeLevelSetResources()
    {
        nfUint32 const nCount = m_pModel->getObjectCount();

        for(nfUint32 nIndex = 0; nIndex < nCount; nIndex++)
        {
            m_pProgressMonitor->IncrementProgress(1);
            CModelLevelSetObject* pLevelSet =
                dynamic_cast<CModelLevelSetObject*>(
                    m_pModel->getObject(nIndex));
            if(!pLevelSet)
            {
                continue;
            }
            writeLevelSetResource(*pLevelSet);
        }
    }

    void CModelWriterNode_LevelSet::writeLevelSetResource(
        CModelLevelSetObject& levelSet)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_BOUNDARY_SHAPE, XML_3MF_NAMESPACEPREFIX_VOLUMETRIC);

        auto function = levelSet.getFunction();
        if(!function)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }

        PPackageResourceID functionPackageID = function->getPackageResourceID();

        

        if(!functionPackageID) throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        writeIntAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_FUNCTIONID,
                          functionPackageID->getModelResourceID());


        PModelMeshObject mesh = levelSet.getMesh();
        if(!mesh)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }

        PPackageResourceID meshPackageID = mesh->getPackageResourceID();
        if(!meshPackageID)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }
        writeIntAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MESH_ID,
                          meshPackageID->getModelResourceID());

        if(!fnMATRIX3_isIdentity(levelSet.getTransform()))
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_TRANSFORM,
                                 fnMATRIX3_toString(levelSet.getTransform()));
        }

        if(!levelSet.getChannelName().empty())
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_CHANNEL,
                                 levelSet.getChannelName());
        }
        else
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_VOLUMEDATA_CHANNEL,
                                 XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_CHANNEL_DEFAULT);
        }

        writeStringAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MESH_BBOX_ONLY,
                             levelSet.getMeshBBoxOnly() ? "true" : "false");
        if(levelSet.getMinFeatureSize() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MIN_FEATURE_SIZE,
                                float(levelSet.getMinFeatureSize()));
        }

        if(levelSet.getFallBackValue() != 0.0)
        {
            writeFloatAttribute(XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_FALLBACK_VALUE,
                                float(levelSet.getFallBackValue()));
        }
               	
        auto pVolumeData = levelSet.getVolumeData();
        if (pVolumeData) {
            PPackageResourceID pID = pVolumeData->getPackageResourceID();
            if (pID->getPath() != m_pModel->currentPath())
                throw CNMRException(NMR_ERROR_MODELRESOURCE_IN_DIFFERENT_MODEL);
            writeIntAttribute(XML_3MF_ATTRIBUTE_LEVELSET_VOLUMEDATA, pID->getModelResourceID());
        }
        writeFullEndElement();
    }
    void CModelWriterNode_LevelSet::writeToXML()
    {
        if (m_pLevelSet)
        {
            writeLevelSetResource(*m_pLevelSet);
        }
    }

}  // namespace NMR
