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

Abstract: This is a stub class definition of CNodeIterator

*/

#include "lib3mf_nodeiterator.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_absnode.hpp"
#include "lib3mf_additionnode.hpp"
#include "lib3mf_arccosnode.hpp"
#include "lib3mf_arcsinnode.hpp"
#include "lib3mf_arctan2node.hpp"
#include "lib3mf_arctannode.hpp"
#include "lib3mf_ceilnode.hpp"
#include "lib3mf_clampnode.hpp"
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
#include "lib3mf_inversenode.hpp"
#include "lib3mf_lengthnode.hpp"
#include "lib3mf_log10node.hpp"
#include "lib3mf_log2node.hpp"
#include "lib3mf_lognode.hpp"
#include "lib3mf_matrixfromcolumnsnode.hpp"
#include "lib3mf_matrixfromrowsnode.hpp"
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
#include "lib3mf_unsignedmeshnode.hpp"
#include "lib3mf_vectorfromscalarnode.hpp"
#include "lib3mf_modnode.hpp"


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CNodeIterator
**************************************************************************************************************************/

IImplicitNode * CNodeIterator::GetCurrent()
{
	throwIfInvalidIndex();
    auto pModelNode = m_pNodes->at(static_cast<size_t>(getCurrentIndex()));
    auto pNode = implicitNodeFromModelImplicitNode(pModelNode); 
    if (!pNode)
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
	return pNode;
}

//  enum class eImplicitNodeType : Lib3MF_int32 {
//     Addition = 1, /** Adds to values (scalar or vector) */
//     Subtraction = 2, /** Subtracts two values (scalar or vector) */
//     Multiplication = 3, /** Multiplies two values (scalar or vector) */
//     Division = 4, /** Divides two values (scalar or vector) */
//     Constant = 5, /** A constant scalar value */
//     ConstVec = 6, /** A constant vector value */
//     ConstMat = 7, /** A constant matrix value */
//     ComposeVector = 8, /** Creates a vector from three scalar values */
//     DecomposeVector = 9, /** Decomposes a vector into three scalar values */
//     ComposeMatrix = 10, /** Creates a matrix from nine scalar values */
//     ComposeMatrixFromColumnVectors = 11, /** Creates a matrix from three column vector values */
//     ComposeMatrixFromRowVectors = 12, /** Creates a matrix from three row vector values */
//     Dot = 13, /** Calculates the dot product of two vector values */
//     Cross = 14, /** Calculates the cross product of two vector values */
//     MatVecMultiplication = 15, /** Multiplies a matrix with a vector */
//     Transpose = 16, /** Transposes a matrix */
//     Inverse = 17, /** Computes the inverse of a matrix */
//     Sinus = 18, /** Calculates the sinus */
//     Cosinus = 19, /** Calculates the cosinus */
//     Tan = 20, /** Calculates the tangent */
//     ArcSin = 21, /** Calculates the arcsinus */
//     ArcCos = 22, /** Calculates the arccosinus */
//     ArcTan = 23, /** Calculates the arctangent */
//     ArcTan2 = 24, /** Calculates the arctangent */
//     Min = 25, /** Calculates the minimum tow values */
//     Max = 26, /** Calculates the maximum of two values */
//     Abs = 27, /** Calcul the absolute value */
//     Fmod = 28, /** Calculates the modulo of two values */
//     Pow = 29, /** Calculates the power A^B */
//     Sqrt = 30, /** Calculates the square root */
//     Exp = 31, /** Exponential function */
//     Log = 32, /** Natural logarithmus */
//     Log2 = 33, /** Logarithmus to the base 2 */
//     Log10 = 34, /** Logarithmus to the base 10 */
//     Select = 35, /** If A is less B returns C, else D */
//     Clamp = 36, /** Clamps the input value to min and max */
//     Sinh = 37, /** Calculates the hyperbolic sine */
//     Cosh = 38, /** Calculates the hyperbolic cosine */
//     Tanh = 39, /** Calculates the hyperbolic tangent */
//     Round = 40, /** Rounds a scalar value to the nearest integer */
//     Ceil = 41, /** Rounds a scalar value up to the nearest integer */
//     Floor = 42, /** Rounds a scalar value down to the nearest integer */
//     Sign = 43, /** Returns the sign */
//     Fract = 44, /** Returns the fractional part */
//     FunctionCall = 45, /** Calls a function */
//     Mesh = 46, /** Calculates the signed distance to a mesh */
//     Length = 47, /** Calculates the length of a vector */
//     Resource = 48 /** Selects a resource (function, mesh etc.) */
//   };

IImplicitNode* Lib3MF::Impl::CNodeIterator::implicitNodeFromModelImplicitNode(
    NMR::PModelImplicitNode pNode)
{
    if (!pNode)
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);

     
    switch (pNode->getNodeType())
    {
    case eImplicitNodeType::Addition:
        return new CAdditionNode(pNode);
    case eImplicitNodeType::Subtraction:
        return new CSubtractionNode(pNode);
    case eImplicitNodeType::Multiplication:
        return new CMultiplicationNode(pNode);
    case eImplicitNodeType::Division:
        return new CDivisionNode(pNode);
    case eImplicitNodeType::Constant:
        return new CConstantNode(pNode);
    case eImplicitNodeType::ConstVec:
        return new CConstVecNode(pNode);
    case eImplicitNodeType::ConstMat:
        return new CConstMatNode(pNode);
    case eImplicitNodeType::ComposeVector:
        return new CComposeVectorNode(pNode);
    case eImplicitNodeType::VectorFromScalar:
        return new CVectorFromScalarNode(pNode);
    case eImplicitNodeType::DecomposeVector:
        return new CDecomposeVectorNode(pNode);
    case eImplicitNodeType::ComposeMatrix:
        return new CComposeMatrixNode(pNode);
    case eImplicitNodeType::MatrixFromColumns:
        return new CMatrixFromColumnsNode(pNode);
    case eImplicitNodeType::MatrixFromRows:
        return new CMatrixFromRowsNode(pNode);
    case eImplicitNodeType::Dot:
        return new CDotNode(pNode);
    case eImplicitNodeType::Cross:
        return new CCrossNode(pNode);
    case eImplicitNodeType::MatVecMultiplication:
        return new CMatVecMultiplicationNode(pNode);
    case eImplicitNodeType::Transpose:
        return new CTransposeNode(pNode);
    case eImplicitNodeType::Inverse:
        return new CInverseNode(pNode);
    case eImplicitNodeType::Sinus:
        return new CSinNode(pNode);
    case eImplicitNodeType::Cosinus:
        return new CCosNode(pNode);
    case eImplicitNodeType::Tan:
        return new CTanNode(pNode);
    case eImplicitNodeType::ArcSin:
        return new CArcSinNode(pNode);
    case eImplicitNodeType::ArcCos:
        return new CArcCosNode(pNode);
    case eImplicitNodeType::ArcTan:
        return new CArcTanNode(pNode);
    case eImplicitNodeType::ArcTan2:
        return new CArcTan2Node(pNode);
    case eImplicitNodeType::Min:
        return new CMinNode(pNode);
    case eImplicitNodeType::Max:
        return new CMaxNode(pNode);
    case eImplicitNodeType::Abs:
        return new CAbsNode(pNode);
    case eImplicitNodeType::Fmod:
        return new CFmodNode(pNode);
    case eImplicitNodeType::Pow:
        return new CPowNode(pNode);
    case eImplicitNodeType::Sqrt:
        return new CSqrtNode(pNode);
    case eImplicitNodeType::Exp:
        return new CExpNode(pNode);
    case eImplicitNodeType::Log:
        return new CLogNode(pNode);
    case eImplicitNodeType::Log2:
        return new CLog2Node(pNode);
    case eImplicitNodeType::Log10:
        return new CLog10Node(pNode);
    case eImplicitNodeType::Select:
        return new CSelectNode(pNode);
    case eImplicitNodeType::Clamp:
        return new CClampNode(pNode);
    case eImplicitNodeType::Sinh:
        return new CSinhNode(pNode);
    case eImplicitNodeType::Cosh:
        return new CCoshNode(pNode);
    case eImplicitNodeType::Tanh:
        return new CTanhNode(pNode);
    case eImplicitNodeType::Round:
        return new CRoundNode(pNode);
    case eImplicitNodeType::Ceil:
        return new CCeilNode(pNode);
    case eImplicitNodeType::Floor:
        return new CFloorNode(pNode);
    case eImplicitNodeType::Sign:
        return new CSignNode(pNode);
    case eImplicitNodeType::Fract:
        return new CFractNode(pNode);
    case eImplicitNodeType::FunctionCall:
        return new CFunctionCallNode(pNode);
    case eImplicitNodeType::Mesh:
        return new CMeshNode(pNode);
    case eImplicitNodeType::UnsignedMesh:
        return new CUnsignedMeshNode(pNode);
    case eImplicitNodeType::Length:
        return new CLengthNode(pNode);
    case eImplicitNodeType::ConstResourceID:
        return new CResourceIdNode(pNode);
    case eImplicitNodeType::Mod:
        return new CModNode(pNode);

    default:
        throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
    }   
    
}

Lib3MF::Impl::CNodeIterator::CNodeIterator(NMR::PImplicitNodes pNodes)
    : m_pNodes(pNodes)
{
}

Lib3MF_uint64 Lib3MF::Impl::CNodeIterator::Count()
{
    return static_cast<Lib3MF_uint64>(m_pNodes->size());
}
