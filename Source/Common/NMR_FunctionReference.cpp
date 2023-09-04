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

Abstract:

This file implements the CFunctionReference class, which represents a reference
to a function resource in a 3D Manufacturing Format (3MF) file. The class
provides methods for setting and getting the unique resource ID of the
referenced function.

--*/

#include "Common/NMR_FunctionReference.h"

#include "Model/Classes/NMR_ModelFunction.h"

namespace NMR
{
    CFunctionReference::CFunctionReference(UniqueResourceID functionResourceId)
        : m_pFunctionResourceID(functionResourceId)
    {
    }

    CFunctionReference::CFunctionReference(PModelFunction pFunction)
        : m_pFunctionResourceID(
              pFunction ? pFunction->getPackageResourceID()->getUniqueID() : 0)
    {
    }

    void CFunctionReference::setFunctionResourceID(UniqueResourceID functionID)
    {
        m_pFunctionResourceID = functionID;
    }

    UniqueResourceID CFunctionReference::getFunctionResourceID() const
    {
        return m_pFunctionResourceID;
    }

    void NMR::CFunctionReference::setChannelName(std::string outputPortName)
    {
        m_outputPortName = std::move(outputPortName);
    }

    std::string const& NMR::CFunctionReference::getChannelName() const
    {
        return m_outputPortName;
    }

    void CFunctionReference::setTransform(NMR::NMATRIX3 transform)
    {
        m_transform = transform;
        m_bHasTransform = true;
    }

    bool CFunctionReference::hasTransform() const
    {
        return m_bHasTransform;
    }

    NMR::NMATRIX3 CFunctionReference::getTransform() const
    {
        return m_transform;
    }

}  // namespace NMR
