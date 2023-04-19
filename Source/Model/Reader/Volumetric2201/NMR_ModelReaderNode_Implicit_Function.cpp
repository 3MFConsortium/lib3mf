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
Reader for implicit function ressources

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Function.h"

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Node.h"

#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"
#include "Model/Classes/NMR_ModelConstants_Implicit.h"
#include "Model/Classes/NMR_ModelImplicitFunction.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

namespace NMR
{

    const implicit::NodeTypes CModelReaderNode_ImplicitFunction::m_nodeTypes;

    NMR::CModelReaderNode_ImplicitFunction::CModelReaderNode_ImplicitFunction(
      CModel * pModel,

      PModelWarnings pWarnings)
        : CModelReaderNode(pWarnings)
        , m_pModel(pModel)
    {
        __NMRASSERT(pModel);
    }

    void CModelReaderNode_ImplicitFunction::parseXML(CXmlReader * pXMLReader)
    {
        __NMRASSERT(pXMLReader);

        // Parse name
        parseName(pXMLReader);

        // Parse Attributes
        parseAttributes(pXMLReader);

        // Create Implicit Function
        m_pImplicitFunction = std::make_shared<CModelImplicitFunction>(m_nID, m_pModel);
        m_pImplicitFunction->setDisplayName(m_displayName);

        // Parse Content
        parseContent(pXMLReader);

        m_pModel->addResource(m_pImplicitFunction);
    }

    void CModelReaderNode_ImplicitFunction::OnAttribute(const nfChar * pAttributeName,
                                                        const nfChar * pAttributeValue)
    {

        if (strcmp(pAttributeName, XML_3MF_ELEMENT_IMPLICIT_FUNCTION_ID) == 0)
        {
            m_nID = fnStringToUint32(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICITNODE_DISPLAYNAME) == 0)
        {
            m_displayName = pAttributeValue;
        }
    }
    void CModelReaderNode_ImplicitFunction::OnNSChildElement(const nfChar * pChildName,
                                                             const nfChar * pNameSpace,
                                                             CXmlReader * pXMLReader)
    {
        __NMRASSERT(pChildName);
        __NMRASSERT(pXMLReader);
        __NMRASSERT(pNameSpace);

        auto const & nodeTypes = m_nodeTypes.getTypes();

        for (auto const & nodeType : nodeTypes) // maeh, no structured bindings in C++11
        {
            // first is the node type, second is the node type info
            if (strcmp(pChildName, nodeType.second.getName().c_str()) == 0)
            {
                auto implicitNode = m_pImplicitFunction->addNode(nodeType.first);
                auto node = std::make_shared<NMR::CModelReaderNode_Implicit_Node>(
                  m_pModel, implicitNode.get(), m_pWarnings);
                node->parseXML(pXMLReader);

                return;
            }
        }
    }
}
