
#include "Common/NMR_SecureContext.h"

namespace NMR {
	bool CSecureContext::hasDekCtx() const {
		return false;
	}
	DEKDESCRIPTOR CSecureContext::getDekCtx() const {
		return DEKDESCRIPTOR();
	}
	void CSecureContext::setDekCtx(DEKDESCRIPTOR const & descriptor) {}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxBegin() const {
		return ClientConsumerMap::const_iterator();
	}
	ClientConsumerMap::const_iterator CSecureContext::kekCtxEnd() const {
		return ClientConsumerMap::const_iterator();
	}
	void CSecureContext::addKekCtx(std::string const & consumerId, KEKDESCRIPTOR const & descriptor) {}
	KEKDESCRIPTOR CSecureContext::getKekCtx(std::string const & consumerId) const {
		return KEKDESCRIPTOR();
	}
	bool CSecureContext::emptyKekCtx() const {
		return false;
	}
}