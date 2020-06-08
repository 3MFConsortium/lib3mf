/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CResourceDataGroup

*/


#ifndef __LIB3MF_RESOURCEDATAGROUP
#define __LIB3MF_RESOURCEDATAGROUP


#include "lib3mf_interfaces.hpp"
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_KeyStore.h"

namespace Lib3MF {
	namespace Impl {

		/*************************************************************************************************************************
		Class declaration of CResourceData
		**************************************************************************************************************************/

		class CResourceDataGroup : public virtual IResourceDataGroup, public virtual CBase {
			NMR::PKeyStoreResourceDataGroup m_pDataGroup;
		public:
			CResourceDataGroup(NMR::PKeyStoreResourceDataGroup const & dg);

			// Inherited via IResourceDataGroup
			virtual IAccessRight * AddAccessRight(IConsumer * pConsumer, const Lib3MF::eWrappingAlgorithm eWrappingAlgorithm, const Lib3MF::eMgfAlgorithm eMgfAlgorithm, const Lib3MF::eDigestMethod eDigestMethod) override;
			virtual IAccessRight * FindAccessRightByConsumer(IConsumer * pConsumerInstance) override;
			virtual void RemoveAccessRight(IConsumer * pConsumerInstance) override;
			virtual std::string GetKeyUUID() override;


			inline NMR::PKeyStoreResourceDataGroup resourceDataGroup() const {
				return m_pDataGroup;
			}

		};
	}
}


#endif // !__LIB3MF_RESOURCEDATAGROUP
