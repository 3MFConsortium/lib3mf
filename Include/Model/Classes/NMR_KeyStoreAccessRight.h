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

NMR_KeyStoreAccessRight.h defines the KeyStoreAccessRight Class. A accessright is an in memory representation of the 3MF keystore resource data accessright.

--*/

#ifndef __NMR_KEYSTOREACCESSRIGHT
#define __NMR_KEYSTOREACCESSRIGHT

#include <memory>
#include <vector>
#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
namespace NMR {
	class CKeyStoreAccessRight {
	private:
		PKeyStoreConsumer m_pConsumer;
		eKeyStoreWrapAlgorithm m_eAlgorithm;
		eKeyStoreMaskGenerationFunction m_eMgf;
		eKeyStoreMessageDigest m_eDigest; 
		std::vector<nfByte> m_rgCipherValue;
	public:
		CKeyStoreAccessRight(PKeyStoreConsumer const & consumer, eKeyStoreWrapAlgorithm const algorithm, eKeyStoreMaskGenerationFunction const mgf, eKeyStoreMessageDigest const digest, std::vector<nfByte> const & cipherValue);
		PKeyStoreConsumer getConsumer() const;
		eKeyStoreWrapAlgorithm getAlgorithm() const;
		eKeyStoreMaskGenerationFunction getMgf() const;
		eKeyStoreMessageDigest getDigest() const;
		std::vector<nfByte> const & getCipherValue() const;
		void setCipherValue(std::vector<nfByte> const & cv);
		nfBool isNew() const;
	};

	typedef std::shared_ptr<CKeyStoreAccessRight> PKeyStoreAccessRight;
}
#endif
