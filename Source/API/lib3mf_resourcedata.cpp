#include "lib3mf_resourcedata.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_packagepart.hpp"
#include "lib3mf_utils.hpp"

#include "Libraries/cpp-base64/base64.h"

#define MAX_INITVECTOR_SIZE (1024 * 1024)

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


		void CResourceData::GetCustomInitVector(Lib3MF_uint64 nIVBufferSize, Lib3MF_uint64* pIVNeededCount, Lib3MF_uint8* pIVBuffer)
		{
			auto initVector = m_pResourceData->getInitVector();
			uint64_t initVectorSize = initVector.size();

			if (pIVNeededCount)
				*pIVNeededCount = initVectorSize;

			if (pIVBuffer) {
				if (nIVBufferSize < initVectorSize)
					throw ELib3MFInterfaceException(LIB3MF_ERROR_BUFFERTOOSMALL);

				for (uint64_t nIndex = 0; nIndex < initVectorSize; nIndex++)
					pIVBuffer[nIndex] = initVector[nIndex];
			}
		}

		void CResourceData::SetCustomInitVector(const Lib3MF_uint64 nIVBufferSize, const Lib3MF_uint8* pIVBuffer)
		{
			std::vector<uint8_t> initVector;
			if (pIVBuffer && (nIVBufferSize > 0)) {
				if (nIVBufferSize > MAX_INITVECTOR_SIZE)
					throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDINITVECTOR);

				initVector.resize(nIVBufferSize);
				for (uint64_t nIndex = 0; nIndex < nIVBufferSize; nIndex++)
					initVector[nIndex] = pIVBuffer[nIndex];
			}
			m_pResourceData->setInitVector(initVector);
		}

	}
}