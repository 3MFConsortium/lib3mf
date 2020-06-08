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

#ifndef NMR_IMPORTSTREAM_ENCRYPTED
#define NMR_IMPORTSTREAM_ENCRYPTED

#include "Common/NMR_Types.h"
#include "Common/NMR_SecureContentTypes.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_EncryptionHeader.h"

#include <functional>

namespace NMR {

	class CImportStream_Encrypted : public CImportStream {
		PImportStream m_pEncryptedStream;
		ContentEncryptionDescriptor m_pDecryptContext;
		CEncryptionHeader m_header;
	public:
		CImportStream_Encrypted(PImportStream pEncryptedStream, ContentEncryptionDescriptor context);

		// Inherited via CImportStream
		virtual nfBool seekPosition(nfUint64 position, nfBool bHasToSucceed) override;
		virtual nfBool seekForward(nfUint64 bytes, nfBool bHasToSucceed) override;
		virtual nfBool seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed) override;
		virtual nfUint64 readBuffer(nfByte * pBuffer, nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll) override;
		virtual nfUint64 retrieveSize() override;
		virtual void writeToFile(const nfWChar * pwszFileName) override;
		virtual PImportStream copyToMemory() override;
		virtual nfUint64 getPosition() override;
	};
}


#endif // !NMR_IMPORTSTREAM_ENCRYPTED


