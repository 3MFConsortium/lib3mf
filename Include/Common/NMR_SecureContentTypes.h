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

	enum eKeyStoreWrapAlgorithm {
		RSA_OAEP = 0,	// http://www.w3.org/2009/xmlenc11#rsa-oaep, http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p
	};

	enum eKeyStoreMaskGenerationFunction {
		MGF1_SHA1 = 160,	// http://www.w3.org/2009/xmlenc11#mgf1sha1
		MGF1_SHA224 = 224,	// http://www.w3.org/2009/xmlenc11#mgf1sha224
		MGF1_SHA256 = 256,	// http://www.w3.org/2009/xmlenc11#mgf1sha256
		MGF1_SHA384 = 384,	// http://www.w3.org/2009/xmlenc11#mgf1sha384
		MGF1_SHA512 = 512	// http://www.w3.org/2009/xmlenc11#mgf1sha512
	};

	enum eKeyStoreMessageDigest {
		SHA1 = 160,			// http://www.w3.org/2000/09/xmldsig#sha1
		SHA256 = 256,		// http://www.w3.org/2001/04/xmlenc#sha256
		SHA384 = 384,		// http://www.w3.org/2001/04/xmlenc#sha384
		SHA512 = 512,		// http://www.w3.org/2001/04/xmlenc#sha512
	};

	enum eKeyStoreEncryptAlgorithm {
		AES256_GCM = 1	// http://www.w3.org/2009/xmlenc11#aes256-gcm
	};

	class CKeyStoreContentEncryptionParams;
	using PKeyStoreContentEncryptionParams = std::shared_ptr<CKeyStoreContentEncryptionParams>;

	struct ContentEncryptionContext {
		void * m_pUserData;
		PKeyStoreContentEncryptionParams m_sParams;
	};
	using ContentEncryptionCbType = std::function<nfUint64(nfUint64, nfByte const *, nfByte *, ContentEncryptionContext &)>;

	struct ContentEncryptionDescriptor {
		ContentEncryptionCbType m_fnCrypt;
		ContentEncryptionContext m_sDekDecryptData;
	};

	class CKeyStoreAccessRight;
	using PKeyStoreAccessRight = std::shared_ptr<CKeyStoreAccessRight>;

	struct KeyWrappingContext {
		void * m_pUserData;
		PKeyStoreAccessRight m_pAccessRight;
	};

	using KeyWrappingCbType = std::function<nfUint64(std::vector<nfByte> const &, std::vector<nfByte> &, KeyWrappingContext &)>;

	struct KeyWrappingDescriptor {
		KeyWrappingCbType m_fnWrap;
		KeyWrappingContext m_sKekDecryptData;
	};

	using CryptoRandCbType = std::function<nfUint64(nfByte *, nfUint64, void *)>;

	struct CryptoRandGenDescriptor {
		CryptoRandCbType m_fnRNG;
		void * m_pUserData;
	};

	constexpr nfUint64 fnGetAlgorithmInitVectorSize(eKeyStoreEncryptAlgorithm ea) {
		return 12;
	}

	constexpr nfUint64 fnGetAlgorithmKeySize(eKeyStoreEncryptAlgorithm ea) {
		return 32;
	}

	constexpr nfUint64 fnGetAlgorithmAuthTagSize(eKeyStoreEncryptAlgorithm ea) {
		return 16;
	}
}

#endif // !NMR_SECURECONTENTTYPES
