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

NMR_ModelLevelSetObject.cpp implements the Model LevelSet Object
Class. A model levelset object is an in memory representation of the 3MF
levelset object.

--*/

#include "Model/Classes/NMR_ModelLevelSetObject.h"

#include "Common/Math/NMR_PairMatchingTree.h"
#include "Common/Mesh/NMR_Mesh.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelVolumeData.h"

namespace NMR
{

    CModelLevelSetObject::CModelLevelSetObject(
        const ModelResourceID sID, CModel* pModel)
        : CModelObject(sID, pModel)
    {
    }

    CModelLevelSetObject::~CModelLevelSetObject() {}

    void CModelLevelSetObject::mergeToMesh(CMesh* pMesh,
                                                const NMATRIX3 mMatrix)
    {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

    PModelMeshObject CModelLevelSetObject::getMesh()
    {
        return m_pMesh;
    }

    void CModelLevelSetObject::setMesh(PModelMeshObject pMesh)
    {
        if(!pMesh) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pMesh = pMesh;
    }

    nfBool CModelLevelSetObject::isValid()
    {
        return true;
    }

    nfBool CModelLevelSetObject::hasSlices(nfBool bRecursive)
    {
        return (this->getSliceStack().get() != nullptr);
    }

    nfBool CModelLevelSetObject::isValidForSlices(
        const NMATRIX3& totalParentMatrix)
    {
        if(!this->getSliceStack().get())
        {
            return true;
        }
        else
        {
            return fnMATRIX3_isplanar(totalParentMatrix);
        }
    }

    void CModelLevelSetObject::extendOutbox(
        NOUTBOX3& vOutBox, const NMATRIX3 mAccumulatedMatrix)
    {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

    PModelFunction CModelLevelSetObject::getFunction()
    {
        return m_pFunction;
    }

    void CModelLevelSetObject::setFunction(PModelFunction pFunction)
    {
        if(!pFunction) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pFunction = pFunction;
    }

    PModelVolumeData CModelLevelSetObject::getVolumeData()
    {
        return m_pVolumeData;
    }

    void CModelLevelSetObject::setVolumeData(
        _In_ PModelVolumeData pVolumeData)
    {
        if(!pVolumeData) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pVolumeData = pVolumeData;
    }

    void CModelLevelSetObject::setChannelName(std::string outputPortName)
    {
        m_outputPortName = std::move(outputPortName);
    }

    std::string const& CModelLevelSetObject::getChannelName() const
    {
        return m_outputPortName;
    }

    void CModelLevelSetObject::setTransform(NMR::NMATRIX3 transform)
    {
        m_transform = transform;
        m_bHasTransform = true;
    }

    NMR::NMATRIX3 CModelLevelSetObject::getTransform() const
    {
        return m_transform;
    }

    bool CModelLevelSetObject::hasTransform() const
    {
        return m_bHasTransform;
    }

    void CModelLevelSetObject::setMinFeatureSize(double minFeatureSize)
    {
        m_minFeatureSize = minFeatureSize;
    }

    double CModelLevelSetObject::getMinFeatureSize() const
    {
        return m_minFeatureSize;
    }

    void CModelLevelSetObject::setFallBackValue(double fallBackValue)
    {
        m_fallBackValue = fallBackValue;
    }

    double CModelLevelSetObject::getFallBackValue() const
    {
        return m_fallBackValue;
    }

    void CModelLevelSetObject::setMeshBBoxOnly(bool bMeshBBoxOnly)
    {
        m_meshBBoxOnly = bMeshBBoxOnly;
    }

    bool CModelLevelSetObject::getMeshBBoxOnly() const
    {
        return m_meshBBoxOnly;
    }
}  // namespace NMR
