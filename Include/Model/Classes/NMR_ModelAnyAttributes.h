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

Abstract:

NMR_ModelAnyAttributes.h defines the Model Attributes List.

--*/

#ifndef __NMR_MODELANYATTRIBUTES
#define __NMR_MODELANYATTRIBUTES

#include "Model/Classes/NMR_ModelAnyAttribute.h" 
#include "Common/NMR_Types.h"

#include <map>
#include <vector>

namespace NMR {
	class CModelAnyAttributes {
	private:
		std::vector<PModelAnyAttribute> m_Attributes;
		std::map<std::string, PModelAnyAttribute> m_AttributesMap;
	public:
		CModelAnyAttributes();
		void clear();

		PModelAnyAttribute addAttribute(_In_ const std::string sNameSpace, _In_ const std::string sName, _In_ const std::string sValue);
		PModelAnyAttribute getAttribute(_In_ nfUint32 nIndex);
		nfUint32 getAttributeCount();
		void removeAttribute(_In_ nfUint32 nIndex);
		nfBool hasAttribute(_In_ std::string sKey);
		void mergeAttribute(_In_ CModelAnyAttributes * pSourceAnyAttributes);
	};

	typedef std::shared_ptr <CModelAnyAttributes> PModelAnyAttributes;

}

#endif // __NMR_MODELANYATTRIBUTES
