#include "lib3mf_resourcedata.hpp"
#include "lib3mf_interfaceexception.hpp"
#include "lib3mf_consumer.hpp"
#include "lib3mf_decryptright.hpp"
#include "lib3mf_packagepath.hpp"

namespace Lib3MF {
	namespace Impl {
		Lib3MF::Impl::CResourceData::CResourceData(NMR::PKeyStoreResourceData resourceData) {
			m_ResourceData = resourceData;
		}

		IDecryptRight * Lib3MF::Impl::CResourceData::AddDecryptRight(IConsumer * pConsumerInstance, const Lib3MF::eEncryptionAlgorithm eaInstance) {
			if (Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p == eaInstance) {
				throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
			}
			CConsumer * consumer = reinterpret_cast<CConsumer *>(pConsumerInstance);
			NMR::PKeyStoreDecryptRight dR = m_ResourceData->addDecryptRight(consumer->consumer(), NMR::eKeyStoreEncryptAlgorithm::Aes256Gcm);
			return new CDecryptRight(dR);
		}

		Lib3MF_uint32 Lib3MF::Impl::CResourceData::GetDecryptRightCount() {
			return  m_ResourceData->getDecryptRightCount();
		}

		IDecryptRight * Lib3MF::Impl::CResourceData::GetDecryptRight(const Lib3MF_uint32 nDecryptRightIndex) {
			NMR::PKeyStoreDecryptRight dR = m_ResourceData->getDecryptRight(nDecryptRightIndex);
			return new CDecryptRight(dR);
		}

		IDecryptRight * Lib3MF::Impl::CResourceData::FindDecryptRightByConsumer(IConsumer * pConsumerInstance) {
			CConsumer * consumer = reinterpret_cast<CConsumer *>(pConsumerInstance);
			NMR::PKeyStoreDecryptRight dR = m_ResourceData->findDecryptRightByConsumer(consumer->consumer());
			return new CDecryptRight(dR);
		}

		void Lib3MF::Impl::CResourceData::RemoveDecrypt(IConsumer * pConsumerInstance) {
			CConsumer * consumer = reinterpret_cast<CConsumer *>(pConsumerInstance);
			m_ResourceData->removeDecryptRight(consumer->consumer());
		}

		Lib3MF::eEncryptionAlgorithm Lib3MF::Impl::CResourceData::GetEncryptionAlgorithm() {
			NMR::eKeyStoreEncryptAlgorithm ea = m_ResourceData->getEncryptionAlgorithm();
			if (ea == NMR::eKeyStoreEncryptAlgorithm::RsaOaepMgf1p) {
				return Lib3MF::eEncryptionAlgorithm::RsaOaepMgf1p;
			}
			return Lib3MF::eEncryptionAlgorithm::Aes256Gcm;
		}

		Lib3MF::eCompression Lib3MF::Impl::CResourceData::GetCompression() {
			bool compression = m_ResourceData->getCompression();
			if (compression != false) {
				return Lib3MF::eCompression::Deflate;
			}
			return Lib3MF::eCompression::None;
		}

		IPackagePath * CResourceData::GetPath()
		{	
			return new CPackagePath(m_ResourceData->getPath());
		}

	}
}