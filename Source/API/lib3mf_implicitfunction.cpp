/*++

Copyright (C) 2024 3MF Consortium (Original Author)

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
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CImplicitFunction 
**************************************************************************************************************************/

std::string CImplicitFunction::GetIdentifier()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::SetIdentifier(const std::string & sIdentifier)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IImplicitNode * CImplicitFunction::AddNode(const Lib3MF::eImplicitNodeType eNodeType, const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISinNode * CImplicitFunction::AddSinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICosNode * CImplicitFunction::AddCosNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITanNode * CImplicitFunction::AddTanNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IArcSinNode * CImplicitFunction::AddArcSinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IArcCosNode * CImplicitFunction::AddArcCosNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IArcTan2Node * CImplicitFunction::AddArcTan2Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISinhNode * CImplicitFunction::AddSinhNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICoshNode * CImplicitFunction::AddCoshNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITanhNode * CImplicitFunction::AddTanhNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IRoundNode * CImplicitFunction::AddRoundNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICeilNode * CImplicitFunction::AddCeilNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFloorNode * CImplicitFunction::AddFloorNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISignNode * CImplicitFunction::AddSignNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFractNode * CImplicitFunction::AddFractNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAbsNode * CImplicitFunction::AddAbsNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IExpNode * CImplicitFunction::AddExpNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILogNode * CImplicitFunction::AddLogNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILog2Node * CImplicitFunction::AddLog2Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILog10Node * CImplicitFunction::AddLog10Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ILengthNode * CImplicitFunction::AddLengthNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ITransposeNode * CImplicitFunction::AddTransposeNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IInverseNode * CImplicitFunction::AddInverseNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISqrtNode * CImplicitFunction::AddSqrtNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IResourceIdNode * CImplicitFunction::AddResourceIdNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IAdditionNode * CImplicitFunction::AddAdditionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISubtractionNode * CImplicitFunction::AddSubtractionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMultiplicationNode * CImplicitFunction::AddMultiplicationNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IDivisionNode * CImplicitFunction::AddDivisionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IDotNode * CImplicitFunction::AddDotNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ICrossNode * CImplicitFunction::AddCrossNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMatVecMultiplicationNode * CImplicitFunction::AddMatVecMultiplicationNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMinNode * CImplicitFunction::AddMinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMaxNode * CImplicitFunction::AddMaxNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFmodNode * CImplicitFunction::AddFmodNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IPowNode * CImplicitFunction::AddPowNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

ISelectNode * CImplicitFunction::AddSelectNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IClampNode * CImplicitFunction::AddClampNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IComposeVectorNode * CImplicitFunction::AddComposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IVectorFromScalarNode * CImplicitFunction::AddVectorFromScalarNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IDecomposeVectorNode * CImplicitFunction::AddDecomposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IComposeMatrixNode * CImplicitFunction::AddComposeMatrixNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMatrixFromRowsNode * CImplicitFunction::AddMatrixFromRowsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMatrixFromColumnsNode * CImplicitFunction::AddMatrixFromColumnsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IConstantNode * CImplicitFunction::AddConstantNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IConstVecNode * CImplicitFunction::AddConstVecNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IConstMatNode * CImplicitFunction::AddConstMatNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IMeshNode * CImplicitFunction::AddMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IUnsignedMeshNode * CImplicitFunction::AddUnsignedMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

IFunctionCallNode * CImplicitFunction::AddFunctionCallNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

INodeIterator * CImplicitFunction::GetNodes()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::RemoveNode(IImplicitNode* pNode)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::AddLink(IImplicitPort* pSource, IImplicitPort* pTarget)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::AddLinkByNames(const std::string & sSource, const std::string & sTarget)
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::Clear()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

void CImplicitFunction::SortNodesTopologically()
{
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

