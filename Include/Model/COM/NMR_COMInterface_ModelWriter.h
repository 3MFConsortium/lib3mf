/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

COM Interface Implementation for Model Writer Class

--*/

#ifndef __NMR_COMINTERFACE_MODELWRITER
#define __NMR_COMINTERFACE_MODELWRITER

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Writer/NMR_ModelWriter.h" 

namespace NMR {

	class CCOMModelWriter : public ILib3MFModelWriter {
	protected:
		PModelWriter m_pModelWriter;

		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();

	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelWriter)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);

		LIB3MFMETHOD(WriteToFile) (_In_z_ LPCWSTR pwszFilename);
		LIB3MFMETHOD(WriteToFileUTF8) (_In_z_ LPCSTR pwszFilename);

#ifndef __GCC
		LIB3MFMETHOD(WriteToStream) (_In_ IStream * pStream);
#endif

		LIB3MFMETHOD(WriteToCallback) (_In_ void * pWriteCallback, _In_opt_ void * pSeekCallback, _In_opt_ void * pUserData);

		CCOMModelWriter();
		void setWriter(_In_ PModelWriter pModelWriter);
	};

}

#endif // __NMR_COMINTERFACE_MODELWRITER
