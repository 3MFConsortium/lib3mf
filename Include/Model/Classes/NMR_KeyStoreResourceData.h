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

NMR_KeyStoreResourceData.h defines the KeyStoreResourceData Class. A ResourceData is an in memory representation of the 3MF keystore resourcedata element.

--*/

#ifndef __NMR_KEYSTORERESOURCEDATA
#define __NMR_KEYSTORERESOURCEDATA

#include <map>
#include <memory>
#include <vector>
#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Model/Classes/NMR_KeyStoreDecryptRight.h"
#include "Model/Classes/NMR_PackageResourceID.h"
namespace NMR {
	enum eResourceDataState {
		NEW = 0,
		CLOSED,
		OPEN
	};
	class CKeyStoreResourceData {
		std::string m_sPath;
		eKeyStoreEncryptAlgorithm m_EncryptionAlgorithm;
		nfBool m_bCompression;
		std::vector<PKeyStoreDecryptRight> m_DecryptRights;
		std::map<std::string, PKeyStoreDecryptRight> m_ConsumerDecryptRight;
		CIPHERVALUE m_sCipherValue;
		nfBool m_bOpen;
		nfUint64 m_nfHandle;
		static nfUint64 s_nfHandleCount;
		eResourceDataState m_eState;
	
		void initializeCipherValue();
	public:
		CKeyStoreResourceData(std::string const& path);
		CKeyStoreResourceData(std::string const& path, eKeyStoreEncryptAlgorithm const& ea, nfBool const& compression);
		PKeyStoreDecryptRight addDecryptRight(NMR::PKeyStoreConsumer const& consumer, eKeyStoreEncryptAlgorithm const& encryptAlgorithm);
		PKeyStoreDecryptRight addDecryptRight(PKeyStoreDecryptRight const & dr);
		nfUint32 getDecryptRightCount();
		PKeyStoreDecryptRight getDecryptRight(nfUint32 index) const;
		PKeyStoreDecryptRight findDecryptRightByConsumer(NMR::PKeyStoreConsumer const& consumer);
		void removeDecryptRight(NMR::PKeyStoreConsumer const& consumer);
		eKeyStoreEncryptAlgorithm getEncryptionAlgorithm() const;
		nfBool getCompression() const;
		NMR::PPackageModelPath getPath() const;
		nfUint64 getHandle() const;

		nfBool empty() const;
		CIPHERVALUE getCipherValue() const;
		void setCipherValue(CIPHERVALUE const & cv);
		void open(std::vector<nfByte> const & newKey);

		void refreshIV(std::vector<nfByte> const &newIV);
		void refreshTag(std::vector<nfByte> const & newTag);

		bool isClosed() const;
		bool isNew() const;
	};
	typedef std::shared_ptr<CKeyStoreResourceData> PKeyStoreResourceData;
}

#endif
