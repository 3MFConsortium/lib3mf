


#ifndef NMR_SECURECONTEXT
#define NMR_SECURECONTEXT

#include <map>
#include <string>
#include "Common/NMR_SecureContentTypes.h"

namespace NMR {

	using ClientConsumerMap = std::map<std::string, KEKDESCRIPTOR>;
	class CSecureContext {
	private:
		 ClientConsumerMap m_ConsumerMap;
		 DEKDESCRIPTOR m_sDekDescriptor;
		 bool m_bHasDek = false;
	public:
		bool hasDekCtx() const;
		DEKDESCRIPTOR getDekCtx() const;
		void setDekCtx(DEKDESCRIPTOR const & descriptor);

		ClientConsumerMap::const_iterator kekCtxBegin() const;
		ClientConsumerMap::const_iterator kekCtxEnd() const;
		void addKekCtx(std::string const & consumerId, KEKDESCRIPTOR const & descriptor);
		KEKDESCRIPTOR getKekCtx(std::string const & consumerId) const;
		bool emptyKekCtx() const;
	};


}

#endif // !NMR_SECURECONTEXT
