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

COM Interface Implementation for Model Resource Iterator Class

--*/

#ifndef __NMR_COMINTERFACE_MODELRESOURCEITERATOR
#define __NMR_COMINTERFACE_MODELRESOURCEITERATOR

#include "Model/COM/NMR_COMInterfaces.h" 
#include "Model/Classes/NMR_ModelResource.h"
#include "Common/NMR_PagedVector.h"

namespace NMR {

	class CCOMModelResourceIterator : public ILib3MFModelResourceIterator {
	protected:
		std::vector<PModelResource> m_pResources;
		nfInt32 m_nCurrentIndex;
		nfError m_nErrorCode;
		std::string m_sErrorMessage;

		LIB3MFRESULT handleNMRException(_In_ CNMRException * pException);
		LIB3MFRESULT handleGenericException();
		LIB3MFRESULT handleSuccess();
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelResourceIterator)

		LIB3MFMETHOD(GetLastError) (_Out_ DWORD * pErrorCode, _Outptr_opt_ LPCSTR * pErrorMessage);
			
		LIB3MFMETHOD(MoveNext) (_Out_ BOOL * pbHasNext);
		LIB3MFMETHOD(MovePrevious) (_Out_ BOOL * pbHasPrevious);
		LIB3MFMETHOD(GetCurrent) (_Outptr_ ILib3MFModelResource ** ppResultResource);
		LIB3MFMETHOD(Clone) (_Outptr_ ILib3MFModelResourceIterator ** ppIterator);

		CCOMModelResourceIterator();
		void addResource(_In_ PModelResource pResource);
	};

}

#endif // __NMR_COMINTERFACE_MODELRESOURCEITERATOR
