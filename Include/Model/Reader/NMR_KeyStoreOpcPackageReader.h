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

NMR_KeyStoreOpcPackageReader.h defines an OPC Package reader in a portable way.

--*/

#ifndef NMR_KEYSTOREOPCPACKAGEREADER
#define NMR_KEYSTOREOPCPACKAGEREADER

#include <memory>

#include "Common/NMR_SecureContentTypes.h"
#include "Common/OPC/NMR_IOpcPackageReader.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/3MF_ProgressMonitor.h"
#include "Model/Reader/NMR_ModelReaderWarnings.h"

namespace NMR {

	class COpcPackageReader;
	using POpcPackageReader = std::shared_ptr<COpcPackageReader>;
	class CKeyStore;
	using PKeyStore = std::shared_ptr<CKeyStore>;

	class CKeyStoreOpcPackageReader: public IOpcPackageReader {
	private:
		POpcPackageReader m_pPackageReader;
		PKeyStore m_KeyStore;
	protected:
		NMR::PImportStream findKeyStoreStream();
		void parseKeyStore(NMR::PImportStream keyStoreStream, NMR::PProgressMonitor pProgressMonitor);
	public:
		CKeyStoreOpcPackageReader(_In_ PImportStream pImportStream, _In_ PModelReaderWarnings pWarnings, _In_ PProgressMonitor pProgressMonitor);

		// Inherited via IOpcPackageReader
		virtual COpcPackageRelationship * findRootRelation(std::string sRelationType, nfBool bMustBeUnique) override;
		virtual POpcPackagePart createPart(std::string sPath) override;
		virtual nfUint64 getPartSize(std::string sPath) override;

		// get Keystore
		PKeyStore getKeyStore() const;
	};

	using PKeyStoreOpcPackageReader = std::shared_ptr<CKeyStoreOpcPackageReader>;

}

#endif // !NMR_KEYSTOREOPCPACKAGEREADER
