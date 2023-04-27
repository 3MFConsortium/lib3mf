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

--*/

#pragma once

#include <Common/Platform/NMR_SAL.h>
#include <Model/Classes/NMR_ModelImplicitPort.h>
#include <lib3mf_types.hpp>

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <optional>

namespace NMR
{
    using Ports = std::vector<PModelImplicitPort>;
    using PPorts = std::shared_ptr<Ports>;

    // std::string elementNameFromNodeType(Lib3MF::eImplicitNodeType nodeType);
  
    class CModelImplicitNode
    {
      private:
        ImplicitIdentifier m_identifier;
        std::string m_displayname;
        Lib3MF::eImplicitNodeType m_type;

        PPorts m_inputs;
        PPorts m_outputs;

        // Optional values for constants
        double m_constant = 0.;
        std::unique_ptr<Lib3MF::sVector> m_vector;
        std::unique_ptr<Lib3MF::sMatrix4x4> m_matrix;
    
      public:
        CModelImplicitNode(Lib3MF::eImplicitNodeType type,
                           ImplicitIdentifier const & identifier,
                           std::string const & displayname);
        CModelImplicitNode(Lib3MF::eImplicitNodeType type);

        ImplicitIdentifier const & getIdentifier() const;
        std::string const & getDisplayName() const;
        void setIdentifier(ImplicitIdentifier const & identifier);
        void setDisplayName(std::string const & displayname);

        void setType(Lib3MF::eImplicitNodeType type);
        Lib3MF::eImplicitNodeType getNodeType() const;

        PModelImplicitPort addInput(const std::string & sIdentifier,
                                    const std::string & sDisplayName);

        PModelImplicitPort addOutput(const std::string & sIdentifier,
                                     const std::string & sDisplayName);

        PPorts getInputs() const;
        PPorts getOutputs() const;

        PModelImplicitPort findInput(const std::string & sIdentifier);
        PModelImplicitPort findOutput(const std::string & sIdentifier);

        void setConstant(double value);
        double getConstant() const;

        void setVector(const Lib3MF::sVector & value);
        Lib3MF::sVector getVector() const;


        void setMatrix(const Lib3MF::sMatrix4x4 & value);
        Lib3MF::sMatrix4x4 getMatrix() const;

    };

    using PModelImplicitNode = std::shared_ptr<CModelImplicitNode>;
}
