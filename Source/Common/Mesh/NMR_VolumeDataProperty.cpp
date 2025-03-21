/*++

Copyright (C) 2019 3MF Consortium

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

NMR_VolumeDataProperty.cpp implements the class CVolumeDataProperty.

--*/

#include "Common/Mesh/NMR_VolumeDataProperty.h"

#include "Model/Classes/NMR_ModelFunction.h"

namespace NMR
{

    CVolumeDataProperty::CVolumeDataProperty(PModelFunction pFunction)
        : CFunctionReference(pFunction), m_sName({}), m_bIsRequired(true)
    {
    }

    CVolumeDataProperty::CVolumeDataProperty(PModelFunction pFunction,
                                             std::string sName)
        : CFunctionReference(pFunction), m_sName(sName), m_bIsRequired(true)
    {
    }

    std::string CVolumeDataProperty::getName()
    {
        return m_sName;
    }

    void CVolumeDataProperty::setName(std::string sPropertyName)
    {
        m_sName = sPropertyName;
    }

    bool CVolumeDataProperty::isRequired()
    {
        return m_bIsRequired;
    }

    void CVolumeDataProperty::setIsRequired(bool bIsRequired)
    {
        m_bIsRequired = bIsRequired;
    }
}  // namespace NMR
