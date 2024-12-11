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

Abstract: This is the class declaration of CImplicitFunction

*/


#ifndef __LIB3MF_IMPLICITFUNCTION
#define __LIB3MF_IMPLICITFUNCTION

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_function.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.


namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CImplicitFunction 
**************************************************************************************************************************/

class CImplicitFunction : public virtual IImplicitFunction, public virtual CFunction {
private:

	/**
	* Put private members here.
	*/

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/


	/**
	* Public member functions to implement.
	*/

	std::string GetIdentifier() override;

	void SetIdentifier(const std::string & sIdentifier) override;

	IImplicitNode * AddNode(const Lib3MF::eImplicitNodeType eNodeType, const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ISinNode * AddSinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ICosNode * AddCosNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ITanNode * AddTanNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IArcSinNode * AddArcSinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IArcCosNode * AddArcCosNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IArcTan2Node * AddArcTan2Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ISinhNode * AddSinhNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ICoshNode * AddCoshNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ITanhNode * AddTanhNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IRoundNode * AddRoundNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ICeilNode * AddCeilNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IFloorNode * AddFloorNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ISignNode * AddSignNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IFractNode * AddFractNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IAbsNode * AddAbsNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IExpNode * AddExpNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ILogNode * AddLogNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ILog2Node * AddLog2Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ILog10Node * AddLog10Node(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ILengthNode * AddLengthNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	ITransposeNode * AddTransposeNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IInverseNode * AddInverseNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	ISqrtNode * AddSqrtNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IResourceIdNode * AddResourceIdNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IAdditionNode * AddAdditionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ISubtractionNode * AddSubtractionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IMultiplicationNode * AddMultiplicationNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IDivisionNode * AddDivisionNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IDotNode * AddDotNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	ICrossNode * AddCrossNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IMatVecMultiplicationNode * AddMatVecMultiplicationNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IMinNode * AddMinNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IMaxNode * AddMaxNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IFmodNode * AddFmodNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IPowNode * AddPowNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	ISelectNode * AddSelectNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IClampNode * AddClampNode(const std::string & sIdentifier, const Lib3MF::eImplicitNodeConfiguration eConfiguration, const std::string & sDisplayName, const std::string & sTag) override;

	IComposeVectorNode * AddComposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IVectorFromScalarNode * AddVectorFromScalarNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IDecomposeVectorNode * AddDecomposeVectorNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IComposeMatrixNode * AddComposeMatrixNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IMatrixFromRowsNode * AddMatrixFromRowsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IMatrixFromColumnsNode * AddMatrixFromColumnsNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IConstantNode * AddConstantNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IConstVecNode * AddConstVecNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IConstMatNode * AddConstMatNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IMeshNode * AddMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IUnsignedMeshNode * AddUnsignedMeshNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	IFunctionCallNode * AddFunctionCallNode(const std::string & sIdentifier, const std::string & sDisplayName, const std::string & sTag) override;

	INodeIterator * GetNodes() override;

	void RemoveNode(IImplicitNode* pNode) override;

	void AddLink(IImplicitPort* pSource, IImplicitPort* pTarget) override;

	void AddLinkByNames(const std::string & sSource, const std::string & sTarget) override;

	void Clear() override;

	void SortNodesTopologically() override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMPLICITFUNCTION
