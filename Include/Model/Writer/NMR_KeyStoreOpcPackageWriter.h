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

NMR_KeyStoreOpcPackageWriter.h defines an OPC Package writer in a portable way.

--*/

#ifndef NMR_KEYSTOREOPCPACKAGEWRITER
#define NMR_KEYSTOREOPCPACKAGEWRITER

#include <memory>

#include "Common/NMR_SecureContentTypes.h"
#include "Common/OPC/NMR_IOpcPackageWriter.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/3MF_ProgressMonitor.h"

namespace NMR {

	class CSecureContext;
	class CKeyStore;
	class CKeyStoreResourceData;
	class CKeyStoreDecryptRight;
	class CXmlWriter;

	using PSecureContext = std::shared_ptr<CSecureContext>;
	using PKeyStore = std::shared_ptr<CKeyStore>;
	using PKeyStoreResourceData = std::shared_ptr<CKeyStoreResourceData>;
	using PKeyStoreDecryptRight = std::shared_ptr<CKeyStoreDecryptRight>;

	class CKeyStoreOpcPackageWriter : public IOpcPackageWriter {
	protected:
		PIOpcPackageWriter m_pPackageWriter;
		PProgressMonitor m_pProgressMonitor;
		PSecureContext m_pSecureContext;
		PKeyStore m_pKeyStore;

		void writeKeyStoreStream(_In_ CXmlWriter * pXMLWriter);
		void updateAllResourceDataIV();
		void wrapPartStream(PKeyStoreResourceData rd, POpcPackagePart part);
		void updateResourceDataTag(PKeyStoreResourceData rd);
		void updateDecryptRightCipher(PKeyStoreDecryptRight dr, PKeyStoreResourceData rd);
	public:
		CKeyStoreOpcPackageWriter(
			_In_ PExportStream pImportStream, 
			_In_ PKeyStore pKeyStore, 
			_In_ PSecureContext pSecureContext, 
			_In_ PProgressMonitor pProgressMonitor);

		POpcPackagePart addPart(_In_ std::string sPath) override;
		void close() override;
		void addContentType(std::string sExtension, std::string sContentType) override;
		void addContentType(_In_ POpcPackagePart pOpcPackagePart, _In_ std::string sContentType) override;
		POpcPackageRelationship addRootRelationship(std::string sType, COpcPackagePart * pTargetPart) override;
		POpcPackageRelationship addPartRelationship(_In_ POpcPackagePart pOpcPackagePart, _In_ std::string sType, _In_ COpcPackagePart * pTargetPart) override;
	};

	using PKeyStoreOpcPackageWriter = std::shared_ptr<CKeyStoreOpcPackageWriter>;

}

#endif // !NMR_KEYSTOREOPCPACKAGEWRITER
