

#include "Model/Classes/NMR_KeyStoreFactory.h"
#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Model/Classes/NMR_KeyStoreCEKParams.h"
#include "Common/NMR_StringUtils.h"

namespace NMR {
	PKeyStore CKeyStoreFactory::makeKeyStore() {
		return std::make_shared<CKeyStore>();
	}

	PKeyStoreResourceDataGroup CKeyStoreFactory::makeResourceDataGroup(PUUID const & keyUUID, std::vector<nfByte> const & key) {
		PUUID uuid = (nullptr != keyUUID) ? keyUUID : std::make_shared<CUUID>();
		return std::make_shared<CKeyStoreResourceDataGroup>(uuid, key);
	}

	PKeyStoreConsumer CKeyStoreFactory::makeConsumer(std::string const & consumerID, std::string const & keyId, std::string const & keyValue) {
		return std::make_shared<CKeyStoreConsumer>(consumerID, keyId, keyValue);
	}

	PKeyStoreAccessRight CKeyStoreFactory::makeAccessRight(PKeyStoreConsumer const & consumer, eKeyStoreWrapAlgorithm const algorithm, eKeyStoreMaskGenerationFunction const mask, eKeyStoreMessageDigest const digest, std::vector<nfByte> const & cipherValue) {
		return std::make_shared<CKeyStoreAccessRight>(consumer, algorithm, mask, digest, cipherValue);
	}

	PKeyStoreResourceData CKeyStoreFactory::makeResourceData(PKeyStoreResourceDataGroup const & rdg, PPackageModelPath const & path, PKeyStoreCEKParams params) {
		return std::make_shared<CKeyStoreResourceData>(rdg, path,
			params->isCompressed(),
			params->getEncryptionAlgorithm(),
			params->getInitVector(),
			params->getAuthTag(),
			params->getAddAuthData());
	}

	PKeyStoreContentEncryptionParams CKeyStoreFactory::makeContentEncryptionParams(PKeyStoreResourceData rd, PKeyStoreResourceDataGroup rdg) {
		return std::make_shared<CKeyStoreContentEncryptionParams>(
			rd->isCompressed(),
			rd->getEncryptionAlgorithm(),
			rdg->getKey(),
			rd->getInitVector(),
			rd->getAuthTag(),
			rd->getAddAuthData(),
			rd->getDescriptor()
		);
	}

	PKeyStoreCEKParams CKeyStoreFactory::makeCEKParams(nfBool compressed, eKeyStoreEncryptAlgorithm algorithm, std::vector<nfByte> const & aad, std::vector<nfByte> const & iv, std::vector<nfByte> const & tag) {
		return std::make_shared<CKeyStoreCEKParams>(compressed, algorithm, iv, tag, aad, 0);
	}
}