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

COM Interface Implementation for Model Build Item Iterator Classes

--*/

#ifndef __NMR_COMINTERFACE_BUILDITEMITERATOR
#define __NMR_COMINTERFACE_BUILDITEMITERATOR

#include "Model/COM/NMR_COMInterface_ModelBuildItem.h" 

namespace NMR {

	class CCOMModelBuildItemIterator : public ILib3MFModelBuildItemIterator {
	protected:
		std::vector<PModelBuildItem> m_pBuildItems;
		nfInt32 m_nCurrentIndex;
	public:
		LIB3MFINTERFACE_DECL(ILib3MFModelBuildItemIterator)
			
		LIB3MFMETHOD(MoveNext) (_Out_ BOOL * pbHasNext);
		LIB3MFMETHOD(MovePrevious) (_Out_ BOOL * pbHasPrevious);
		LIB3MFMETHOD(GetCurrent) (_Outptr_ ILib3MFModelBuildItem ** ppResultBuildItem);
		LIB3MFMETHOD(Clone) (_Outptr_ ILib3MFModelBuildItemIterator ** ppIterator);

		CCOMModelBuildItemIterator();
		void addBuildItem(_In_ PModelBuildItem pBuildItem);
	};

}

#endif // __NMR_COMINTERFACE_BUILDITEMITERATOR
