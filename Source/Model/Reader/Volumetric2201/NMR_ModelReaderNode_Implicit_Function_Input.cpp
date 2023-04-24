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
Reader for a single input of a function for implicit modelling

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Function_Input.h"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <iostream>

namespace NMR
{
    CModelReaderNode_Implicit_Function_Input::CModelReaderNode_Implicit_Function_Input(
      CModelImplicitFunction * pParentFunction,
      PModelWarnings pWarnings,
      Lib3MF::eImplicitPortType portType)
        : CModelReaderNode(pWarnings)
        , m_portType(portType)
    {

        __NMRASSERT(pParentFunction);
        m_pParentFunction = pParentFunction;
    }

    void CModelReaderNode_Implicit_Function_Input::parseXML(CXmlReader * pXMLReader)
    {
        __NMRASSERT(pXMLReader);

        // Parse name
        parseName(pXMLReader);

        // Parse Attributes
        parseAttributes(pXMLReader);

        // Parse Content
        parseContent(pXMLReader);

        // Create Implicit Port
        createInput();
    }

    void CModelReaderNode_Implicit_Function_Input::OnAttribute(const nfChar * pAttributeName,
                                                               const nfChar * pAttributeValue)
    {
        if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_PORT_ID) == 0)
        {
            m_identifier = pAttributeValue;
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_PORT_DISPLAY_NAME) == 0)
        {
            m_displayName = pAttributeValue;
        }
    }

    void CModelReaderNode_Implicit_Function_Input::createInput()
    {
        auto input = m_pParentFunction->findInput(m_identifier);
        if (!input)
        {
            input = m_pParentFunction->addInput(m_identifier, m_displayName, m_portType);
        }
        input->setDisplayName(m_displayName);
    }

}
