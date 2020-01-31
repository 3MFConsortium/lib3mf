


#ifndef NMR_SECURECONTEXT
#define NMR_SECURECONTEXT

#include <map>
#include <string>
#include "Common/NMR_SecureContentTypes.h"

namespace NMR {

	using RegisteredConsumerMap = std::map<std::string, KEKDESCRIPTOR>;
	class CSecureContext {
	private:
		RegisteredConsumerMap m_ConsumerMap;
	public:
		bool hasClientContext() const;
		bool hasClientContextForConsumer(std::string const & consumerId) const;

		DEKDESCRIPTOR getDEKClientContext() const;
		KEKDESCRIPTOR getKEKClientContext(std::string const & consumerId);

		void setDEKClientDescriptor(DEKDESCRIPTOR const & descriptor);
		void addClientDescriptor(std::string const & consumerId, KEKDESCRIPTOR const & descriptor);
	};


}

#endif // !NMR_SECURECONTEXT
