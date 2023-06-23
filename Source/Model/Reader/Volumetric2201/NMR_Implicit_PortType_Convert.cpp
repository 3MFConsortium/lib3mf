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
Helper methods for reading implicit ports

--*/

#include "Model/Reader/Volumetric2201/NMR_Implicit_PortType_Convert.h"
#include "Common/NMR_Exception.h"
#include <iostream>

namespace NMR
{
    namespace implicit
    {
        const char * portTypeToRefName(Lib3MF::eImplicitPortType type)
        {
            switch (type)
            {
            case Lib3MF::eImplicitPortType::Scalar:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR_REF;
            case Lib3MF::eImplicitPortType::Vector:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR_REF;
            case Lib3MF::eImplicitPortType::Matrix:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX_REF;
            case Lib3MF::eImplicitPortType::ResourceID:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_REF;
            default:
                return "Invalid";
            }
        }

        const char * portTypeToName(Lib3MF::eImplicitPortType type)
        {
            switch (type)
            {
            case Lib3MF::eImplicitPortType::Scalar:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR;
            case Lib3MF::eImplicitPortType::Vector:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR;
            case Lib3MF::eImplicitPortType::Matrix:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX;
            case Lib3MF::eImplicitPortType::ResourceID:
                return XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_ID;
            default:
                return "Invalid";
            }
        }

        bool portTypeFromRefName(std::string const & name, Lib3MF::eImplicitPortType & resultType)
        {
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR_REF)
            {
                resultType = Lib3MF::eImplicitPortType::Scalar;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR_REF)
            {
                resultType = Lib3MF::eImplicitPortType::Vector;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX_REF)
            {
                resultType = Lib3MF::eImplicitPortType::Matrix;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_REF)
            {
                resultType = Lib3MF::eImplicitPortType::ResourceID;
                return true;
            }

            return false;
        }

        bool portTypeFromName(std::string const & name, Lib3MF::eImplicitPortType & resultType)
        {
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_SCALAR)
            {
                resultType = Lib3MF::eImplicitPortType::Scalar;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_VECTOR)
            {
                resultType = Lib3MF::eImplicitPortType::Vector;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_MATRIX)
            {
                resultType = Lib3MF::eImplicitPortType::Matrix;
                return true;
            }
            if (name == XML_3MF_ELEMENT_IMPLICIT_PORT_TYPE_RESOURCE_ID)
            {
                resultType = Lib3MF::eImplicitPortType::ResourceID;
                return true;
            }

            return false;
        }
    }
}