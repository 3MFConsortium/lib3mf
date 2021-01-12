/*++

Copyright (C) 2019 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract:

NMR_KeyStoreConsumer.h defines the KeyStore Consumer Class. A consumer is an in memory representation of the 3MF file KeyStore Consumer.

--*/

#ifndef __NMR_KEYSTORECONSUMER
#define __NMR_KEYSTORECONSUMER

#include <list>
#include <map>
#include <memory>
#include <string>
#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"

namespace NMR {
	class CKeyStoreConsumer {
		std::string m_sConsumerID;
		std::string m_sKeyID;
		std::string m_sKeyValue;
	public:
		CKeyStoreConsumer(std::string const & consumerID, std::string const & keyID, std::string keyValue);

		std::string getConsumerID() const;

		std::string getKeyID() const;
		void setKeyID(std::string const & keyID);
		nfBool hasKeyID() const;

		std::string getKeyValue() const;
		void setKeyValue(std::string const & keyValue);
		nfBool hasKeyValue() const;
	};

	typedef std::shared_ptr<CKeyStoreConsumer> PKeyStoreConsumer;
}
#endif