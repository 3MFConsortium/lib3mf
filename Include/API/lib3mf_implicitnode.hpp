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

Abstract: This is the class declaration of CImplicitNode

*/

#ifndef __LIB3MF_IMPLICITNODE
#define __LIB3MF_IMPLICITNODE

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelImplicitNode.h"

namespace Lib3MF
{
    namespace Impl
    {

        /*************************************************************************************************************************
         Class declaration of CImplicitNode
        **************************************************************************************************************************/

        class CImplicitNode : public virtual IImplicitNode, public virtual CBase
        {
          private:
            NMR::PModelImplicitNode m_pImplicitNode;

          protected:
            /**
             * Put protected members here.
             */

          public:
            // Ctor
            CImplicitNode(NMR::PModelImplicitNode pImplicitNode);

            /**
             * Public member functions to implement.
             */

            /**
             * IImplicitNode::GetIdentifier - Retrieves the identifier of the node
             * @return the identifier
             */
            std::string GetIdentifier() override;

            /**
             * IImplicitNode::SetIdentifier - Sets the identifier of the node
             * @param[in] sIdentifier - the identifier
             */
            void SetIdentifier(const std::string & sIdentifier) override;

            /**
             * IImplicitNode::GetDisplayName - Retrieves the display name of the node
             * @return the display name
             */
            std::string GetDisplayName() override;

            /**
             * IImplicitNode::SetDisplayName - Sets the display name of the node
             * @param[in] sDisplayName - the display name
             */
            void SetDisplayName(const std::string & sDisplayName) override;
        };

    } // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMPLICITNODE
