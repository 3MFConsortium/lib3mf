
#include "Common/NMR_SecureContext.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	bool CSecureContext::hasDekCtx() const {
		return m_bHasDek;
	}
	DEKDESCRIPTOR CSecureContext::getDekCtx() const {
		return m_sDekDescriptor
	}
	void CSecureContext::setDekCtx(DEKDESCRIPTOR const & descriptor) {}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxBegin() const {
		return m_ConsumerMap.cbegin();
	}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxEnd() const {
		return m_ConsumerMap.cend();
	}
	void CSecureContext::addKekCtx(std::string const & consumerId, KEKDESCRIPTOR const & descriptor) {
		if (m_ConsumerMap.find(consumerId) != m_ConsumerMap.end())
			throw CNMRException(NMR_ERROR_DUPLICATE_KEYSTORECONSUMERID);
		m_ConsumerMap[consumerId] = descriptor;
	}
	KEKDESCRIPTOR CSecureContext::getKekCtx(std::string const & consumerId) const {
		auto it = m_ConsumerMap.find(consumerId);
		if (it != m_ConsumerMap.end())
			return (*it).second;
		throw CNMRException(NMR_ERROR_RANGEERROR);
	}
	bool CSecureContext::emptyKekCtx() const {
		return m_ConsumerMap.empty();
	}
}