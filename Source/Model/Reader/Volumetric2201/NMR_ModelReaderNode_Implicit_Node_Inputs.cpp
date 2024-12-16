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
Reader for ports of a node of a graph representing a function for implicit modelling

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Node_Inputs.h"
#include "Model/Reader/Volumetric2201/NMR_Implicit_PortType_Convert.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Port.h"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR
{
    CModelReaderNode_Implicit_Node_Inputs::CModelReaderNode_Implicit_Node_Inputs(
      _In_ CModelImplicitNode * pParentNode,
      _In_ PModelWarnings pWarnings)
        : CModelReaderNode(pWarnings)
    {
        __NMRASSERT(pParentNode);
        m_pImplicitNode = pParentNode;
    }

    void CModelReaderNode_Implicit_Node_Inputs::parseXML(_In_ CXmlReader * pXMLReader)
    {
        __NMRASSERT(pXMLReader);

        // Parse name
        parseName(pXMLReader);

        // Parse Content
        parseContent(pXMLReader);
    }

    void CModelReaderNode_Implicit_Node_Inputs::OnNSChildElement(const nfChar * pChildName,
                                                                 const nfChar * pNameSpace,
                                                                 CXmlReader * pXMLReader)
    {
        Lib3MF::eImplicitPortType portType;
        if (implicit::portTypeFromRefName(pChildName, portType))
        {
            auto pXMLNode = CModelReaderNode_Implicit_Port(
              m_pImplicitNode, m_pWarnings, portType, ImplicitPortInOut::Input);
            pXMLNode.parseXML(pXMLReader);
        }
    }
}
