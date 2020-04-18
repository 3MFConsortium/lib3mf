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

NMR_KeyStoreDecryptRight.h defines the KeyStoreDecryptRight Class. A decryptright is an in memory representation of the 3MF keystore resource data decrypright.

--*/

#ifndef __NMR_KEYSTORECEKPARAMS
#define __NMR_KEYSTORECEKPARAMS

#include <list>
#include <map>
#include <memory>
#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
namespace NMR {

	class CKeyStoreCEKParams {
	protected:
		eKeyStoreEncryptAlgorithm m_eAlgorithm;
		nfBool m_bCompression;
		std::vector<nfByte> m_rgIv, m_rgTag, m_rgAad;
		nfUint64 m_nDescriptor;
	public:
		CKeyStoreCEKParams(nfBool const & compression,
			eKeyStoreEncryptAlgorithm const & encryptionAlgorithm,
			std::vector<nfByte> const & iv,
			std::vector<nfByte> const & tag, 
			std::vector<nfByte> const & aad, 
			nfUint64 descriptor);

		eKeyStoreEncryptAlgorithm getEncryptionAlgorithm() const;
		nfBool isCompressed() const;
		std::vector<nfByte> const & getInitVector() const;
		std::vector<nfByte> const & getAuthTag() const;
		std::vector<nfByte> const & getAddAuthData() const;
		nfUint64 getDescriptor() const;

		void setAuthTag(std::vector<nfByte> const & buf);
		void setAddAuthData(std::vector<nfByte> const & buf);
	};

	typedef std::shared_ptr<CKeyStoreCEKParams> PKeyStoreCEKParams;

	class CKeyStoreContentEncryptionParams: public CKeyStoreCEKParams {
		std::vector<nfByte> m_rgKey;
	public:
		CKeyStoreContentEncryptionParams(nfBool const & compression,
			eKeyStoreEncryptAlgorithm const & encryptionAlgorithm,
			std::vector<nfByte> const & key,
			std::vector<nfByte> const & iv,
			std::vector<nfByte> const & tag,
			std::vector<nfByte> const & aad,
			nfUint64 descriptor);

		std::vector<nfByte> const & getKey() const;
	};

	typedef std::shared_ptr<CKeyStoreContentEncryptionParams> PCKeyStoreContentEncryptionParams;
}
#endif