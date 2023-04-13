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
NMR_ModelReaderNode_Volumetric2201_Image3D.h covers the official 3MF volumetric extension.

--*/

#pragma once

#include "Model/Classes/NMR_ModelImplicitNode.h"
#include "Model/Reader/NMR_ModelReaderNode.h"

namespace NMR
{
    class CModelReaderNode_Volumetric2201_ImplicitNode : public CModelReaderNode
    {
      private:
        CModelImplicitNode * m_pImplicitNode;
        CModel * m_pModel;

      public:
        CModelReaderNode_Volumetric2201_ImplicitNode() = delete;
        CModelReaderNode_Volumetric2201_ImplicitNode(_In_ CModel * pModel,
                                                     _In_ CModelImplicitNode * pImplicitNode,
                                                     _In_ PModelWarnings pWarnings);

        void parseXML(_In_ CXmlReader * pXMLReader) override;

        void OnAttribute(_In_z_ const nfChar * pAttributeName,
                         _In_z_ const nfChar * pAttributeValue) override;

        void OnNSChildElement(_In_z_ const nfChar * pChildName,
                              _In_z_ const nfChar * pNameSpace,
                              _In_ CXmlReader * pXMLReader) override;
    };

    typedef std::shared_ptr<CModelReaderNode_Volumetric2201_ImplicitNode>
      PModelReaderNode_Volumetric2201_ImplicitNode;
}