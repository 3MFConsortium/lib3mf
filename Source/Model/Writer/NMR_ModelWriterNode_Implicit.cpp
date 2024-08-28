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
Writer node for implicit functions

--*/
#include "Model/Writer/NMR_ModelWriterNode_Implicit.h"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Common/NMR_Exception.h"
#include "lib3mf_types.hpp"

#include <sstream>

namespace NMR
{
    const implicit::NodeTypes CModelWriterNode_Implicit::m_nodeTypes;

    CModelWriterNode_Implicit::CModelWriterNode_Implicit(CModel * pModel,
                                                         CXmlWriter * pXMLWriter,
                                                         PProgressMonitor pProgressMonitor)
        : CModelWriterNode_ModelBase(pModel, pXMLWriter, pProgressMonitor)
    {
    }

    void CModelWriterNode_Implicit::writeImplicitFunctions()
    {
        nfUint32 const nCount = m_pModel->getFunctionCount();

        for (nfUint32 nIndex = 0; nIndex < nCount; nIndex++)
        {
            m_pProgressMonitor->IncrementProgress(1);
            CModelImplicitFunction * pFunction = m_pModel->getImplicitFunction(nIndex);
            if (!pFunction)
            {
                continue;
            }
            
            writeStartElementWithPrefix(XML_3MF_ELEMENT_IMPLICIT_FUNCTION,
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
            {
                writeIntAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_FUNCTION_ID,
                                  pFunction->getPackageResourceID()->getModelResourceID());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_FUNCTION_DISPLAY_NAME,
                                     pFunction->getDisplayName());

                writeImplicitFunctionElements(*pFunction);
            }
            writeFullEndElement();
        }
    }

    void CModelWriterNode_Implicit::writeToXML()
    {
        throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
    }

    void CModelWriterNode_Implicit::writeImplicitFunctionElements(CModelImplicitFunction & function)
    {
        writeImplicitFunctionInputs(*function.getInputs());

        for (auto node : *function.getNodes())
        {
            writeImplicitNode(*node);
        }
        
        writeImplicitFunctionOutputs(*function.getOutputs());
    }

    std::string mat4x4ToString(Lib3MF::sMatrix4x4 const & mat)
    {
        std::stringstream outStream;
        outStream << mat.m_Field[0][0] << " " << mat.m_Field[0][1] << " " << mat.m_Field[0][2] << " "
           << mat.m_Field[0][3] << " " << mat.m_Field[1][0] << " " << mat.m_Field[1][1] << " "
           << mat.m_Field[1][2] << " " << mat.m_Field[1][3] << " " << mat.m_Field[2][0] << " "
           << mat.m_Field[2][1] << " " << mat.m_Field[2][2] << " " << mat.m_Field[2][3] << " "
           << mat.m_Field[3][0] << " " << mat.m_Field[3][1] << " " << mat.m_Field[3][2] << " "
           << mat.m_Field[3][3];
        return outStream.str();
    }

   

    void CModelWriterNode_Implicit::writeImplicitNode(CModelImplicitNode & node)
    {
        
        auto name = m_nodeTypes.getNodeType(node.getNodeType()).getName();
        writeStartElementWithPrefix(name.c_str(),
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
        {
            writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_ID, node.getIdentifier());
            writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_DISPLAY_NAME,
                                 node.getDisplayName());
            writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_TAG,
                node.getTag());

            if (node.getNodeType() == Lib3MF::eImplicitNodeType::Constant)
            {

                writeDoubleAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_VALUE, node.getConstant());
            }
            else if (node.getNodeType() == Lib3MF::eImplicitNodeType::ConstVec)
            {
                auto vec = node.getVector();
                writeDoubleAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_X, vec.m_Coordinates[0]);
                writeDoubleAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_Y, vec.m_Coordinates[1]);
                writeDoubleAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_Z, vec.m_Coordinates[2]);
            }
            else if (node.getNodeType() == Lib3MF::eImplicitNodeType::ConstMat)
            {
            auto const mat = node.getMatrix();
            writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_MATRIX, mat4x4ToString(mat));
            }
            else if(node.getNodeType() == Lib3MF::eImplicitNodeType::ConstResourceID)
            {
				auto resource = node.getResource();
				if (resource != nullptr)
				{
					writeIntAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_NODE_VALUE, resource->getPackageResourceID()->getModelResourceID());
				}
				
            }

            bool const isNodeWithoutInputs = node.getNodeType() == Lib3MF::eImplicitNodeType::Constant ||
                node.getNodeType() == Lib3MF::eImplicitNodeType::ConstVec ||
                node.getNodeType() == Lib3MF::eImplicitNodeType::ConstMat ||
                node.getNodeType() == Lib3MF::eImplicitNodeType::ConstResourceID;

            if (!isNodeWithoutInputs)   // Certain node types are not allowed to have inputs, but may have been set by the producer
            {
                auto inputs = node.getInputs();
                if (!inputs->empty())
                {
                    writeStartElementWithPrefix(XML_3MF_ELEMENT_IMPLICIT_NODE_INPUT,
                                            XML_3MF_NAMESPACEPREFIX_IMPLICIT);
                    {
                        writeImplicitInputs(*inputs);
                    }
                    writeFullEndElement();
                }
            }

            writeStartElementWithPrefix(XML_3MF_ELEMENT_IMPLICIT_NODE_OUTPUT,
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
            {
                auto outputs = node.getOutputs();
                writeImplicitOutputs(*outputs);
            }
            writeFullEndElement();
        }
        writeFullEndElement();
    }

    const char * portTypeToRefName(Lib3MF::eImplicitPortType type)
    {
        switch (type)
        {
        case Lib3MF::eImplicitPortType::Scalar:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR_REF;
        case Lib3MF::eImplicitPortType::Vector:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR_REF;
        case Lib3MF::eImplicitPortType::Matrix:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX_REF;
		case Lib3MF::eImplicitPortType::ResourceID:
			return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_REF;
        default:
            return "Invalid";
        }
    }

    void NMR::CModelWriterNode_Implicit::writeImplicitInputs(NMR::Ports & ports)
    {
        for (auto & port : ports)
        {
            writeStartElementWithPrefix(portTypeToRefName(port->getType()),
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
            {
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_ID, port->getIdentifier());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_DISPLAY_NAME,
                                     port->getDisplayName());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_REFERENCE,
                                     port->getReference());
            }
            writeFullEndElement();
        }
    }

    const char * portTypeToName(Lib3MF::eImplicitPortType type)
    {
        switch (type)
        {
        case Lib3MF::eImplicitPortType::Scalar:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR;
        case Lib3MF::eImplicitPortType::Vector:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR;
        case Lib3MF::eImplicitPortType::Matrix:
            return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX;
		case Lib3MF::eImplicitPortType::ResourceID:
			return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_ID;
        default:
            return "Invalid";
        }
    }

    void CModelWriterNode_Implicit::writeImplicitFunctionInputs(NMR::Ports & ports)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_IMPLICIT_FUNCTION_INPUT,
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);

        for (auto & port : ports)
        {
            writeStartElement(portTypeToName(port->getType()));
            {
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_ID, port->getIdentifier());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_DISPLAY_NAME,
                                     port->getDisplayName());
            }
            writeFullEndElement();
        }
        writeFullEndElement();
    }

    void CModelWriterNode_Implicit::writeImplicitFunctionOutputs(NMR::Ports & ports)
    {
        writeStartElementWithPrefix(XML_3MF_ELEMENT_IMPLICIT_FUNCTION_OUTPUT,
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);

        for (auto & port : ports)
        {
            writeStartElementWithPrefix(portTypeToRefName(port->getType()),
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
            {
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_ID, port->getIdentifier());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_DISPLAY_NAME,
                                     port->getDisplayName());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_REFERENCE,
                                     port->getReference());
            }
            writeFullEndElement();
        }
        writeFullEndElement();
    }

    void NMR::CModelWriterNode_Implicit::writeImplicitOutputs(NMR::Ports & ports)
    {
        for (auto & port : ports)
        {
            writeStartElementWithPrefix(portTypeToName(port->getType()),
                                        XML_3MF_NAMESPACEPREFIX_IMPLICIT);
            {
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_ID, port->getIdentifier());
                writeStringAttribute(XML_3MF_ATTRIBUTE_IMPLICIT_PORT_DISPLAY_NAME,
                                     port->getDisplayName());
            }
            writeFullEndElement();
        }
    }
}
