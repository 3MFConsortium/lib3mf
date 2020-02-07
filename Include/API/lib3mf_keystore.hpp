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

Abstract: This is the class declaration of CKeyStore

*/


#ifndef __LIB3MF_KEYSTORE
#define __LIB3MF_KEYSTORE

#include "lib3mf_interfaces.hpp"
#include "lib3mf_base.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_KeyStore.h"
namespace Lib3MF {
	namespace Impl {


		/*************************************************************************************************************************
		Class declaration of CKeyStore
		**************************************************************************************************************************/

		class CKeyStore : public virtual IKeyStore, public virtual CBase {
		private:

			/**
			* Put private members here.
			*/
			NMR::PKeyStore m_pKeyStore;
		protected:

			/**
			* Put protected members here.
			*/

		public:

			/**
			* Put additional public members here. They will not be visible in the external API.
			*/
			CKeyStore(NMR::PKeyStore pKeyStore);
		
			/**
			* Public member functions to implement.
			*/


			// Inherited via IKeyStore
			IConsumer * AddConsumer(const std::string & sConsumerID, const std::string & sKeyID, const std::string & sKeyValue) override;

			Lib3MF_uint32 GetConsumerCount() override;

			IConsumer * GetConsumer(const Lib3MF_uint64 nConsumerIndex) override;

			IConsumer * FindConsumer(const std::string & sConsumerID) override;

			virtual void RemoveConsumer(IConsumer * pConsumerInstance) override;

			Lib3MF_uint32 GetResourceDataCount() override;

			IResourceData * GetResourceData(const Lib3MF_uint64 nResourceDataIndex) override;

			IResourceData * AddResourceData(IPackagePart * pPackagePath, const Lib3MF::eEncryptionAlgorithm eEncryptionAlgorithm, const Lib3MF::eCompression eCompression) override;

			IResourceData * FindResourceData(const std::string & sPath) override;

			std::string GetUUID(bool & bHasUUID) override;
			
			void SetUUID(const std::string & sUUID) override;

		};

	} // namespace Impl
} // namespace Lib3MF

#endif // __LIB3MF_KEYSTORE