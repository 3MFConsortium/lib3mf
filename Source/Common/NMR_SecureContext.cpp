
#include "Common/NMR_SecureContext.h"

namespace NMR {



	bool CSecureContext::hasClientContext() const {
		return false;
	}

	bool CSecureContext::hasClientContextForConsumer(std::string const & consumerId) const {
		return false;
	}

	DEKDESCRIPTOR CSecureContext::getDEKClientContext() const {
		return DEKDESCRIPTOR();
	}

	KEKDESCRIPTOR CSecureContext::getKEKClientContext(std::string const & consumerId) {
		return KEKDESCRIPTOR();
	}

	void CSecureContext::setDEKClientDescriptor(DEKDESCRIPTOR const & descriptor) {}

	void CSecureContext::addClientDescriptor(std::string const & consumerId, KEKDESCRIPTOR const & descriptor) {}

}