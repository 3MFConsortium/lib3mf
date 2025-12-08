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
Reader for boundary shape objects

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_LevelSet.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"
#include "Model/Classes/NMR_ModelLevelSetObject.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelResource.h"
#include "Model/Classes/NMR_ModelConstants.h"

namespace NMR
{

    NMR::CModelReaderNode_LevelSet::CModelReaderNode_LevelSet(
       CModel * pModel, PModelLevelSetObject levelSet,
       PModelWarnings pWarnings,  PProgressMonitor pProgressMonitor)
        : CModelReaderNode(pWarnings, pProgressMonitor), m_parentModel(pModel), m_pLevelSet(std::move(levelSet))
    {
        
    }

    void CModelReaderNode_LevelSet::parseXML(CXmlReader* pXMLReader)
    {
        __NMRASSERT(pXMLReader);
        // Parse name
        parseName(pXMLReader);

        // Parse Attributes
        parseAttributes(pXMLReader);

        // Parse Content
        parseContent(pXMLReader);

        PPackageResourceID functionPackageId = m_parentModel->findPackageResourceID(
            m_parentModel->currentPath(), m_nFunctionID);
        if(!functionPackageId.get())
        {
            throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
        }

        auto pFunction = m_parentModel->findFunction(functionPackageId->getUniqueID());
        if(!pFunction)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }

        m_pLevelSet->setFunction(pFunction);

        PPackageResourceID meshPackageId = m_parentModel->findPackageResourceID(
            m_parentModel->currentPath(), m_nMeshID);

        if(!meshPackageId.get())
        {
            throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
        }

        PModelResource pMeshResource = m_parentModel->findResource(meshPackageId);
        
        if(!pMeshResource)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }

        PModelMeshObject pMeshObject = std::dynamic_pointer_cast<CModelMeshObject>(pMeshResource);

        if(!pMeshObject)
        {
            throw CNMRException(NMR_ERROR_INVALIDMODELRESOURCE);
        }

        m_pLevelSet->setMesh(pMeshObject);
    	
        if(m_bHasTransform)
        {
            m_pLevelSet->setTransform(m_Transform);
        }

        if(m_bHasChannel)
        {
            m_pLevelSet->setChannelName(m_sChannel);
        }

        if(m_bHasMeshBBoxOnly)
        {
            m_pLevelSet->setMeshBBoxOnly(m_meshBBoxOnly);
        }

        if(m_bHasMinFeatureSize)
        {
            m_pLevelSet->setMinFeatureSize(m_dMinFeatureSize);
        }

        if(m_bHasFallBackValue)
        {
            m_pLevelSet->setFallBackValue(m_dFallBackValue);
        }

        if(m_bHasVolumeDataID)
        {
            PPackageResourceID volumePackageId =
                m_parentModel->findPackageResourceID(m_parentModel->currentPath(),
                                                m_nVolumeDataID);
            
            if(!volumePackageId.get())
            {
                throw CNMRException(NMR_ERROR_UNKNOWNMODELRESOURCE);
            }

            auto pVolumeData = m_parentModel->findVolumeData(
                volumePackageId->getUniqueID());

            m_pLevelSet->setVolumeData(pVolumeData);
        }
    }

    void CModelReaderNode_LevelSet::OnAttribute(
        const nfChar* pAttributeName, const nfChar* pAttributeValue)
    {        
        if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_ID) == 0)
        {
            m_resID = fnStringToUint32(pAttributeValue);
        }
        
        if(strcmp(pAttributeName,
                  XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_FUNCTION_ID) == 0)
        {
            if(m_bHasFunctionID)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_FUNCTION_ID);
            }
            m_bHasFunctionID = true;
            m_nFunctionID = fnStringToUint32(pAttributeValue);
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_CHANNEL) == 0)
        {
            if(m_bHasChannel)
            {
                throw CNMRException(NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_CHANNEL);
            }
            m_bHasChannel = true;
            m_sChannel = pAttributeValue;
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_TRANSFORM) == 0)
        {
            if(m_bHasTransform)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_TRANSFORM);
            }
            m_bHasTransform = true;
            m_Transform = fnMATRIX3_fromString(pAttributeValue);
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MIN_FEATURE_SIZE) == 0)
        {
            if(m_bHasMinFeatureSize)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_MIN_FEATURE_SIZE);
            }
            m_bHasMinFeatureSize = true;
            m_dMinFeatureSize = fnStringToDouble(pAttributeValue);
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MESH_BBOX_ONLY) == 0)
        {
            m_bHasMeshBBoxOnly = true;
            m_meshBBoxOnly = !(std::string(pAttributeValue) == "false");
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_FALLBACK_VALUE) == 0)
        {
            if(m_bHasFallBackValue)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_FALLBACK_VALUE);
            }
            m_bHasFallBackValue = true;
            m_dFallBackValue = fnStringToDouble(pAttributeValue);
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_BOUNDARY_SHAPE_MESH_ID) == 0)
        {
            if(m_bHasMeshID)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_MESH_ID);
            }
            m_bHasMeshID = true;
            m_nMeshID = fnStringToUint32(pAttributeValue);
        }
        else if(strcmp(pAttributeName,
                       XML_3MF_ATTRIBUTE_MESH_VOLUMEDATA) == 0)
        {
            if(m_bHasVolumeDataID)
            {
                throw CNMRException(
                    NMR_ERROR_DUPLICATE_BOUNDARY_SHAPE_VOLUME_ID);
            }
            m_bHasVolumeDataID = true;
            m_nVolumeDataID = fnStringToUint32(pAttributeValue);
        }
    }
    void CModelReaderNode_LevelSet::OnNSChildElement(
        const nfChar* pChildName, const nfChar* pNameSpace,
        CXmlReader* pXMLReader)
    {
        __NMRASSERT(pChildName);
        __NMRASSERT(pXMLReader);
        __NMRASSERT(pNameSpace);
    }
}  // namespace NMR
