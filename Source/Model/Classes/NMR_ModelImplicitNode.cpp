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

--*/

#include "Model/Classes/NMR_ModelImplicitNode.h"
#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Common/NMR_Exception.h"

namespace NMR
{
    // std::string elementNameFromNodeType(Lib3MF::eImplicitNodeType nodeType)
    // {
        

    //     switch (nodeType)
    //     {
    //     case Lib3MF::eImplicitNodeType::Addition:
    //         return "addition";
    //     case Lib3MF::eImplicitNodeType::Subtraction:
    //         return "subtraction";
    //     case Lib3MF::eImplicitNodeType::Multiplication:
    //         return "multiplication";
    //     case Lib3MF::eImplicitNodeType::Division:
    //         return "division";
    //     case Lib3MF::eImplicitNodeType::Constant:
    //         return "constant";
    //     case Lib3MF::eImplicitNodeType::ConstVec:
    //         return "constVec";
    //     case Lib3MF::eImplicitNodeType::ConstMat:
    //         return "constMat";
    //     case Lib3MF::eImplicitNodeType::ComposeVector:
    //         return "composeVector";
    //     case Lib3MF::eImplicitNodeType::ComposeMatrix:
    //         return "composeMatrix";
    //     case Lib3MF::eImplicitNodeType::ComposeMatrixFromColumnVectors:
    //         return "composeMatrixFromColumnVectors";
    //     case Lib3MF::eImplicitNodeType::DotProduct:
    //         return "dotProduct";
    //     case Lib3MF::eImplicitNodeType::CrossProduct:
    //         return "crossProduct";
    //     case Lib3MF::eImplicitNodeType::MatVecMultiplication:
    //         return "matVecMultiplication";
    //     case Lib3MF::eImplicitNodeType::Transpose:
    //         return "transpose";
    //     case Lib3MF::eImplicitNodeType::Sinus:
    //         return "sinus";
    //     case Lib3MF::eImplicitNodeType::Cosinus:
    //         return "cosinus";
    //     case Lib3MF::eImplicitNodeType::Tan:
    //         return "tan";
    //     case Lib3MF::eImplicitNodeType::ArcSin:
    //         return "arcSin";
    //     case Lib3MF::eImplicitNodeType::ArcCos:
    //         return "arcCos";
    //     case Lib3MF::eImplicitNodeType::ArcTan:
    //         return "arcTan";
    //     case Lib3MF::eImplicitNodeType::Min:
    //         return "min";
    //     case Lib3MF::eImplicitNodeType::Max:
    //         return "max";
    //     case Lib3MF::eImplicitNodeType::Abs:
    //         return "abs";
    //     case Lib3MF::eImplicitNodeType::Fmod:
    //         return "fmod";
    //     case Lib3MF::eImplicitNodeType::Pow:
    //         return "pow";
    //     case Lib3MF::eImplicitNodeType::Sqrt:
    //         return "sqrt";
    //     case Lib3MF::eImplicitNodeType::FunctionCall:
    //         return "functionCall";
    //     case Lib3MF::eImplicitNodeType::Dot:
    //         return "dot";
    //     case Lib3MF::eImplicitNodeType::Cross:
    //         return "cross";
    //     case Lib3MF::eImplicitNodeType::Mesh:
    //         return "mesh";
    //     case Lib3MF::eImplicitNodeType::Length:
    //         return "length";
    //     default:
    //         return "";
    //     }
    // }

    CModelImplicitNode::CModelImplicitNode(Lib3MF::eImplicitNodeType type,
                                           ImplicitIdentifier const & identifier,
                                           std::string const & displayname)
        : m_type(type)
        , m_identifier(identifier)
        , m_displayname(displayname)
    {
        m_outputs = std::make_shared<Ports>();
        m_inputs = std::make_shared<Ports>();
    }

    CModelImplicitNode::CModelImplicitNode(Lib3MF::eImplicitNodeType type)
        : m_type(type)
    {
        m_outputs = std::make_shared<Ports>();
        m_inputs = std::make_shared<Ports>();
    }

    ImplicitIdentifier const & CModelImplicitNode::getIdentifier() const
    {
        return m_identifier;
    }

    std::string const & CModelImplicitNode::getDisplayName() const
    {
        return m_displayname;
    }

    void CModelImplicitNode::setIdentifier(ImplicitIdentifier const & identifier)
    {
        m_identifier = identifier;
    }

    void CModelImplicitNode::setDisplayName(std::string const & displayname)
    {
        m_displayname = displayname;
    }

    void NMR::CModelImplicitNode::setType(Lib3MF::eImplicitNodeType type)
    {
        m_type = type;
    }

    Lib3MF::eImplicitNodeType CModelImplicitNode::getNodeType() const
    {
        return m_type;
    }

    PModelImplicitPort CModelImplicitNode::addInput(const std::string & sIdentifier,
                                                    const std::string & sDisplayName)
    {

        auto newPort = std::make_shared<CModelImplicitPort>(this, sIdentifier, sDisplayName);
        m_inputs->push_back(newPort);
        return newPort;
    }

    PModelImplicitPort CModelImplicitNode::addOutput(const std::string & sIdentifier,
                                                     const std::string & sDisplayName)
    {
        auto newPort = std::make_shared<CModelImplicitPort>(this, sIdentifier, sDisplayName);
        m_outputs->push_back(newPort);
        return newPort;
    }

    PPorts NMR::CModelImplicitNode::getInputs() const
    {
        return m_inputs;
    }

    PPorts CModelImplicitNode::getOutputs() const
    {
        return m_outputs;
    }

    PModelImplicitPort NMR::CModelImplicitNode::findInput(const std::string & sIdentifier)
    {
        for (auto & port : *m_inputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    PModelImplicitPort CModelImplicitNode::findOutput(const std::string & sIdentifier)
    {
        for (auto & port : *m_outputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    void NMR::CModelImplicitNode::setConstant(double value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::Constant)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_constant = value;
    }
    double CModelImplicitNode::getConstant() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::Constant)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        return m_constant;
    }

    void NMR::CModelImplicitNode::setVector(const Lib3MF::sVector & value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstVec)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_vector = std::make_unique<Lib3MF::sVector>(value);
    }

    Lib3MF::sVector CModelImplicitNode::getVector() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstVec)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (!m_vector)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        return *m_vector;
    }

    void CModelImplicitNode::setMatrix(const Lib3MF::sMatrix4x4 & value)
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstMat)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        m_matrix = std::make_unique<Lib3MF::sMatrix4x4>(value);
    }

    Lib3MF::sMatrix4x4 CModelImplicitNode::getMatrix() const
    {
        if (m_type != Lib3MF::eImplicitNodeType::ConstMat)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);

        if (!m_matrix)
            throw CNMRException(NMR_ERROR_INVALIDPARAM);
        return *m_matrix;
    }
}
