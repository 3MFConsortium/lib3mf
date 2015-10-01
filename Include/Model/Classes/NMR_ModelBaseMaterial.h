/*++

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

NMR_ModelBaseMaterial.h defines the Model Base Material Resource Class.
A model material resource is an in memory representation of the 3MF
material resource object.

--*/

#include "Common/NMR_Local.h"
#include "Common/NMR_Types.h"
#include <string>
#include <memory>

#ifndef __NMR_MODELBASEMATERIAL
#define __NMR_MODELBASEMATERIAL

namespace NMR {

	class CModelBaseMaterial {
	private:
		std::wstring m_sName;
		nfColor m_cDisplayColor;
	public:
		CModelBaseMaterial() = delete;
		CModelBaseMaterial(_In_ const std::wstring sName, _In_ nfColor cDisplayColor);

		nfColor getDisplayColor();
		void setColor(_In_ nfColor cColor);
		std::wstring getDisplayColorString();

		std::wstring getName();
		void setName(_In_ const std::wstring sName);
	};

	typedef std::shared_ptr <CModelBaseMaterial> PModelBaseMaterial;

}

#endif // __NMR_MODELBASEMATERIAL
