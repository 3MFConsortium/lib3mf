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

Abstract: This is the class declaration of CImplicitFunction

*/

#ifndef __LIB3MF_IMPLICITFUNCTION
#define __LIB3MF_IMPLICITFUNCTION

#include "lib3mf_interfaces.hpp"
#include "lib3mf_function.hpp"

// Parent classes
#include "lib3mf_base.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelImplicitFunction.h"

namespace Lib3MF
{
    namespace Impl
    {

        /*************************************************************************************************************************
         Class declaration of CImplicitFunction
        **************************************************************************************************************************/

        class CImplicitFunction : public virtual IImplicitFunction, public virtual CFunction
        {
          private:
            /**
             * Put private members here.
             */
            NMR::CModelImplicitFunction* function();

          protected:
            /**
             * Put protected members here.
             */

          public:
            /**
             * Put additional public members here. They will not be visible in the external API.
             */

            CImplicitFunction() = delete;
            CImplicitFunction(NMR::PModelResource pResource);
            /**
             * Public member functions to implement.
             */

            /**
             * IImplicitFunction::GetIdentifier - Retrieves the identifier of the function
             * @return the identifier
             */
            std::string GetIdentifier() override;

            /**
             * IImplicitFunction::SetIdentifier - Sets the identifier of the function
             * @param[in] sIdentifier - the identifier
             */
            void SetIdentifier(const std::string & sIdentifier) override;

            /**
             * IImplicitFunction::AddNode - Add a node
             * @param[in] eNodeType - the type of the node
             * @param[in] sIdentifier - the identifier of the input
             * @param[in] sDisplayName - the display name of the input
             * @return the added node
             */
            IImplicitNode * AddNode(const Lib3MF::eImplicitNodeType eNodeType,
                                    const std::string & sIdentifier,
                                    const std::string& sDisplayName,
                                    const std::string& sTag) override;

            /**
             * IImplicitFunction::GetNodes - Retrieves the nodes
             * @return the accessor to the nodes
             */
            INodeIterator * GetNodes() override;

            /**
             * IImplicitFunction::RemoveNode - Removes a node
             * @param[in] pNode - The node to be removed
             */
            void RemoveNode(IImplicitNode * pNode) override;

            /**
             * IImplicitFunction::AddLink - Add a link
             * @param[in] pSource - the source port
             * @param[in] pTarget - the target port
             */
            void AddLink(IImplicitPort * pSource, IImplicitPort * pTarget) override;

            /**
             * IImplicitFunction::AddLinkByNames - Add a link
             * @param[in] sSource - name of the source port in the format nodename.portname
             * @param[in] sTarget - name of the target port in the format nodename.portname
             */
            void AddLinkByNames(const std::string & sSource, const std::string & sTarget) override;
        };

    } // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMPLICITFUNCTION
