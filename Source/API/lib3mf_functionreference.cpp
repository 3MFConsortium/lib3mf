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

Abstract: This is a stub class definition of CFunctionReference

*/

#include "lib3mf_functionreference.hpp"

#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.
#include "lib3mf_utils.hpp"

using namespace Lib3MF::Impl;

/*************************************************************************************************************************
 Class definition of CFunctionReference
**************************************************************************************************************************/

Lib3MF_uint32 CFunctionReference::GetFunctionResourceID()
{
    return m_pFunctionReference->getFunctionResourceID();
}

void CFunctionReference::SetFunctionResourceID(
    const Lib3MF_uint32 nUniqueResourceID)
{
    m_pFunctionReference->setFunctionResourceID(nUniqueResourceID);
}

CFunctionReference::CFunctionReference(
    NMR::PFunctionReference pFunctionReference)
    : m_pFunctionReference(pFunctionReference)
{
}

void CFunctionReference::SetTransform(
    const Lib3MF::sTransform Transform)
{
     m_pFunctionReference->setTransform(TransformToMatrix(Transform));
}


Lib3MF::sTransform CFunctionReference::GetTransform()
{
    NMR::NMATRIX3 const matrix = m_pFunctionReference->getTransform();
	return MatrixToTransform(matrix);
}

//std::string GetOutputName()
std::string CFunctionReference::GetOutputName()
{
    return m_pFunctionReference->getOutputName();
}

void CFunctionReference::SetOutputName(
    const std::string & sOutputName)
{
    m_pFunctionReference->setOutputName(sOutputName);
}