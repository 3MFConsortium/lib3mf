/*++

Copyright (C) 2020 3MF Consortium

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

NMR_Extension.cpp implements the Extension Class.
An extension is an in memory representation of the extensions supported by a 3MF package.

--*/

#include "Model/Classes/NMR_Extension.h"

namespace NMR {

	CExtension::CExtension(_In_ const std::string sNameSpaceURI, _In_ std::string sNameSpacePrefix, _In_ nfBool bIsRequired)
		: m_sNameSpaceURI(sNameSpaceURI), m_sNameSpacePrefix(sNameSpacePrefix), m_bIsRequired(bIsRequired)
	{
	}

	nfBool CExtension::getIsRequired() {
		return m_bIsRequired;
	}

	void CExtension::setIsRequired(_In_ nfBool bIsRequired) {
		m_bIsRequired = bIsRequired;
	}

	std::string CExtension::getNameSpaceURI() {
		return m_sNameSpaceURI;
	}

	void CExtension::setNameSpaceURI(_In_ const std::string sNameSpaceURI) {
		m_sNameSpaceURI = sNameSpaceURI;
	}

	std::string CExtension::getNameSpacePrefix() {
		return m_sNameSpacePrefix;
	}

	void CExtension::setNameSpacePrefix(_In_ const std::string sNameSpacePrefix) {
		m_sNameSpacePrefix = sNameSpacePrefix;
	}

}

