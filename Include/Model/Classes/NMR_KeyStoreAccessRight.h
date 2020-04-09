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

#ifndef __NMR_KEYSTOREACCESSRIGHT
#define __NMR_KEYSTOREACCESSRIGHT

#include <list>
#include <map>
#include <memory>
#include "Common/NMR_Types.h"
#include "Model/Classes/NMR_KeyStoreConsumer.h"
namespace NMR {
	class CKeyStoreAccessRight {
	private:
		PKeyStoreConsumer m_pConsumer;
		KEKPARAMS m_sKekParams;
		nfBool m_bNew;
	public:
		CKeyStoreAccessRight(PKeyStoreConsumer const & consumer,
			eKeyStoreEncryptAlgorithm const & encryptionAlgorithm);

		CKeyStoreAccessRight(PKeyStoreConsumer const & consumer,
			eKeyStoreEncryptAlgorithm const & encryptionAlgorithm, 
			KEKPARAMS const & kekParams);
		
		KEKPARAMS getKEKParams();
		PKeyStoreConsumer getConsumer();
		eKeyStoreEncryptAlgorithm getEncryptionAlgorithm();
		bool isNew();
	};

	typedef std::shared_ptr<CKeyStoreAccessRight> PKeyStoreAccessRight;
}
#endif