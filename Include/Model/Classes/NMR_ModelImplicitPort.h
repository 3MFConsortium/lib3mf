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

#include <lib3mf_types.hpp>
#include <memory>
#include <vector>
#include <string>

namespace NMR
{

    using ImplicitIdentifier = std::string;
    class CModelImplicitNode;

   

    class CModelImplicitPort
    {
      private:
        ImplicitIdentifier m_identifier;
        std::string m_displayname;
        Lib3MF::eImplicitPortType m_type = Lib3MF::eImplicitPortType::Scalar;
        ImplicitIdentifier m_reference;
        CModelImplicitNode * m_parent = nullptr;

      public:
        CModelImplicitPort(CModelImplicitNode * parent,
                           ImplicitIdentifier const & identifier,
                           std::string const & displayname);
        CModelImplicitPort(ImplicitIdentifier const & identifier,
                           std::string const & displayname,
                           Lib3MF::eImplicitPortType type);
        CModelImplicitPort(ImplicitIdentifier const & identifier,
                           std::string const & displayname,
                           Lib3MF::eImplicitPortType type,
                           ImplicitIdentifier const & reference);
        CModelImplicitPort() = delete;

        ImplicitIdentifier const & getIdentifier() const;
        std::string const & getDisplayName() const;
        void setIdentifier(ImplicitIdentifier const & identifier);
        void setDisplayName(std::string const & displayname);

        Lib3MF::eImplicitPortType getType() const;
        void setType(Lib3MF::eImplicitPortType type);

        ImplicitIdentifier const & getReference() const;
        void setReference(ImplicitIdentifier const & reference);

        CModelImplicitNode * getParent() const;
    };

    using PModelImplicitPort = std::shared_ptr<CModelImplicitPort>;

    using Ports = std::vector<PModelImplicitPort>;
    using PPorts = std::shared_ptr<Ports>;
}
