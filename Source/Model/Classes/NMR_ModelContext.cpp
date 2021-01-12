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


--*/

#include "Model/Classes/NMR_ModelContext.h"

#include "Model/Classes/NMR_Model.h"
#include "Model/Classes/NMR_KeyStore.h"
#include "Common/NMR_SecureContext.h"
#include "Common/3MF_ProgressMonitor.h"
#include "Common/NMR_ModelWarnings.h"

namespace NMR {



	CModelContext::CModelContext(_In_ PModel pModel) {
		if (!pModel)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		m_pModel = pModel;

		m_pKeystore = pModel->getKeyStore();

		m_pProgressMonitor = std::make_shared<CProgressMonitor>();

		m_pSecureContext = std::make_shared<CSecureContext>();

		m_pWarnings = std::make_shared<CModelWarnings>();
	}

	nfBool CModelContext::isComplete() const {
		return (nullptr != m_pModel && nullptr != m_pProgressMonitor && nullptr != m_pSecureContext);
	}

	void CModelContext::SetProgressCallback(Lib3MFProgressCallback callback, void* userData) {
		m_pProgressMonitor->SetProgressCallback(callback, userData);
	}


}