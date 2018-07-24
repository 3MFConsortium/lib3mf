/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ExportStream.h implements the ExportStream Class.
This is an abstract base stream class for exporting from various data sources.


--*/

#include "Common/Platform/NMR_ExportStream.h" 
#include "Common/NMR_Exception.h" 
#include <vector>

namespace NMR {

	void CExportStream::copyFrom(_In_ CImportStream * pImportStream, _In_ nfUint64 cbCount, _In_ nfUint32 cbBufferSize)
	{
		if (pImportStream == nullptr)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);
		if (cbBufferSize == 0)
			throw CNMRException(NMR_ERROR_INVALIDBUFFERSIZE);

		if (cbCount == 0)
			return;

		std::vector<nfByte> Buffer;
		Buffer.resize(cbBufferSize);

		nfUint32 cbBytesToCopy;
		while (cbCount > 0) {
	
			if (cbCount > cbBufferSize)
				cbBytesToCopy = cbBufferSize;
			else
				cbBytesToCopy = (nfUint32)cbCount;

			pImportStream->readBuffer(&Buffer[0], cbBytesToCopy, true);
			writeBuffer(&Buffer[0], cbBytesToCopy);

			cbCount -= cbBytesToCopy;
		}
	}


}
