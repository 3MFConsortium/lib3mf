#include "lib3mf_resourcedata.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_packagepart.hpp"
#include "lib3mf_utils.hpp"

namespace Lib3MF {
	namespace Impl {
		Lib3MF::Impl::CResourceData::CResourceData(NMR::PKeyStoreResourceData resourceData) 
			:m_pResourceData(resourceData)
		{
			if (!resourceData)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}

		Lib3MF::eEncryptionAlgorithm Lib3MF::Impl::CResourceData::GetEncryptionAlgorithm() {
			NMR::eKeyStoreEncryptAlgorithm ea = m_pResourceData->getEncryptionAlgorithm();
			return static_cast<eEncryptionAlgorithm>(ea);
		}

		Lib3MF::eCompression Lib3MF::Impl::CResourceData::GetCompression() {
			return translateCompression(m_pResourceData->isCompressed());
		}

		IPackagePart * CResourceData::GetPath() {
			return new CPackagePart(m_pResourceData->packagePath());
		}

		void CResourceData::GetAdditionalAuthenticationData(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 * pByteDataNeededCount, Lib3MF_uint8 * pByteDataBuffer) {
			std::vector<NMR::nfByte> const & aad = m_pResourceData->getAddAuthData();
			if (nByteDataBufferSize < aad.size()) {
				*pByteDataNeededCount = aad.size();
			} else {
				std::copy(aad.begin(), aad.end(), pByteDataBuffer);
			}
		}
	}
}