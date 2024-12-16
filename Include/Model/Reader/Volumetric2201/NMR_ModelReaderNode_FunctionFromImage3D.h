/*++

Copyright (C) 2023 3MF Consortium

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
Reader for FunctionFromImage3D

--*/

#pragma once

#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_ModelFunctionFromImage3D.h"
#include "Model/Reader/NMR_ModelReaderNode.h"

namespace NMR
{
    class CModelReaderNode_FunctionFromImage3D : public CModelReaderNode
    {
      private:
        PModelFunctionFromImage3D m_pFunctionFromImage3D;
        CModel * m_pModel = nullptr;

        ModelResourceID m_nID = 0;
        ModelResourceID m_imageModelResourceID = 0;

        std::string m_displayName;

        eModelTextureTileStyle m_tileStyleU = {};
        eModelTextureTileStyle m_tileStyleV = {};
        eModelTextureTileStyle m_tileStyleW = {};

        eModelTextureFilter m_filter = {eModelTextureFilter::MODELTEXTUREFILTER_LINEAR};

        double m_offset = {};
        double m_scale = {1.};

      public:
        CModelReaderNode_FunctionFromImage3D() = delete;
        CModelReaderNode_FunctionFromImage3D(_In_ CModel * pModel, _In_ PModelWarnings pWarnings);

        void parseXML(_In_ CXmlReader * pXMLReader) override;

        void OnAttribute(_In_z_ const nfChar * pAttributeName,
                         _In_z_ const nfChar * pAttributeValue) override;

    };

    typedef std::shared_ptr<CModelReaderNode_FunctionFromImage3D>
      PModelReaderNode_FunctionFromImage3D;

}