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

Abstract: This is the class declaration of CResourceData

*/


#ifndef __LIB3MF_RESOURCEDATA
#define __LIB3MF_RESOURCEDATA

#include "lib3mf_interfaces.hpp"
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

#include "Model/Classes/NMR_KeyStoreResourceData.h"
// Include custom headers here.
namespace Lib3MF {
	namespace Impl {


		/*************************************************************************************************************************
		Class declaration of CResourceData
		**************************************************************************************************************************/

		class CResourceData : public virtual IResourceData, public virtual CBase {
		private:
			NMR::PKeyStoreResourceData m_pResourceData;
		public:
			CResourceData(NMR::PKeyStoreResourceData resourceData);


			// Inherited via IResourceData
			IPackagePart * GetPath() override;

			Lib3MF::eEncryptionAlgorithm GetEncryptionAlgorithm() override;

			Lib3MF::eCompression GetCompression() override;

			void GetAdditionalAuthenticationData(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 * pByteDataNeededCount, Lib3MF_uint8 * pByteDataBuffer) override;

			inline NMR::PKeyStoreResourceData resourceData() const {
				return m_pResourceData;
			}
		};
	}
}
#endif
