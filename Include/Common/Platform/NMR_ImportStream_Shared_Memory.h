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

NMR_ImportStream_Shared_Memory.h implements the CImportStream_Memory Class.
This is a platform independent class for keeping data in a memory stream that doesn't own the wrapped buffer.

--*/

#ifndef __NMR_IMPORTSTREAM_SHARED_MEMORY
#define __NMR_IMPORTSTREAM_SHARED_MEMORY

#include "Common/Platform/NMR_ImportStream_Memory.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

namespace NMR {
	
	class CImportStream_Shared_Memory : public CImportStream_Memory {
		private:
			const nfByte * m_Buffer;
		protected:
			virtual const nfByte * getAt(nfUint64 nPosition);
		public:
			CImportStream_Shared_Memory(_In_ const nfByte * pBuffer, _In_ nfUint64 cbBytes);
			virtual PImportStream copyToMemory();
	};

} // namespace NMR

#endif // __NMR_IMPORTSTREAM_SHARED_MEMORY
