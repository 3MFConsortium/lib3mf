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

#ifndef NMR_MODELCONTEXT
#define NMR_MODELCONTEXT

#include <memory>

#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"
#include "Common/3MF_ProgressTypes.h"

namespace NMR {

	class CModel;
	class CKeyStore;
	class CSecureContext;
	class CProgressMonitor;
	class CModelWarnings;

	using PModel = std::shared_ptr<CModel>;
	using PKeyStore = std::shared_ptr<CKeyStore>;
	using PSecureContext = std::shared_ptr<CSecureContext>;
	using PProgressMonitor = std::shared_ptr<CProgressMonitor>;
	using PModelWarnings = std::shared_ptr<CModelWarnings>;

	class CModelContext {
	private:
		PModel m_pModel;
		PSecureContext m_pSecureContext;
		PProgressMonitor m_pProgressMonitor;
		PModelWarnings m_pWarnings;
		PKeyStore m_pKeystore;

	protected:


	public:
		CModelContext() = delete;
		CModelContext(_In_ PModel pModel);
		virtual ~CModelContext() = default;

		inline PModel const & model() const{
			return m_pModel;
		}
		inline PProgressMonitor const & monitor() const{
			return m_pProgressMonitor;
		}

		inline PModelWarnings const & warnings() const{
			return m_pWarnings;
		}

		inline PKeyStore const & keyStore() const{
			return m_pKeystore;
		}

		inline PSecureContext const & secureContext() const{
			return m_pSecureContext;
		}

		nfBool isComplete() const;

		void SetProgressCallback(Lib3MFProgressCallback callback, void* userData);
	};
}
#endif // !NMR_MODELCONTEXT

