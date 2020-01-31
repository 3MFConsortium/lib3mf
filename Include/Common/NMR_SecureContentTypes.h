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


--*/


#ifndef NMR_SECURECONTENTTYPES
#define NMR_SECURECONTENTTYPES

#define KEYSTORE_TYPES_MODULUSBUFFERSIZE 257
#define KEYSTORE_TYPES_EXPONENTBUFFERSIZE 5
#define KEYSTORE_TYPES_IVSIZE 12
#define KEYSTORE_TYPES_TAGSIZE 16

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <vector>
#include <functional>
namespace NMR {

	struct RSAKEYVALUE {
		nfByte m_modulus[KEYSTORE_TYPES_MODULUSBUFFERSIZE];
		nfByte m_exponent[KEYSTORE_TYPES_EXPONENTBUFFERSIZE];
	};

	struct CIPHERVALUE {
		std::vector<nfByte> m_iv;
		std::vector<nfByte> m_key;
		std::vector<nfByte> m_tag;
	};

	enum eKeyStoreEncryptAlgorithm {
		RsaOaepMgf1p = 0,
		Aes256Gcm = 1
	};

	struct DEKDECRYPTCTX {
		nfUint64 m_nfHandler;
		CIPHERVALUE m_sCipherValue;
		void * m_pUserData;
	};
	using ImportStream_DEKDecryptCallbackType = std::function<nfUint32(nfByte*, nfUint64, nfByte*, DEKDECRYPTCTX)>;

	struct KEKDECRYPTCTX {
		void * m_pUserData;
	};

	using ImportStream_KEKDecryptCallbackType = std::function<nfUint32(nfByte*, nfUint64, nfByte*, KEKDECRYPTCTX)>;

	struct DEKDESCRIPTOR {
		ImportStream_DEKDecryptCallbackType m_fnDecrypt;
		DEKDECRYPTCTX m_sDekDecryptData;
	};

	struct KEKDESCRIPTOR {
		ImportStream_KEKDecryptCallbackType m_fnDecrypt;
		KEKDECRYPTCTX m_sKekDecryptData;
	};
}

#endif // !NMR_SECURECONTENTTYPES
