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
Reader for an output element

--*/

#pragma once

#include "Model/Classes/NMR_ModelImplicitFunction.h"
#include "Model/Reader/NMR_ModelReaderNode.h"

namespace NMR
{
    class CModelReaderNode_Implicit_Function_Output : public CModelReaderNode
    {
      private:
        CModelImplicitFunction * m_pParentFunction;

        std::string m_identifier;
        std::string m_displayName;
        std::string m_reference;
        Lib3MF::eImplicitPortType m_portType;
        void createOutput();
      public:
        CModelReaderNode_Implicit_Function_Output() = delete;
        CModelReaderNode_Implicit_Function_Output(CModelImplicitFunction * pParentFunction,
                                       PModelWarnings pWarnings, 
                                       Lib3MF::eImplicitPortType portType
                                       );

        void parseXML(CXmlReader * pXMLReader) override;

        void OnAttribute(const nfChar * pAttributeName, const nfChar * pAttributeValue) override;

        
    };

    typedef std::shared_ptr<CModelReaderNode_Implicit_Function_Output> PModelReaderNode_Implicit_Function_Output;
}