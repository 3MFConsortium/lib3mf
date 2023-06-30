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
                {eImplicitNodeType::Addition,    {"addition", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Subtraction, {"subtraction", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Multiplication, {"multiplication", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Division, {"division", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Constant, {"constant", {}, {"value"}}},
                {eImplicitNodeType::ConstVec, {"constvec", {}, {"vector"}}},
                {eImplicitNodeType::ConstMat, {"constmat", {}, {"matrix"}}},
                {eImplicitNodeType::ComposeVector, {"composevector", {"x", "y", "z"}, {"vector"}}},
                {eImplicitNodeType::ComposeMatrix, {"composematrix", {"A", "B", "C", "D", "E", "F", "G", "H", "I"}, {"matrix"}}},
                {eImplicitNodeType::ComposeMatrixFromColumnVectors, {"composematrixfromcolumnvectors", {"A", "B", "C"}, {"matrix"}}},
                {eImplicitNodeType::Dot, {"dot", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Cross, {"cross", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::MatVecMultiplication, {"matrixvectormultiplication", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Transpose, {"transpose", {"A"}, {"result"}}},
                {eImplicitNodeType::Sinus, {"sinus", {"A"}, {"result"}}},
                {eImplicitNodeType::Cosinus, {"cosinus", {"A"}, {"result"}}},
                {eImplicitNodeType::Tan, {"tan", {"A"}, {"tan"}}},
                {eImplicitNodeType::ArcSin, {"arcsin", {"A"}, {"result"}}},
                {eImplicitNodeType::ArcCos, {"arccos", {"A"}, {"result"}}},
                {eImplicitNodeType::ArcTan, {"arctan", {"A"}, {"result"}}},
                {eImplicitNodeType::Min, {"min", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Max, {"max", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Abs, {"abs", {"A"}, {"result"}}},
                {eImplicitNodeType::Fmod, {"fmod", {"A", "B"}, {"result"}}},
                {eImplicitNodeType::Pow, {"pow", {"base", "exponent"}, {"result"}}},
                {eImplicitNodeType::Sqrt, {"sqrt", {"A"}, {"result"}}},
                {eImplicitNodeType::Mesh, {"mesh", {"pos", "mesh"}, {"distance"}}}, 
                {eImplicitNodeType::FunctionCall, {"functioncall", {"functionID"}, {}}}, // besides of the functionIdm inputs and outputs are user defined
                {eImplicitNodeType::Length , {"length", {"A"}, {"result"}}},
                {eImplicitNodeType::DecomposeVector , {"decomposevector", {"vector"}, {"x","y", "z"}}},
                {eImplicitNodeType::Resource, {"resource", {}, {"value"}}}
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
