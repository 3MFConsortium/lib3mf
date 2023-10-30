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

Abstract: This is a stub class definition of CComposeMatrixNode

*/

#include "lib3mf_composematrixnode.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CComposeMatrixNode 
**************************************************************************************************************************/

IImplicitPort * CComposeMatrixNode::GetInputM00()
{
	return FindInputOrThrow(NMR::InputNames::m00);
}

IImplicitPort * CComposeMatrixNode::GetInputM01()
{
	return FindInputOrThrow(NMR::InputNames::m01);
}

IImplicitPort * CComposeMatrixNode::GetInputM02()
{
	return FindInputOrThrow(NMR::InputNames::m02);
}

IImplicitPort * CComposeMatrixNode::GetInputM03()
{
	return FindInputOrThrow(NMR::InputNames::m03);
}

IImplicitPort * CComposeMatrixNode::GetInputM10()
{
	return FindInputOrThrow(NMR::InputNames::m10);
}

IImplicitPort * CComposeMatrixNode::GetInputM11()
{
	return FindInputOrThrow(NMR::InputNames::m11);
}

IImplicitPort * CComposeMatrixNode::GetInputM12()
{
	return FindInputOrThrow(NMR::InputNames::m12);
}

IImplicitPort * CComposeMatrixNode::GetInputM13()
{
	return FindInputOrThrow(NMR::InputNames::m13);
}

IImplicitPort * CComposeMatrixNode::GetInputM20()
{
	return FindInputOrThrow(NMR::InputNames::m20);
}

IImplicitPort * CComposeMatrixNode::GetInputM21()
{
	return FindInputOrThrow(NMR::InputNames::m21);
}

IImplicitPort * CComposeMatrixNode::GetInputM22()
{
	return FindInputOrThrow(NMR::InputNames::m22);
}

IImplicitPort * CComposeMatrixNode::GetInputM23()
{
	return FindInputOrThrow(NMR::InputNames::m23);
}

IImplicitPort * CComposeMatrixNode::GetInputM30()
{
	return FindInputOrThrow(NMR::InputNames::m30);
}

IImplicitPort * CComposeMatrixNode::GetInputM31()
{
	return FindInputOrThrow(NMR::InputNames::m31);
}

IImplicitPort * CComposeMatrixNode::GetInputM32()
{
	return FindInputOrThrow(NMR::InputNames::m32);
}

IImplicitPort * CComposeMatrixNode::GetInputM33()
{
	return FindInputOrThrow(NMR::InputNames::m33);
}

IImplicitPort * CComposeMatrixNode::GetOutputMatrix()
{
	return FindInputOrThrow(NMR::OutputNames::matrix);
}

Lib3MF::Impl::CComposeMatrixNode::CComposeMatrixNode(
    NMR::PModelImplicitNode pImplicitNode) 
	: CImplicitNode{pImplicitNode}
{
	CImplicitNode::m_pImplicitNode = pImplicitNode;
}
