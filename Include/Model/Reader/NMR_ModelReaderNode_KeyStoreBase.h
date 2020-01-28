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

NMR_ModelReaderNode_KeyStoreBase.h defines the base class for all Model Reader Node classes that are related to <keystore>.

--*/

#ifndef __NMR_MODELREADERNODE_KEYSTOREBASE
#define __NMR_MODELREADERNODE_KEYSTOREBASE

#include "Model/Reader/NMR_ModelReaderNode.h"
#include "Model/Classes/NMR_KeyStore.h"

namespace NMR {

	class CModelReaderNode_KeyStoreBase : public CModelReaderNode {
	protected:
		CKeyStore * m_pKeyStore;

	public:
		CModelReaderNode_KeyStoreBase() = delete;
		CModelReaderNode_KeyStoreBase(_In_ CKeyStore * pKeyStore, _In_ PModelReaderWarnings pWarnings);
	};
}

#endif // __NMR_MODELREADERNODE_KEYSTOREBASE
