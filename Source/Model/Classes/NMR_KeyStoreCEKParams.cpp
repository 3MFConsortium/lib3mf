#include "Model/Classes/NMR_KeyStoreCEKParams.h"
namespace NMR {
	CKeyStoreCEKParams::CKeyStoreCEKParams()
	{
		m_bCompression = false;
	}
	CKeyStoreCEKParams::CKeyStoreCEKParams(nfBool const & compression, eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, std::vector<nfByte> const & iv, std::vector<nfByte> const & tag, std::vector<nfByte> const & aad)
	{
		m_bCompression = compression;
		m_encryptionAlgorithm = encryptionAlgorithm;
		m_iv = iv;
		m_tag = tag;
		m_aad = aad;
	}
}
