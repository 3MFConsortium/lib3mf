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

NMR_KeyStoreFactory.h defines the CKeyStoreFactory Class. It is responsible for creating logic of keystore entities.

--*/

#ifndef __NMR_KEYSTOREFACTORY
#define __NMR_KEYSTOREFACTORY

#include <memory>
#include <string>
#include <vector>

#include <vector>
#include "Common/NMR_SecureContentTypes.h"


namespace NMR {

	class CModel;
	using PModel = std::shared_ptr<CModel>;
	class CKeyStore;
	using PKeyStore = std::shared_ptr<CKeyStore>;
	class CKeyStoreConsumer;
	using PKeyStoreConsumer = std::shared_ptr<CKeyStoreConsumer>;
	class CKeyStoreResourceDataGroup;
	using PKeyStoreResourceDataGroup = std::shared_ptr<CKeyStoreResourceDataGroup>;
	class CKeyStoreAccessRight;
	using PKeyStoreAccessRight = std::shared_ptr<CKeyStoreAccessRight>;
	class CKeyStoreResourceData;
	using PKeyStoreResourceData = std::shared_ptr<CKeyStoreResourceData>;
	class CKeyStoreContentEncryptionParams;
	using PKeyStoreContentEncryptionParams = std::shared_ptr<CKeyStoreContentEncryptionParams>;
	class CKeyStoreCEKParams;
	using PKeyStoreCEKParams = std::shared_ptr<CKeyStoreCEKParams>;

	class CUUID;
	using PUUID = std::shared_ptr<CUUID>;

	class CPackageModelPath;
	using PPackageModelPath = std::shared_ptr<CPackageModelPath>;

	class CKeyStoreFactory {
	public:
		static PKeyStore makeKeyStore();
		static PKeyStoreResourceDataGroup makeResourceDataGroup(PUUID const & keyUUID = nullptr, std::vector<nfByte> const & key = std::vector<nfByte>());
		static PKeyStoreConsumer makeConsumer(std::string const & consumerID, std::string const & keyId = std::string(), std::string const & keyValue = std::string());
		static PKeyStoreAccessRight makeAccessRight(PKeyStoreConsumer const & consumer, eKeyStoreWrapAlgorithm const algorithm, eKeyStoreMaskGenerationFunction const mask, eKeyStoreMessageDigest const digest, std::vector<nfByte> const & cipherValue = std::vector<nfByte>());
		static PKeyStoreResourceData makeResourceData(PKeyStoreResourceDataGroup const & rdg, PPackageModelPath const & path, PKeyStoreCEKParams params = nullptr);
		static PKeyStoreContentEncryptionParams makeContentEncryptionParams(PKeyStoreResourceData rd, PKeyStoreResourceDataGroup rdg);
		static PKeyStoreCEKParams makeCEKParams(nfBool compressed, eKeyStoreEncryptAlgorithm algorithm, std::vector<nfByte> const & aad, std::vector<nfByte> const & iv = std::vector<nfByte>(), std::vector<nfByte> const & tag = std::vector<nfByte>());
	};
}


#endif // !__NMR_KEYSTOREFACTORY


