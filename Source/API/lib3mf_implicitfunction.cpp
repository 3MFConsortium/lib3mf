/*++

Copyright (C) 2023 3MF Consortium (Original Author)

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

Abstract: This is a stub class definition of CImplicitFunction

*/

#include "lib3mf_implicitfunction.hpp"

#include "lib3mf_absnode.hpp"
#include "lib3mf_additionnode.hpp"
#include "lib3mf_arccosnode.hpp"
#include "lib3mf_arcsinnode.hpp"
#include "lib3mf_arctan2node.hpp"
#include "lib3mf_ceilnode.hpp"
#include "lib3mf_clampnode.hpp"
#include "lib3mf_composematrixfromrowvectorsnode.hpp"
#include "lib3mf_composematrixnode.hpp"
#include "lib3mf_composevectornode.hpp"
#include "lib3mf_constantnode.hpp"
#include "lib3mf_constmatnode.hpp"
#include "lib3mf_constvecnode.hpp"
#include "lib3mf_coshnode.hpp"
#include "lib3mf_cosnode.hpp"
#include "lib3mf_crossnode.hpp"
#include "lib3mf_decomposevectornode.hpp"
#include "lib3mf_divisionnode.hpp"
#include "lib3mf_dotnode.hpp"
#include "lib3mf_expnode.hpp"
#include "lib3mf_floornode.hpp"
#include "lib3mf_fmodnode.hpp"
#include "lib3mf_fractnode.hpp"
#include "lib3mf_functioncallnode.hpp"
#include "lib3mf_implicitnode.hpp"
#include "lib3mf_implicitport.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_inversenode.hpp"
#include "lib3mf_lengthnode.hpp"
#include "lib3mf_log10node.hpp"
#include "lib3mf_log2node.hpp"
#include "lib3mf_lognode.hpp"
#include "lib3mf_matvecmultiplicationnode.hpp"
#include "lib3mf_maxnode.hpp"
#include "lib3mf_meshnode.hpp"
#include "lib3mf_minnode.hpp"
#include "lib3mf_multiplicationnode.hpp"
#include "lib3mf_pownode.hpp"
#include "lib3mf_resourceidnode.hpp"
#include "lib3mf_roundnode.hpp"
#include "lib3mf_selectnode.hpp"
#include "lib3mf_signnode.hpp"
#include "lib3mf_sinhnode.hpp"
#include "lib3mf_sinnode.hpp"
#include "lib3mf_sqrtnode.hpp"
#include "lib3mf_subtractionnode.hpp"
#include "lib3mf_tanhnode.hpp"
#include "lib3mf_tannode.hpp"
#include "lib3mf_transposenode.hpp"

// Include custom headers here.
#include "lib3mf_implicitportiterator.hpp"
#include "lib3mf_nodeiterator.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImplicitFunction
**************************************************************************************************************************/

NMR::CModelImplicitFunction* CImplicitFunction::function()
{
    NMR::CModelImplicitFunction* pFunction =
        dynamic_cast<NMR::CModelImplicitFunction*>(resource().get());
    if(pFunction == nullptr)
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDOBJECT);

    return pFunction;
}

CImplicitFunction::CImplicitFunction(NMR::PModelResource pResource)
    : CResource(pResource), CFunction(pResource)
{
}

std::string CImplicitFunction::GetIdentifier()
{
    return function()->getIdentifier();
}

void CImplicitFunction::SetIdentifier(const std::string& sIdentifier)
{
    function()->setIdentifier(sIdentifier);
}

IImplicitNode* CImplicitFunction::AddNode(
    const Lib3MF::eImplicitNodeType eNodeType, const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode = function()->addNode(eNodeType, eConfiguration, sIdentifier,
                                       sDisplayName, sTag);
    return new CImplicitNode(newNode);
}

ISinNode* CImplicitFunction::AddSinNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Sinus, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CSinNode(newNode);
}

ICosNode* CImplicitFunction::AddCosNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Cosinus, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CCosNode(newNode);
}

ITanNode* CImplicitFunction::AddTanNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Tan, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CTanNode(newNode);
}

IArcSinNode* CImplicitFunction::AddArcSinNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ArcSin, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CArcSinNode(newNode);
}

IArcCosNode* CImplicitFunction::AddArcCosNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ArcCos, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CArcCosNode(newNode);
}

IArcTan2Node* CImplicitFunction::AddArcTan2Node(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ArcTan2, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CArcTan2Node(newNode);
}

ISinhNode* CImplicitFunction::AddSinhNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Sinh, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CSinhNode(newNode);
}

ICoshNode* CImplicitFunction::AddCoshNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Cosh, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CCoshNode(newNode);
}

ITanhNode* CImplicitFunction::AddTanhNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Tanh, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CTanhNode(newNode);
}

IRoundNode* CImplicitFunction::AddRoundNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Round, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CRoundNode(newNode);
}

ICeilNode* CImplicitFunction::AddCeilNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Ceil, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CCeilNode(newNode);
}

IFloorNode* CImplicitFunction::AddFloorNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Floor, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CFloorNode(newNode);
}

ISignNode* CImplicitFunction::AddSignNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Sign, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CSignNode(newNode);
}

IFractNode* CImplicitFunction::AddFractNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Fract, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CFractNode(newNode);
}

IAbsNode* CImplicitFunction::AddAbsNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Abs, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CAbsNode(newNode);
}

IExpNode* CImplicitFunction::AddExpNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Exp, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CExpNode(newNode);
}

ILogNode* CImplicitFunction::AddLogNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Log, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CLogNode(newNode);
}

ILog2Node* CImplicitFunction::AddLog2Node(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Log2, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CLog2Node(newNode);
}

ILog10Node* CImplicitFunction::AddLog10Node(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Log10, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CLog10Node(newNode);
}

ILengthNode* CImplicitFunction::AddLengthNode(const std::string& sIdentifier,
                                              const std::string& sDisplayName,
                                              const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Length,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CLengthNode(newNode);
}

ITransposeNode* CImplicitFunction::AddTransposeNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Transpose,
                            eConfiguration, sIdentifier, sDisplayName, sTag);
    return new CTransposeNode(newNode);
}

IInverseNode* CImplicitFunction::InverseNode(const std::string& sIdentifier,
                                             const std::string& sDisplayName,
                                             const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Inverse,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CInverseNode(newNode);
}

ISqrtNode* CImplicitFunction::AddSqrtNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Sqrt, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CSqrtNode(newNode);
}

IResourceIdNode* CImplicitFunction::AddResourceIdNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Resource,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CResourceIdNode(newNode);
}

IAdditionNode* CImplicitFunction::AddAdditionNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Addition, eConfiguration,
                            sIdentifier, sDisplayName, sTag);

    return new CAdditionNode(newNode);
}

ISubtractionNode* CImplicitFunction::AddSubtractionNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Subtraction,
                            eConfiguration, sIdentifier, sDisplayName, sTag);
    return new CSubtractionNode(newNode);
}

IMultiplicationNode* CImplicitFunction::AddMultiplicationNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Multiplication,
                            eConfiguration, sIdentifier, sDisplayName, sTag);
    return new CMultiplicationNode(newNode);
}

IDivisionNode* CImplicitFunction::AddDivisionNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Division, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CDivisionNode(newNode);
}

IDotNode* CImplicitFunction::AddDotNode(const std::string& sIdentifier,
                                        const std::string& sDisplayName,
                                        const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Dot,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CDotNode(newNode);
}

ICrossNode* CImplicitFunction::AddCrossNode(const std::string& sIdentifier,
                                            const std::string& sDisplayName,
                                            const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Cross,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CCrossNode(newNode);
}

IMatVecMultiplicationNode* CImplicitFunction::AddMatVecMultiplicationNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::MatVecMultiplication,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CMatVecMultiplicationNode(newNode);
}

IMinNode* CImplicitFunction::AddMinNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Min, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CMinNode(newNode);
}

IMaxNode* CImplicitFunction::AddMaxNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Max, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CMaxNode(newNode);
}

IFmodNode* CImplicitFunction::AddFmodNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Fmod, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CFmodNode(newNode);
}

IPowNode* CImplicitFunction::AddPowNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Pow, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CPowNode(newNode);
}

ISelectNode* CImplicitFunction::AddSelectNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Select, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CSelectNode(newNode);
}

IClampNode* CImplicitFunction::AddClampNode(
    const std::string& sIdentifier,
    const Lib3MF::eImplicitNodeConfiguration eConfiguration,
    const std::string& sDisplayName, const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Clamp, eConfiguration,
                            sIdentifier, sDisplayName, sTag);
    return new CClampNode(newNode);
}

IComposeVectorNode* CImplicitFunction::AddComposeVectorNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ComposeVector,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CComposeVectorNode(newNode);
}

IDecomposeVectorNode* CImplicitFunction::AddDecomposeVectorNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::DecomposeVector,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CDecomposeVectorNode(newNode);
}

IComposeMatrixNode* CImplicitFunction::AddComposeMatrixNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ComposeMatrix,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CComposeMatrixNode(newNode);
}

IComposeMatrixFromRowVectorsNode*
CImplicitFunction::AddComposeMatrixFromRowVectorsNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode = function()->addNode(
        Lib3MF::eImplicitNodeType::ComposeMatrixFromRowVectors,
        Lib3MF::eImplicitNodeConfiguration::Default, sIdentifier, sDisplayName,
        sTag);
    return new CComposeMatrixFromRowVectorsNode(newNode);
}

IConstantNode* CImplicitFunction::AddConstantNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Constant,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CConstantNode(newNode);
}

IConstVecNode* CImplicitFunction::AddConstVecNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ConstVec,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CConstVecNode(newNode);
}

IConstMatNode* CImplicitFunction::AddConstMatNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::ConstMat,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CConstMatNode(newNode);
}

IMeshNode* CImplicitFunction::AddMeshNode(const std::string& sIdentifier,
                                          const std::string& sDisplayName,
                                          const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::Mesh,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CMeshNode(newNode);
}

IFunctionCallNode* CImplicitFunction::AddFunctionCallNode(
    const std::string& sIdentifier, const std::string& sDisplayName,
    const std::string& sTag)
{
    auto newNode =
        function()->addNode(Lib3MF::eImplicitNodeType::FunctionCall,
                            Lib3MF::eImplicitNodeConfiguration::Default,
                            sIdentifier, sDisplayName, sTag);
    return new CFunctionCallNode(newNode);
}

INodeIterator* CImplicitFunction::GetNodes()
{
    return new CNodeIterator(function()->getNodes());
}

void CImplicitFunction::RemoveNode(IImplicitNode* pNode)
{
    if(pNode == nullptr)
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    function()->removeNode(pNode->GetIdentifier());
}

void CImplicitFunction::AddLink(IImplicitPort* pSource, IImplicitPort* pTarget)
{
    if (pSource == nullptr || pTarget == nullptr)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }
    CImplicitPort* pSourcePort = dynamic_cast<CImplicitPort*>(pSource);
    CImplicitPort* pTargetPort = dynamic_cast<CImplicitPort*>(pTarget);
    
    if (pSourcePort == nullptr || pTargetPort == nullptr)
    {
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }

    function()->addLink(pSourcePort->getPort(), pTargetPort->getPort());
}

void CImplicitFunction::AddLinkByNames(const std::string& sSource,
                                       const std::string& sTarget)
{
    function()->addLink(sSource, sTarget);
}

void CImplicitFunction::Clear()
{
    function()->clear();
}

void CImplicitFunction::SortNodesTopologically()
{
    function()->sortNodesTopologically();
}
