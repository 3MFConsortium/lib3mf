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

Abstract: This is the class declaration of CImplicitFunction

*/

#ifndef __LIB3MF_IMPLICITFUNCTION
#define __LIB3MF_IMPLICITFUNCTION

#include "lib3mf_function.hpp"
#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelImplicitFunction.h"

namespace Lib3MF
{
    namespace Impl
    {

        /*************************************************************************************************************************
         Class declaration of CImplicitFunction
        **************************************************************************************************************************/

        class CImplicitFunction : public IImplicitFunction, public CFunction
        {
           private:
            /**
             * Put private members here.
             */
            NMR::CModelImplicitFunction* function();

           protected:
            /**
             * Put protected members here.
             */

           public:
            /**
             * Put additional public members here. They will not be visible in
             * the external API.
             */

            CImplicitFunction() = delete;
            CImplicitFunction(NMR::PModelResource pResource);
            /**
             * Public member functions to implement.
             */

            /**
             * IImplicitFunction::GetIdentifier - Retrieves the identifier of
             * the function
             * @return the identifier
             */
            std::string GetIdentifier() override;

            /**
             * IImplicitFunction::SetIdentifier - Sets the identifier of the
             * function
             * @param[in] sIdentifier - the identifier
             */
            void SetIdentifier(const std::string& sIdentifier) override;

            /**
             * IImplicitFunction::AddNode - Add a node
             * @param[in] eNodeType - the type of the node
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IImplicitNode* AddNode(
                const Lib3MF::eImplicitNodeType eNodeType,
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSinNode - Add a SinNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISinNode* AddSinNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddCosNode - Add a CosNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ICosNode* AddCosNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddTanNode - Add a TanNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ITanNode* AddTanNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddArcSinNode - Add a ArcSinNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IArcSinNode* AddArcSinNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddArcCosNode - Add a ArcCosNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IArcCosNode* AddArcCosNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddArcTan2Node - Add a ArcTan2Node
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IArcTan2Node* AddArcTan2Node(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSinhNode - Add a SinhNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISinhNode* AddSinhNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddCoshNode - Add a CoshNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ICoshNode* AddCoshNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddTanhNode - Add a TanhNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ITanhNode* AddTanhNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddRoundNode - Add a RoundNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IRoundNode* AddRoundNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddCeilNode - Add a CeilNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ICeilNode* AddCeilNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddFloorNode - Add a FloorNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IFloorNode* AddFloorNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSignNode - Add a SignNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISignNode* AddSignNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddFractNode - Add a FractNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IFractNode* AddFractNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddAbsNode - Add a AbsNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IAbsNode* AddAbsNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddExpNode - Add a ExpNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IExpNode* AddExpNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddLogNode - Add a LogNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ILogNode* AddLogNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddLog2Node - Add a Log2Node
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ILog2Node* AddLog2Node(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddLog10Node - Add a Log10Node
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ILog10Node* AddLog10Node(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddLengthNode - Add a LengthNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ILengthNode* AddLengthNode(const std::string& sIdentifier,
                                       const std::string& sDisplayName,
                                       const std::string& sTag) override;

            /**
             * IImplicitFunction::AddTransposeNode - Add a TransposeNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ITransposeNode* AddTransposeNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::InverseNode - Add a InverseNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IInverseNode* InverseNode(const std::string& sIdentifier,
                                      const std::string& sDisplayName,
                                      const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSqrtNode - Add a SqrtNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISqrtNode* AddSqrtNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddResourceIdNode - Add a ResourceIdNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IResourceIdNode* AddResourceIdNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddAdditionNode - Add an AdditionNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IAdditionNode* AddAdditionNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSubtractionNode - Add a SubtractionNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISubtractionNode* AddSubtractionNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddMultiplicationNode - Add a
             * MultiplicationNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IMultiplicationNode* AddMultiplicationNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddDivisionNode - Add a DivisionNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IDivisionNode* AddDivisionNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddDotNode - Add a DotNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IDotNode* AddDotNode(const std::string& sIdentifier,
                                 const std::string& sDisplayName,
                                 const std::string& sTag) override;

            /**
             * IImplicitFunction::AddCrossNode - Add a CrossNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ICrossNode* AddCrossNode(const std::string& sIdentifier,
                                     const std::string& sDisplayName,
                                     const std::string& sTag) override;

            /**
             * IImplicitFunction::AddMatVecMultiplicationNode - Add a
             * MatVecMultiplicationNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IMatVecMultiplicationNode* AddMatVecMultiplicationNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddMinNode - Add a MinNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IMinNode* AddMinNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddMaxNode - Add a MaxNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IMaxNode* AddMaxNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddFmodNode - Add a FmodNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IFmodNode* AddFmodNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddPowNode - Add a PowNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IPowNode* AddPowNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddSelectNode - Add a SelectNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            ISelectNode* AddSelectNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddClampNode - Add a ClampNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] eConfiguration - the configuration of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IClampNode* AddClampNode(
                const std::string& sIdentifier,
                const Lib3MF::eImplicitNodeConfiguration eConfiguration,
                const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddComposeVectorNode - Add a ComposeVectorNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IComposeVectorNode* AddComposeVectorNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddDecomposeVectorNode - Add a
             * DecomposeVectorNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IDecomposeVectorNode* AddDecomposeVectorNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddComposeMatrixNode - Add a ComposeMatrixNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IComposeMatrixNode* AddComposeMatrixNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddComposeMatrixFromRowVectorsNode - Add a
             * ComposeMatrixFromRowVectorsNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IComposeMatrixFromRowVectorsNode*
            AddComposeMatrixFromRowVectorsNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::AddConstantNode - Add a ConstantNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IConstantNode* AddConstantNode(const std::string& sIdentifier,
                                           const std::string& sDisplayName,
                                           const std::string& sTag) override;

            /**
             * IImplicitFunction::AddConstVecNode - Add a ConstVecNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IConstVecNode* AddConstVecNode(const std::string& sIdentifier,
                                           const std::string& sDisplayName,
                                           const std::string& sTag) override;

            /**
             * IImplicitFunction::AddConstMatNode - Add a ConstMatNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IConstMatNode* AddConstMatNode(const std::string& sIdentifier,
                                           const std::string& sDisplayName,
                                           const std::string& sTag) override;

            /**
             * IImplicitFunction::AddMeshNode - Add a MeshNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IMeshNode* AddMeshNode(const std::string& sIdentifier,
                                   const std::string& sDisplayName,
                                   const std::string& sTag) override;

            /**
             * IImplicitFunction::AddFunctionCallNode - Add a FunctionCallNode
             * @param[in] sIdentifier - the identifier of the node
             * @param[in] sDisplayName - the display name of the node
             * @param[in] sTag - the tag of the node
             * @return the added node
             */
            IFunctionCallNode* AddFunctionCallNode(
                const std::string& sIdentifier, const std::string& sDisplayName,
                const std::string& sTag) override;

            /**
             * IImplicitFunction::GetNodes - Retrieves the nodes
             * @return the accessor to the nodes
             */
            INodeIterator* GetNodes() override;

            /**
             * IImplicitFunction::RemoveNode - Removes a node
             * @param[in] pNode - The node to be removed
             */
            void RemoveNode(IImplicitNode* pNode) override;

            /**
             * IImplicitFunction::AddLink - Add a link
             * @param[in] pSource - the source port
             * @param[in] pTarget - the target port
             */
            void AddLink(IImplicitPort* pSource,
                         IImplicitPort* pTarget) override;

            /**
             * IImplicitFunction::AddLinkByNames - Add a link
             * @param[in] sSource - name of the source port in the format
             * nodename.portname
             * @param[in] sTarget - name of the target port in the format
             * nodename.portname
             */
            void AddLinkByNames(const std::string& sSource,
                                const std::string& sTarget) override;

            /**
             * IImplicitFunction::Clear - Clears the function
             */
            void Clear() override;

            /**
             * IImplicitFunction::SortNodesTopologically - Sorts the nodes
             * topologically
             */
            void SortNodesTopologically() override;
        };

    }  // namespace Impl
}  // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif  // __LIB3MF_IMPLICITFUNCTION
