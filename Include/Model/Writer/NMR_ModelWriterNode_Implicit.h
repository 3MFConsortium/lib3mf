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


--*/

#pragma once

#include "Common/3MF_ProgressMonitor.h"
#include "Common/Platform/NMR_XmlWriter.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Model/Classes/NMR_ModelImplicitNode.h"
#include "Model/Writer/NMR_ModelWriterNode_ModelBase.h"

namespace NMR
{
    namespace implicit
    {
        class NodeTypes;
    }

    class CModelWriterNode_Implicit : public CModelWriterNode_ModelBase
    {

      public:
        CModelWriterNode_Implicit() = delete;
        CModelWriterNode_Implicit(CModel * pModel,
                                  CXmlWriter * pXMLWriter,
                                  PProgressMonitor pProgressMonitor);

        void writeImplicitFunctions();

        void writeToXML() override;

      private:
        static const implicit::NodeTypes m_nodeTypes;

        void writeImplicitFunctionElements(CModelImplicitFunction & function);
        void writeImplicitNode(CModelImplicitNode & node);
        void writeImplicitOutputs(NMR::Ports & ports);
        void writeImplicitInputs(NMR::Ports & ports);

        void writeImplicitFunctionInputs(NMR::Ports & ports);
        void writeImplicitFunctionOutputs(NMR::Ports & ports);
    };

}
