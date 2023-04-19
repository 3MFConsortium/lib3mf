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

#include "Model/Classes/NMR_ImplicitNodeTypes.h"
#include "Common/NMR_Exception.h"
#include "Model/Classes/NMR_ModelImplicitNode.h"

namespace NMR
{
    namespace implicit
    {

        NodeType::NodeType(std::string const & name,
                           ExpectedPorts const & inputs,
                           ExpectedPorts const & outputs)
            : m_name(name)
            , m_inputs(inputs)
            , m_outputs(outputs)

        {
        }

        std::string const & NodeType::getName() const
        {
            return m_name;
        }

        ExpectedPorts const & NodeType::getInputs() const
        {
            return m_inputs;
        }

        ExpectedPorts const & NodeType::getOutputs() const
        {
            return m_outputs;
        }
        using namespace Lib3MF;
        NodeTypes::NodeTypes()
            : m_nodeTypes{
                // clang-format off
                // type, {name, {inputs}, {outputs}}
                {eImplicitNodeType::Addition,    {"addition", {"A", "B"}, {"sum"}}},
                {eImplicitNodeType::Subtraction, {"subtraction", {"A", "B"}, {"difference"}}},
                {eImplicitNodeType::Multiplication, {"multiplication", {"A", "B"}, {"product"}}},
                {eImplicitNodeType::Division, {"division", {"A", "B"}, {"quotient"}}},
                {eImplicitNodeType::Constant, {"constant", {}, {"value"}}},
                {eImplicitNodeType::ConstVec, {"constvec", {}, {"value"}}},
                {eImplicitNodeType::ConstMat, {"constmat", {}, {"value"}}},
                {eImplicitNodeType::ComposeVector, {"composeVector", {"A", "B", "C"}, {"value"}}},
                {eImplicitNodeType::ComposeMatrix, {"composeMatrix", {"A", "B", "C", "D", "E", "F", "G", "H", "I"}, {"value"}}},
                {eImplicitNodeType::ComposeMatrixFromColumnVectors, {"composeMatrixFromColumnVectors", {"A", "B", "C"}, {"value"}}},
                {eImplicitNodeType::DotProduct, {"dotproduct", {"A", "B"}, {"product"}}},
                {eImplicitNodeType::CrossProduct, {"crossproduct", {"A", "B"}, {"crossproduct"}}},
                {eImplicitNodeType::MatVecMultiplication, {"matrixvectormultiplication", {"A", "B"}, {"product"}}},
                {eImplicitNodeType::Transpose, {"transpose", {"A"}, {"transposed"}}},
                {eImplicitNodeType::Sinus, {"sinus", {"A"}, {"sinus"}}},
                {eImplicitNodeType::Cosinus, {"cosinus", {"A"}, {"cosinus"}}},
                {eImplicitNodeType::Tan, {"tan", {"A"}, {"tan"}}},
                {eImplicitNodeType::ArcSin, {"arcsin", {"A"}, {"arcsin"}}},
                {eImplicitNodeType::ArcCos, {"arccos", {"A"}, {"arccos"}}},
                {eImplicitNodeType::ArcTan, {"arctan", {"A"}, {"arctan"}}},
                {eImplicitNodeType::Min, {"min", {"A", "B"}, {"min"}}},
                {eImplicitNodeType::Max, {"max", {"A", "B"}, {"max"}}},
                {eImplicitNodeType::Abs, {"abs", {"A"}, {"abs"}}},
                {eImplicitNodeType::Fmod, {"fmod", {"A", "B"}, {"fmod"}}},
                {eImplicitNodeType::Pow, {"pow", {"A", "B"}, {"pow"}}},
                {eImplicitNodeType::Sqrt, {"sqrt", {"A"}, {"sqrt"}}},
                {eImplicitNodeType::Mesh, {"mesh", {"point", "mesh"}, {"value"}}}
                // clang-format on
              }
        {
        }

        NodeType const & NodeTypes::getNodeType(Lib3MF::eImplicitNodeType type) const
        {
            auto it = m_nodeTypes.find(type);
            if (it == m_nodeTypes.end())
            {
                throw CNMRException(NMR_ERROR_UNKNOWN_NODETYPE_IMPLICITMODEL);
            }
            return it->second;
        }
        NodeTypesMap const & NodeTypes::getTypes() const
        {
            return m_nodeTypes;
        }

        void NodeTypes::addExpectedPortsToNode(NMR::CModelImplicitNode & node) const
        {
            auto const & nodeType = getNodeType(node.getNodeType());

            for (auto const & input : nodeType.getInputs())
            {
                node.addInput(input, input);
            }
            for (auto const & output : nodeType.getOutputs())
            {
                node.addOutput(output, output);
            }
        }

    }
}
