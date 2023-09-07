/*++

Copyright (C) 2023 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,276529
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

#include "Model/Classes/NMR_ModelFunction.h"

#include "Common/NMR_Exception.h"

namespace NMR
{

    CModelFunction::CModelFunction(const ModelResourceID sID, CModel * pModel)
        : CModelResource(sID, pModel)
    {
        m_inputs = std::make_shared<Ports>();
        m_outputs = std::make_shared<Ports>();
    }

    std::string const & CModelFunction::getDisplayName() const
    {
        return m_displayname;
    }

    void CModelFunction::setDisplayName(std::string const & displayname)
    {
        m_displayname = displayname;
    }

    PModelImplicitPort CModelFunction::addInput(const std::string & sPortIdentifier,
                                                        const std::string & sDisplayName,
                                                        const Lib3MF::eImplicitPortType ePortType)
    {
        auto newPort =
          std::make_shared<CModelImplicitPort>(sPortIdentifier, sDisplayName, ePortType);
        m_inputs->push_back(newPort);
        return newPort;
    }

    PModelImplicitPort CModelFunction::addOutput(const std::string & sPortIdentifier,
                                                         const std::string & sDisplayName,
                                                         const Lib3MF::eImplicitPortType ePortType)
    {
        auto newPort =
          std::make_shared<CModelImplicitPort>(sPortIdentifier, sDisplayName, ePortType);
        m_outputs->push_back(newPort);
        return newPort;
    }

    PPorts const& CModelFunction::getInputs() const
    {
        return m_inputs;
    }

    PPorts const& CModelFunction::getOutputs() const
    {
        return m_outputs;
    }

    PModelImplicitPort CModelFunction::findInput(const std::string & sIdentifier)
    {
        for (auto & port : *m_inputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    PModelImplicitPort CModelFunction::findOutput(const std::string & sIdentifier)
    {
        for (auto & port : *m_outputs)
        {
            if (port->getIdentifier() == sIdentifier)
            {
                return port;
            }
        }
        return {};
    }

    void CModelFunction::clear()
    {
        m_inputs->clear();
        m_outputs->clear();
    }
}
