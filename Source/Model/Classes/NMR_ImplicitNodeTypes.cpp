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
#include "lib3mf_interfaceexception.hpp"

namespace NMR
{
    namespace implicit
    {

        NodeType::NodeType(std::string const& name,
                           AllowedInputOutputs const& allowedInputOutputs)
            : m_name(name), m_allowedInputOutputs(allowedInputOutputs)
        {
        }

        std::string const& NodeType::getName() const
        {
            return m_name;
        }

        AllowedInputOutputs const& NodeType::getAllowedInputOutputs() const
        {
            return m_allowedInputOutputs;
        }

        using namespace Lib3MF;
        NodeTypes::NodeTypes()
        {
            static AllowedInputOutputs const twoParameterSameDimensions{
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::ScalarToScalar,
                    In{{"A", eImplicitPortType::Scalar},
                       {"B", eImplicitPortType::Scalar}},
                    Out{{"result", eImplicitPortType::Scalar}}},
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::VectorToVector,
                    In{{"A", eImplicitPortType::Vector},
                       {"B", eImplicitPortType::Vector}},
                    Out{{"result", eImplicitPortType::Vector}}},
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::MatrixToMatrix,
                    In{{"A", eImplicitPortType::Matrix},
                       {"B", eImplicitPortType::Matrix}},
                    Out{{"result", eImplicitPortType::Matrix}}}};

            static AllowedInputOutputs const oneParameterFunctionSameDimensions{
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::ScalarToScalar,
                    In{{"A", eImplicitPortType::Scalar}},
                    Out{{"result", eImplicitPortType::Scalar}}},
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::VectorToVector,
                    In{{"A", eImplicitPortType::Vector}},
                    Out{{"result", eImplicitPortType::Vector}}},
                InputOutputRule{
                    Lib3MF::eImplicitNodeConfiguration::MatrixToMatrix,
                    In{{"A", eImplicitPortType::Matrix}},
                    Out{{"result", eImplicitPortType::Matrix}}}};

            m_nodeTypes = NodeTypesMap{
                {eImplicitNodeType::Addition,
                 {"addition", twoParameterSameDimensions}},
                {eImplicitNodeType::Subtraction,
                 {"subtraction", twoParameterSameDimensions}},
                {eImplicitNodeType::Constant,
                 {"constant",
                  {InputOutputRule{
                      Lib3MF::eImplicitNodeConfiguration::Default, In{},
                      Out{{"value", eImplicitPortType::Scalar}}}}}},
                {eImplicitNodeType::Multiplication,
                 {"multiplication", twoParameterSameDimensions}},
                {eImplicitNodeType::Division,
                 {"division", twoParameterSameDimensions}},
                {eImplicitNodeType::ConstVec,
                 {
                     "constvec",
                     {
                         InputOutputRule{
                             Lib3MF::eImplicitNodeConfiguration::Default, In{},
                             Out{{"vector", eImplicitPortType::Vector}}},
                     },
                 }},
                {eImplicitNodeType::ConstMat,
                 {
                     "constmat",
                     {
                         InputOutputRule{
                             Lib3MF::eImplicitNodeConfiguration::Default, In{},
                             Out{{"matrix", eImplicitPortType::Matrix}}},
                     },
                 }},
                {eImplicitNodeType::ComposeVector,
                 {
                     "composevector",
                     {
                         InputOutputRule{
                             Lib3MF::eImplicitNodeConfiguration::Default,
                             In{{"x", eImplicitPortType::Scalar},
                                {"y", eImplicitPortType::Scalar},
                                {"z", eImplicitPortType::Scalar}},
                             Out{{"result", eImplicitPortType::Vector}}},
                     },
                 }},
                {eImplicitNodeType::DecomposeVector,
                 {
                     "decomposevector",
                     {
                         InputOutputRule{
                             Lib3MF::eImplicitNodeConfiguration::Default,
                             In{{"A", eImplicitPortType::Vector}},
                             Out{{"x", eImplicitPortType::Scalar},
                                 {"y", eImplicitPortType::Scalar},
                                 {"z", eImplicitPortType::Scalar}}},
                     },
                 }},
                {eImplicitNodeType::ComposeMatrix,
                 {"composematrix",  // 4x4
                  {InputOutputRule{
                      Lib3MF::eImplicitNodeConfiguration::Default,
                      In{{"m00", eImplicitPortType::Scalar},
                         {"m01", eImplicitPortType::Scalar},
                         {"m02", eImplicitPortType::Scalar},
                         {"m03", eImplicitPortType::Scalar},
                         {"m10", eImplicitPortType::Scalar},
                         {"m11", eImplicitPortType::Scalar},
                         {"m12", eImplicitPortType::Scalar},
                         {"m13", eImplicitPortType::Scalar},
                         {"m20", eImplicitPortType::Scalar},
                         {"m21", eImplicitPortType::Scalar},
                         {"m22", eImplicitPortType::Scalar},
                         {"m23", eImplicitPortType::Scalar},
                         {"m30", eImplicitPortType::Scalar},
                         {"m31", eImplicitPortType::Scalar},
                         {"m32", eImplicitPortType::Scalar},
                         {"m33", eImplicitPortType::Scalar}},
                      Out{{"result", eImplicitPortType::Matrix}}}}}},
                {eImplicitNodeType::MatrixFromColumns,
                 {"ematrixfromcolumns",  // 4x4
                  {InputOutputRule{
                      Lib3MF::eImplicitNodeConfiguration::Default,
                      In{{"A", eImplicitPortType::Vector},
                         {"B", eImplicitPortType::Vector},
                         {"C", eImplicitPortType::Vector},
                         {"D", eImplicitPortType::Vector}},
                      Out{{"result", eImplicitPortType::Matrix}}}}}},
                {eImplicitNodeType::MatrixFromRows,
                 {
                     "matrixfromrows",  // 4x4
                     {InputOutputRule{
                         Lib3MF::eImplicitNodeConfiguration::Default,
                         In{{"A", eImplicitPortType::Vector},
                            {"B", eImplicitPortType::Vector},
                            {"C", eImplicitPortType::Vector},
                            {"D", eImplicitPortType::Vector}},
                         Out{{"result", eImplicitPortType::Matrix}}}},
                 }},
                {eImplicitNodeType::Dot,
                 {"dot",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Vector},
                             {"B", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Scalar}}},
                  }}},
                {eImplicitNodeType::Cross,
                 {"cross",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Vector},
                             {"B", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Vector}}},
                  }}},
                {eImplicitNodeType::MatVecMultiplication,
                 {"matvecmultiplication",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Matrix},
                             {"B", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Vector}}},
                  }}},
                {eImplicitNodeType::Transpose,
                 {"transpose",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Matrix}},
                          Out{{"result", eImplicitPortType::Matrix}}},
                  }}},
                {eImplicitNodeType::Inverse,
                 {"inverse",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Matrix}},
                          Out{{"result", eImplicitPortType::Matrix}}},
                  }}},
                {eImplicitNodeType::Sinus,
                 {"sin", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Cosinus,
                 {"cos", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Tan,
                 {"tan", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::ArcSin,
                 {"asin", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::ArcCos,
                 {"acos", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::ArcTan,
                 {"atan", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::ArcTan2,
                 {"atan2", twoParameterSameDimensions}},
                {eImplicitNodeType::Min, {"min", twoParameterSameDimensions}},
                {eImplicitNodeType::Max, {"max", twoParameterSameDimensions}},
                {eImplicitNodeType::Abs,
                 {"abs", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Fmod, {"fmod", twoParameterSameDimensions}},
                {eImplicitNodeType::Mod, {"mod", twoParameterSameDimensions}},
                {eImplicitNodeType::Pow, {"pow", twoParameterSameDimensions}},
                {eImplicitNodeType::Sqrt,
                 {"sqrt", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Exp,
                 {"exp", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Log,
                 {"log", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Log2,
                 {"log2", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Log10,
                 {"log10", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Cosh,
                 {"cosh", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Sinh,
                 {"sinh", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Tanh,
                 {"tanh", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Round,
                 {"round", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Ceil,
                 {"ceil", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Floor,
                 {"floor", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Sign,
                 {"sign", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Fract,
                 {"fract", oneParameterFunctionSameDimensions}},
                {eImplicitNodeType::Select,
                 {"select",  // result = A < B ? C : D
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::ScalarToScalar,
                          In{{"A", eImplicitPortType::Scalar},
                             {"B", eImplicitPortType::Scalar},
                             {"C", eImplicitPortType::Scalar},
                             {"D", eImplicitPortType::Scalar}},
                          Out{{"result", eImplicitPortType::Scalar}}},
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::VectorToVector,
                          In{{"A", eImplicitPortType::Vector},
                             {"B", eImplicitPortType::Vector},
                             {"C", eImplicitPortType::Vector},
                             {"D", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Vector}}},
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::MatrixToMatrix,
                          In{{"A", eImplicitPortType::Matrix},
                             {"B", eImplicitPortType::Matrix},
                             {"C", eImplicitPortType::Matrix},
                             {"D", eImplicitPortType::Matrix}},
                          Out{{"result", eImplicitPortType::Matrix}}},

                  }}},
                {eImplicitNodeType::Clamp,  // result = min(max(A, min), max)
                 {"clamp",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::ScalarToScalar,
                          In{{"A", eImplicitPortType::Scalar},
                             {"min", eImplicitPortType::Scalar},
                             {"max", eImplicitPortType::Scalar}},
                          Out{{"result", eImplicitPortType::Scalar}}},
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::VectorToVector,
                          In{{"A", eImplicitPortType::Vector},
                             {"min", eImplicitPortType::Vector},
                             {"max", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Vector}}},
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::MatrixToMatrix,
                          In{{"A", eImplicitPortType::Matrix},
                             {"min", eImplicitPortType::Matrix},
                             {"max", eImplicitPortType::Matrix}},
                          Out{{"result", eImplicitPortType::Matrix}}},
                  }}},
                {eImplicitNodeType::Length,
                 {"length",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Vector}},
                          Out{{"result", eImplicitPortType::Scalar}}},
                  }}},
                  {eImplicitNodeType::VectorFromScalar,
                 {"vectorfromscalar",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"A", eImplicitPortType::Scalar}},
                          Out{{"result", eImplicitPortType::Vector}}},
                  }}},
                {eImplicitNodeType::ConstResourceID,
                 {"constresourceid",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default, In{},
                          Out{{"value", eImplicitPortType::ResourceID}}},
                  }}},
                {eImplicitNodeType::Mesh,
                 {"mesh",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"pos", eImplicitPortType::Vector},
                             {"mesh", eImplicitPortType::ResourceID}},
                          Out{{"distance", eImplicitPortType::Scalar}}},
                  }}},
                {eImplicitNodeType::UnsignedMesh,
                 {"mesh",
                  {
                      InputOutputRule{
                          Lib3MF::eImplicitNodeConfiguration::Default,
                          In{{"pos", eImplicitPortType::Vector},
                             {"mesh", eImplicitPortType::ResourceID}},
                          Out{{"distance", eImplicitPortType::Scalar}}},
                  }}},
                {eImplicitNodeType::FunctionCall,
                 {"functioncall",
                  {InputOutputRule{
                      Lib3MF::eImplicitNodeConfiguration::Default,
                      In{{"functionID",
                          eImplicitPortType::ResourceID}},  // not predefined
                                                            // besides the
                                                            // functionID
                      Out{}                                 // not predefined
                  }}}}};
        }

        NodeType const& NodeTypes::getNodeType(
            Lib3MF::eImplicitNodeType type) const
        {
            auto it = m_nodeTypes.find(type);
            if(it == m_nodeTypes.end())
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDNODECONFIGURATION,
                    "Unknown node type " +
                        std::to_string(static_cast<int>(type)));
            }
            return it->second;
        }
        NodeTypesMap const& NodeTypes::getTypes() const
        {
            return m_nodeTypes;
        }

        void NodeTypes::addExpectedPortsToNode(
            NMR::CModelImplicitNode& node) const
        {
            auto const& nodeType = getNodeType(node.getNodeType());

            if(nodeType.getAllowedInputOutputs().empty())
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDNODECONFIGURATION,
                    "No known configuration for node type " +
                        std::to_string(static_cast<int>(node.getNodeType())));
            }
            applyInputOutputRuleToNode(
                node, nodeType.getAllowedInputOutputs().front());
        }

        void NodeTypes::addExpectedPortsToNode(
            NMR::CModelImplicitNode& node,
            Lib3MF::eImplicitNodeConfiguration const configuration) const
        {
            if(configuration == Lib3MF::eImplicitNodeConfiguration::Default)
            {
                addExpectedPortsToNode(node);
                return;
            }

            auto const& nodeType = getNodeType(node.getNodeType());
            if(nodeType.getAllowedInputOutputs().empty())
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDNODECONFIGURATION,
                    "No known configuration for node type " +
                        std::to_string(static_cast<int>(node.getNodeType())));
            }

            if((nodeType.getAllowedInputOutputs().size() == 1u) &&
               configuration != Lib3MF::eImplicitNodeConfiguration::Default)
            {
                throw ELib3MFInterfaceException(
                    LIB3MF_ERROR_INVALIDNODECONFIGURATION,
                    "Node type: " +
                        std::to_string(static_cast<int>(node.getNodeType())) +
                        " only supports the default configuration");
            }

            for(auto const& rule : nodeType.getAllowedInputOutputs())
            {
                if(rule.configuration == configuration)
                {
                    applyInputOutputRuleToNode(node, rule);
                    return;
                }
            }

            throw ELib3MFInterfaceException(
                LIB3MF_ERROR_INVALIDNODECONFIGURATION,
                "Node type: " +
                    std::to_string(static_cast<int>(node.getNodeType())) +
                    " does not support configuration: " +
                    std::to_string(static_cast<int>(configuration)));
        }

        void NodeTypes::applyInputOutputRuleToNode(
            NMR::CModelImplicitNode& node, InputOutputRule const& rule) const
        {
            for(auto const& input : rule.inputs)
            {
                auto inPort = node.addInput(input.first, input.first);
                inPort->setType(input.second);
            }
            for(auto const& output : rule.outputs)
            {
                auto outPort = node.addOutput(output.first, output.first);
                outPort->setType(output.second);
            }
        }

        bool NodeTypes::arePortsValidForNode(
            NMR::CModelImplicitNode const& node) const
        {
            auto const& nodeType = getNodeType(node.getNodeType());
            for(auto const& rule : nodeType.getAllowedInputOutputs())
            {
                if(arePortsValidForRule(node, rule))
                {
                    return true;
                }
            }
            return false;
        }

        bool NodeTypes::arePortsValidForRule(
            NMR::CModelImplicitNode const& node,
            InputOutputRule const& rule) const
        {
            for(auto const& input : rule.inputs)
            {
                auto inPort = node.findInput(input.first);
                if(inPort == nullptr)
                {
                    return false;
                }
                if(inPort->getType() != input.second)
                {
                    return false;
                }
            }

            for(auto const& output : rule.outputs)
            {
                auto outPort = node.findOutput(output.first);
                if(outPort == nullptr)
                {
                    return false;
                }
                if(outPort->getType() != output.second)
                {
                    return false;
                }
            }

            return true;
        }

    }  // namespace implicit

}  // namespace NMR
