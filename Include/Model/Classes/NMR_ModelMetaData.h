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

NMR_ModelMetaData.h defines the Model MetaData Class.
A metadata item is an in memory representation of the 3MF
metadata, and can be attached to any 3MF model node.

--*/

#ifndef __NMR_MODELMETADATA
#define __NMR_MODELMETADATA

#include "Common/NMR_Types.h"
#include "Common/Platform/NMR_SAL.h"
#include <memory>
#include <map>
#include <string>

namespace NMR {

	class CModelMetaData {
	private:
		std::string m_sName;
		std::string m_sNameSpace;
		std::string m_sValue;
		std::string m_sType;
		nfBool m_bPreserve;
	public:
		CModelMetaData() = delete;
		CModelMetaData(_In_ std::string sNameSpace, _In_ std::string sName, _In_ std::string sValue,
			_In_ std::string sType, _In_ nfBool bPreserve);

		std::string getName();
		std::string getNameSpace();
		std::string getValue();
		std::string getType();
		nfBool getPreserve();

		void setName(std::string );
		void setNameSpace(std::string);
		void setValue(std::string);
		void setType(std::string);
		void setPreserve(nfBool);

		std::string getKey();
		
		static bool isValidNamespaceAndName(std::string sNameSpace, std::string sName);
	};

	typedef std::shared_ptr <CModelMetaData> PModelMetaData;

}

#endif // __NMR_MODELMETADATA
