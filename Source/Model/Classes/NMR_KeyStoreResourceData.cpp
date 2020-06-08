#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {


	nfUint64 CKeyStoreResourceData::s_nfHandleCount = 0;

	CKeyStoreResourceData::CKeyStoreResourceData(
		PKeyStoreResourceDataGroup const & rdg,
		PPackageModelPath const& path,
		bool compression,
		eKeyStoreEncryptAlgorithm alg,
		std::vector<nfByte> const & iv,
		std::vector<nfByte> const & tag,
		std::vector<nfByte> const & aad) 
		: CKeyStoreCEKParams(compression, alg, iv, tag, aad, ++s_nfHandleCount), 
		m_pGroup(rdg), m_pPath(path)
	{
	}

	void CKeyStoreResourceData::setInitVector(std::vector<nfByte> const & newIV) {
		m_rgIv = newIV;
	}

	void CKeyStoreResourceData::setAuthTag(std::vector<nfByte> const & newTag) {
		m_rgTag = newTag;
	}
}
