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
Reader for nodes of graph representing a function for implicit modelling

--*/

#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Node.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Node_Inputs.h"
#include "Model/Reader/Volumetric2201/NMR_ModelReaderNode_Implicit_Node_Outputs.h"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_ModelConstants.h"

#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"
#include "Common/NMR_StringUtils.h"

#include <sstream>

namespace NMR
{
    CModelReaderNode_Implicit_Node::CModelReaderNode_Implicit_Node(
      _In_ CModel * pModel,
      _In_ CModelImplicitNode * pImplicitNode,
      _In_ PModelWarnings pWarnings)
        : CModelReaderNode(pWarnings)
    {
        __NMRASSERT(pModel);
        __NMRASSERT(pImplicitNode);

        m_pModel = pModel;
        m_pImplicitNode = pImplicitNode;
    }

    void CModelReaderNode_Implicit_Node::parseXML(_In_ CXmlReader * pXMLReader)
    {
        __NMRASSERT(pXMLReader);

        // Parse name
        parseName(pXMLReader);

        // Parse Attributes
        parseAttributes(pXMLReader);

        // Parse Content
        parseContent(pXMLReader);
    }

    Lib3MF::sMatrix4x4 matFromString(std::string const & str)
    {
        Lib3MF::sMatrix4x4 mat;
        std::stringstream ss(str);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (!(ss >> mat.m_Field[i][j]))
                {
                    throw CNMRException(NMR_ERROR_NOTENOUGHVALUESINMATRIXSTRING);
                }
            }
        }

        // Check if there is any extra content in the input string
        std::string extra;
        if (ss >> extra && !extra.empty())
        {
            throw CNMRException(NMR_ERROR_TOOMANYVALUESINMATRIXSTRING);
        }

        return mat;
    }

    void CModelReaderNode_Implicit_Node::OnAttribute(_In_z_ const nfChar * pAttributeName,
                                                     _In_z_ const nfChar * pAttributeValue)
    {
        if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_ID) == 0)
        {
            m_pImplicitNode->setIdentifier(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_DISPLAY_NAME) == 0)
        {
            m_pImplicitNode->setDisplayName(pAttributeValue);
        }
        else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_TAG) == 0)
        {
            m_pImplicitNode->setTag(pAttributeValue);
        }
        else if (m_pImplicitNode->getNodeType() == Lib3MF::eImplicitNodeType::Constant &&
                 strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_VALUE) == 0)
        {
            m_pImplicitNode->setConstant(strtod(pAttributeValue, nullptr));
        }
        else if (m_pImplicitNode->getNodeType() == Lib3MF::eImplicitNodeType::ConstVec)
        {
            if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_X) == 0)
            {
                m_vector.m_Coordinates[0] = strtod(pAttributeValue, nullptr);
            }
            else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_Y) == 0)
            {
                m_vector.m_Coordinates[1] = strtod(pAttributeValue, nullptr);
            }
            else if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_Z) == 0)
            {
                m_vector.m_Coordinates[2] = strtod(pAttributeValue, nullptr);
            }
            m_pImplicitNode->setVector(m_vector);
        }
        else if (m_pImplicitNode->getNodeType() == Lib3MF::eImplicitNodeType::ConstMat)
        {
            if (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_MATRIX) == 0)
            {
                m_pImplicitNode->setMatrix(matFromString(pAttributeValue));
            }
        }
		else if (m_pImplicitNode->getNodeType() == Lib3MF::eImplicitNodeType::ConstResourceID)
		{
			if ((strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_RESOURCE_ID_VALUE_DEPRECATED) == 0) 
                || (strcmp(pAttributeName, XML_3MF_ATTRIBUTE_IMPLICIT_NODE_VALUE) == 0))
            {
                m_pImplicitNode->setModelResourceID(std::stoi(pAttributeValue));
            }
		}
    }

    void NMR::CModelReaderNode_Implicit_Node::OnNSChildElement(_In_z_ const nfChar * pChildName,
                                                               _In_z_ const nfChar * pNameSpace,
                                                               _In_ CXmlReader * pXMLReader)
    {
        __NMRASSERT(pChildName);
        __NMRASSERT(pXMLReader);
        __NMRASSERT(pNameSpace);

        if (strcmp(pChildName, XML_3MF_ELEMENT_IMPLICIT_NODE_INPUT) == 0)
        {
            auto pXMLNode =
              std::make_shared<CModelReaderNode_Implicit_Node_Inputs>(m_pImplicitNode, m_pWarnings);
            pXMLNode->parseXML(pXMLReader);
        }
        else if (strcmp(pChildName, XML_3MF_ELEMENT_IMPLICIT_NODE_OUTPUT) == 0)
        {
            auto pXMLNode = std::make_shared<CModelReaderNode_Implicit_Node_Outputs>(
              m_pImplicitNode, m_pWarnings);
            pXMLNode->parseXML(pXMLReader);
        }
    }
}
