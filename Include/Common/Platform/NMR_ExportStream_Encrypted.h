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

NMR_ImportStream_Encrypted.h defines the CImportStream_Encrypted Class.
This is a stream class for importing encrypted resources from the underlying stream and handing them
to the proper callbacks that will decrypt the content before returning back to the consumer.

--*/

#ifndef NMR_EXPORTSTREAM_ENCRYPTED
#define NMR_EXPORTSTREAM_ENCRYPTED

#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/Platform/NMR_ExportStream.h"
#include "Common/Platform/NMR_EncryptionHeader.h"

namespace NMR {



	class CExportStream_Encrypted : public CExportStream {
		PExportStream m_pEncryptedStream;
		ContentEncryptionDescriptor m_pDecryptContext;
		CEncryptionHeader m_header;
	public:
		CExportStream_Encrypted(PExportStream pEncryptedStream, ContentEncryptionDescriptor context);

		
		// Inherited via CExportStream
		virtual nfBool seekPosition(nfUint64 position, nfBool bHasToSucceed) override;
		virtual nfBool seekForward(nfUint64 bytes, nfBool bHasToSucceed) override;
		virtual nfBool seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed) override;
		virtual nfUint64 getPosition() override;
		virtual nfUint64 writeBuffer(const void * pBuffer, nfUint64 cbTotalBytesToWrite) override;

	};
}


#endif // !NMR_EXPORTSTREAM_ENCRYPTED


