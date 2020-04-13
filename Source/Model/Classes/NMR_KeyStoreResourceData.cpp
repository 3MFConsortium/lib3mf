#include "Model/Classes/NMR_KeyStoreResourceData.h"
#include "Common/NMR_Exception.h"
#include <memory>

#define IV_SIZE 12
#define TAG_SIZE 16
#define KEY_SIZE 32

namespace NMR {


	nfUint64 CKeyStoreResourceData::s_nfHandleCount = 0;

	void CKeyStoreResourceData::initializeCipherValue() {
		m_sCipherValue.m_iv.resize(IV_SIZE);
		m_sCipherValue.m_key.resize(KEY_SIZE);
		m_sCipherValue.m_tag.resize(TAG_SIZE);
	}

	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path) {
		m_sPath = path;
		m_EncryptionAlgorithm = eKeyStoreEncryptAlgorithm::Aes256Gcm;
		m_nfHandle = ++s_nfHandleCount;
		m_eState = eResourceDataState::NEW;
		initializeCipherValue();
	}

	CKeyStoreResourceData::CKeyStoreResourceData(std::string const & path, eKeyStoreEncryptAlgorithm const & ea, nfBool const & compression)
	{
		m_sPath = path;
		m_EncryptionAlgorithm = ea;
		m_bCompression = compression;
		m_nfHandle = ++s_nfHandleCount;
		m_eState = eResourceDataState::NEW;
		initializeCipherValue();
	}

	eKeyStoreEncryptAlgorithm CKeyStoreResourceData::getEncryptionAlgorithm() const
	{	
		return m_EncryptionAlgorithm;
	}

	nfBool CKeyStoreResourceData::getCompression() const
	{
		return m_bCompression;
	}
	NMR::PPackageModelPath CKeyStoreResourceData::getPath() const
	{
		NMR::CResourceHandler * pResourceHandler = new NMR::CResourceHandler();
		return pResourceHandler->makePackageModelPath(m_sPath);
	}

	nfUint64 CKeyStoreResourceData::getHandle() const {
		return m_nfHandle;
	}

	nfBool CKeyStoreResourceData::empty() const {
		return m_DecryptRights.empty();
	}

	CIPHERVALUE CKeyStoreResourceData::getCipherValue() const {
		return m_sCipherValue;
	}

	void CKeyStoreResourceData::setCipherValue(CIPHERVALUE const & cv) {
		m_sCipherValue = cv;
		m_eState = eResourceDataState::CLOSED;
	}

	void CKeyStoreResourceData::open(std::vector<nfByte> const & newKey) {
		if (newKey.size() != KEY_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_sCipherValue.m_key = newKey;
		m_eState = eResourceDataState::OPEN;
	}

	void CKeyStoreResourceData::refreshIV(std::vector<nfByte> const & newIV) {
		if (newIV.size() != IV_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_sCipherValue.m_iv = newIV;
	}

	void CKeyStoreResourceData::refreshTag(std::vector<nfByte> const & newTag) {
		if (newTag.size() != TAG_SIZE)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		m_sCipherValue.m_tag = newTag;
	}

	bool CKeyStoreResourceData::isClosed() const {
		return m_eState == eResourceDataState::CLOSED;
	}

	bool CKeyStoreResourceData::isNew() const {
		return m_eState == eResourceDataState::NEW;
	}

}
