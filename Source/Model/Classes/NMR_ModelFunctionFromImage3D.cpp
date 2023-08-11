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

#include "Model/Classes/NMR_ModelFunctionFromImage3D.h"

namespace NMR
{
    NMR::CModelFunctionFromImage3D::CModelFunctionFromImage3D(const ModelResourceID sID,
                                                              CModel * pModel)
        : CModelFunction(sID, pModel)
    {
    }

    void CModelFunctionFromImage3D::setImage3DUniqueResourceID(const UniqueResourceID sID)
    {
        m_image3DUniqueResourceID = sID;
    }

    UniqueResourceID CModelFunctionFromImage3D::getImage3DUniqueResourceID()
    {
        return m_image3DUniqueResourceID;
    }

    void CModelFunctionFromImage3D::setImage3DModelResourceID(
        const ModelResourceID modelResoureId)
    {
        auto * model = getModel();
        PModelResource pResource = model->findResource(model->currentPath(), modelResoureId);
        if (pResource == nullptr)
            throw CNMRException(NMR_ERROR_RESOURCENOTFOUND);
        m_image3DUniqueResourceID = pResource->getPackageResourceID()->getUniqueID();
        
    }

    void CModelFunctionFromImage3D::setTileStyleU(
        const eModelTextureTileStyle tileStyleU)
    {
        m_tileStyleU = tileStyleU;
    }

    eModelTextureTileStyle CModelFunctionFromImage3D::getTileStyleU()
    {
        return m_tileStyleU;
    }

    void CModelFunctionFromImage3D::setTileStyleV(const eModelTextureTileStyle tileStyleV)
    {
        m_tileStyleV = tileStyleV;
    }

    eModelTextureTileStyle CModelFunctionFromImage3D::getTileStyleV()
    {
        return m_tileStyleV;
    }

    void CModelFunctionFromImage3D::setTileStyleW(const eModelTextureTileStyle tileStyleW)
    {
        m_tileStyleW = tileStyleW;
    }

    eModelTextureTileStyle CModelFunctionFromImage3D::getTileStyleW()
    {
        return m_tileStyleW;
    }

    void CModelFunctionFromImage3D::setFilter(const eModelTextureFilter filter)
    {
        m_filter = filter;
    }

    eModelTextureFilter CModelFunctionFromImage3D::getFilter()
    {
        return m_filter;
    }

    void CModelFunctionFromImage3D::setOffset(double offset)
    {
        m_offset = offset;
    }

    double CModelFunctionFromImage3D::getOffset()
    {
        return m_offset;
    }

    void CModelFunctionFromImage3D::setScale(double scale)
    {
        m_scale = scale;
    }

    double CModelFunctionFromImage3D::getScale()
    {
        return m_scale;
    }
} // namespace NMR