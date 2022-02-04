#include "lib3mf_contentencryptionparams.hpp"
#include "lib3mf_interfaceexception.hpp"

#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"

using namespace Lib3MF::Impl;


namespace Lib3MF {
	namespace Impl {
		CContentEncryptionParams::CContentEncryptionParams(NMR::PCKeyStoreContentEncryptionParams const & p): m_pParams(p) {
			if (!p)
				throw ELib3MFInterfaceException(LIB3MF_ERROR_INVALIDPARAM);
		}

		Lib3MF_uint64 CContentEncryptionParams::GetDescriptor() {
			return m_pParams->getDescriptor();
		}

		eEncryptionAlgorithm CContentEncryptionParams::GetEncryptionAlgorithm() {
			return(eEncryptionAlgorithm)m_pParams->getEncryptionAlgorithm();
		}

		void CContentEncryptionParams::GetKey(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {
			const std::vector<NMR::nfByte> & buf = m_pParams->getKey();
			if (nByteDataBufferSize < buf.size() || nullptr == pByteDataBuffer)
				*pByteDataNeededCount = buf.size();
			else {
				std::copy(buf.begin(), buf.end(), pByteDataBuffer);
			}
		}

		void CContentEncryptionParams::GetInitializationVector(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {
			const std::vector<NMR::nfByte> & buf = m_pParams->getInitVector();
			if (nByteDataBufferSize < buf.size() || nullptr == pByteDataBuffer)
				*pByteDataNeededCount = buf.size();
			else {
				std::copy(buf.begin(), buf.end(), pByteDataBuffer);
			}
		}

		void CContentEncryptionParams::GetAuthenticationTag(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {
			const std::vector<NMR::nfByte> & buf = m_pParams->getAuthTag();
			if (nByteDataBufferSize < buf.size() || nullptr == pByteDataBuffer)
				*pByteDataNeededCount = buf.size();
			else {
				std::copy(buf.begin(), buf.end(), pByteDataBuffer);
			}
		}

		void CContentEncryptionParams::SetAuthenticationTag(Lib3MF_uint64 const nByteDataBufferSize, const Lib3MF_uint8 *pByteDataBuffer) {
			std::vector<NMR::nfByte> tag(pByteDataBuffer, pByteDataBuffer + nByteDataBufferSize);
			m_pParams->setAuthTag(tag);
		}

		void CContentEncryptionParams::GetAdditionalAuthenticationData(Lib3MF_uint64 nByteDataBufferSize, Lib3MF_uint64 *pByteDataNeededCount, Lib3MF_uint8 *pByteDataBuffer) {
			const std::vector<NMR::nfByte> & buf = m_pParams->getAddAuthData();
			if (nByteDataBufferSize < buf.size() || nullptr == pByteDataBuffer)
				*pByteDataNeededCount = buf.size();
			else {
				std::copy(buf.begin(), buf.end(), pByteDataBuffer);
			}
		}

		void CContentEncryptionParams::SetAdditionalAuthenticationData(Lib3MF_uint64 const nByteDataBufferSize, const Lib3MF_uint8 *pByteDataBuffer) {
			std::vector<NMR::nfByte> aad(pByteDataBuffer, pByteDataBuffer + nByteDataBufferSize);
			m_pParams->setAddAuthData(aad);
		}

		std::string CContentEncryptionParams::GetKeyUUID() {
			return m_pParams->getKeyUUID ();
		}

		std::string CContentEncryptionParams::GetPackagePath()
		{
			return m_pParams->getPath();
		}

		bool CContentEncryptionParams::HasCustomInformation(const std::string& sNameSpace, const std::string& sName)
		{
			return m_pParams->customInformation()->has(sNameSpace, sName);
		}

		std::string CContentEncryptionParams::GetCustomInformation(const std::string& sNameSpace, const std::string& sName)
		{
			return m_pParams->customInformation()->get(sNameSpace, sName);
		}


	}
}

