
#include "Common/NMR_SecureContext.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	bool CSecureContext::hasDekCtx() const {
		return m_bHasDek;
	}
	ContentEncryptionDescriptor CSecureContext::getDekCtx() const {
		return m_sDekDescriptor;
	}
	void CSecureContext::setDekCtx(ContentEncryptionDescriptor const & descriptor) {
		m_sDekDescriptor = descriptor;
		m_bHasDek = true;
	}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxBegin() const {
		return m_ConsumerMap.cbegin();
	}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxEnd() const {
		return m_ConsumerMap.cend();
	}
	void CSecureContext::addKekCtx(std::string const & consumerId, KeyWrappingDescriptor const & descriptor) {
		if (m_ConsumerMap.find(consumerId) != m_ConsumerMap.end())
			throw CNMRException(NMR_ERROR_KEYSTOREDUPLICATECONSUMERID);
		m_ConsumerMap[consumerId] = descriptor;
	}
	KeyWrappingDescriptor CSecureContext::getKekCtx(std::string const & consumerId) const {
		auto it = m_ConsumerMap.find(consumerId);
		if (it != m_ConsumerMap.end())
			return (*it).second;
		throw CNMRException(NMR_ERROR_KEKDESCRIPTORNOTFOUND);
	}
	bool CSecureContext::emptyKekCtx() const {
		return m_ConsumerMap.empty();
	}
}