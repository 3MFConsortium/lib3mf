/*++

Copyright (C) 2015 Microsoft Corporation
Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_ImportStream_GCC.cpp defines the CImportStream_GCC Class.
This is an abstract base stream class for importing from IStreams with GCC.

--*/

#include "Common/Platform/NMR_ImportStream_GCC.h"
#include "Common/NMR_Exception.h"
#include "Common/NMR_Exception_Windows.h"

namespace NMR {

	CImportStream_GCC::CImportStream_GCC()
	{
		// empty on purpose, to be implemented
	}

	CImportStream_GCC::CImportStream_GCC(_In_ const nfWChar * pwszFileName)
	{
		// empty on purpose, to be implemented
	}

	nfBool CImportStream_GCC::seekPosition(_In_ nfUint64 position, _In_ nfBool bHasToSucceed)
	{
			throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfBool CImportStream_GCC::seekForward(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfBool CImportStream_GCC::seekFromEnd(_In_ nfUint64 bytes, _In_ nfBool bHasToSucceed)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfUint64 CImportStream_GCC::getPosition()
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

	nfUint64 CImportStream_GCC::readBuffer(_In_ nfByte * pBuffer, _In_ nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll)
	{
		throw CNMRException(NMR_ERROR_NOTIMPLEMENTED);
	}

}
