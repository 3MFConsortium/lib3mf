/*++

Copyright (C) 2018 3MF Consortium

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

NMR_ModelWriter.h defines the Model Writer Class.
A model writer exports the in memory represenation into the 3MF file.

--*/

#ifndef __NMR_MODELWRITER_TEXCOORDMAPPING
#define __NMR_MODELWRITER_TEXCOORDMAPPING

#include "Model/Classes/NMR_ModelTypes.h"
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

#include <map>
#include <vector>
#include <memory>

namespace NMR {

	typedef struct {
		nfFloat m_fU;
		nfFloat m_fV;
	} WRITERTEXCOORDMAPPINGTREEENTRY;

	bool operator< (_In_ const WRITERTEXCOORDMAPPINGTREEENTRY & entry1, _In_ const WRITERTEXCOORDMAPPINGTREEENTRY & entry2);

	class CModelWriter_TexCoordMapping {
	private:
		ModelResourceIndex m_nCurrentIndex;
		std::map<WRITERTEXCOORDMAPPINGTREEENTRY, ModelResourceIndex> m_IndexMap;
		std::vector<WRITERTEXCOORDMAPPINGTREEENTRY> m_TexCoordVector;
		ModelResourceID m_ResourceID;
		ModelResourceID m_TextureID;

	public:
		CModelWriter_TexCoordMapping() = delete;
		CModelWriter_TexCoordMapping(_In_ ModelResourceID ResourceID, _In_ ModelResourceID TextureID);

		void registerTexCoords(_In_ nfFloat fU, _In_ nfFloat fV);
		nfBool findTexCoords(_In_ nfFloat fU, _In_ nfFloat fV, _Out_ ModelResourceIndex & nResourceIndex);

		ModelResourceID getResourceID();
		ModelResourceID getTextureID();

		nfUint32 getCount();
		void getTexCoords(_In_ nfUint32 nIndex, _Out_ nfFloat & fU, _Out_ nfFloat & fV);
	};

	typedef std::shared_ptr <CModelWriter_TexCoordMapping> PModelWriter_TexCoordMapping;

}

#endif // __NMR_MODELWRITER_TEXCOORDMAPPING
