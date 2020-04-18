#include "Model/Classes/NMR_KeyStoreCEKParams.h"

namespace NMR {
	CKeyStoreCEKParams::CKeyStoreCEKParams(
		nfBool const & compression, 
		eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, 
		std::vector<nfByte> const & iv,
		std::vector<nfByte> const & tag, 
		std::vector<nfByte> const & aad, 
		nfUint64 descriptor): m_bCompression(compression), m_eAlgorithm(encryptionAlgorithm), m_rgIv(iv), m_rgTag(tag), m_rgAad(aad), m_nDescriptor(descriptor) 
	{
	}

	eKeyStoreEncryptAlgorithm CKeyStoreCEKParams::getEncryptionAlgorithm() const {
		return m_eAlgorithm;
	}
	nfBool CKeyStoreCEKParams::isCompressed() const {
		return m_bCompression;
	}
	std::vector<nfByte> const & CKeyStoreCEKParams::getInitVector() const {
		return m_rgIv;
	}
	std::vector<nfByte> const & CKeyStoreCEKParams::getAuthTag() const {
		return m_rgTag;
	}
	std::vector<nfByte> const & CKeyStoreCEKParams::getAddAuthData() const {
		return m_rgAad;
	}
	nfUint64 CKeyStoreCEKParams::getDescriptor() const {
		return m_nDescriptor;
	}
	void CKeyStoreCEKParams::setAuthTag(std::vector<nfByte> const & buf) {
		m_rgTag = buf;
	}
	void CKeyStoreCEKParams::setAddAuthData(std::vector<nfByte> const & buf) {
		m_rgAad = buf;
	}



	CKeyStoreContentEncryptionParams::CKeyStoreContentEncryptionParams(
		nfBool const & compression,
		eKeyStoreEncryptAlgorithm const & encryptionAlgorithm,
		std::vector<nfByte> const & key,
		std::vector<nfByte> const & iv,
		std::vector<nfByte> const & tag,
		std::vector<nfByte> const & aad,
		nfUint64 descriptor) : CKeyStoreCEKParams(compression, encryptionAlgorithm, iv, tag, aad, descriptor), m_rgKey(key) {}

	std::vector<nfByte> const & CKeyStoreContentEncryptionParams::getKey() const {
		return m_rgKey;
	}

}
