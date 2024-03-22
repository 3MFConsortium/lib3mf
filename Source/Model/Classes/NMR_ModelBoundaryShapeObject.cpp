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

NMR_ModelBoundaryShapeObject.cpp implements the Model BoundaryShape Object
Class. A model boundaryshape object is an in memory representation of the 3MF
boundaryshape object.

--*/

#include "Model/Classes/NMR_ModelBoundaryShapeObject.h"

#include "Common/Math/NMR_PairMatchingTree.h"
#include "Common/Mesh/NMR_Mesh.h"
#include "Model/Classes/NMR_ModelMeshObject.h"
#include "Model/Classes/NMR_ModelObject.h"
#include "Model/Classes/NMR_ModelVolumeData.h"

namespace NMR
{

    CModelBoundaryShapeObject::CModelBoundaryShapeObject(
        const ModelResourceID sID, CModel* pModel)
        : CModelObject(sID, pModel)
    {
        m_pVolumeData = std::make_shared<CModelVolumeData>();
    }

    CModelBoundaryShapeObject::~CModelBoundaryShapeObject() {}

    void CModelBoundaryShapeObject::mergeToMesh(CMesh* pMesh,
                                                const NMATRIX3 mMatrix)
    {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

    PModelMeshObject CModelBoundaryShapeObject::getMesh()
    {
        return m_pMesh;
    }

    void CModelBoundaryShapeObject::setMesh(PModelMeshObject pMesh)
    {
        if(!pMesh) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pMesh = pMesh;
    }

    nfBool CModelBoundaryShapeObject::isValid()
    {
        return true;
    }

    nfBool CModelBoundaryShapeObject::hasSlices(nfBool bRecursive)
    {
        return (this->getSliceStack().get() != nullptr);
    }

    nfBool CModelBoundaryShapeObject::isValidForSlices(
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

    void CModelBoundaryShapeObject::extendOutbox(
        NOUTBOX3& vOutBox, const NMATRIX3 mAccumulatedMatrix)
    {
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

    PModelFunction CModelBoundaryShapeObject::getFunction()
    {
        return m_pFunction;
    }

    void CModelBoundaryShapeObject::setFunction(PModelFunction pFunction)
    {
        if(!pFunction) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pFunction = pFunction;
    }

    PModelVolumeData CModelBoundaryShapeObject::getVolumeData()
    {
        return m_pVolumeData;
    }

    void CModelBoundaryShapeObject::setVolumeData(
        _In_ PModelVolumeData pVolumeData)
    {
        if(!pVolumeData) throw CNMRException(NMR_ERROR_INVALIDPARAM);

        m_pVolumeData = pVolumeData;
    }

    void CModelBoundaryShapeObject::setChannelName(std::string outputPortName)
    {
        m_outputPortName = std::move(outputPortName);
    }

    std::string const& CModelBoundaryShapeObject::getChannelName() const
    {
        return m_outputPortName;
    }

    void CModelBoundaryShapeObject::setTransform(NMR::NMATRIX3 transform)
    {
        m_transform = transform;
        m_bHasTransform = true;
    }

    NMR::NMATRIX3 CModelBoundaryShapeObject::getTransform() const
    {
        return m_transform;
    }

    bool CModelBoundaryShapeObject::hasTransform() const
    {
        return m_bHasTransform;
    }

    void CModelBoundaryShapeObject::setMinFeatureSize(double minFeatureSize)
    {
        m_minFeatureSize = minFeatureSize;
    }

    double CModelBoundaryShapeObject::getMinFeatureSize() const
    {
        return m_minFeatureSize;
    }

    void CModelBoundaryShapeObject::setFallBackValue(double fallBackValue)
    {
        m_fallBackValue = fallBackValue;
    }

    double CModelBoundaryShapeObject::getFallBackValue() const
    {
        return m_fallBackValue;
    }

    void CModelBoundaryShapeObject::setMeshBBoxOnly(bool bMeshBBoxOnly)
    {
        m_meshBBoxOnly = bMeshBBoxOnly;
    }

    bool CModelBoundaryShapeObject::getMeshBBoxOnly() const
    {
        return m_meshBBoxOnly;
    }
}  // namespace NMR
